#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

#include <ae2f/Float.h>

int main() {
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue 
        = clCreateCommandQueueWithProperties(context, device, 0, NULL);


    const char *kernelSource = 
    #include "../../src/CLCode/Perc.clh"
    "";

    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, 0);


    size_t log_size;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
    printf("log info len: %d\n", log_size);
    char *log = (char *)malloc(log_size);
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
    printf("Build log:\n%s\n", log);
    free(log);

    cl_kernel kernel = clCreateKernel(program, "ae2fCL_eAnnKernsPercMkRand", NULL);

    const size_t numElements = 5;
    ae2f_float_t *output = (ae2f_float_t*)malloc(numElements * sizeof(ae2f_float_t));

    for(size_t i = 0; i < numElements; i++)
    output[i] = -1;

    cl_mem outputBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, numElements * sizeof(ae2f_float_t), NULL, NULL);
    uint64_t seed = 234;

    cl_int d;

    d = clSetKernelArg(kernel, 0, sizeof(cl_mem), &outputBuffer);
    printf("err: %d\n", d);
    d = clSetKernelArg(kernel, 1, sizeof(uint32_t), &seed);
    printf("err: %d\n", d);

    d = clSetKernelArg(kernel, 2, sizeof(uint32_t), ((uint32_t*)(&seed)) + 1);
    printf("err: %d\n", d);
    
    d = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &numElements, NULL, 0, NULL, NULL);
    printf("err: %d\n", d);

    clEnqueueReadBuffer(queue, outputBuffer, CL_TRUE, 0, numElements * sizeof(ae2f_float_t), output, 0, NULL, NULL);

    for (size_t i = 0; i < numElements; i++) {
        printf("%f\n", output[i]);
    }

    free(output);
    clReleaseMemObject(outputBuffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
