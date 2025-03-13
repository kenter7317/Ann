#include "../test.h"
#include <ae2f/Ann/Mlp.h>
#include <stdio.h>

#include <math.h>

#define gLearningRate 0.1
#define gEpochs 25000

static ae2f_float_t
Step(ae2f_float_t x) {
    return x >= 0;
}


static ae2f_float_t
Calculate(ae2f_float_t out, ae2f_float_t goal) {
    return goal - out;
}

static ae2f_float_t
Forward(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t
ForwardPrime(ae2f_float_t output) {
    return output * (1.0 - output);
}

static ae2f_float_t
Backward(ae2f_float_t output, ae2f_float_t target) {
    return (target - output) * ForwardPrime(output);
}

int mainc() {
    ae2f_err_t err_ae2f = 0;
    ae2f_err_t err2 = 0;
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

    Mlp = ae2f_AnnMlpMk(
        sizeof(sizes) / sizeof(size_t), 0, sizes, 
        0, 0, Forward, Backward, 0,
        &err_ae2f
    );
    
    #if 1
    if(err_ae2f) {
        goto __failure;
    }
    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = ae2f_mAnnSlpTrainB(
            &Mlp->Slp,
            ins, goals + 2, gLearningRate
        );
        if(err2) {
            err_ae2f = err2; goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            &Mlp->Slp, ins + 2, 
            goals, gLearningRate
        );
        if(err2) {
             goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            &Mlp->Slp, ins + 4, 
            goals, gLearningRate
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
        &Mlp->Slp, ins, outbuff
    ); if(err2) {
        goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 1 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Slp, ins + 6, outbuff
    ); if(err2) {
        err2 = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 0 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Slp, ins + 4, outbuff 
    ); if(err2) {
        err_ae2f = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 0, 1 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
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
    ae2f_mAnnSlpDel(&Mlp->Slp);
    printf("Something is over, code: %d\n", err_ae2f | err2);
    return err_ae2f | err2;
}

int maincc() {
    ae2f_err_t err_ae2f = 0;
    ae2f_err_t err2 = 0;
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

    Mlp = ae2f_AnnMlpMk(
        sizeof(sizes) / sizeof(size_t), 0, sizes, 
        0, 0, Forward, Backward, 0,
        &err_ae2f
    );
    
    #if 1
    if(err_ae2f) {
        goto __failure;
    }
    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = Mlp->Slp.TrainB(
            ins, goals + 2, gLearningRate
        );
        if(err2) {
            err_ae2f = err2; goto __failure;
        }

        err2 = Mlp->Slp.TrainB(
            ins + 2, goals, gLearningRate
        );
        if(err2) {
             goto __failure;
        }

        err2 = Mlp->Slp.TrainB(
            ins + 4, goals, gLearningRate
        );
        if(err2) {
             goto __failure;
        }

        err2 = Mlp->Slp.TrainB(
            ins + 6, goals + 2, gLearningRate
        );
        if(err2) {
             goto __failure;
        }
    }
    #endif

    #if 1
    puts("Predict time");

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Slp, ins, outbuff
    ); if(err2) {
        goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 1 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Slp, ins + 6, outbuff
    ); if(err2) {
        err2 = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 0 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
        &Mlp->Slp, ins + 4, outbuff 
    ); if(err2) {
        err_ae2f = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 0, 1 no good\n");
        err_ae2f = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_mAnnSlpPredict(
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
    delete Mlp;
    printf("Something is over, code: %d\n", err_ae2f | err2);
    return err_ae2f | err2;
}

int main() {
    return mainc() + maincc();
}