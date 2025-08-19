#include "ae2f/Float.h"
#define ae2f_NEED_CLASS 0

#if defined(__ae2f_MACRO_GENERATED) && __ae2f_MACRO_GENERATED
#define ae2fVK_clspv_IS_OPENCL_C 1
#else
#define barrier(...)
#endif

#include <ae2fVK/clspv_clkeys.h>
#include <ae2f/Ann/Slp.h>

#ifndef ACT
#define ACT(layer_idx, r, x)
#endif

#ifndef ACT_DERIV
#define ACT_DERIV(layer_idx, r, x)
#endif

#ifndef LOSS_DERIV
#define LOSS_DERIV(layer_idx, r, y, y_desired, i, c)
#endif


ae2f_structdef(struct, lr_t) {
	ae2f_float_t	m_weight;
	ae2f_float_t	m_bias;
};

#define pgsz		sz
#define pgsz_sqr	(pgsz * pgsz)

#define p_layerszlist	ae2f_reinterpret_cast(__global uint32_t*, glob)
#define p_weight	ae2f_reinterpret_cast(__global ae2f_float_t*, p_layerszlist + lsz)
#define p_bias		(p_weight + pgsz_sqr * lsz)
#define p_outstream	(p_bias + pgsz * lsz)
#define p_deltastream	(p_outstream + (lsz - 1) * pgsz)

#define r_inp		(p_outstream + pgsz * lidx)
#define r_out		(p_outstream + pgsz * (lidx + 1))
#define r_weight	(p_weight + pgsz_sqr * lidx)
#define r_bias		(p_bias + pgsz * lidx)
#define r_delta		(p_delta + pgsz * lidx)
#define r_isz		(p_layerszlist)[lidx]
#define r_osz		(p_layerszlist)[lidx + 1]

#define lidx_then	(lidx & 1)
#define lidx_now	((lidx) ^ 1 & 1)
#define l_inp		((loc) + pgsz * lidx_then)
#define l_out		((loc) + pgsz * lidx_now)


/** For every runners */
#define ACT_RUN(r, x)		ACT(lidx, r, x)
#define ACT_DERIV_RUN(r, x)	ACT_DERIV(lidx, r, x)
#define LOSS_DERIV_RUN(r, x)	LOSS_DERIV(lidx, r, x)

__kernel void kPredict(__global void* glob, __local ae2f_float_t* loc, const uint lsz) {
	const size_t
		idx = get_global_id(0)
		, sz = get_global_size(0);

	ae2f_AnnSlpPredictOne_t	v_predict;
	size_t	lidx = 0;

	if(idx < r_osz) {
		__ae2f_AnnSlpPredictOne_imp(
				v_predict
				, r_inp
				, r_weight
				, r_bias
				, ACT_RUN
				, idx
				, r_isz
				);

		l_out[idx] = (v_predict).m_ret;
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	while(++lidx < lsz - 1) {
		if(idx < r_osz) {
			__ae2f_AnnSlpPredictOne_imp(
					v_predict
					, l_inp
					, r_weight
					, r_bias
					, ACT_RUN
					, idx
					, r_isz
					);

			l_out[idx] = (v_predict).m_ret;
		}

		barrier(CLK_LOCAL_MEM_FENCE);
	}

	if(idx < r_osz) {
		__ae2f_AnnSlpPredictOne_imp(
				v_predict
				, l_inp
				, r_weight
				, r_bias
				, ACT_RUN
				, idx
				, r_isz
				);

		r_out[idx] = (v_predict).m_ret;
	}
}

__kernel void kPredictStream(__global void* glob, __local ae2f_float_t* loc, uint lsz) {

}
