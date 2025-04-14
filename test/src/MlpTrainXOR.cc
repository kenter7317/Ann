#include "../test.h"
#include <ae2f/Ann/Mlp.h>
#include <stdio.h>

#include <math.h>

#define gLearningRate 0.1
#define gEpochs 5000

#define SOMETHINGSMALL 0.0000001

// Activation and loss functions
static ae2f_float_t relu(ae2f_float_t x) { return x > 0 ? x : 0; }
static ae2f_float_t relu_deriv(ae2f_float_t x) { 
    return ae2f_float_t(x > 0); 
}
static ae2f_float_t sigmoid(ae2f_float_t x) { 
    return 1 / (1 + exp(-x));
}

static ae2f_float_t sigmoid_deriv(ae2f_float_t x) {
    return x * (1 - x);
}

static ae2f_float_t ce_gradient(ae2f_float_t y_pred, ae2f_float_t y_true) {
    ae2f_float_t grad = y_pred - y_true;
    if (grad > 1.0) grad = 1.0;
    if (grad < -1.0) grad = -1.0;
    return grad;
}

static ae2f_float_t
Sub(ae2f_float_t output, ae2f_float_t target) {
    return (target - output);
}
static ae2f_float_t ce_value(ae2f_float_t y_pred, ae2f_float_t y_true) {
    y_pred = y_pred < 1e-7 ? 1e-7 : y_pred;
    y_pred = y_pred > 1 - 1e-7 ? 1 - 1e-7 : y_pred;
    return -y_true * log(y_pred) - (1 - y_true) * log(1 - y_pred);
}

// XOR dataset
static const ae2f_float_t xor_inputs[4][2] = {
    {0, 0}, {0, 1}, {1, 0}, {1, 1}
};
static const ae2f_float_t xor_targets[4][1] = {
    {0}, {1}, {1}, {0}
};

static ae2f_float_t 
_wv[12]
, * wv[] { 0, _wv, _wv + 8 };

static size_t wc[] = {2, 4, 1};

void WeightMaker(ae2f_float_t** makerv, const size_t* makercv, const size_t makercc) {
    srand(0);
    for(size_t l = 1; l < makercc; l++) {
        const size_t 
        fan_in = makercv[l - 1]
        , fan_out = makercv[l];

        float scale = sqrt(6.0 / (fan_in + fan_out));
        for (int j = 0; j < fan_out; j++) {
            for (int i = 0; i < fan_in; i++) {
                makerv[l][j * fan_out + i] = (rand() / (ae2f_float_t)RAND_MAX) * 2 * scale - scale;
            }
        }
    }
}

int main() {
    // MLP: 2 input, 4 hidden, 1 output (3 layers)
    size_t layerlenv[] = {2, 4, 1};
    size_t layerc = sizeof(layerlenv) / sizeof(layerlenv[0]);

    // Function pointers
    ae2f_fpAnnAct_t actv[] = {sigmoid, sigmoid}; // Output: no activation
    ae2f_fpAnnAct_t act_deriv_v[] = {sigmoid_deriv, sigmoid_deriv}; // Last unused
    ae2f_fpAnnLoss_t loss_v[] = {Sub, Sub}; 

    WeightMaker(wv, layerlenv, layerc);

    // Create MLP
    ae2f_err_t err;
    ae2f_AnnMlp* mlp = ae2f_AnnMlpMk(
        layerc,
        0,
        layerlenv,
        NULL,
        NULL,
        actv,
        act_deriv_v,
        loss_v,
        _wv,
        &err
    );

    if (!mlp || err != ae2f_errGlob_OK) {
        printf("Failed to create MLP: err=%d\n", err);
        return 1;
    }

    // Training
    const int epochs = 1000;
    const ae2f_float_t learning_rate = 0.1;

    {
            // Loss monitoring
            ae2f_float_t output[1];
            for(size_t i = 0; i < 4; i++) {
                ae2f_mAnnSlpPredict(&mlp->Slp, xor_inputs[i], output);
                printf(
                    "Check Output for debug: in %f %f -> out %f\n"
                    , xor_inputs[i][0]
                    , xor_inputs[i][1]
                    , output[0]
                );
            }
    }

    for (int epoch = 0; epoch < epochs; epoch++) {
        ae2f_float_t total_loss = 0;
        for (int i = 0; i < 4; i++) {
            err = ae2f_mAnnSlpTrain(
                &mlp->Slp
                , xor_inputs[i]
                , NULL
                , xor_targets[i]
                , learning_rate
            );
            if (err) {
                printf("Training failed: err=%d\n", err);
                ae2f_AnnMlpDel(mlp);
                return 1;
            }

            // Loss monitoring
            ae2f_float_t output[1];
            ae2f_mAnnSlpPredict(&mlp->Slp, xor_inputs[i], output);
            total_loss += ce_value(output[0], xor_targets[i][0]);
        }
        if (epoch % 100 == 0) {
            // Loss monitoring
            ae2f_float_t output[1];
            for(size_t i = 0; i < 4; i++) {
                ae2f_mAnnSlpPredict(&mlp->Slp, xor_inputs[i], output);
                printf(
                    "Check Output for debug: in %f %f -> out %f\n"
                    , xor_inputs[i][0]
                    , xor_inputs[i][1]
                    , output[0]
                );
            }
            printf("Epoch %d, Loss: %.4f\n", epoch, total_loss / 4);
        }
    }

    // Testing
    printf("\nTesting XOR:\n");
    for (int i = 0; i < 4; i++) {
        ae2f_float_t output[1];
        ae2f_mAnnSlpPredict(&mlp->Slp, xor_inputs[i], output);
        ae2f_float_t y_pred = output[0]; // Post-process for prediction
        int predicted = y_pred > 0.5 ? 1 : 0;
        printf("Input: [%.0f, %.0f], Raw: %.4f, Predicted: %d, Target: %.0f\n",
               xor_inputs[i][0], xor_inputs[i][1], output[0], predicted, xor_targets[i][0]);
    }

    ae2f_AnnMlpDel(mlp);
    return 0;
}