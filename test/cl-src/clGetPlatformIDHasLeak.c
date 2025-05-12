/**
 * @file clGetPlatformIDHasLeak.c
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2025
 * 
 * Think in some implementation, clGetPlatformIDs has feature of memeory leaking.
 * 
 */

#include <CL/cl.h>
#include <stdio.h>

int main() {
    cl_platform_id platform;
    cl_int err;
    err = clGetPlatformIDs(1, &platform,0);
    err = clGetPlatformIDs(1, &platform,0);
    err = clGetPlatformIDs(1, &platform,0);
    err = clGetPlatformIDs(1, &platform,0);
    err = clGetPlatformIDs(1, &platform,0);
    err = clGetPlatformIDs(1, &platform,0);
    err = clGetPlatformIDs(1, &platform,0);
    puts("clGetPlatformIDHasLeak");
    return err;
}
