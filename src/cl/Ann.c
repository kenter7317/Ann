#include <ae2fCL/Ann.h>
#include <ae2fCL/Loc.h>
static const char* __clsrc =
#include "CLCode/Sp.clh"
"";

#include <stdio.h>

ae2f_SHAREDEXPORT struct ae2fCL_Ann_t ae2fCL_Ann = {0, };

static cl_program LIB;

ae2f_SHAREDEXPORT
ae2f_err_t ae2fCL_AnnMk(
    cl_command_queue premade_queue,
    cl_context ctx,
    cl_uint devCount,
    const cl_device_id* devs,
    cl_int* reterr
) {
    ae2f_err_t _err = 0;
    cl_int err2 = 0;

    ae2fCL_Ann.Ctx = ctx;
    ae2fCL_Ann.Q = premade_queue;
    ae2fCL_Ann.LErr = CL_SUCCESS;

    #define return(n) { err2 = n; _err = (_err | ae2f_errGlob_NFOUND) & ~ae2f_errGlob_DONE_HOWEV; goto RET; }

    if(!LIB) {
        LIB = clCreateProgramWithSource(
            ctx, 1, &__clsrc, 0, &err2 
        );
        err2 = clBuildProgram(LIB, devCount, devs, 0, 0, 0);
    }

    if(err2 != CL_SUCCESS) return(err2) ;

    #define LocCreateKernel(name) \
    if(!ae2fCL_AnnKerns[name]) { \
        ae2fCL_AnnKerns[name] = \
        clCreateKernel(LIB, #name, &err2); \
        if(_err != CL_SUCCESS) return(_err); \
    }
    LocCreateKernel(ae2fCL_eAnnKernsSpPredict);
    LocCreateKernel(ae2fCL_eAnnKernsSpTrain);

    #undef LocCreateKernel
    RET:
    if(reterr) *reterr = err2;
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
    ae2fCL_Ann.LErr = 0;

    return err;
}
