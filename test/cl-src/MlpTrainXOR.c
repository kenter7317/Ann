#include "../test.h"
#include <ae2fCL/Ann/Mlp.h>
#include <ae2fCL/Ann.h>
#include <stdio.h>

#include <math.h>

#define gLearningRate 0.1
#define gEpochs 5000

static ae2f_float_t
Forward(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t
ForwardPrime(ae2f_float_t output) {
    return (output) * (1.0f - output);
}

static ae2f_float_t
Backward(ae2f_float_t output, ae2f_float_t target) {
    return (target - output) * ForwardPrime(output);
}

int main() {
    ae2f_err_t err_ae2f = 0;
    cl_int err2 = 0;
    ae2f_float_t diff_got[2] = {0, };
    ae2f_AnnMlp* Mlp = 0;
    size_t sizes[] = {2, 3, 1};
    ae2f_float_t outbuff[2] = {  0, 0 };

    // [1, 1], [1, 0], [0, 1], [0, 0]
    ae2f_float_t ins[] = {
        1, 1, 1, 0, 0, 1, 0, 0
    };

    // 1, 1, 0, 0
    const ae2f_float_t goals[] = {
        1, 1, 0, 0
    };

    err_ae2f = ae2fCL_AnnMkEasy(&err2);
    CHECK_ERR(err_ae2f, CL_SUCCESS, __failure);

    Mlp = ae2fCL_AnnMlpMk(
        sizeof(sizes) / sizeof(size_t), 0, sizes, 
        0, 0, Forward, Backward, 0,
        &err_ae2f, 0
    );
    #if 0
    puts("Hello");
    if(!(Mlp->inc == 2 && Mlp->outc == 1 && Mlp->layerc == 3)) {
        puts("ae2fCL_AnnM.lpMk has UB");
        printf(
            "INC: %u\n"
            "OUTC: %u\n"
            "LAYERC: %u\n\n",

            Mlp->inc, Mlp->outc, Mlp->layerc
        );

        goto __failure;
    } else {
        puts("ae2fCL_AnnMlpMk was good");
    }
    #endif
    #if 1
    if(err_ae2f) {
        goto __failure;
    }
    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = ae2f_AnnSlpTrainB(
            &Mlp->Slp, ins, 
            goals + 2, gLearningRate
        );
        if(err2) {
            err_ae2f = err2; goto __failure;
        }

        err2 = ae2f_AnnSlpTrainB(
            &Mlp->Slp, ins + 2, 
            goals, gLearningRate
        );
        if(err2) {
             goto __failure;
        }

        err2 = ae2f_AnnSlpTrainB(
            &Mlp->Slp, ins + 4, 
            goals, gLearningRate
        );
        if(err2) {
             goto __failure;
        }

        err2 = ae2f_AnnSlpTrainB(
            &Mlp->Slp, ins + 6, 
            goals + 2, gLearningRate
        );
        if(err2) {
             goto __failure;
        }
    }
    #endif

    #if 1
    puts("Predict time");

    err2 = ae2f_AnnSlpPredict(
        &Mlp->Slp, ins, outbuff
    ); if(err2) {
        goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 1 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_AnnSlpPredict(
        &Mlp->Slp, ins + 6, outbuff
    ); if(err2) {
        err2 = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 0 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_AnnSlpPredict(
        &Mlp->Slp, ins + 4, outbuff 
    ); if(err2) {
        err_ae2f = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 0, 1 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_AnnSlpPredict(
        &Mlp->Slp, ins + 2, outbuff
    ); if(err2) {
        err_ae2f = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 1, 0 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    #endif

    __failure:
    ae2f_AnnSlpDel(&Mlp->Slp);
    ae2fCL_AnnDel();
    printf("Something is over, code: %d\n", err_ae2f | err2);
    return err_ae2f | err2;
}