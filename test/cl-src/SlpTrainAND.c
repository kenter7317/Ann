#include "../test.h"
#include <ae2fCL/Ann/Slp.h>
#include <ae2fCL/Ann.h>
#include <stdio.h>

#define gLearningRate 0.1
#define gEpochs 5000

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

int main() {
    cl_int errcl;
    ae2f_err_t err2, err;
    // [1, 1], [1, 0], [0, 1], [0, 0]
    ae2f_float_t ins[] = {
        1, 1, 1, 0, 0, 1, 0, 0
    };


    const ae2f_float_t goals[] = {
        1, 1, 0, 0
    };

    ae2f_float_t diff_got[2];

    err = ae2fCL_AnnMkEasy(&errcl);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    ae2f_mAnnSlp* Slp = ae2fCL_AnnSlpMkB(
        2, 0, 0, Forward, Backward, 1, 
        0, &err2, &errcl
    );
    ae2f_float_t outbuff[2] = {  5 };

    #define ae2fCL_AnnSlpPredict(obj, inb, _, in_idx, __, out, ...) \
    ae2f_mAnnSlpPredict(obj, inb + in_idx, out)

    err2 = ae2fCL_AnnSlpPredict(
        Slp, ins, 0,
        0/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f (1, 1)\n", outbuff[0]);

    err2 = ae2fCL_AnnSlpPredict(
        Slp, ins, 0,
        6/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f (0, 0)\n", outbuff[0]);

    err2 = ae2fCL_AnnSlpPredict(
        Slp, ins, 0,
        4/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f (0, 1)\n", outbuff[0]);

    err2 = ae2fCL_AnnSlpPredict(
        Slp, ins, 0,
        2/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f (1, 0)\n", outbuff[0]);

    if(err2) {
        err = err2; goto __failure;
    }
    #define ae2fCL_AnnSlpTrain(obj, ins, _, in_idx, __, goal, learnrate, ...) ae2f_mAnnSlpTrainB(obj, ins + in_idx, goal, learnrate)

    if(err) goto __failure;
    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = ae2f_mAnnSlpTrainB(
            Slp, ins,
            goals + 0, gLearningRate
        );
        if(err2) {
            err = err2; 
            puts("TRAIN DOOMED");
            printf("%d\n", err2);
            goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            Slp, ins + 2/*in_idx*/,
            goals + 2, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            Slp, ins + 4/*in_idx*/,
            goals + 2, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2f_mAnnSlpTrainB(
            Slp, ins + 6/*in_idx*/,
            goals + 2, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }
    }

    puts("we are going for predict");

    err2 = ae2fCL_AnnSlpPredict(
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

    err2 = ae2fCL_AnnSlpPredict(
        Slp, ins, 0,
        6/*in_idx*/, 0, outbuff, 
        queue, CL_TRUE, 0, 0, 0, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        err2 = ae2f_mAnnSpPredict(ae2f_mAnnSlpPerV(Slp, 0), ins + 6, outbuff);
        printf("Checking the value (SP): %f\n", outbuff[0]);

        printf("AND 0, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_AnnSlpPredict(
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

    err2 = ae2fCL_AnnSlpPredict(
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
    ae2f_mAnnSlpDel(Slp);
    ae2fCL_AnnDel();
    printf("ERR: %d\n", err);
    return err;
}