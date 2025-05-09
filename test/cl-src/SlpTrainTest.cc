#include <ae2fCL/Ann/Slp.h>
#include <ae2f/Ann/LcgRand.h>
#include <math.h>
#include <stdio.h>

static ae2f_float_t
Forward(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t
ForwardPrime(ae2f_float_t output) {
    return output * (1.0 - output);
}

static ae2f_AnnLoss_t Backward;

static ae2f_float_t
Backward(const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
    return (output[i] - target[i]) / c;
}

#define EPSILON 0.00000001

int a() {
    ae2f_float_t goal[] = {1, 14, 5, 2, 7, 5, 3};
    #define goallen (sizeof(goal) / sizeof(goal[0]))
    ae2f_float_t in[5] = {1, 0, 0.2, 0.3, 1};
    #define inlen (sizeof(in) / sizeof(in[0]))

    ae2f_float_t out0[goallen] = {-1, -1}, out1[goallen] = {-1, -1};
    ae2f_AnnSlp* slp = 0;
    ae2fCL_AnnSlp* clslp = 0;

    ae2fCL_AnnMkEasy(0);

    slp = ae2f_AnnSlpMkB(inlen, 0, 0, Forward, ForwardPrime, Backward, goallen, 0, 0);
    clslp = ae2fCL_AnnSlpMkB(inlen, 0, 0, Forward, ForwardPrime, Backward, goallen, 0, 0, 0);
    clslp->CL_Slp.Add()->Changed = 1;

    for(size_t i = 0; i < goallen * (inlen + 1); i++) {
        if(!slp->Slp.pField) {
            puts("SLP NONE");
            return 1;
        }
        
        if(!clslp->Slp.pField) {
            puts("CLSLP NONE");
            return 1;
        }

        slp->Slp.pField[i] = clslp->Slp.pField[i] = 0.5;
    }

    puts("WEIGHT");
    for(size_t i = 0; i < goallen * (inlen + 1); i++) {
        if(!slp->Slp.pField) {
            puts("SLP NONE");
            return 1;
        }
        
        if(!clslp->Slp.pField) {
            puts("CLSLP NONE");
            return 1;
        }

        printf("%f %f \n", slp->Slp.pField[i], clslp->Slp.pField[i]);
    }

    printf("%u %u\n", slp->Slp.outc, clslp->Slp.outc);


    printf("SLP Predict: %d\n", slp->Slp.Predict(in, out0));
    printf("CLSLP Predict: %d\n", clslp->Slp.Predict(in, out1));
    clslp->CL_Slp.Add()->Changed = 1;

    puts("BEFORE TRAIN");
    for(size_t i = 0; i < goallen; i++) {
        printf("Attempt %u: SLP: %f, CLSLP: %f\n", i, out0[i], out1[i]);
        if((out0[i] - out1[i]) * (out0[i] - out1[i]) > EPSILON) {
            puts("FAILED");
            delete slp;
            delete clslp;
            return 1;
        }
    }

    printf("TRAIN: %d\n", slp->Slp.TrainB(in, goal, 0.1));
    printf("CL-TRAIN: %d\n", clslp->Slp.TrainB(in, goal, 0.1));

    puts("WEIGHT AFTER TRAIN");
    for(size_t i = 0; i < goallen * (inlen + 1); i++) {
        if(!slp->Slp.pField) {
            puts("SLP NONE");
            return 1;
        }
        
        if(!clslp->Slp.pField) {
            puts("CLSLP NONE");
            return 1;
        }

        printf("%f %f \n", slp->Slp.pField[i], clslp->Slp.pField[i]);
    }

    slp->Slp.Predict(in, out0);
    clslp->Slp.Predict(in, out1);

    puts("AFTER TRAIN");
    for(size_t i = 0; i < goallen; i++) {
        printf("Attempt %u: SLP: %f, CLSLP: %f\n", i, out0[i], out1[i]);
        printf("RES %f\n", (out0[i] - out1[i]) * (out0[i] - out1[i]));
        if((out0[i] - out1[i]) * (out0[i] - out1[i]) > EPSILON) {
            puts("FAILED");
            delete slp;
            delete clslp;
            return 1;
        }
    }

    delete slp;
    delete clslp;
    return 0;
}

int main() {
    int res = a();
    clReleaseContext(ae2fCL_Ann.Ctx);
    clReleaseCommandQueue(ae2fCL_Ann.Q);
    return res;
}