#include "../test.h"
#include <ae2f/Ann/Mlp.h>
#include <stdio.h>

#include <math.h>

#define gLearningRate 0.1
#define gEpochs 25000

static ae2f_float_t
Forward(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t
ForwardPrime(ae2f_float_t output) {
    return output * (1.0 - output);
}

static ae2f_AnnDelta_t Backward;

static ae2f_float_t
Backward(ae2f_float_t o, ae2f_float_t T) {
    return (T - o) * ForwardPrime(o);
}


static ae2f_float_t
Step(ae2f_float_t x) {
    return x >= 0;
}

int main() {
    ae2f_err_t err_ae2f = 0;
    ae2f_err_t err2 = 0;
    ae2f_float_t diff_got[2] = {0, };
    ae2f_AnnMlp* Mlp = 0;
    size_t sizes[] = {2, 3, 1};
    ae2f_float_t outbuff[3] = {  5 };

    // [1, 1], [1, 0], [0, 1], [0, 0]
    ae2f_float_t ins[] = {
        1, 1, 1, 0, 0, 1, 0, 0
    };

    // 1, 1, 0, 0
    const ae2f_float_t goals[] = {
        1, 1, 0, 0
    };

    Mlp = ae2f_AnnMlpMk(
        sizeof(sizes) / sizeof(size_t), 0, sizes, 
        0, 0, Forward, Backward, 0,
        &err_ae2f
    );
    #if 1

    puts("Hello");

    if(err_ae2f) {
        goto __failure;
    }
    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = ae2f_mAnnSlpTrainB(
            &Mlp->Slp, ins, 
            goals + 0, gLearningRate
        );
        if(err2) {
            err_ae2f = err2; goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            &Mlp->Slp, ins + 2, 
            goals + 2, gLearningRate
        );
        if(err2) {
             goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            &Mlp->Slp, ins + 4, 
            goals + 2, gLearningRate
        );
        if(err2) {
             goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
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

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Mlp, ins, outbuff
    ); if(err2) {
        goto __failure;
    }
    printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        puts("AND 1, 1 no good");

        ae2f_float_t tmp[3] = {};
        err2 = ae2f_mAnnSlpPredict(
            ae2f_mAnnMlpLayerV(&Mlp->Mlp, 0),
            ins, tmp
        );

        err2 = ae2f_mAnnSlpPredict(
            ae2f_mAnnMlpLayerV(&Mlp->Mlp, 1),
            tmp, outbuff
        );
        printf("Checking the value(SLP): %f\n", outbuff[0]);

        err2 = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Slp, ins + 6, outbuff
    ); if(err2) {
        err2 = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 0 no good\n");

        err2 = ae2f_mAnnSlpPredict(
            ae2f_mAnnMlpLayerV(&Mlp->Mlp, 0),
            ins, outbuff
        );
        printf("Checking the value(SLP): %f\n", outbuff[0]);

        err2 = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Slp, ins + 4, outbuff 
    ); if(err2) {
        err_ae2f = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 1 no good\n");
        err2 = ae2f_mAnnSlpPredict(
            ae2f_mAnnMlpLayerV(&Mlp->Mlp, 0),
            ins, outbuff
        );
        printf("Checking the value(SLP): %f\n", outbuff[0]);
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Mlp, ins + 2, outbuff
    ); if(err2) {
        err_ae2f = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 0 no good\n");
        err2 = ae2f_mAnnSlpPredict(
            ae2f_mAnnMlpLayerV(&Mlp->Mlp, 0),
            ins, outbuff
        );
        printf("Checking the value(SLP): %f\n", outbuff[0]);
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    #endif

    __failure:
    ae2f_mAnnSlpDel(&Mlp->Slp);
    printf("Something is over, code: %d\n", err_ae2f);
    return err_ae2f | err2;
}