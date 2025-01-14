#ifndef ae2fCL_Ann_h
#define ae2fCL_Ann_h
#include <CL/cl.h>
#include <ae2f/Call.h>
#include <ae2f/Cast.h>

enum ae2fCL_eAnnKerns {
    ae2fCL_eAnnKernsPercMkRand,

    /// @brief Count refers to @see ae2fCL_AnnKerns.
    ae2fCL_eAnnKerns_LEN
};

ae2f_extern ae2f_SHAREDCALL 
cl_kernel ae2fCL_AnnKerns[
    ae2fCL_eAnnKerns_LEN
];

ae2f_extern ae2f_SHAREDCALL cl_int ae2fCL_AnnMk(
    cl_context ctx,
    cl_uint devCount,
    const cl_device_id* devs
);

ae2f_extern ae2f_SHAREDCALL cl_int ae2fCL_AnnDel();

#endif