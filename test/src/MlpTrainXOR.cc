#include "../test.h"
#include <ae2f/Ann/Mlp.h>
#include <stdio.h>
#include <chrono>

#include <math.h>

#define gLearningRate 0.5
#define gEpochs 1000
#define gEpochsVerbose 100

// XOR dataset
static const ae2f_float_t xor_inputs[4][2] = {
    {0, 0}, {0, 1}, {1, 0}, {1, 1}
};
static const ae2f_float_t xor_targets[4][1] = {
    {0}, {1}, {1}, {0}
};

static ae2f_float_t
Forward(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t
ForwardPrime(ae2f_float_t output) {
    return output * (1.0 - output);
}

static ae2f_float_t
Backward(const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
    return (output[i] - target[i]) / c;
}

int main() {
    // MLP: 2 input, 4 hidden, 1 output (3 layers)
    size_t layerlenv[] = {2, 4, 1};
    size_t layerc = sizeof(layerlenv) / sizeof(layerlenv[0]);

    // Function pointers
    ae2f_fpAnnAct_t actv[] = {Forward, Forward}; // Output: no activation
    ae2f_fpAnnAct_t act_deriv_v[] = {ForwardPrime, ForwardPrime}; // Last unused
    ae2f_fpAnnLoss_t loss_v[] = { Backward, Backward };

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
        NULL,
        &err
    );

    if (!mlp || err != ae2f_errGlob_OK) {
        printf("Failed to create MLP: err=%d\n", err);
        return 1;
    }

    puts("Training start");

    const auto now = std::chrono::system_clock::now();
    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
    for (int epoch = 0; epoch < gEpochs; epoch++) {
        ae2f_float_t total_loss = 0;
        for (int i = 0; i < 4; i++) {
            err = ae2f_mAnnSlpTrain(
                &mlp->Slp
                , xor_inputs[i]
                , NULL
                , xor_targets[i]
                , gLearningRate
            );
            if (err) {
                printf("Training failed: err=%d\n", err);
                ae2f_AnnMlpDel(mlp);
                return 1;
            }
        }
    }
    const std::time_t t_c2 = std::chrono::system_clock::to_time_t(now);
    
    // Testing
    printf("\nTesting XOR: Elapsed: %llu\n", t_c2);
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
