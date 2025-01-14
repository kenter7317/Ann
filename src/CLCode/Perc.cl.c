#include "Perc.h"
#include "Rand.h"

__kernel void ae2fCL_eAnnKernsPercMkRand(
    __global ae2f_float_t* _out,
    #if cl_mem_SIZE == 8
    uint32_t seedA,
    uint32_t seedB
) {
    uint64_t seed = seedA | (seedB << 32);
    #else
    uint64_t seed) {
    #endif
    size_t i = get_global_id(0);
    uint64_t __rand = ae2fCL_AnnLcgRand(seed + i * 500);
    ae2f_float_t __randReal = ae2fCL_AnnLcgRandDistribute(__rand);
    _out[i] = __randReal;
}