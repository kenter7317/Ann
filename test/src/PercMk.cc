#include <ae2fCL/Ann.h>
#include "../test.h"
#include <ae2fCL/Ann/Perc.h>
#include <stdio.h>


#define gThreshold 0.0001

int main() {
    cl_int err = 0;
    cl_platform_id platform = 0;
    cl_device_id device = 0;
    cl_context context = 0;
    ae2f_float_t outfloat = 0;
    ae2f_float_t Buff[] = {
        -1, -1, -1, -1, -1
    };
    ae2fCL_AnnPerc perceptron;
    cl_command_queue queue = 0;
    cl_mem inbuff = 0;
    cl_mem outbuff = 0;
    ae2f_float_t out_checksum = 0;

    err = clGetPlatformIDs(1, &platform, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, 0);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    context = clCreateContext(0, 1, &device, 0, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);
    queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    err = ae2fCL_AnnMk(context, 1, &device);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    err = ae2fCL_AnnPercMk(
        &perceptron,
        0, sizeof(Buff)/sizeof(ae2f_float_t), 
        ae2fCL_efAnnAct_t::ae2fCL_eAnnActSigmoid, 0, context, 
        queue, CL_TRUE, 0, 0, 0
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

    inbuff = clCreateBuffer(
        context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(Buff), Buff, &err
    );
    if(!inbuff) goto __failure;
    if(err) goto __failure;

    outbuff = clCreateBuffer(
        context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,  
        sizeof(ae2f_float_t), &outfloat, &err
    );
    if(!outbuff) goto __failure;
    if(err) goto __failure;

    err = ae2fCL_AnnPercPredict(
        &perceptron, inbuff, outbuff, 0, 0, 
        queue, CL_TRUE, 0, 0, 0
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

    for(size_t i = 0; i < perceptron.mg_fieldLen; i++) 
    {
        ae2f_float_t got = Buff[i] * Buff[i];

        #pragma region Switch
        #define __Case(name) \
        case ae2fCL_mAnnActEnumDef(name): \
        out_checksum = ae2fCL_mAnnActFuncDef(name)(out_checksum); \
        break;
        #pragma endregion

        out_checksum += got;
        printf("Check-got: %f\n", got);
    }

    out_checksum += perceptron.m_bias;
    switch(perceptron.act) {
        __Case(Sigmoid);
        default: break;
    }
    printf("Checking two values match...: %f %f\n", out_checksum, outfloat);
    if((out_checksum - outfloat) * (out_checksum - outfloat) > gThreshold) {
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