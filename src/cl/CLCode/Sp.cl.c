#include "Sp.cl.h"

__kernel void ae2fCL_eAnnKernsSpPredict(
    __global ae2f_float_t* io,
    const __global ae2f_float_t* field,
    ae2fCL_whenCL(__local) ae2f_float_t* loc
) {
    ae2f_float_t outbuff = 0;
    const size_t globalid = get_global_id(0);
    const size_t localid = get_local_id(0);

    __global ae2f_float_t* const out = io + get_global_size(0);

    #undef out_idxtent
    ae2fCL_whenCL(__local) ae2f_float_t* __got
        = loc + globalid;

    *__got = io[globalid] * field[globalid];

    ae2fCL_whenCL(barrier(CLK_LOCAL_MEM_FENCE));

    size_t blockSize = get_local_size(0);
    size_t halfBlockSize = blockSize >> 1;
    while (halfBlockSize>0) {
        if (localid < halfBlockSize) {
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
    if (!localid) {
        outbuff = loc[0];
        *out = outbuff;
    }
}

#include "uf.h"

__kernel void ae2fCL_eAnnKernsSpTrain(
    __global const ae2f_float_t* _in,
    __global ae2f_float_t* field,

    #if cl_mem_SIZE == ae2f_float_t_SIZE
        cl_mem_half_t LrErrA,
        cl_mem_half_t LrErrB
    #define LrErrTent() \
        const ae2f_float_t LrErr = \
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