#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_POINTS 4
#define NUM_FEATURES 2
#define NUM_WEIGHTS 3
#define EPOCHS 1000
#define LEARNING_RATE 0.1f

size_t 
_NUM_POINTS = NUM_POINTS,
_NUM_FEATURES = NUM_FEATURES,
_NUM_WEIGHTS = NUM_WEIGHTS,
_EPOCHS = EPOCHS,
_LEARNING_RATE = LEARNING_RATE;

const char *kernel_source = R"(
__kernel void logistic_regression(
    __global const float *X,
    __global const float *y_true,
    __global float *weights,
    __global float *gradients,
    const float learning_rate,
    const int num_points
) {
    int gid = get_global_id(0);
    if (gid >= num_points) return;
    float x1 = X[gid * 2];
    float x2 = X[gid * 2 + 1];
    float y = y_true[gid];
    float z = weights[0] * x1 + weights[1] * x2 + weights[2];
    float y_pred = 1.0f / (1.0f + exp(-z));
    float error = y_pred - y;
    gradients[gid * 3] = error * x1;
    gradients[gid * 3 + 1] = error * x2;
    gradients[gid * 3 + 2] = error;
}

__kernel void update_weights(
    __global float *weights,
    __global const float *gradients,
    const float learning_rate,
    const int num_points
) {
    int gid = get_global_id(0);
    if (gid >= 3) return;
    float grad_sum = 0.0f;
    for (int i = 0; i < num_points; i++) {
        grad_sum += gradients[i * 3 + gid];
    }
    weights[gid] -= learning_rate * (grad_sum / num_points);
}
)";

int main() {
    // XOR dataset
    float X[NUM_POINTS * NUM_FEATURES] = {0, 0, 0, 1, 1, 0, 1, 1};
    float y_true[NUM_POINTS] = {0, 1, 1, 0};
    float weights[NUM_WEIGHTS] = {0.0f, 0.0f, 0.0f}; // w1, w2, b

    // OpenCL setup
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel_lr, kernal_update;
    cl_mem X_buf, y_buf, weights_buf, gradients_buf;

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    queue = clCreateCommandQueue(context, device, 0, NULL);

    // Create and build program
    program = clCreateProgramWithSource(context, 1, &kernel_source, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create kernels
    kernel_lr = clCreateKernel(program, "logistic_regression", NULL);
    kernal_update = clCreateKernel(program, "update_weights", NULL);

    // Create buffers
    X_buf = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                           sizeof(float) * NUM_POINTS * NUM_FEATURES, X, NULL);
    y_buf = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                           sizeof(float) * NUM_POINTS, y_true, NULL);
    weights_buf = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                 sizeof(float) * NUM_WEIGHTS, weights, NULL);
    gradients_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                  sizeof(float) * NUM_POINTS * NUM_WEIGHTS, NULL, NULL);

    // Training loop
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        // Set kernel arguments for logistic_regression
        clSetKernelArg(kernel_lr, 0, sizeof(cl_mem), &X_buf);
        clSetKernelArg(kernel_lr, 1, sizeof(cl_mem), &y_buf);
        clSetKernelArg(kernel_lr, 2, sizeof(cl_mem), &weights_buf);
        clSetKernelArg(kernel_lr, 3, sizeof(cl_mem), &gradients_buf);
        clSetKernelArg(kernel_lr, 4, sizeof(float), &_LEARNING_RATE);
        clSetKernelArg(kernel_lr, 5, sizeof(int), &_NUM_POINTS);

        // Execute logistic_regression kernel
        size_t global_size = NUM_POINTS;
        clEnqueueNDRangeKernel(queue, kernel_lr, 1, NULL, &global_size, NULL, 0, NULL, NULL);

        // Set kernel arguments for update_weights
        clSetKernelArg(kernal_update, 0, sizeof(cl_mem), &weights_buf);
        clSetKernelArg(kernal_update, 1, sizeof(cl_mem), &gradients_buf);
        clSetKernelArg(kernal_update, 2, sizeof(float), &_LEARNING_RATE);
        clSetKernelArg(kernal_update, 3, sizeof(int), &_NUM_POINTS);

        // Execute update_weights kernel
        global_size = NUM_WEIGHTS;
        clEnqueueNDRangeKernel(queue, kernal_update, 1, NULL, &global_size, NULL, 0, NULL, NULL);
    }

    // Read final weights
    clEnqueueReadBuffer(queue, weights_buf, CL_TRUE, 0, sizeof(float) * NUM_WEIGHTS, weights, 0, NULL, NULL);

    // Print final weights
    printf("Final weights: w1=%.4f, w2=%.4f, b=%.4f\n", weights[0], weights[1], weights[2]);

    // Cleanup
    clReleaseMemObject(X_buf);
    clReleaseMemObject(y_buf);
    clReleaseMemObject(weights_buf);
    clReleaseMemObject(gradients_buf);
    clReleaseKernel(kernel_lr);
    clReleaseKernel(kernal_update);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}