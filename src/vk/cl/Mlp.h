#ifndef Mlp_h
#define Mlp_h

#include "./Slp.auto.h"

typedef void clMlpGetHD1_t(
		ae2f_float_t* const		r_delta,

		const ae2f_float_t* const	i_weight,
		const ae2f_float_t* const	i_delta,

		const size_t			i_iidx,
		const size_t			i_isz,
		const size_t			i_oidx,
		const size_t			i_osz
		);

/** 
 * @brief 
 * delta to delta 
 * Propagate
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
		i_actderiv_then(&(v_tmp), (i_inp)[i_iidx]);
		(r_delta_then)[i_iidx] = (v_tmp) * (i_deltaseed)[i_iidx];
	}
}

ae2f_MAC() clMlpGetHD1_Q(
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
		(r_delta_then)[(i_iidx)]
			= work_group_reduce_add(
					(i_weight)[(i_isz) * (i_oidx) + (i_iidx)] * (i_delta)[i_oidx]
					);
	}
}

ae2f_MAC() clMlpGetHD1(
		ae2f_float_t* const		r_delta_then,

		const ae2f_float_t* const	i_weight,
		const ae2f_float_t* const	i_delta,

		const size_t			i_iidx,
		const size_t			i_isz,
		const size_t			i_oidx,
		const size_t			i_osz
		)
{
	if((i_oidx) == 0 && (i_iidx) < (i_isz)) {
		size_t v_oidx = (i_osz);
		ae2f_float_t	v_ret = 0;

		while((v_oidx)--) {
			(v_ret) += (i_weight)[(i_isz) * (v_oidx) + (i_iidx)] * (i_delta)[(v_oidx)];
		}

		(r_delta_then)[(i_iidx)] = (v_ret);
	}
}

ae2f_MAC() clMlpGetHD(
		clMlpGetHD1_t		ONE,
		ae2f_float_t* const		r_delta_then,

		const ae2f_float_t* const	i_weight,
		const ae2f_float_t* const	i_delta,

		const size_t			i_iidx,
		const size_t			i_isz,
		const size_t			i_oidx,
		const size_t			i_osz
		)
{
	if((i_iidx) < (i_isz) && (i_oidx) < (i_osz)) {
		ONE(r_delta_then, i_weight, i_delta, i_iidx, i_isz, i_oidx, i_osz);
	}
}

/** @brief GetHidDelta Need no structure. */
#define clMlpGetHD(...)	_clMlpGetHD(CL_Q_CVRT(_clMlpGetHD1), __VA_ARGS__)


#endif
