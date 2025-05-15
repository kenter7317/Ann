#include "Sp.cl.h"

__kernel void ae2fCL_eAnnKernsSpPredict(
		__global ae2f_float_t* io,
		const __global ae2f_float_t* field,
		ae2fCL_whenCL(__local) ae2f_float_t* loc
) {
    
	const size_t globalid = get_global_id(0);

	__global ae2f_float_t* const out = io + get_global_size(0);

    #if 1
    ae2fCL_AnnDevSpPredict_Loc
	(io, field, loc, globalid);
	ae2fCL_AnnDevSpPredict_OutTent(localid, blockSize, halfBlockSize);
	ae2fCL_AnnDevSpPredict_Out(localid, blockSize, halfBlockSize, loc, *out);

    // printf("PREDICT OVER %d %d\n", get_global_id(0), get_global_size(0));
    #endif
}

#include "uf.h"

__kernel void ae2fCL_eAnnKernsSpTrain(
    __global const ae2f_float_t* _in,
    __global ae2f_float_t* field,

    #if cl_mem_SIZE == ae2f_float_t_SIZE
        ae2f_float_half_t LrErrA,
        ae2f_float_half_t LrErrB
    #define LrErrTent() \
        const ae2f_float_t LrErr = \
        *((UF_t) { LrErrA, LrErrB }).F
    #else
        ae2f_float_t LrErr
        #define LrErrTent()
    #endif
) {
    const size_t i = get_global_id(0);
    LrErrTent();
    ae2fCL_AnnDevSpTrain(_in, field, LrErr, i);
}
