
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
    __global ae2fCL_AnnPerc* _this,
    __global ae2f_float_t* out,

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
    #undef idxtent
    
    ae2f_float_t got = _this->mg_field[get_global_id(0)] + _this->m_bias;

    #pragma region Switch
    #define __Case(name) \
    case name: \
    got = __##name(got); \
    break;
    #pragma endregion
    switch(_this->act) {
        // __Case(ae2fCL_eAnnActSigmoid);
        default: break;
    }

    ae2fCL_whenCL(mem_fence(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE));
    out[idx] += got;
}