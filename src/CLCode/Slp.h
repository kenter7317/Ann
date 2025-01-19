#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>
#include <ae2fCL/Ann/Slp.h>

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

