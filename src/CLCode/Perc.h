#include <ae2f/Float.h>
#include <ae2fCL/Loc.h>
#include <ae2fCL/Ann/Sizes/cl_mem.h>

__kernel void ae2fCL_eAnnKernsPercMkRand(
    __global ae2f_float_t* _out,
    #if cl_mem_SIZE == 8
    uint32_t seedA,
    uint32_t seedB
    #else
    uint64_t seed
    #endif
);

#include <ae2fCL/Ann/Perc.h>