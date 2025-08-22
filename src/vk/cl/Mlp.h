#ifndef Mlp_h
#define Mlp_h

#include <ae2fVK/clspv_clkeys.h>
#include "./Slp.auto.h"

#if !(defined(__ae2f_MACRO_GENERATED) && (__ae2f_MACRO_GENERATED))
#include "./Mlp.auto.h"


#undef	ae2f_MAC
#define	ae2f_MAC() inline void

/** Dummy function. Unused. */
void Dummy() {}

#endif

typedef void clGetHidDeltaOne_t(
		ae2f_float_t* const		r_delta,

		const ae2f_float_t* const	i_weight,
		const ae2f_float_t* const	i_delta,

		const size_t			i_iidx,
		const size_t			i_isz,
		const size_t			i_oidx,
		const size_t			i_osz
		);


/** 
 * @brief delta to delta 
 * @inp aka out_then
 * @deltaseed 
 * */
ae2f_MAC() clMlpPropagate(
		ae2f_float_t* const		r_delta_then,

		const size_t			i_oidx,
		const size_t			i_osz,

		ae2f_AnnAct_t			i_actderiv_then,

		const ae2f_float_t* const	i_inp,
		const ae2f_float_t* const	i_deltaseed
		)
{
	if((i_oidx) < (i_osz)) {
		ae2f_float_t v_tmp;
		i_actderiv_then(&v_tmp, (i_inp)[i_oidx]);
		(r_delta_then)[i_oidx] = (v_tmp) * (i_deltaseed)[i_oidx];
	}
}

ae2f_MAC() clGetHidDeltaOne(
		ae2f_float_t* const		r_delta,

		const ae2f_float_t* const	i_weight,
		const ae2f_float_t* const	i_delta,

		const size_t			i_iidx,
		const size_t			i_isz,
		const size_t			i_oidx,
		const size_t			i_osz
		)
{
	if(i_oidx == 0) {
		size_t v_oidx = (i_osz);
		ae2f_float_t	v_ret = 0;

		while((v_oidx)--) {
			(v_ret) += (i_weight)[(i_isz) * (v_oidx) + (i_iidx)];
		}

		(r_delta)[(i_iidx)] = (v_ret);
	}
}

ae2f_MAC() clGetHidDelta_imp(
		clGetHidDeltaOne_t		ONE,
		ae2f_float_t* const		r_delta,

		const ae2f_float_t* const	i_weight,
		const ae2f_float_t* const	i_delta,

		const size_t			i_iidx,
		const size_t			i_isz,
		const size_t			i_oidx,
		const size_t			i_osz
		)
{
	if((i_iidx) < (i_isz) && (i_oidx) < (i_osz))
		ONE(r_delta, i_weight, i_delta, i_iidx, i_isz, i_oidx, i_osz);
}

#define _clGetHidDelta(...) _clGetHidDelta_imp(_clGetHidDeltaOne, __VA_ARGS__)

#endif
