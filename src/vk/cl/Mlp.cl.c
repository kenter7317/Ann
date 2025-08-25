#define ae2f_NEED_CLASS 0
#include <ae2fVK/clspv_clkeys.h>
#include <ae2f/Ann/Slp.h>

#include "./Mlp.auto.h"

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

/** length of `p_layerszlist` */
#define lsz		lsz

/** count of layer. */
#define llsz		(lsz - 1)

#define p_layerszlist	ae2f_reinterpret_cast(__global uint32_t*, glob)
#define p_weight	ae2f_reinterpret_cast(__global ae2f_float_t*, p_layerszlist + lsz)
#define p_bias		(p_weight + pgsz_sqr * (llsz))
#define p_outstream	(p_bias + pgsz * (llsz))
#define p_inp		p_outstream
#define p_deltastream	(p_outstream + (lsz) * pgsz)

#define _r_inp(lidx)	(p_outstream + pgsz * (lidx))
#define _r_out(lidx)	(p_outstream + pgsz * ((lidx) + 1))
#define _r_weight(lidx)	(p_weight + pgsz_sqr * (lidx))
#define _r_bias(lidx)	(p_bias + pgsz * (lidx))
#define _r_delta(lidx)	(p_deltastream + pgsz * (lidx))
#define _r_isz(lidx)	(p_layerszlist)[lidx]
#define _r_osz(lidx)	(p_layerszlist)[(lidx) + 1]

#define r_inp		_r_inp(lidx)
#define r_out		_r_out(lidx+1)
#define r_weight	_r_weight(lidx)
#define r_bias		_r_bias(lidx)
#define r_delta		_r_delta(lidx)
#define r_isz		_r_isz(lidx)
#define r_osz		_r_osz(lidx)

#define r_inp_then	_r_inp(lidx-1)
#define r_out_then	_r_inp(lidx)
#define r_weight_then	_r_weight(lidx-1)
#define r_bias_then	_r_bias(lidx-1)
#define r_delta_then	_r_delta(lidx-1)
#define r_isz_then	_r_isz(lidx-1)
#define r_osz_then	_r_isz(lidx)

#define l_inp(O_R)	(((loc) + pgsz * ((lidx) O_R)))
#define l_out(O_R)	(((loc) + pgsz * ((lidx + 1) O_R)))

#define lp_deltastream	((loc) + pgsz * ((lsz)))
#define l_delta		(lp_deltastream + pgsz * ((lidx) & 1))
#define l_delta_then	(lp_deltastream + pgsz * ((lidx) & 1 ? 0 : 1))

/** For every runners */
#define ACT_RUN(r, x)			ACT(lidx, r, x)
#define ACT_DERIV_RUN(r, x)		ACT_DERIV(lidx, r, x)
#define LOSS_DERIV_RUN(r, x)		LOSS_DERIV(lidx, r, x)

#define ACT_RUN_THEN(r, x)		ACT((lidx - 1), r, x)
#define ACT_DERIV_RUN_THEN(r, x)	ACT_DERIV((lidx - 1), r, x)
#define LOSS_DERIV_RUN_THEN(r, x)	LOSS_DERIV((lidx - 1), r, x)


/**
 * @brief loc
 * ae2f_float_t[Page]:	inp
 * ae2f_float_t[Page]:	out
 * */
__kernel void kPredict(__global void* glob, __local ae2f_float_t* loc, const uint lsz) {
	const size_t
		oidx = get_global_id(0)
		, iidx = get_global_id(1)
		, sz = get_global_size(0);

	size_t	lidx = 0;
	clSlpPredict_t	v_predict;

	_clSlpPredict(v_predict, l_out(&1), r_inp, r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
	barrier(CLK_ALL_MEM_FENCE);

	while(++lidx < llsz - 1) {
		_clSlpPredict(v_predict, l_out(&1), l_inp(&1), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
		barrier(CLK_ALL_MEM_FENCE);
	}
	_clSlpPredict(v_predict, r_out, l_inp(&1), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
}


/**
 * @brief loc
 * ae2f_float_t[Page]:	inp
 * ae2f_float_t[Page]:	out
 * */
__kernel void kPredictStream(__global void* glob, __local ae2f_float_t* loc, const uint lsz) {
	const size_t
		oidx = get_global_id(0)
		, iidx = get_global_id(1)
		, sz = get_global_size(0);

	size_t	lidx = 0;
	clSlpPredict_t	v_predict;

	_clSlpPredict(v_predict, l_out(&1), r_inp, r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
	barrier(CLK_ALL_MEM_FENCE);

	while(++lidx < llsz - 1) {
		_clSlpPredict(v_predict, l_out(&1), l_inp(&1), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
		if(oidx < r_osz && iidx == 0) 
			r_out[oidx] = l_out(&1)[oidx];
		barrier(CLK_ALL_MEM_FENCE);
	}

	_clSlpPredict(v_predict, r_out, l_inp(&1), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
}

const ae2f_structdef(struct, lrlsz_t)
{
	uint32_t	m_lsz;
	ae2f_float_t	m_weight;
	ae2f_float_t	m_bias;
};

/**
 * @brief loc
 * ae2f_float_t[lsz - 1][Page]: OutStream
 * ae2f_float_t[lsz - 1][Page]: DeltaStream
 * */
__kernel void kFollow(__global void* glob, __local ae2f_float_t* loc, const lrlsz_t lr) {
#undef	lsz
#define lsz	lr.m_lsz
	if(lsz < 3) {
		/** ASSERT */
		return;
	}

	size_t		lidx = llsz - 1;
	ae2f_float_t	v_tmp;

	const size_t
		oidx = get_global_id(0)
		, iidx = get_global_id(1)
		, sz = get_global_size(0);

	if(oidx < r_osz && iidx < r_isz) {
		__ae2f_AnnSlpFollowOneW_imp(
				r_inp[iidx]
				, r_delta[oidx]
				, r_weight
				, lr.m_weight
				, r_isz, iidx
				, r_osz, oidx
				);

		if(iidx == 0) {
			__ae2f_AnnSlpFollowOneB_imp(
					r_bias[oidx]
					, r_delta[oidx]
					, lr.m_bias
					);
		}
	}

	clMlpGetHD(
			l_delta_then
			, r_weight_then
			, r_delta
			, iidx, r_isz
			, oidx, r_osz
		  );

	_clMlpRvrse(
			v_tmp
			, l_delta_then
			, oidx, iidx
			, r_isz, ACT_DERIV_RUN_THEN
			, r_inp, l_delta_then
		   );

	/** Needs to be procedural */
	barrier(CLK_ALL_MEM_FENCE);


	while(--lidx) {
		if(oidx < r_osz && iidx < r_isz) {
			__ae2f_AnnSlpFollowOneW_imp(
					l_inp()[iidx]
					, l_delta[oidx]
					, r_weight
					, lr.m_weight
					, r_isz, iidx
					, r_osz, oidx
					);

			if(iidx == 0) {
				__ae2f_AnnSlpFollowOneB_imp(
						r_bias[oidx]
						, l_delta[oidx]
						, lr.m_bias
						);
			}
		}

		clMlpGetHD(
				l_delta_then
				, r_weight_then
				, l_delta
				, iidx, r_isz
				, oidx, r_osz
			  );

		_clMlpRvrse(
				v_tmp
				, l_delta_then
				, oidx, iidx
				, r_isz, ACT_DERIV_RUN_THEN
				, r_inp, l_delta_then
			   );

		/** Needs to be procedural */
		barrier(CLK_ALL_MEM_FENCE);
	}

	if(oidx < r_osz && iidx < r_isz) {
		__ae2f_AnnSlpFollowOneW_imp(
				r_inp[iidx]
				, l_delta[oidx]
				, r_weight
				, lr.m_weight
				, r_isz, iidx
				, r_osz, oidx
				);

		if(iidx == 0) {
			__ae2f_AnnSlpFollowOneB_imp(
					r_bias[oidx]
					, l_delta[oidx]
					, lr.m_bias
					);
		}
	}

#undef lsz
}

/**
 * @brief loc
 * ae2f_float_t[lsz - 1][Page]: OutStream
 * ae2f_float_t[lsz - 1][Page]: DeltaStream
 * */
__kernel void kTrainAuto(__global void* glob, __local ae2f_float_t* loc, const lrlsz_t lr) {
#define lsz	lr.m_lsz
	if(lsz < 3) {
		/** ASSERT */
		return;
	}

	size_t		lidx = 0;
	ae2f_float_t	v_tmp;

	const size_t
		oidx = get_global_id(0)
		, iidx = get_global_id(1)
		, sz = get_global_size(0);

	clSlpPredict_t	v_predict;

	if(iidx < r_isz && oidx == 0) l_inp()[iidx] = r_inp[iidx];
	barrier(CLK_ALL_MEM_FENCE);

	for(; lidx < llsz - 1; lidx++) {
		_clSlpPredict(v_predict, l_out(), l_inp(), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
		barrier(CLK_ALL_MEM_FENCE);
	}

	/** lidx == llsz - 1 */
	_clSlpPredict(v_predict, l_out(), l_inp(), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
	barrier(CLK_ALL_MEM_FENCE);

	++lidx;

	while(--lidx) {
		if(oidx < r_osz && iidx < r_isz) {
			__ae2f_AnnSlpFollowOneW_imp(
					l_inp()[iidx]
					, l_delta[oidx]
					, r_weight
					, lr.m_weight
					, r_isz, iidx
					, r_osz, oidx
					);

			if(iidx == 0) {
				__ae2f_AnnSlpFollowOneB_imp(
						r_bias[oidx]
						, l_delta[oidx]
						, lr.m_bias
						);
			}
		}

		clMlpGetHD(
				l_delta_then
				, r_weight_then
				, l_delta
				, iidx, r_isz
				, oidx, r_osz
			  );

		_clMlpRvrse(
				v_tmp
				, l_delta_then
				, oidx, iidx
				, r_isz, ACT_DERIV_RUN_THEN
				, l_inp(), l_delta_then
			   );

		/** Needs to be procedural */
		barrier(CLK_ALL_MEM_FENCE);
	}

	if(oidx < r_osz && iidx < r_isz) {
		__ae2f_AnnSlpFollowOneW_imp(
				l_inp()[iidx]
				, l_delta[oidx]
				, r_weight
				, lr.m_weight
				, r_isz, iidx
				, r_osz, oidx
				);

		if(iidx == 0) {
			__ae2f_AnnSlpFollowOneB_imp(
					r_bias[oidx]
					, l_delta[oidx]
					, lr.m_bias
					);
		}
	}

#undef lsz
}
