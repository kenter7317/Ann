#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>
#include <ae2fCL/Ann/Perc.h>

__kernel void ae2fCL_eAnnKernsPercPredict(
    __global const ae2fCL_AnnPerc* _this,
    __global ae2f_float_t* out,
    const __global ae2f_float_t* field,
    const __global ae2f_float_t* in,
    ae2fCL_whenCL(__local) ae2f_float_t* loc,
    #if cl_mem_SIZE == 4
    uint16_t idxA,
    uint16_t idxB,
    #else
    uint32_t idx
    #endif
);

