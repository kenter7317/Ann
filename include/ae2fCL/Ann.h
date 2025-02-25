/**
 * @file Ann.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ae2fCL_Ann_h
#define ae2fCL_Ann_h

#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <ae2f/errGlob.h>
#include <CL/cl.h>

/// @brief 
/// The list of OpenCL device functions that is able to run after the load of library.
/// 
/// Every element of this enum has a same name as device function. (kernel)
enum ae2fCL_eAnnKerns {
    ae2fCL_eAnnKernsSpPredict,
    ae2fCL_eAnnKernsSpTrain,

    /// @brief Count refers to @see ae2fCL_AnnKerns.
    ae2fCL_eAnnKerns_LEN
};

ae2f_extern ae2f_SHAREDCALL 
/// @brief 
/// # Singleton
/// 
/// Manager for Library ae2fCL::Ann. \n
/// @ref ae2fCL_Ann is its only instance.
struct ae2fCL_Ann_t {

    /// @brief 
    /// The vector where kernels are located.
    cl_kernel Kerns[
        ae2fCL_eAnnKerns_LEN
    ];

    /// @brief 
    /// Command queue \n
    /// You need to manually clean it.
    cl_command_queue Q;

    /// @brief
    /// OpenCL Context for allocating buffers or so. \n
    /// You need to manually clean it.
    cl_context Ctx;

    /// @brief 
    /// Last Error
    cl_int LErr;

} ae2fCL_Ann;

/// @deprecated
/// See @ref ae2fCL_Ann's @ref ae2fCL_Ann_t::Kerns
#define ae2fCL_AnnKerns ae2fCL_Ann.Kerns

/// @brief 
/// Setup for a library
/// @param premade_queue
/// The library will not take care of its memory scope...
/// which means it will not release it.
/// 
/// You need to manually free it after its use.
/// @param ctx 
/// The library will not take care of its memory scope...
/// which means it will not release it.
/// 
/// You need to manually free it after its use.
/// @param devCount
/// Count of parameter [devs] 
/// @param[in] devs 
/// Vector of device ids.
/// @param reterr 
/// Detailed error checkable when it returns @ref ae2f_errGlob_NFOUND
/// @return 
ae2f_extern ae2f_SHAREDCALL 
ae2f_err_t ae2fCL_AnnMk(
    cl_command_queue premade_queue,
    cl_context ctx,
    cl_uint devCount,
    const cl_device_id* devs,
    cl_int* reterr
);

/// @warning
/// It will not take care of releasing 
/// @ref ae2fCL_Ann_t::Q and @ref ae2fCL_Ann_t::Ctx.
/// 
/// @brief 
/// This function could be used if you want a library 
/// just works, and think you don't need any control of it.
///
/// After your operation, the library's resources may be released by
/// calling @ref ae2fCL_AnnDel
/// @param reterr 
/// Error number. represents if operations about OpenCL has succeed.
/// @return 
static ae2f_err_t ae2fCL_AnnMkEasy(
    cl_int* reterr
) {
    ae2f_err_t err = 0;
    cl_int er[1];
    cl_platform_id platform;
    cl_device_id device;
    ae2fCL_Ann.LErr = 0;

    #define return(n) { err = n; goto END; }

    *er = clGetPlatformIDs(1, &platform, 0);
    if(*er) return(ae2f_errGlob_NFOUND);
    
    *er = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, 0);
    if(*er) return(ae2f_errGlob_NFOUND);

    ae2fCL_Ann.Ctx = clCreateContext(0, 1, &device, 0, 0, er);
    if(er[0]) return(ae2f_errGlob_NFOUND);
    ae2fCL_Ann.Q = clCreateCommandQueueWithProperties(ae2fCL_Ann.Ctx, device, 0, er);
    if(er[0]) return(ae2f_errGlob_NFOUND);

    END:
    #undef return
    if(reterr) *reterr = *er;
    if(device) clReleaseDevice(device);
    return err | ae2fCL_AnnMk(ae2fCL_Ann.Q, ae2fCL_Ann.Ctx, 1, &device, reterr);
} 

/// @warning
/// This will not release 
/// @ref ae2fCL_Ann_t::Q and @ref ae2fCL_Ann_t::Ctx.
/// 
/// You'll need to manually free it.
/// @brief 
/// Release the library
/// @return 
ae2f_extern ae2f_SHAREDCALL cl_int ae2fCL_AnnDel();

#endif