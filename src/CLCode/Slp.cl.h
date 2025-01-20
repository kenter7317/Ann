/// @file Slp.cl.h

#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>
#include <ae2fCL/Ann/Slp.h>
#include <ae2fCL/Ann/LcgRand.h>

/// @brief 
/// @param _out 
/// @param seedTent 
/// @return 
__kernel void ae2fCL_eAnnKernsSlpMkRand(
    __global ae2f_float_t* _out,
    #if cl_mem_SIZE == 8
    uint32_t seedA,
    uint32_t seedB
    #else
    uint64_t seedTent
    #endif
);

/// @brief 
/// @param _this 
/// @param out 
/// @param field 
/// @param in 
/// @param loc 
/// @if cl_mem_SIZE == 4
/// @param out_idxA 
/// @param out_idxB 
/// @param in_idxA
/// @param in_idxB
/// @else
/// @param out_idx 
/// @param in_idx
/// @endif 
/// @return 
__kernel void ae2fCL_eAnnKernsSlpPredict(
    __global const ae2fCL_AnnSlp* _this,
    __global ae2f_float_t* out,
    const __global ae2f_float_t* field,
    const __global ae2f_float_t* in,
    ae2fCL_whenCL(__local) ae2f_float_t* loc,
    #if cl_mem_SIZE == 4
    uint16_t out_idxA,
    uint16_t out_idxB,

    uint16_t in_idxA,
    uint16_t in_idxB
    #else
    uint32_t out_idx,
    uint32_t in_idx
    #endif
);

/// @brief 
/// @param _in 
/// @param field 
/// @param LrErr 
/// @param in_idx 
/// @return 
__kernel void ae2fCL_eAnnKernsSlpTrain(
    __global const ae2f_float_t* _in,
    __global ae2f_float_t* field,

    #if cl_mem_SIZE == ae2f_float_t_SIZE
        cl_mem_half_t LrErrA,
        cl_mem_half_t LrErrB,
    #else
        ae2f_float_t LrErr,
    #endif

    #if cl_mem_SIZE == 4
        uint16_t in_idxA,
        uint16_t in_idxB
    #else
        uint32_t in_idx
    #endif
);