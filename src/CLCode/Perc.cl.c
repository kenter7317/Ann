
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

__kernel void ae2fCL_eAnnKernsPercPredict(
    __global const ae2fCL_AnnPerc* _this,
    __global ae2f_float_t* out,
    const __global ae2f_float_t* field,
    const __global ae2f_float_t* in,
    ae2fCL_whenCL(__local) ae2f_float_t* loc,
    #if cl_mem_SIZE == 4
    uint16_t out_idxA,
    uint16_t out_idxB,

    uint16_t in_idxA,
    uint16_t in_idxB

    #define out_idxtent() uint32_t out_idx = out_idxA | (out_idxB << 16)
    #define in_idxtent() uint32_t in_idx = in_idxA | (in_idxB << 16)
    #else
    uint32_t out_idx,
    uint32_t in_idx
    #define out_idxtent()
    #define in_idxtent()
    #endif
) {
    out_idxtent();
    in_idxtent();

    const size_t globalid = get_global_id(0);
    const size_t localid = get_local_id(0);
    #undef out_idxtent
    ae2fCL_whenCL(__local) ae2f_float_t* __got
        = loc + globalid;

    #define got (*__got)
    got = in[globalid + in_idx] * field[globalid] + _this->m_bias;

    
    #pragma region Switch
    #define __Case(name) \
    case ae2fCL_mAnnActEnumDef(name): \
    got = ae2fCL_mAnnActFuncDef(name)(got); \
    break;
    #pragma endregion
    switch(_this->act) {
        __Case(Sigmoid);
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
        out[out_idx] = loc[0];
    }
}

#include <ae2fCL/Ann/Sizes/ae2f_float_t.h>

#if cl_mem_SIZE == 8
typedef uint32_t cl_mem_half_t;
#endif

#if cl_mem_SIZE == 4
typedef uint16_t cl_mem_half_t;
#endif

#if cl_mem_SIZE == ae2f_float_t_SIZE
typedef union UF_t {
    cl_mem_half_t UHalf[2];
    ae2f_float_t F;
} UF_t;
#endif

__kernel void ae2fCL_eAnnKernsPercTrain(
    __global const ae2f_float_t* _in,
    __global ae2f_float_t* field,

    #if cl_mem_SIZE == ae2f_float_t_SIZE
    cl_mem_half_t LrErrA,
    cl_mem_half_t LrErrB
    #define LrErrTent() \
        ae2f_float_t LrErr = \
        ((UF_t) { LrErrA, LrErrB }).F
    #else
    ae2f_float_t LrErr
    #define LrErrTent()
    #endif
) {
    const size_t i = get_global_id(0);
    LrErrTent();
    field[i] += LrErr * _in[i];
}