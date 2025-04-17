// 17.6 sec on 2:4:1
// TIMEOUT sec on 2:500:1

#include "../test.h"
#include <ae2fCL/Ann/Mlp.h>
#include <stdio.h>

#include <math.h>

#define gLearningRate 0.2
#define gEpochs 2000
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
    ae2fCL_AnnMkEasy(0);

    // MLP: 2 input, 4 hidden, 1 output (3 layers)
    size_t layerlenv[] = {2, 3, 1};
    size_t layerc = sizeof(layerlenv) / sizeof(layerlenv[0]);

    // Function pointers
    ae2f_fpAnnAct_t actv[] = {Forward, Forward}; // Output: no activation
    ae2f_fpAnnAct_t act_deriv_v[] = {ForwardPrime, ForwardPrime}; // Last unused
    ae2f_fpAnnLoss_t loss_v[] = { Backward, Backward };

    // Create MLP
    ae2f_err_t err;
    
    ae2f_AnnMlp* mlp = ae2fCL_AnnMlpMk(
        layerc,
        0,
        layerlenv,
        NULL,
        NULL,
        actv,
        act_deriv_v,
        loss_v,
        NULL,
        &err, 0
    );

    if (!mlp || err != ae2f_errGlob_OK) {
        printf("Failed to create MLP: err=%d\n", err);
        return 1;
    }

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

    // Testing
    printf("\nTesting XOR:\n");
    for (int i = 0; i < 4; i++) {
        ae2f_float_t output[1];
        ae2f_mAnnSlpPredict(&mlp->Slp, xor_inputs[i], output);
        ae2f_float_t y_pred = output[0]; // Post-process for prediction
        int predicted = y_pred > 0.5 ? 1 : 0;
        printf("Input: [%.0f, %.0f], Raw: %.4f, Predicted: %d, Target: %.0f\n",
               xor_inputs[i][0], xor_inputs[i][1], output[0], predicted, xor_targets[i][0]);
        if(predicted != xor_targets[i][0]) {
            puts("NO MATCH");
            return 1;
        }
    }

    ae2f_AnnMlpDel(mlp);
    ae2fCL_AnnDel();
    if(ae2fCL_Ann.Q) clReleaseCommandQueue(ae2fCL_Ann.Q);
    if(ae2fCL_Ann.Ctx) clReleaseContext(ae2fCL_Ann.Ctx);
    return 0;
}
