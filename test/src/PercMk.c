#include <ae2fCL/Ann.h>
#include "../test.h"
#include <ae2fCL/Ann/Perc.h>
#include <stdio.h>
int main() {
    cl_int err = 0;
    cl_platform_id platform = 0;
    cl_device_id device = 0;
    cl_context context = 0;

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

    ae2f_float_t Buff[] = {
        -1, -1, -1, -1, -1
    };
    ae2fCL_AnnPerc perceptron;
    err = ae2fCL_AnnPercMk(
        &perceptron,
        0, sizeof(Buff)/sizeof(ae2f_float_t), 
        0, context, queue, 0, 0, 0
    );
    #if 1
    CHECK_ERR(err, CL_SUCCESS, __failure);

    err = ae2fCL_AnnPercCheck(&perceptron, Buff, queue);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    for(size_t i = 0; i < sizeof(Buff) / sizeof(ae2f_float_t); i++) {
        printf("Buff check: %f\n", Buff[i]);
        if(Buff[i] < 0){
            err = ae2f_errGlob_WRONG_OPERATION;
            printf("Negative value found on %d\n", i);
            goto __failure;
        }
    }
    #endif

    __failure:
    ae2fCL_AnnDel();
    if(context) clReleaseContext(context);
    if(device) clReleaseDevice(device);
    if(queue) clReleaseCommandQueue(queue);
    if(!perceptron.mg_field) ae2fCL_AnnPercDel(&perceptron); 
    return err;
}