#ifndef Mlp_h
#define Mlp_h

#include "./Slp.auto.h"
#include "./mac.h"

/** 
 * @brief 
 * delta to delta 
 * Bwd
 *
 * @inp aka out_then
 * @deltaseed 
 * */
ae2f_MAC() clMlpRvrse(
		ae2f_float_t			v_tmp,

		ae2f_float_t* const		r_delta_then,

		const size_t			i_oidx,
		const size_t			i_iidx,
		const size_t			i_isz,

		ae2f_AnnAct_t			i_actderiv_then,

		const ae2f_float_t* const	i_inp,
		const ae2f_float_t* const	i_deltaseed
		)
{
	if((i_iidx) < (i_isz) && !(i_oidx)) {
		i_actderiv_then(&(v_tmp), (i_inp), i_iidx, i_isz);
		(r_delta_then)[i_iidx] = (v_tmp) * (i_deltaseed)[i_iidx];
	}
}


#define _clMlpGetHD1_t	_clAtomAddF_t
typedef _clMlpGetHD1_t(host_float_t) clMlpGetHD1_t;

ae2f_MAC(__global, ) clMlpGetHD1(
		clMlpGetHD1_t			v_mem,

		ae2f_float_t* const		r_delta_then,

		const ae2f_float_t* const	i_weight,
		const ae2f_float_t* const	i_delta,

		const size_t			i_iidx,
		const size_t			i_isz,
		const size_t			i_oidx,
		const size_t			i_osz
		)
{
	if((i_oidx) < (i_osz) && (i_iidx) < (i_isz)) {
		unless((i_oidx)) (r_delta_then)[i_iidx] = 0;
		_clAtomAddF(__global
				, (v_mem)
				, &(r_delta_then)[(i_iidx)]
				, (i_weight)[(i_isz) * (i_oidx) + (i_iidx)] * (i_delta)[i_oidx]
			   );
	}
}

#endif
