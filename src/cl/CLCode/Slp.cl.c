
#include "Slp.cl.h"
#include <stdio.h>

__kernel void ae2fCL_eAnnKernsSlpPredict(
	__global ae2f_float_t* in_fieldsqr_out,
	ae2fCL_whenCL(__local) ae2f_float_t* _loc
) {
	const size_t globalid0 = get_global_id(0);
	const size_t globalid1 = get_global_id(1);

#define in_id		globalid0
#define out_id		globalid1

	const size_t globalsize0 = get_global_size(0);
	const size_t globalsize1 = get_global_size(1);

#define in_sz		globalsize0
#define perc_count	globalsize1
#define oper_count	perc_count
#define out_count	perc_count

	/** Does three will be defined as variable */
	ae2fCL_AnnDevSpPredict_OutTent(localid, blockSize, halfBlockSize);
	const size_t locid2 = get_local_id(1);

	__global const ae2f_float_t* const in = in_fieldsqr_out;

	__global const ae2f_float_t* field = 
		  in + (in_sz) + (in_sz + 1) * (out_id) + 1;

	__global ae2f_float_t* out = 
		in_fieldsqr_out
		+ in_sz
		+ (in_sz + 1) * out_count
		+ out_id;

	ae2fCL_whenCL(__local) ae2f_float_t* loc = 
		_loc + in_sz * out_id;


	/* in, field, loc, globalid **/
	ae2fCL_AnnDevSpPredict_Loc(
			in, field, loc, globalid0 
			);

	ae2fCL_AnnDevSpPredict_Out(localid, blockSize, halfBlockSize, loc, (*out));


#undef in_sz
#undef perc_count
#undef oper_count
#undef out_count
#undef in_id
#undef out_id 
}

__kernel void ae2fCL_eAnnKernsSlpTrain(
	__global ae2f_float_t* in
) {
	const size_t 
		iid = get_global_id(0)
		,oid = get_global_id(1)
		,isz = get_global_size(0)
		,osz = get_global_size(1);


	__global ae2f_float_t
		* _field = in + isz,
		* field = _field + (isz + 1) * oid + 1,
		* _LrErr = _field + (isz + 1) * osz;

	/** in, field, lrerr, i */
	ae2fCL_AnnDevSpTrain(in, field, _LrErr[oid], iid);
}
