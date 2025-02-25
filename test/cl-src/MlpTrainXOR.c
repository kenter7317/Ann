#include <ae2fCL/Ann.h>
#include "../test.h"
#include <ae2fCL/Ann/Mlp.h>
#include <stdio.h>

#include <math.h>

#define gLearningRate 0.1
#define gEpochs 1000

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

static ae2f_float_t
Step(ae2f_float_t x) {
    return x >= 0;
}

int main() {
    ae2f_err_t err_ae2f = 0;
    cl_int err = 0;
    cl_platform_id platform = 0;
    cl_device_id device = 0;
    cl_context context = 0;
    ae2f_err_t err2 = 0;
    cl_command_queue queue = 0;
    ae2f_float_t diff_got[2] = {0, };
    ae2f_AnnMlp* Mlp = 0;
    size_t sizes[] = {2, 3, 1};
    ae2f_float_t outbuff[2] = {  5 };

    // [1, 1], [1, 0], [0, 1], [0, 0]
    ae2f_float_t ins[] = {
        1, 1, 1, 0, 0, 1, 0, 0
    };

    // 1, 1, 0, 0
    const ae2f_float_t goals[] = {
        1, 1, 0, 0
    };

    err = clGetPlatformIDs(1, &platform, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    context = clCreateContext(0, 1, &device, 0, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    err_ae2f = ae2fCL_AnnMk(queue, context, 1, &device, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    CHECK_ERR(err_ae2f, CL_SUCCESS, __failure);
    puts("Hello");

    Mlp = ae2fCL_AnnMlpMk(
        sizeof(sizes) / sizeof(size_t), 0, sizes, 
        0, 0, Forward, Backward, 0,
        &err_ae2f, &err 
    );

    puts("Hello");
    if(!(Mlp->inc == 2 && Mlp->outc == 1 && Mlp->layerc == 2)) {
        puts("ae2fCL_AnnMlpMk has UB");
        printf(
            "INC: %u\n"
            "OUTC: %u\n"
            "LAYERC: %u\n\n",

            Mlp->inc, Mlp->outc, Mlp->layerc
        );
        goto __failure;
    }

    if(err2) {
        err = err2; goto __failure;
    }

    if(err) goto __failure;
    for(size_t _ = 0; _ < gEpochs; _++) {
        err2 = ae2f_AnnMlpTrainB(
            Mlp, ins, 
            goals + 2, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2f_AnnMlpTrainB(
            Mlp, ins + 2, 
            goals, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2f_AnnMlpTrainB(
            Mlp, ins + 4, 
            goals, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }

        err2 = ae2f_AnnMlpTrainB(
            Mlp, ins + 6, 
            goals + 2, gLearningRate
        );
        if(err2) {
            err = err2; goto __failure;
        }
    }

    puts("Predict time");

    err2 = ae2f_AnnMlpPredict(
        Mlp, ins, outbuff
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f %f\n", outbuff[0], outbuff[1]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_AnnMlpPredict(
        Mlp, ins + 6, outbuff
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_AnnMlpPredict(
        Mlp, ins + 4, outbuff 
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 0, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2f_AnnMlpPredict(
        Mlp, ins + 2, outbuff
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] < 0.5) {
        printf("AND 1, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    __failure:
    printf("Something is over, code: %d\n", err);
    ae2fCL_AnnDel();
    if(context) clReleaseContext(context);
    if(device) clReleaseDevice(device);
    if(queue) clReleaseCommandQueue(queue);
    return err;
}