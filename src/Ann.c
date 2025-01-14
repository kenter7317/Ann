#include <ae2fCL/Ann.h>
#include <ae2fCL/Loc.h>
static const char* __clsrc =
#include "CLCode/Perc.clh"
"";

#include <stdio.h>


static cl_program LIB;
ae2f_SHAREDEXPORT cl_kernel ae2fCL_AnnKerns[
    ae2fCL_eAnnKerns_LEN
] = {0, };

ae2f_SHAREDEXPORT
cl_int ae2fCL_AnnMk(
    cl_context ctx,
    cl_uint devCount,
    const cl_device_id* devs
) {
    cl_int _err;
    if(!LIB) {
        LIB = clCreateProgramWithSource(
            ctx, 1, &__clsrc, 0, &_err 
        );
        _err = clBuildProgram(LIB, devCount, devs, 0, 0, 0);
    }

    if(_err != CL_SUCCESS) return _err;

    #define LocCreateKernel(name) \
    if(!ae2fCL_AnnKerns[name]) { \
        ae2fCL_AnnKerns[name] = \
        clCreateKernel(LIB, #name, &_err); \
        if(_err != CL_SUCCESS) return _err; \
    }
    LocCreateKernel(ae2fCL_eAnnKernsPercMkRand);
    LocCreateKernel(ae2fCL_eAnnKernsPercPredict);

    #undef LocCreateKernel
    return _err;
}

ae2f_SHAREDEXPORT
cl_int ae2fCL_AnnDel() {
    cl_int err = CL_SUCCESS;

    for(size_t i = 0; i < ae2fCL_eAnnKerns_LEN; i++) {
        if(ae2fCL_AnnKerns[i]) {
            err = clReleaseKernel(ae2fCL_AnnKerns[i]);
            ae2fCL_AnnKerns[i] = 0;
            if(err != CL_SUCCESS) return err;
        }
    }

    if(LIB) err = clReleaseProgram(LIB);
    LIB = 0;

    return err;
}