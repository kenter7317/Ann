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

ae2f_MAC(__global, ) clMlpGetHD1(
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
		ae2f_float_t v_tmp = (i_weight)[(i_isz) * (i_oidx) + (i_iidx)] * (i_delta)[i_oidx];

		_clAtomAddF(__global
				, &(r_delta_then)[(i_iidx)]
				, v_tmp
			   );
	}
}

#if !__ae2f_MACRO_GENERATED
#define _clMlpGetHD1(_, a, b, c, d, e, f, ed)	clMlpGetHD1(a, b, c, d, e, f, ed)
#endif

ae2f_MAC(__global, _clMlpGetHD1, ) clMlpGetHD(
		ae2f_float_t* const		r_delta_then,

		const ae2f_float_t* const	i_weight,
		const ae2f_float_t* const	i_delta,

		const size_t			i_iidx,
		const size_t			i_isz,
		const size_t			i_oidx,
		const size_t			i_osz
		)
{
	_clMlpGetHD1(__global, r_delta_then, i_weight, i_delta, i_iidx, i_isz, i_oidx, i_osz);
}

/** @brief GetHidDelta Need no structure. */
#define clMlpGetHD(a, ...)	_clMlpGetHD(a, CL_Q_CVRT(_clMlpGetHD1), __VA_ARGS__)


#endif
