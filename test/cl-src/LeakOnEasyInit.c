/**
 * @file LeakOnEasyInit.c
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2025
 * 
 * Yes, the library initiating function is leaking here.
 * 
 */

#include <ae2fCL/Ann.h>

int main() {
    ae2f_err_t err;
    cl_int err2;

    err = ae2fCL_AnnMkEasy(&err2);
    err2 |= ae2fCL_AnnDel();
    clReleaseContext(ae2fCL_Ann.Ctx);
    clReleaseCommandQueue(ae2fCL_Ann.Q);
    return err | err2;
}
