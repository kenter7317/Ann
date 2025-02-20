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