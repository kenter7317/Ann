#pragma OPENCL EXTENSION cl-fast-relaxed-math : enable
#pragma OPENCL EXTENSION pod-pushconstant : enable

#define ae2f_NEED_CLASS 0

#define host_float_t ae2f_float

#ifdef ae2f_float_t
#define ae2f_Float_h
#include <ae2f/Float.auto.h>
#endif

#if __ae2f_MACRO_GENERATED
#define ae2fVK_clspv_IS_OPENCL 1
#endif

#include "./Mlp.auto.h"

#ifndef ACT
#define ACT(layer_idx, r, y, i, c) *(r) = (y)[i]
#endif

#ifndef ACT_DERIV
#define ACT_DERIV(layer_idx, r, y, i, c) *(r) = 1 
#endif

#ifndef LOSS_DERIV
#define LOSS_DERIV(r, y, y_desired, i, c) *(r) = (y)[i] - (y_desired)[i]
#endif

#define pgsz		sz

#define weightsz	weightsz
#define pgsz_sqr	weightsz

/** length of `p_layerszlist` */
#define lsz		lsz

/** count of layer. */
#define llsz		(lsz - 1)

/** @brief lsz * sizeof(uint32_t) */
#define p_layerszlist	CAST(__global uint32_t*, glob)
/** @brief sizeof(ae2f_float_t) * lsz * pgsz */
#define p_outstream	(CAST(__global host_float_t*, p_layerszlist + lsz))
#define p_inp		p_outstream

/** @brief sizeof(ae2f_float_t) * pgsz_sqr * llsz */
#define p_weight	(p_outstream + lsz * pgsz)

/** @brief sizeof(ae2f_float_t) * pgsz * llsz */
#define p_bias		(p_weight + pgsz_sqr * llsz)

/** @brief sizeof(ae2f_float_t) * pgsz * llsz */
#define p_deltastream	(p_bias + llsz * pgsz)

/** @brief sizeof(ae2f_float_t) * pgsz */
#define p_goal		(p_deltastream + llsz * pgsz)

#define _r_inp(lidx)	(p_outstream + pgsz * (lidx))
#define _r_out(lidx)	(p_outstream + pgsz * ((lidx) + 1))
#define _r_weight(lidx)	(p_weight + pgsz_sqr * (lidx))
#define _r_bias(lidx)	(p_bias + pgsz * (lidx))
#define _r_delta(lidx)	(p_deltastream + pgsz * (lidx))
#define _r_isz(lidx)	((p_layerszlist)[lidx])
#define _r_osz(lidx)	((p_layerszlist)[(lidx) + 1])

#define r_inp		_r_inp(lidx)
#define r_out		_r_out(lidx)
#define r_weight	_r_weight(lidx)
#define r_bias		_r_bias(lidx)
#define r_delta		_r_delta(lidx)
#define r_isz		_r_isz(lidx)
#define r_osz		_r_osz(lidx)

#define r_inp_then	_r_inp(lidx-1)
#define r_out_then	_r_out(lidx-1)
#define r_weight_then	_r_weight(lidx-1)
#define r_bias_then	_r_bias(lidx-1)
#define r_delta_then	_r_delta(lidx-1)
#define r_isz_then	_r_isz(lidx-1)
#define r_osz_then	_r_osz(lidx-1)

#define l_inp(O_R)	(((loc) + pgsz * ((lidx) O_R)))
#define l_out(O_R)	(((loc) + pgsz * ((lidx + 1) O_R)))

#define lp_deltastream	((loc) + pgsz * ((2/* lsz */)))
#define l_delta		(lp_deltastream + pgsz * ((lidx) & 1))
#define l_delta_then	(lp_deltastream + pgsz * (!((lidx) & 1)))
#define l_tmpoutc	(lp_deltastream + (pgsz << 1))

/** For every runners */
#define ACT_RUN(r, y, i, c)			ACT(lidx, r, y, i, c)
#define ACT_DERIV_RUN(r, y, i, c)		ACT_DERIV(lidx, r, y, i, c)

#define ACT_RUN_THEN(r, y, i, c)		ACT((lidx - 1), r, y, i, c)
#define ACT_DERIV_RUN_THEN(r, y, i, c)	ACT_DERIV((lidx - 1), r, y, i, c)

typedef const struct sz2_t {
	uint32_t	m_lsz;
	uint32_t	m_wsz;
} sz2_t;

#undef	lsz
#undef	weightsz
#define lsz		push.m_lsz
#define weightsz	push.m_wsz

/**
 * @brief loc
 * ae2f_float_t[Page]:	inp
 * ae2f_float_t[Page]:	out
 * */
__kernel void kPredict(__global void* glob, __local ae2f_float_t* loc, sz2_t push) {
	const size_t
		oidx = get_global_id(0)
		, iidx = get_global_id(1)
		, sz = get_global_size(0);

	size_t	lidx = 0;
	ae2f_float_t	predict;

	clSlpPredict(predict, l_out(&1), r_inp, r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);

	while(++lidx < llsz - 1) {
		if(iidx < r_isz && !oidx) l_inp(&1)[iidx] = predict;
		clSlpPredict(predict, l_out(&1), l_inp(&1), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
	}

	if(iidx < r_isz && !oidx) l_inp(&1)[iidx] = predict;
	clSlpPredict(r_out[oidx], l_out(&1), l_inp(&1), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
}


/**
 * @brief loc
 * ae2f_float_t[Page]:	inp
 * ae2f_float_t[Page]:	out
 * */
__kernel void kPredictStream(__global void* glob, __local ae2f_float_t* loc, const sz2_t push) {
	const size_t
		oidx = get_global_id(0)
		, iidx = get_global_id(1)
		, sz = get_global_size(0);

	size_t	lidx = 0;
	ae2f_float_t	v_predict;

	clSlpPredict(v_predict, l_out(&1), r_inp, r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
	

	while(++lidx < llsz - 1) {
		if(iidx < r_isz && !oidx) l_inp(&1)[iidx] = v_predict;
		clSlpPredict(v_predict, l_out(&1), l_inp(&1), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);

		if(oidx < r_osz && !iidx)
			r_out[oidx] = v_predict;
	}


	if(iidx < r_isz && !oidx) l_inp(&1)[iidx] = v_predict;
	clSlpPredict(r_out[oidx], l_out(&1), l_inp(&1), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
}

ae2f_structdef(union, lrlszel_t) {
	host_float_t	m_f;
	uint32_t	m_u;
};

const ae2f_structdef(struct, lrlsz_t)
{
	lrlszel_t m_lsz, m_weight, m_bias, m_wsz;
};

typedef char STATIC_ASSERT_LRLSZEL_SZ[
	sizeof(lrlszel_t) == (sizeof(uint32_t) > sizeof(host_float_t) ? sizeof(uint32_t) : sizeof(host_float_t))
	? 1 : -1
];

typedef char STATIC_ASSERT_LRLSZ_SZ[sizeof(lrlsz_t) ==  sizeof(lrlszel_t) * 4 ? 1 : -1];

#undef	lsz
#undef	m_weight
#undef	m_bias
#undef	weightsz
#define lsz		lr.m_lsz.m_u
#define m_weight	m_weight.m_f
#define m_bias		m_bias.m_f
#define weightsz	lr.m_wsz.m_u

/**
 * @brief loc
 * ae2f_float_t[lsz - 1][Page]: OutStream
 * ae2f_float_t[lsz - 1][Page]: DeltaStream
 * */
__kernel void kFollow(__global void* glob, __local ae2f_float_t* loc, lrlsz_t lr) {
	size_t		lidx = llsz - 1;
	ae2f_float_t	v_tmp;

	const size_t
		oidx = get_global_id(0)
		, iidx = get_global_id(1)
		, sz = get_global_size(0);

	if(lsz < 3) {
		/** ASSERT */
		return;
	}

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
	/* barrier(CLK_ALL_MEM_FENCE); */


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
		/** barrier(CLK_LOCAL_MEM_FENCE); */
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
}

/**
 * @brief loc
 * ae2f_float_t[lsz - 1][Page]: OutStream
 * ae2f_float_t[lsz - 1][Page]: DeltaStream
 * */
__kernel void kTrainAuto(__global void* glob, __local ae2f_float_t* loc, lrlsz_t lr) {
	size_t		lidx = 0;
	const size_t
		oidx = get_global_id(0)
		, iidx = get_global_id(1)
		, sz = get_global_size(0);

	ae2f_float_t	tmp0;
	ae2f_float_t	tmp1;
	ae2f_float_t	tmp2;

	if(lsz < 3) {
		return;
	}

	if(iidx < r_isz && oidx == 0)
		l_inp()[iidx] = r_inp[iidx];

	for(; lidx < llsz - 1; lidx++) {
		clSlpPredict(l_out()[oidx], l_out(), l_inp(), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);
	}

	/** lidx == llsz - 1 */
	clSlpPredict(tmp2, l_out(), l_inp(), r_weight, r_bias, iidx, r_isz, oidx, r_osz, ACT_RUN);

	if(oidx < r_osz && iidx == 0) {
		r_out[oidx] = tmp2;
		l_out()[oidx] = tmp2;

		__ae2f_AnnSlpFetchDeltaOne_imp(
				tmp0
				, &tmp2, &tmp1
				, l_out()
				, p_goal
				, oidx, r_osz
				, ACT_DERIV_RUN
				, LOSS_DERIV 
				);
	}

	/** lidx == llsz */
	++lidx;

	/** lidx == llsz - 1 */
	while(--lidx) {
		if(oidx < r_osz && iidx < r_isz) {
			__ae2f_AnnSlpFollowOneW_imp(
					l_inp()[iidx]
					, tmp0
					, r_weight
					, lr.m_weight
					, r_isz, iidx
					, r_osz, oidx
					);
			unless(iidx) {
				__ae2f_AnnSlpFollowOneB_imp(
						r_bias[oidx]
						, tmp0
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
				tmp0
				, l_delta_then
				, oidx, iidx
				, r_isz, ACT_DERIV_RUN_THEN
				, l_inp(), l_delta_then
			   );
		tmp0 = l_delta_then[iidx];
		/** Needs to be procedural */
	}

	/** lidx == 0 */
	if(oidx < r_osz && iidx < r_isz) {
		__ae2f_AnnSlpFollowOneW_imp(
				l_inp()[iidx]
				, tmp0
				, r_weight
				, lr.m_weight
				, r_isz, iidx
				, r_osz, oidx
				);

		unless(iidx) {
			__ae2f_AnnSlpFollowOneB_imp(
					r_bias[oidx]
					, tmp0
					, lr.m_bias
					);
		}
	}
}
