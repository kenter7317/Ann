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
    cl_int err = 0;
    cl_platform_id platform = 0;
    cl_device_id device = 0;
    cl_context context = 0;
    ae2f_err_t err2 = 0;

    err = clGetPlatformIDs(1, &platform, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    context = clCreateContext(0, 1, &device, 0, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    err = ae2fCL_AnnMk(context, 1, &device);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    size_t sizes[] = {2, 1};
    ae2fCL_AnnMlp Mlp;
    err2 = ae2fCL_AnnMlpMk(
        &Mlp, sizes, 0,
        0, sizeof(sizes) / sizeof(size_t), Step, 0,
        context, queue, CL_TRUE, 0, 0, 0 
    );
    if(err2) {
        err = err2; goto __failure;
    }

    // [1, 1], [1, 0], [0, 1], [0, 0]
    ae2f_float_t ins[] = {
        1, 1, 1, 0, 0, 1, 0, 0
    };

    // 1, 1, 0, 0
    const ae2f_float_t goals[] = {
        1, 1, 0, 0
    };
    // [1, 1], [1, 0], [0, 1], [0, 0]
    cl_mem inbuff = clCreateBuffer(
        context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        sizeof(ins), ins, &err
    );
    if(err) goto __failure;
    ae2f_float_t diff_got[2];
    for(size_t _ = 0; _ < gEpochs; _++) {
        #if 1
        err2 = ae2fCL_AnnMlpTrain(
            &Mlp, inbuff,
            0,0, 0/*in_idx*/,
            0, 0, diff_got, 0, 
            goals + 0, gLearningRate,
            queue, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 1, 1: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnMlpTrain(
            &Mlp, inbuff,
            0, 0, 2/*in_idx*/,
            0, 0, diff_got, 0, 
            goals + 2, gLearningRate,
            queue, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 1, 0: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnMlpTrain(
            &Mlp, inbuff,
            0, 0, 4/*in_idx*/,
            0, 0, diff_got, 0, 
            goals + 2, gLearningRate,
            queue, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 0, 1: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnMlpTrain(
            &Mlp, inbuff,
            0, 0, 6/*in_idx*/,
            0, 0, diff_got, 0, 
            goals + 2, gLearningRate,
            queue, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 0, 0: %f\n\n", diff_got[0]);
        #else
        err2 = ae2fCL_AnnSlpTrain(
            Mlp.List, inbuff,
            0,0/*in_idx*/, 0,
            goals/*goal*/, gLearningRate, 0, diff_got,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 1, 1: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnSlpTrain(
            Mlp.List, inbuff,
            0,2/*in_idx*/, 0,
            goals + 2/*goal*/, gLearningRate, 0, diff_got,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 1, 0: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnSlpTrain(
            Mlp.List, inbuff,
            0,4/*in_idx*/, 0,
            goals + 2/*goal*/, gLearningRate, 0, diff_got,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 0, 1: %f\n", diff_got[0]);

        err2 = ae2fCL_AnnSlpTrain(
            Mlp.List, inbuff,
            0,6/*in_idx*/, 0,
            goals + 2/*goal*/, gLearningRate, 0, diff_got,
            queue, CL_TRUE, 0, 0, 0, context
        );
        if(err2) {
            err = err2; goto __failure;
        }
        printf("Diff from 0, 0: %f\n\n", diff_got[0]);
        #endif
    }
    ae2f_float_t outbuff[2] = {  5 };

    puts("Predict time");

    err2 = ae2fCL_AnnMlpPredict(
        &Mlp, inbuff, 0,
        0/*in_idx*/, 0, outbuff, 0, 
        queue, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f %f\n", outbuff[0], outbuff[1]);
    if(outbuff[0] < 0.5) {
        printf("AND 1, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_AnnMlpPredict(
        &Mlp, inbuff, 0,
        6/*in_idx*/, 0, outbuff, 0, 
        queue, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_AnnMlpPredict(
        &Mlp, inbuff, 0,
        4/*in_idx*/, 0, outbuff, 0, 
        queue, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 0, 1 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    err2 = ae2fCL_AnnMlpPredict(
        &Mlp, inbuff, 0,
        2/*in_idx*/, 0, outbuff, 0, 
        queue, context
    ); if(err2) {
        err = err2; goto __failure;
    } printf("Checking the value: %f\n", outbuff[0]);
    if(outbuff[0] > 0.5) {
        printf("AND 1, 0 no good\n");
        err = ae2f_errGlob_IMP_NOT_FOUND;
    }

    __failure:
    printf("Something is over, code: %d\n", err);
    ae2fCL_AnnDel();
    if(context) clReleaseContext(context);
    if(inbuff) clReleaseMemObject(inbuff);
    if(device) clReleaseDevice(device);
    if(queue) clReleaseCommandQueue(queue);
    if(!Mlp.List) ae2fCL_AnnMlpDel(&Mlp); 
    return err;
}