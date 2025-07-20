/** 
 * FIXME
 * */

#ifndef ae2f_Ann_Mlp_h
#define ae2f_Ann_Mlp_h

#include "ae2f/Cast.h"
#include "ae2f/Float.auto.h"
#include <ae2f/Ann/Slp.h>
#include <ae2f/Pack/Beg.h>
#include <ae2f/Macro.h>
#include <assert.h>

ae2f_structdef(struct, ae2f_AnnMlp_t)
{
	size_t	m_depth;

	/** @brief possible greatest output size. */
	size_t	m_outc;
};

ae2f_structdef_n(struct, ae2f_AnnMlpPredict_t, ae2f_AnnMlpPredictStream_t) {
	size_t		m_inc, m_outc, m_i, m_j, m_k, m_depth, m_outc_max;
	ae2f_float_t	m_ret;
};

ae2f_structdef(struct, ae2f_AnnMlpHidDeltaSingle_t) {
	/** 
	 * @brief delta seed, not delta. \n
	 * Use __ae2f_AnnPropagate to make delta.
	 * */
	ae2f_float_t	m_ret;
	size_t		m_outc, m_i;
};

ae2f_structdef(union, ae2f_AnnMlpPropagateAllStack_t) {
	size_t m_send;
};

ae2f_structdef(struct, ae2f_AnnMlpPropagateAll_t) {
	size_t				m_i, m_j, m_inc, m_outc;
	size_t				m_k;
	size_t				m_pg_weight;
	size_t				m_pg_out;
	ae2f_AnnMlpPropagateAllStack_t	m_stack;
	ae2f_float_t			m_ret;
}; 

#include <ae2f/Pack/End.h>

#endif


#if !defined(ae2f_Ann_Mlp_c)
#if !__ae2f_MACRO_GENERATED
#include <ae2f/Ann/Mlp.h>
#endif

#if !__ae2f_MACRO_GENERATED
#define OPER_NEG
#define OPER_NONE
#endif

#define ae2f_Ann_Mlp_c

/** layer must be more than 2 */
ae2f_MAC(OPER_NEG, OPER_NONE, ) _ae2f_AnnMlpPredictPrimal_imp(
		ae2f_AnnMlpPredict_t		v_predict
		, const ae2f_AnnMlp_t		mlp

		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out

		, const size_t* const		sz
		, const ae2f_float_t* const	weight
		, const ae2f_float_t* const	bias
		, ae2f_float_t* const		outcache

		, ae2f_AnnAct_t* const * const	act_opt
		)
{
	assert((mlp).m_depth > 2);

	(v_predict).m_depth = (mlp).m_depth - 1;
	(v_predict).m_outc_max = (mlp).m_outc;

	(v_predict).m_inc = (sz)[0];
	(v_predict).m_outc = (sz)[1];


	if((act_opt)[0]) {
		__ae2f_AnnSlpPredict_imp(
				(v_predict)
				, (v_predict)
				, inp
				, outcache
				, weight
				, bias
				, (act_opt)[0]
				);
	} else {
		__ae2f_AnnSlpPredict_imp(
				(v_predict)
				, (v_predict)
				, inp
				, outcache
				, weight
				, bias
				,
				);
	}


	for(
			(v_predict).m_k = 1; 
			(v_predict).m_k < (v_predict).m_depth - 1; 
			++(v_predict).m_k
	   )
	{
		(v_predict).m_inc = 	(v_predict).m_outc;
		(v_predict).m_outc =	(sz)[(v_predict).m_k + 1];

		if((act_opt)[(v_predict).m_k]) {
			__ae2f_AnnSlpPredict_imp(
					(v_predict)
					, (v_predict)
					, (outcache) + ((((v_predict).m_k OPER_NEG)) * (v_predict).m_outc_max)
					, (outcache) + ((((v_predict).m_k OPER_NONE)) * (v_predict).m_outc_max)
					, (weight) + ((v_predict).m_k)
					* (v_predict).m_outc_max * (v_predict).m_outc_max
					, (bias) + ((v_predict).m_k) * (v_predict).m_outc_max
					, (act_opt)[(v_predict).m_k]
					);
		} else {
			__ae2f_AnnSlpPredict_imp(
					(v_predict)
					, (v_predict)
					, (outcache) + ((((v_predict).m_k OPER_NEG)) * (v_predict).m_outc_max)
					, (outcache) + ((((v_predict).m_k OPER_NONE)) * (v_predict).m_outc_max)
					, (weight) + ((v_predict).m_k) * (v_predict).m_outc_max * (v_predict).m_outc_max
					, (bias) + ((v_predict).m_k) * (v_predict).m_outc_max
					, 
					);
		}
	}

	(v_predict).m_inc = 	(v_predict).m_outc;
	(v_predict).m_outc =	(sz)[(v_predict).m_k + 1];


	if((act_opt)[(v_predict).m_k]) {
		__ae2f_AnnSlpPredict_imp(
				(v_predict)
				, (v_predict)
				, (outcache) + ((((v_predict).m_k OPER_NEG)) * (v_predict).m_outc_max)
				, (out)
				, (weight) + ((v_predict).m_k) * (v_predict).m_outc_max * (v_predict).m_outc_max
				, (bias) + ((v_predict).m_k) * (v_predict).m_outc_max
				, (act_opt)[(v_predict).m_k]
				);
	} else {
		__ae2f_AnnSlpPredict_imp(
				(v_predict)
				, (v_predict)
				, (outcache) + ((((v_predict).m_k OPER_NEG)) * (v_predict).m_outc_max)
				, (out)
				, (weight) + ((v_predict).m_k) * (v_predict).m_outc_max * (v_predict).m_outc_max
				, (bias) + ((v_predict).m_k) * (v_predict).m_outc_max
				, 
				);
	}
}

ae2f_MAC() _ae2f_AnnMlpPredictStream_imp(
		ae2f_AnnMlpPredict_t		v_predict
		, const ae2f_AnnMlp_t		mlp

		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out

		, const size_t* const		sz
		, const ae2f_float_t* const	weight
		, const ae2f_float_t* const	bias
		, ae2f_float_t* const		outcache

		, ae2f_AnnAct_t* const * const	act_opt
		) {
	__ae2f_AnnMlpPredictPrimal_imp(-1 , , v_predict, mlp, inp, out, sz, weight, bias, outcache, act_opt);
}


ae2f_MAC() _ae2f_AnnMlpPredict_imp(
		ae2f_AnnMlpPredict_t		v_predict
		, const ae2f_AnnMlp_t		mlp

		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out

		, const size_t* const		sz
		, const ae2f_float_t* const	weight
		, const ae2f_float_t* const	bias
		, ae2f_float_t* const		outcache

		, ae2f_AnnAct_t* const * const	act_opt
		) {
	__ae2f_AnnMlpPredictPrimal_imp(^ 1 & 1, & 1, v_predict, mlp, inp, out, sz, weight, bias, outcache, act_opt)
}




ae2f_MAC() _ae2f_AnnMlpHidDeltaSingle_imp(
		ae2f_AnnMlpHidDeltaSingle_t	v_single
		, const ae2f_AnnSlp_t		slp
		, const ae2f_float_t* const	weight
		, const ae2f_float_t* const	delta
		, const size_t			oidx_then
		)
{
	(v_single).m_outc = (slp).m_outc;
	(v_single).m_ret = 0;

	for((v_single).m_i = 0; (v_single).m_i < (v_single).m_outc; ++(v_single).m_i)
	{
		(v_single).m_ret += 
			((weight) + (slp).m_inc * (v_single).m_i)[oidx_then] * (delta)[(v_single).m_i];
	}
}

/** @brief delta to delta */
ae2f_MAC() _ae2f_AnnMlpPropagate_imp(
		size_t				v_send
		, const ae2f_AnnSlp_t		slp_then
		, ae2f_float_t* const		retdelta_then

		, const ae2f_float_t* const	deltaseed
		, ae2f_AnnAct_t			actderiv_then

		, const ae2f_float_t* const	inp
		)
{
	for((v_send) = (slp_then).m_outc; (v_send)--;) {
		retdelta_then[v_send] = actderiv_then(inp[v_send]) * (deltaseed)[v_send];
	}
}

ae2f_MAC() _ae2f_AnnMlpPropagateAll_imp(
		ae2f_AnnMlpPropagateAll_t	v_follow
		, const ae2f_AnnMlp_t		mlp

		, const ae2f_float_t* const	inp
		, const ae2f_float_t* const	delta
		, const size_t* const		lenv

		, const ae2f_float_t* const	outstream

		, ae2f_float_t* const		deltacache
		, ae2f_float_t* const		weight
		, ae2f_float_t* const		bias

		, const ae2f_float_t		learningrate
		, const ae2f_float_t		learningrate_bias

		, ae2f_AnnAct_t* const * const	actderiv
		)
{
	assert(((mlp).m_depth > 2) && "m_depth must be more than 1.");
	assert((inp) && "inp is null");
	assert((weight) && "weight is null");
	assert((bias) && "bias is null");
	assert((actderiv) && "actderiv list is null");
	assert((delta) && "delta is null");

	/** m_k: index for then */
	(v_follow).m_k = (mlp).m_depth - 2;

	/** permanent */
	(v_follow).m_pg_weight = (v_follow).m_pg_out = (mlp).m_outc;
	(v_follow).m_pg_weight *= (v_follow).m_pg_out;

	(v_follow).m_outc = (lenv)[(v_follow).m_k + 1];
	(v_follow).m_inc = (lenv)[(v_follow).m_k];

	/** __ae2f_AnnSlpFollow_imp(v_follow, _this, prm_in, delta, weight, bias, learningrate, learningrate_bias) */

	/** 
	 * out field update (delta is specified)
	 * */
	__ae2f_AnnSlpFollow_imp(
			v_follow
			, (v_follow)
			, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
			, (delta)
			, ((weight) + ((v_follow).m_pg_weight * (v_follow).m_k))
			, (bias) + ((v_follow).m_pg_out * ((v_follow).m_k))
			, learningrate
			, learningrate_bias
			);

	(v_follow).m_j = (v_follow).m_inc;
	while((v_follow).m_j--) {
		__ae2f_AnnMlpHidDeltaSingle_imp(
				v_follow
				, v_follow
				, weight
				, delta
				, (v_follow).m_j
				);

		((deltacache) + (v_follow).m_pg_out * ((v_follow).m_k - 1))[(v_follow).m_j]
			= (v_follow).m_ret;
	}

	(v_follow).m_outc = (lenv)[(v_follow).m_k];
	(v_follow).m_inc = (lenv)[(v_follow).m_k - 1];

	/** nxt-delta to then-delta */
	if((actderiv)[(v_follow).m_k]) {
		/** __ae2f_AnnMlpPropagate_imp(v_send, slp_then, retdelta_then, deltaseed, actderiv_then, inp) */
		__ae2f_AnnMlpPropagate_imp(
				(v_follow).m_stack.m_send
				, (v_follow)
				, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k - 1)))
				, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k - 1)))
				, (actderiv)[(v_follow).m_k - 1]
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
				);
	} else {
		__ae2f_AnnMlpPropagate_imp(
				(v_follow).m_stack.m_send
				, (v_follow)
				, ((deltacache) + (((v_follow).m_pg_out) * ((v_follow).m_k - 1)))
				, ((deltacache) + (((v_follow).m_pg_out) * ((v_follow).m_k - 1)))
				, /** actderiv */
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
				);
	}

	while(--(v_follow).m_k > 0) {
		/** 
		 * out field update (delta is specified)
		 * */
		__ae2f_AnnSlpFollow_imp(
				v_follow
				, (v_follow)
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
				, (delta)
				, ((weight) + ((v_follow).m_pg_weight * (v_follow).m_k))
				, (bias) + ((v_follow).m_pg_out * ((v_follow).m_k))
				, learningrate
				, learningrate_bias
				);

		(v_follow).m_j = (v_follow).m_inc;
		while((v_follow).m_j--) {
			__ae2f_AnnMlpHidDeltaSingle_imp(
					v_follow
					, v_follow
					, weight
					, delta
					, (v_follow).m_j
					);

			((deltacache) + (v_follow).m_pg_out * ((v_follow).m_k - 1))[(v_follow).m_j]
				= (v_follow).m_ret;
		}

		(v_follow).m_outc = (lenv)[(v_follow).m_k];
		(v_follow).m_inc = (lenv)[(v_follow).m_k - 1];

		/** nxt-delta to then-delta */
		if((actderiv)[(v_follow).m_k]) {
			/** __ae2f_AnnMlpPropagate_imp(v_send, slp_then, retdelta_then, deltaseed, actderiv_then, inp) */
			__ae2f_AnnMlpPropagate_imp(
					(v_follow).m_stack.m_send
					, (v_follow)
					, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k - 1)))
					, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k - 1)))
					, (actderiv)[(v_follow).m_k - 1]
					, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
					);
		} else {
			__ae2f_AnnMlpPropagate_imp(
					(v_follow).m_stack.m_send
					, (v_follow)
					, ((deltacache) + (((v_follow).m_pg_out) * ((v_follow).m_k - 1)))
					, ((deltacache) + (((v_follow).m_pg_out) * ((v_follow).m_k - 1)))
					, /** actderiv */
					, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
					);
		}
	}

	assert(((v_follow).m_k) == 0 && "(v_follow).m_k must be zero.");
	assert((v_follow).m_inc == (lenv)[0] && "inc must be same as first element of lenv.");
	assert((v_follow).m_outc == (lenv)[1] && "outc must be same as second element of lenv.");

	/** 
	 * out field update (delta is specified)
	 * */
	__ae2f_AnnSlpFollow_imp(
			v_follow
			, (v_follow)
			, (inp)
			, (delta)
			, ((weight) + ((v_follow).m_pg_weight * (v_follow).m_k))
			, (bias) + ((v_follow).m_pg_out * ((v_follow).m_k))
			, learningrate
			, learningrate_bias
			);
}

#endif
