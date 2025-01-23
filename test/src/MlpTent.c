#include <ae2fCL/Ann.h>
#include "../test.h"
#include <ae2fCL/Ann/Mlp.h>
#include <stdio.h>
#include <math.h>

#define gLearningRate 0.1
#define gEpochs 10

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

    ae2fCL_AnnMlp Mlp;

    size_t layerlength[] = {2, 18, 1};
    err2 = ae2fCL_AnnMlpMk(
        &Mlp, layerlength, 0, 0, sizeof(layerlength) / sizeof(layerlength[0]),
        Forward, Backward, context, queue, CL_TRUE, 0, 0, 0 
    );
    if(err2) {
        err = err2; goto __failure;
    }

    // sandbox code here...

    __failure:
    ae2fCL_AnnDel();
    if(context) clReleaseContext(context);
    if(device) clReleaseDevice(device);
    if(queue) clReleaseCommandQueue(queue);
    if(Mlp.List) ae2fCL_AnnMlpDel(&Mlp); 
    return err;
}