#include <ae2f/Ann/Mlp.h>
#include <cstdlib>
#include <cstdio>
#include <math.h>

static void Act(ae2f_float_t* r, ae2f_float_t x) {    
    *r = 1.0 / (1.0 + exp(-x));
}

static void ActDeriv(ae2f_float_t* r, ae2f_float_t output) {
    *r = output * (1.0 - output);
}

static void LossDerivCROSS(ae2f_float_t* r, const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
    const ae2f_float_t epsilon = 1e-7;
    ae2f_float_t o_i = output[i];
    o_i = o_i < epsilon ? epsilon : (o_i > 1.0 - epsilon ? 1.0 - epsilon : o_i);
    r[0] = (o_i - target[i]) / (c * o_i * (1.0 - o_i));
}

const ae2f_float_t inp[4][2] = { 
    {0, 0}, 
    {0, 1}, 
    {1, 0},
    {1, 1}
};

const ae2f_float_t goal_xor[4] = {0, 1, 1, 0}; // Fixed -0 to 0

ae2f_float_t output[2] = {0};
ae2f_err_t err[1] = {0};
ae2f_AnnMlp* mlp;
size_t lenv[] = {2, 5, 5, 1}; // Simplified architecture: 2 input, 4 hidden, 1 output

int main() {
    // Use cross-entropy loss and higher learning rate
    ae2f_AnnMlpMk(err, &mlp, 4, lenv, 0, 0, 0, LossDerivCROSS, 0, 0, 0, 0, 0.5, 0.5, 13, 9);

    if (err[0]) {
        printf("[Error]: %d\n", err[0]);
        return 1;
    }

    // Initialize weights and biases with larger range, all layers with sigmoid
    for (size_t i = 0; i < mlp->m_depth - 1; i++) {
        mlp->m_act[i] = Act;
        mlp->m_actderiv[i] = ActDeriv;
        size_t outc = lenv[i + 1];
        size_t inc = lenv[i];
        // Initialize biases
        for (size_t j = 0; j < outc; j++) {
            mlp->m_bias[i * mlp->m_outc + j] = ((ae2f_float_t)rand() / RAND_MAX - 0.5) * 2.0; // [-1, 1]
        }
        // Initialize weights
        for (size_t j = 0; j < inc * outc; j++) {
            mlp->m_weight[i * mlp->m_outc * mlp->m_outc + j] = ((ae2f_float_t)rand() / RAND_MAX - 0.5) * 2.0; // [-1, 1]
        }
    }

    // Initial predictions
    for (size_t j = 0; j < 4; j++) {
        mlp->PredictStream(err, inp[j], output);
        if (err[0]) {
            printf("[Error in Predict]: %d\n", err[0]);
            ae2f_AnnMlpDel(mlp);
            return 1;
        }
        printf("Before train: %f %f -> %f (target: %f)\n", inp[j][0], inp[j][1], output[0], goal_xor[j]);
    }

    // Training loop with mini-batch (all 4 inputs per epoch)
    for (size_t i = 0; i < 10000; i++) {
        for (size_t j = 0; j < 4; j++) {
            mlp->TrainAuto(err, inp[j], goal_xor + j);
            if (err[0]) {
                printf("[Error in TrainAutoStream]: %d\n", err[0]);
                ae2f_AnnMlpDel(mlp);
                return 1;
            }
        }
        if (i % 1000 == 0) {
            printf("\nEpoch %zu:\n", i);
            for (size_t j = 0; j < 4; j++) {
                output[0] = 0;
                mlp->PredictStream(err, inp[j], output);
                if (err[0]) {
                    printf("[Error in PredictStream]: %d\n", err[0]);
                    ae2f_AnnMlpDel(mlp);
                    return 1;
                }
                printf("\t%f %f -> %f (target: %f)\n", inp[j][0], inp[j][1], output[0], goal_xor[j]);
            }
        }
    }

    // Final predictions
    printf("\nFinal results:\n");
    for (size_t j = 0; j < 4; j++) {
        output[0] = 0;
        mlp->Predict(err, inp[j], output);
        if (err[0]) {
            printf("[Error in Predict]: %d\n", err[0]);
            ae2f_AnnMlpDel(mlp);
            return 1;
        }
        printf("%zu, %f %f -> %f (target: %f)\n", j, inp[j][0], inp[j][1], output[0], goal_xor[j]);
    }

    ae2f_AnnMlpDel(mlp);
    return 0;
}
