/// @file Sp.cl.h

#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>
#include <ae2fCL/Ann/Sizes/ae2f_float_t.h>

__kernel void ae2fCL_eAnnKernsSpPredict(
    __global ae2f_float_t* io,
    const __global ae2f_float_t* field,
    ae2fCL_whenCL(__local) ae2f_float_t* loc
);

/// @brief 
/// @param _in 
/// @param field 
/// @param LrErr 
/// @param in_idx 
/// @return 
__kernel void ae2fCL_eAnnKernsSpTrain(
    __global const ae2f_float_t* _in,
    __global ae2f_float_t* field,

    #if cl_mem_SIZE == ae2f_float_t_SIZE
        ae2f_float_half_t LrErrA,
        ae2f_float_half_t LrErrB
    #else
        ae2f_float_t LrErr
    #endif
);