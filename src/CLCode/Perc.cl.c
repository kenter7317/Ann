
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
    uint64_t seed
) {
    #endif
    size_t i = get_global_id(0);
    uint64_t __rand = ae2fCL_AnnLcgRand(seed + i * 500);
    ae2f_float_t __randReal = ae2fCL_AnnLcgRandDistribute(__rand);
    _out[i] = __randReal;
}
#include "Act.h"

#if 1
__kernel void ae2fCL_eAnnKernsPercPredict(
    __global const ae2fCL_AnnPerc* _this,
    __global ae2f_float_t* out,
    const __global ae2f_float_t* field,
    const __global ae2f_float_t* in,
    ae2fCL_whenCL(__local) ae2f_float_t* loc,
    #if cl_mem_SIZE == 4
    uint16_t idxA,
    uint16_t idxB,

    #define idxtent() uint32_t idx = idxA | (idxB << 16)
    #else
    uint32_t idx
    #define idxtent()
    #endif
) {
    idxtent();

    const size_t globalid = get_global_id(0);
    const size_t localid = get_local_id(0);
    #undef idxtent
    ae2fCL_whenCL(__local) ae2f_float_t* __got
        = loc + globalid;

    #define got (*__got)
    got = in[globalid] * field[globalid] + _this->m_bias;

    #pragma region Switch
    #define __Case(name) \
    case name: \
    got = __##name(got); \
    break;
    #pragma endregion
    switch(_this->act) {
        __Case(ae2fCL_eAnnActSigmoid);
        default: break;
    }

    ae2fCL_whenCL(barrier(CLK_LOCAL_MEM_FENCE));

    size_t blockSize = get_local_size(0);
    size_t halfBlockSize = blockSize >> 1;
    while (halfBlockSize>0) {
        if (localid <halfBlockSize) {
            loc[localid] += loc[localid + halfBlockSize];
            if ((halfBlockSize*2)<blockSize) {
                if (localid==0) { 
                    loc[localid] += loc[localid + (blockSize-1)];
                }
            }
        }
        ae2fCL_whenCL(barrier(CLK_LOCAL_MEM_FENCE));
        blockSize = halfBlockSize;
        halfBlockSize = blockSize >> 1;
    }
    if (localid==0) {
        out[idx] = loc[0];
    }
}
#endif