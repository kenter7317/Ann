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
        1, 0, context, queue, 0, 0, 0
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

    cl_mem inbuff = clCreateBuffer(
        context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(Buff), Buff, &err
    );
    if(!inbuff) goto __failure;
    if(err) goto __failure;

    ae2f_float_t outfloat = 0;
    cl_mem outbuff = clCreateBuffer(
        context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,  
        sizeof(ae2f_float_t), &outfloat, &err
    );
    if(!outbuff) goto __failure;
    if(err) goto __failure;

    err = ae2fCL_AnnPercPredict(
        &perceptron, inbuff, outbuff, 0, 0, 
        queue, 0, 0, 0
    );
    if(err) goto __failure;

    err = clEnqueueReadBuffer(
        queue, outbuff, 1, 
        0, sizeof(ae2f_float_t), &outfloat,
        0, 0, 0
    );
    if(err) goto __failure;
    printf("out: %f\n", outfloat);
    printf(
        "Bias global: %f, with bias: %f\n", 
        perceptron.m_bias, 
        perceptron.m_bias * sizeof(Buff)/ sizeof(ae2f_float_t) + outfloat
    );

    ae2f_float_t out_checksum = 0;
    for(size_t i = 0; i < perceptron.mg_fieldLen; i++) 
    {
        #undef idxtent
        ae2f_float_t got = Buff[i] * Buff[i] + perceptron.m_bias;

        #pragma region Switch
        #define __Case(name) \
        case ae2fCL_mAnnActEnumDef(name): \
        got = ae2fCL_mAnnActFuncDef(name)(got); \
        break;
        #pragma endregion
        switch(perceptron.act) {
            __Case(Sigmoid);
            default: break;
        }

        out_checksum += got;
        printf("Check-got: %f\n", got);
    }
    printf("Checking two values match...: %f %f\n", out_checksum, outfloat);
    if((out_checksum - outfloat) * (out_checksum - outfloat) > 0.0001) {
        printf("Check failed\n");
        return ae2f_errGlob_WRONG_OPERATION;
    }

    __failure:
    ae2fCL_AnnDel();
    if(context) clReleaseContext(context);
    if(device) clReleaseDevice(device);
    if(queue) clReleaseCommandQueue(queue);
    if(outbuff) clReleaseMemObject(outbuff);
    if(inbuff) clReleaseMemObject(inbuff); 
    if(!perceptron.mg_field) ae2fCL_AnnPercDel(&perceptron); 
    return err;
}