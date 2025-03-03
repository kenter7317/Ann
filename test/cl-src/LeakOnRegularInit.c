/**
 * @file LeakOnRegularInit.c
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2025
 * 
 * In my machine it is leaking. And I don't know why.
 * 
 */

#include <ae2fCL/Ann.h>
#include "../test.h"

int main() {
    cl_int err = 0;
    cl_platform_id platform = 0;
    cl_device_id device = 0;
    cl_context context = 0;
    ae2f_err_t err2 = 0;
    
    err = clGetPlatformIDs(1, &platform, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    goto __failure;
    
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    context = clCreateContext(0, 1, &device, 0, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    err = ae2fCL_AnnMk(queue, context, 1, &device, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    __failure:
    ae2fCL_AnnDel();
    if(device) clReleaseDevice(device);
    if(queue) clReleaseCommandQueue(queue);
    if(context) clReleaseContext(context);
    return err;
}