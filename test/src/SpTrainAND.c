#include "../test.h"
#include <ae2f/Ann/Sp.h>
#include <stdio.h>

#define gLearningRate 0.1
#define gEpochs 1000

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
    return (output) * (1.0f - output);
}

static ae2f_float_t
Backward(ae2f_float_t output, ae2f_float_t target) {
    return (target - output) * ForwardPrime(output);
}

int main() {
    // [1, 1], [1, 0], [0, 1], [0, 0]
    ae2f_float_t ins[] = {
        1, 1, 1, 0, 0, 1, 0, 0
    };


    const ae2f_float_t goals[] = {
        1, 1, 0, 0
    };

    ae2f_float_t diff_got[2];

    ae2f_err_t err2, err;
    ae2f_mAnnSp* Slp = (ae2f_mAnnSp*)ae2f_AnnSpMk(
        2, 0, Forward, Backward, &err2, 0 
    );

    if(err2) {
        err = err2; goto __failure;
    }
    #define ae2fCL_mAnnSlpTrain(obj, ins, _, in_idx, __, goal, learnrate, ...) ae2f_mAnnSpTrainB(obj, ins + in_idx, *(goal), learnrate)

    if(err) goto __failure;
    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = ae2f_mAnnSpTrainB(
            Slp, ins,
            *goals + 0, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2fCL_mAnnSlpTrain(
            Slp, ins,
            0, 2/*in_idx*/,
            0, goals + 2, gLearningRate, 
            0, (void*)diff_got, 0,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2fCL_mAnnSlpTrain(
            Slp, ins,
            0, 4/*in_idx*/,
            0, goals + 2, gLearningRate, 
            0, (void*)diff_got, 0,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2fCL_mAnnSlpTrain(
            Slp, ins,
            0, 6/*in_idx*/,
            0, goals + 2, gLearningRate, 
            0, diff_got, 0,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
    }
    ae2f_float_t outbuff[2] = {  5 };

    #define ae2fCL_mAnnSlpPredict(obj, inb, _, in_idx, __, out, ...) \
    ae2f_mAnnSpPredict(obj, inb + in_idx, (out))

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        0/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 1, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        6/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        4/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_mAnnSlpPredict(
        Slp, ins, 0,
        2/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    __failure:
    ae2f_mAnnSpDel(Slp);
    return err | err2;
}