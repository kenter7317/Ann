#include <ae2f/Ann/Util.h>

#if !ae2f_MAC_BUILD || !__ae2f_MACRO_GENERATED
#include <assert.h>
#include <stdlib.h>
#endif

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Macro.h>
#include <ae2f/Ann/Slp.h>
#endif

#ifndef	ae2f_Ann_Mlp_c

#if !__ae2f_MACRO_GENERATED
#include <ae2f/Ann/Mlp.h>
#endif

#if !__ae2f_MACRO_GENERATED
#define OPER_NEG
#define OPER_NONE
#endif

#define ae2f_Ann_Mlp_c

#if	ae2f_NEED_CLASS

#define __ae2f_AnnMlpDel_C(a) free(ae2f_reinterpret_cast(void*, a))

ae2f_MAC() _ae2f_AnnMlpMk_C(
		ae2f_err_t* const			reterr
		, ae2f_AnnMlp** const			retmk

		, const size_t				depth
		, const size_t* const			szvector
		, ae2f_opt size_t* const		szswap_opt

		, ae2f_opt ae2f_AnnAct_t** const	act
		, ae2f_opt ae2f_AnnAct_t** const	actderiv
		, ae2f_AnnLoss_t* const			lossderiv

		, ae2f_opt ae2f_float_t* const	deltastream
		, ae2f_opt ae2f_float_t* const	outcache
		, ae2f_opt ae2f_float_t* const	weight
		, ae2f_opt ae2f_float_t* const	bias

		, ae2f_float_t const	learningrate
		, ae2f_float_t const	learningrate_bias

		, ae2f_opt const size_t	offset
, ae2f_opt const size_t	extra
) {
	if((reterr) && *(reterr)) {}
	else unless((szvector) && (lossderiv) && (retmk))
		(reterr) && (*(reterr) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		ae2f_AnnMlpMk_t v_mk;
		__ae2f_AnnMlpMk_imp(
				v_mk
				, depth, szvector, szswap_opt
				, act, actderiv, lossderiv
				, deltastream, outcache, weight
				, bias, learningrate, learningrate_bias
				, offset, extra
				);

		assert(v_mk.m_mkbase && "Initialising has failed");
		*(retmk) = v_mk.m_mkbase;
		unless(v_mk.m_mkbase) {
			(reterr) && (*(reterr) |= ae2f_errGlob_ALLOC_FAILED);
		}
	}
}

#else

#define	__ae2f_AnnMlpDel_C(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#define	__ae2f_AnnMlpMk_C(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]


#endif

#if	ae2f_NEED_CLASS

ae2f_MAC() _ae2f_AnnMlpMk_imp(
		ae2f_AnnMlpMk_t			v_mk
		, const size_t			depth

		, const size_t* const		szvector
		, size_t*	const		szswap_opt

		, ae2f_opt ae2f_AnnAct_t** const	act
		, ae2f_opt ae2f_AnnAct_t** const	actderiv
		, ae2f_AnnLoss_t* const			lossderiv

		, ae2f_opt ae2f_float_t* const	deltastream
		, ae2f_opt ae2f_float_t* const	outcache
		, ae2f_opt ae2f_float_t* const	weight
		, ae2f_opt ae2f_float_t* const	bias

		, ae2f_float_t const learningrate
		, ae2f_float_t const learningrate_bias

		, ae2f_opt const size_t		offset
		, ae2f_opt const size_t		extra
)
{
	assert((szvector) && "Size vector is null");
	assert((lossderiv) && "Loss deriv must be initialised");
	assert((depth) > 2 && "Depth must be greater than 2");

	(v_mk).m_outc = 2;
	for((v_mk).m_i = (depth); (v_mk).m_i--; ) {
		assert((szvector)[(v_mk).m_i] && "Zero value is permitted");
		(v_mk).m_outc < (szvector)[(v_mk).m_i] && ((v_mk).m_outc = (szvector)[(v_mk).m_i]);
	}

	__ae2f_AnnMlpSz_imp(
			(v_mk).m_i
			, (v_mk).m_outc
			, depth, szswap_opt
			, act, actderiv
			, deltastream, outcache
			, weight, bias
			);

	(v_mk).m_mkptr.m_void = calloc(1, (v_mk).m_i + (offset) + (extra));
	(v_mk).m_mkbase = (v_mk).m_mkptr.m_mlp;

	if((v_mk).m_mkptr.m_void) {
		__ae2f_AnnMlpInitWithOutSz_imp(
				*(v_mk).m_mkptr.m_mlp
				, (v_mk).m_i
				, depth
				, (v_mk).m_outc
				, szvector
				, (szswap_opt) ? (szswap_opt) : ae2f_reinterpret_cast(
					size_t*
					, (v_mk).m_mkptr.m_mlp + 1)
				, act
				, actderiv
				, lossderiv
				, deltastream
				, outcache
				, weight
				, bias
				, learningrate
				, learningrate_bias
				);

		(v_mk).m_mkptr.m_mlp += 1;
		(v_mk).m_mkptr.m_byte += (offset);
		(v_mk).m_mkptr.m_sz += (depth);

		unless(act) {
			(v_mk).m_mkbase->m_act = (v_mk).m_mkptr.m_Act;
			(v_mk).m_mkptr.m_Act += (depth) - 1;
		}

		unless(actderiv) {
			(v_mk).m_mkbase->m_actderiv = (v_mk).m_mkptr.m_Act;
			(v_mk).m_mkptr.m_Act += (depth) - 1;
		}

		unless(deltastream) {
			(v_mk).m_mkbase->m_deltastream = (v_mk).m_mkptr.m_f;
			(v_mk).m_mkptr.m_f += ((depth) - 1) * (v_mk).m_outc;
		}

		unless(outcache) {
			(v_mk).m_mkbase->m_outcache = (v_mk).m_mkptr.m_f;
			(v_mk).m_mkptr.m_f += ((depth) - 1) * (v_mk).m_outc;
		}

		unless(bias) {
			(v_mk).m_mkbase->m_bias = (v_mk).m_mkptr.m_f;
			(v_mk).m_mkptr.m_f += ((depth) - 1) * (v_mk).m_outc;
		}

		unless(weight) {
			(v_mk).m_mkbase->m_weight = (v_mk).m_mkptr.m_f;
		}
	}
}

ae2f_MAC() _ae2f_AnnMlpSz_imp(
		size_t			ret_sz
		, const size_t		outc
		, const size_t		depth
		, ae2f_opt const size_t* const		szswap
		, ae2f_opt ae2f_AnnAct_t** const	act
		, ae2f_opt ae2f_AnnAct_t** const	actderiv
		, ae2f_opt ae2f_float_t* const		deltastream
		, ae2f_opt ae2f_float_t* const		outcache
		, ae2f_opt ae2f_float_t* const		weight
		, ae2f_opt ae2f_float_t* const		bias
		)
{
	assert((depth) > 2);

	(ret_sz) = sizeof(ae2f_AnnMlp) + (!(szswap)) * sizeof(size_t);
	(ret_sz) += (sizeof(void*) * ((depth) - 1)) * (!(act) + !(actderiv));
	(ret_sz) += sizeof(ae2f_float_t)
		* ((depth)) * (outc)
		* (/**/
				!(deltastream) + !(outcache) + !(bias)
				+ ((!(weight)) * (outc))
		  );
}

ae2f_MAC() _ae2f_AnnMlpInitWithOutSz_imp(
		ae2f_AnnMlp			v_mlp
		, size_t			v_init
		, const size_t			depth
		, const size_t			outsz

		, const size_t* const		szvector
		, size_t*	const		szswap_opt

		, ae2f_opt ae2f_AnnAct_t** const	act
		, ae2f_opt ae2f_AnnAct_t** const	actderiv
		, ae2f_AnnLoss_t* const			lossderiv

		, ae2f_opt ae2f_float_t* const	deltastream
		, ae2f_opt ae2f_float_t* const	outcache
		, ae2f_opt ae2f_float_t* const	weight
		, ae2f_opt ae2f_float_t* const	bias

		, ae2f_float_t const learningrate
		, ae2f_float_t const learningrate_bias
		)
{
	assert((depth) >= 2 && "At lest you need input and output layer");
	(v_mlp).m_depth = (depth);
	(v_mlp).m_outc = (outsz);

	assert((lossderiv) && "loss deriv is null");
	(v_mlp).m_lossderiv = lossderiv;

	(v_mlp).m_sz = (szswap_opt);
	(v_mlp).m_act = (act);
	(v_mlp).m_actderiv = (actderiv);

	(v_mlp).m_deltastream = deltastream;
	(v_mlp).m_outcache = outcache;
	(v_mlp).m_weight = weight;
	(v_mlp).m_bias = bias;

	(v_mlp).m_learningrate = learningrate;
	(v_mlp).m_learningrate_bias = learningrate_bias;

	if((szswap_opt) && (szswap_opt) != (szvector))
		for((v_init) = (depth); (v_init)--; ) {
			(szswap_opt)[(v_init)] = (szvector)[(v_init)];
		}
}

ae2f_MAC() _ae2f_AnnMlpInit_imp(
		ae2f_AnnMlp			v_mlp
		, ae2f_AnnMlpInit_t		v_init
		, const size_t			depth
		, const size_t* const		szvector
		, size_t*	const		szswap_opt

		, ae2f_opt ae2f_AnnAct_t** const	act
		, ae2f_opt ae2f_AnnAct_t** const	actderiv
		, ae2f_AnnLoss_t* const			lossderiv

		, ae2f_opt ae2f_float_t* const	deltastream
		, ae2f_opt ae2f_float_t* const	outcache
		, ae2f_opt ae2f_float_t* const	weight
		, ae2f_opt ae2f_float_t* const	bias

		, ae2f_float_t const learningrate
		, ae2f_float_t const learningrate_bias
		)
{
	(v_init).m_outc = 0;

	assert((szvector) && "Size vector is null");
	for((v_init).m_i = (depth); (v_init).m_i--; ) {
		assert((szvector)[(v_init).m_i] && "Zero value is permitted");
		(v_init).m_outc < (szvector)[(v_init).m_i] && ((v_init).m_outc = (szvector)[(v_init).m_i]);
	}

	__ae2f_AnnMlpInitWithOutSz_imp(
			v_mlp, (v_init).m_i, depth, (v_mlp).m_outc
			, szvector, szswap_opt, act, actderiv, lossderiv
			, deltastream, outcache, weight, bias, learningrate, learningrate_bias
			);
}

#else

#define __ae2f_AnnMlpMk_imp(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#define __ae2f_AnnMlpSz_imp(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#define __ae2f_AnnMlpInitWithOutSz_imp(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#define __ae2f_AnnMlpInit_imp(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#endif

/** @brief layer must be more than 2 */
ae2f_MAC(OPER_NEG, OPER_NONE, ) _ae2f_AnnMlpPredictPrimal_imp(
		ae2f_AnnMlpPredict_t			v_predict
		, const ae2f_AnnMlp_t			mlp

		, const ae2f_float_t* const		inp
		, ae2f_float_t* const			out

		, const size_t* const			sz
		, const ae2f_float_t* const		weight
		, const ae2f_float_t* const		bias
		, ae2f_float_t* const			outcache

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
				, ((outcache) + (0 OPER_NONE) * (v_predict).m_outc_max)
				, weight
				, bias
				, (act_opt)[0]
				);
	} else {
		__ae2f_AnnSlpPredict_imp(
				(v_predict)
				, (v_predict)
				, inp
				, (outcache + (0 OPER_NONE) * (v_predict).m_outc_max)
				, weight
				, bias
				,
				);
	}


	for(
			(v_predict).m_k = 1; 
			(v_predict).m_k < (v_predict).m_depth - 1; 
			(v_predict).m_k++
	   )
	{
		(v_predict).m_inc =	(v_predict).m_outc;
		(v_predict).m_outc =	(sz)[(v_predict).m_k + 1];

		if((act_opt)[(v_predict).m_k]) {
			assert((v_predict).m_k);
			assert(((v_predict).m_k OPER_NEG) != ((v_predict).m_k OPER_NONE));
			assert(((v_predict).m_k OPER_NEG) == (((v_predict).m_k - 1) OPER_NONE));
			assert((((v_predict).m_k + 1) OPER_NEG) == (((v_predict).m_k) OPER_NONE));

			__ae2f_AnnSlpPredict_imp(
					(v_predict)
					, (v_predict)
					, ((outcache) + (((v_predict).m_k OPER_NEG) * (v_predict).m_outc_max))
					, ((outcache) + (((v_predict).m_k OPER_NONE) * (v_predict).m_outc_max))
					, (weight) + ((v_predict).m_k) * (
						(v_predict).m_outc_max
						* (v_predict).m_outc_max
						)
					, (bias) + ((v_predict).m_k) * (v_predict).m_outc_max
					, (act_opt)[(v_predict).m_k]
					);
		} else {
			__ae2f_AnnSlpPredict_imp(
					(v_predict)
					, (v_predict)
					, ((outcache) + (
							((v_predict).m_k OPER_NEG)
							* (v_predict).m_outc_max
							))
					, ((outcache) + (
							((v_predict).m_k OPER_NONE)
							* (v_predict).m_outc_max
							))
					, (weight) 
					+ (((v_predict).m_k) 
						* (v_predict).m_outc_max 
						* (v_predict).m_outc_max)
					, (bias) + ((v_predict).m_k) * (v_predict).m_outc_max
					,
					);
		}
	}

	(v_predict).m_inc =	(v_predict).m_outc;
	(v_predict).m_outc =	(sz)[(v_predict).m_k + 1];
	assert((v_predict).m_k == (mlp).m_depth - 2);

	if((act_opt)[(v_predict).m_k]) {
		__ae2f_AnnSlpPredict_imp(
				(v_predict)
				, (v_predict)
				, ((outcache) + ((((v_predict).m_k OPER_NEG)) * (v_predict).m_outc_max))
				, (out)
				, (weight) + ((v_predict).m_k) * (v_predict).m_outc_max * (v_predict).m_outc_max
				, (bias) + ((v_predict).m_k) * (v_predict).m_outc_max
				, (act_opt)[(v_predict).m_k]
				);
	} else {
		__ae2f_AnnSlpPredict_imp(
				(v_predict)
				, (v_predict)
				, ((outcache) + ((((v_predict).m_k OPER_NEG)) * (v_predict).m_outc_max))
				, (out)
				, ((weight) + ((v_predict).m_k) * (v_predict).m_outc_max * (v_predict).m_outc_max)
				, ((bias) + ((v_predict).m_k) * (v_predict).m_outc_max)
				,
				);
	}
}

#if	ae2f_NEED_CLASS
ae2f_MAC(OPER_NEG, OPER_NONE, ) _ae2f_AnnMlpPredictPrimal(
		ae2f_err_t*			reterr
		, const ae2f_AnnMlp* const	mlp
		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out
		) 
{
	if((reterr) && *(reterr))
		;
	else unless((mlp) && (inp) && (out)) {
		assert(0 && "Null");
		(reterr) && (*(reterr) |= ae2f_errGlob_PTR_IS_NULL);
	} else {
		ae2f_AnnMlpPredict_t v_predict;

		__ae2f_AnnMlpPredictPrimal_imp(
				OPER_NEG, OPER_NONE
				, v_predict, *(mlp)
				, inp, out
				, (mlp)->m_sz, (mlp)->m_weight
				, (mlp)->m_bias, (mlp)->m_outcache
				, (mlp)->m_act
				);
	}
}
#else

#define __ae2f_AnnMlpPredictPrimal(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#endif


#define	__ae2f_AnnMlpPredictStream_imp(...) \
	__ae2f_AnnMlpPredictPrimal_imp(-1, , __VA_ARGS__)

#define __ae2f_AnnMlpPredictStream_C(reterr, mlp, inp, out) \
	__ae2f_AnnMlpPredictPrimal(-1, , reterr, mlp, inp, out)

#define __ae2f_AnnMlpPredict_imp(...) \
	__ae2f_AnnMlpPredictPrimal_imp(&1 ? 0 : 1, &1, __VA_ARGS__)

#define	__ae2f_AnnMlpPredict_C(...) \
	__ae2f_AnnMlpPredictPrimal(&1 ? 0 : 1, &1, __VA_ARGS__)

ae2f_MAC() _ae2f_AnnMlpHidDeltaSingle_imp(
		ae2f_AnnMlpHidDeltaSingle_t	v_single
		, const ae2f_AnnSlp_t		slp
		, const ae2f_float_t* const	weight
		, const ae2f_float_t* const	delta
		, const size_t			iidx
		)
{
	(v_single).m_ret = 0;

	for((v_single).m_i = (slp).m_outc; (v_single).m_i--; )
	{
		(v_single).m_ret +=
			((weight) + (slp).m_inc * (v_single).m_i)[iidx] * (delta)[(v_single).m_i];
	}
}

/** @brief delta to delta */
ae2f_MAC() _ae2f_AnnMlpPropagate_imp(
		ae2f_float_t			v_tmp,
		size_t				v_send
		, const ae2f_AnnSlp_t		slp_then
		, ae2f_float_t* const		retdelta_then

		, const ae2f_float_t* const		deltaseed
		, ae2f_AnnAct_t			actderiv_then

		, const ae2f_float_t* const		inp
		)
{
	for((v_send) = (slp_then).m_outc; (v_send)--;) {
		actderiv_then(&(v_tmp), (inp)[v_send]);
		(retdelta_then)[v_send] = (v_tmp) * (deltaseed)[v_send];
	}
}

#define __ae2f_AnnMlpFollowStream_imp(...) \
	__ae2f_AnnMlpFollowPrimal_imp(-1,ae2f_NONE,__VA_ARGS__)


#define	__ae2f_AnnMlpFollow_imp(...) \
	__ae2f_AnnMlpFollowPrimal_imp(&1 ? 0 : 1,&1, __VA_ARGS__)

ae2f_MAC(OPER_NEG, OPER_NONE,) _ae2f_AnnMlpFollowPrimal_imp(
		ae2f_AnnMlpFollow_t			v_follow
		, const ae2f_AnnMlp_t			mlp

		, const ae2f_float_t* const		inp
		, const ae2f_float_t* const		delta
		, const size_t* const			lenv

		, const ae2f_float_t* const		outstream

		, ae2f_float_t* const			deltacache
		, ae2f_float_t* const			weight
		, ae2f_float_t* const			bias

		, const ae2f_float_t			learningrate
		, const ae2f_float_t			learningrate_bias

		, ae2f_AnnAct_t* const * const	actderiv
		)
{
	assert(((mlp).m_depth > 2) && "m_depth must be more than 1.");
	assert((inp) && "inp is null");
	assert((weight) && "weight is null");
	assert((bias) && "bias is null");
	assert((actderiv) && "actderiv list is null");
	assert((delta) && "delta is null");
	assert(lenv);

	/** m_k: index for then */
	(v_follow).m_k = (mlp).m_depth - 2;

	/** permanent */
	(v_follow).m_pg_weight = (v_follow).m_pg_out = (mlp).m_outc;
	(v_follow).m_pg_weight *= (v_follow).m_pg_out;

	(v_follow).m_outc = (lenv)[(v_follow).m_k + 1];
	(v_follow).m_inc = (lenv)[(v_follow).m_k];

	/** 
	 * out field update (delta is specified)
	 * */
	__ae2f_AnnSlpFollow_imp(
			v_follow
			, (v_follow)
			, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
			, (delta)
			, ((weight) + ((v_follow).m_pg_weight * ((v_follow).m_k)))
			, ((bias) + ((v_follow).m_pg_out * ((v_follow).m_k)))
			, learningrate
			, learningrate_bias
			);

	(v_follow).m_j = (v_follow).m_inc;
	while((v_follow).m_j--) {
		__ae2f_AnnMlpHidDeltaSingle_imp(
				v_follow
				, v_follow
				, ((weight) + ((v_follow).m_pg_weight * (v_follow).m_k))
				, (delta)
				, (v_follow).m_j
				);

		((deltacache) + (v_follow).m_pg_out * ((v_follow).m_k OPER_NEG))[(v_follow).m_j]
			= (v_follow).m_ret;
	}

	(v_follow).m_outc = (lenv)[(v_follow).m_k];
	(v_follow).m_inc = (lenv)[(v_follow).m_k - 1];

	/** nxt-delta to then-delta */
	if((actderiv)[(v_follow).m_k - 1]) {
		__ae2f_AnnMlpPropagate_imp(
				(v_follow).m_ret
				, (v_follow).m_stack.m_send
				, (v_follow)
				, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k OPER_NEG)))
				, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k OPER_NEG)))
				, (actderiv)[(v_follow).m_k - 1]
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1)) /** input of (v_follow).m_k */
				);
	} else {
		__ae2f_AnnMlpPropagate_imp(
				(v_follow).m_ret
				, (v_follow).m_stack.m_send
				, (v_follow)
				, ((deltacache) + (((v_follow).m_pg_out) * ((v_follow).m_k OPER_NEG)))
				, ((deltacache) + (((v_follow).m_pg_out) * ((v_follow).m_k OPER_NEG)))
				, /** actderiv */
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
				);
	}


	while(--(v_follow).m_k) {
		assert((v_follow).m_k);
		assert(((v_follow).m_k OPER_NEG) != ((v_follow).m_k OPER_NONE));
		assert(((v_follow).m_k OPER_NEG) == (((v_follow).m_k - 1) OPER_NONE));
		assert((((v_follow).m_k + 1) OPER_NEG) == (((v_follow).m_k) OPER_NONE));

		/** 
		 * out field update (delta is generated)
		 * */
		__ae2f_AnnSlpFollow_imp(
				(v_follow)
				, (v_follow)
				, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
				, ((deltacache) + (v_follow).m_pg_out * ((v_follow).m_k OPER_NONE))
				, ((weight) + ((v_follow).m_pg_weight * (v_follow).m_k))
				, ((bias) + ((v_follow).m_pg_out * ((v_follow).m_k)))
				, learningrate
				, learningrate_bias
				);

		(v_follow).m_j = (v_follow).m_inc;
		assert((v_follow).m_inc == (lenv)[(v_follow).m_k]);

		while((v_follow).m_j--) {
			(v_follow).m_ret = 0;

			__ae2f_AnnMlpHidDeltaSingle_imp(
					v_follow
					, v_follow
					, ((weight) + (v_follow).m_pg_weight * ((v_follow).m_k))
					, ((deltacache) + (v_follow).m_pg_out * ((v_follow).m_k OPER_NONE))
					, (v_follow).m_j
					);

			((deltacache) + (v_follow).m_pg_out * ((v_follow).m_k OPER_NEG))[(v_follow).m_j]
				= (v_follow).m_ret;
		}

		(v_follow).m_outc = (v_follow).m_inc;
		(v_follow).m_inc = (lenv)[(v_follow).m_k - 1];
		assert((v_follow).m_outc == (lenv)[(v_follow).m_k]);

		/** nxt-delta to then-delta */
		if((actderiv)[(v_follow).m_k - 1]) {
			__ae2f_AnnMlpPropagate_imp(
					(v_follow).m_ret
					, (v_follow).m_stack.m_send
					, (v_follow)
					, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k OPER_NEG)))
					, ((deltacache) + ((v_follow).m_pg_out * ((v_follow).m_k OPER_NEG)))
					, (actderiv)[(v_follow).m_k - 1]
					, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
					);
		} else {
			__ae2f_AnnMlpPropagate_imp(
					(v_follow).m_ret
					, (v_follow).m_stack.m_send
					, (v_follow)
					, ((deltacache) + (((v_follow).m_pg_out) * ((v_follow).m_k OPER_NEG)))
					, ((deltacache) + (((v_follow).m_pg_out) * ((v_follow).m_k OPER_NEG)))
					, /** actderiv */
					, ((outstream) + (v_follow).m_pg_out * ((v_follow).m_k - 1))
					);
		}
	}

	assert(((v_follow).m_k) == 0 && "(v_follow).m_k must be zero.");
	assert((v_follow).m_inc == (lenv)[0] && "inc must be same as first element of lenv.");
	assert((v_follow).m_outc == (lenv)[1] && "outc must be same as second element of lenv.");

	__ae2f_AnnSlpFollow_imp(
			v_follow
			, (v_follow)
			, (inp)
			, ((deltacache) + (v_follow).m_pg_out * ((v_follow).m_k OPER_NONE))
			, ((weight))
			, (bias)
			, learningrate
			, learningrate_bias
			);
}

#if ae2f_NEED_CLASS
ae2f_MAC(OPER_NEG, OPER_NONE,) _ae2f_AnnMlpFollowPrimal(
		ae2f_err_t* const		reterr
		, const ae2f_AnnMlp*			mlp

		, const ae2f_float_t* const	inp
		, const ae2f_float_t* const	delta
		) 
{
	if((reterr) && *(reterr)) {}
	else unless((mlp) && (inp) && (delta)) {
		assert(0 && "nullref");
		(reterr) && (*(reterr) |= ae2f_errGlob_PTR_IS_NULL);
	} else {
		ae2f_AnnMlpFollow_t v_follow;

		__ae2f_AnnMlpFollowPrimal_imp(
				OPER_NEG, OPER_NONE
				, v_follow
				, *(mlp), inp, delta, (mlp)->m_sz
				, (mlp)->m_outcache, (mlp)->m_deltastream
				, (mlp)->m_weight
				, (mlp)->m_bias
				, (mlp)->m_learningrate, (mlp)->m_learningrate_bias
				, (mlp)->m_actderiv
				);
	}
}

#else

#define	__ae2f_AnnMlpFollowPrimal(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#endif

#define	__ae2f_AnnMlpFollow_C(...) \
	__ae2f_AnnMlpFollowPrimal(&1 ? 0 : 1,&1, __VA_ARGS__)

#define	__ae2f_AnnMlpFollowStream_C(...) \
	__ae2f_AnnMlpFollowPrimal(-1, , __VA_ARGS__)

ae2f_MAC(OPER_NEG, OPER_NONE, ) _ae2f_AnnMlpTrainPrimal_imp(
		ae2f_AnnMlpTrain_t			v_train
		, const ae2f_AnnMlp_t			mlp

		, const ae2f_float_t* const		inp
		, ae2f_float_t* const			out
		, const ae2f_float_t* const		out_desired
		, const size_t* const			lenv

		, ae2f_float_t* const			outstream

		, ae2f_float_t* const			deltacache
		, ae2f_float_t* const			weight
		, ae2f_float_t* const			bias

		, const ae2f_float_t			learningrate
		, const ae2f_float_t			learningrate_bias

		, ae2f_opt ae2f_AnnAct_t* const * const	act
		, ae2f_opt ae2f_AnnAct_t* const * const	actderiv
		, ae2f_AnnLoss_t*			lossderiv
)
{
	assert((lenv) && "lengh vector nil");
	assert(((mlp).m_depth > 2) && "I see no hidden layer");
	assert(lossderiv);
	assert((actderiv) && "actderiv list");

	(v_train).m_inc = (lenv)[(mlp).m_depth - 3];
	(v_train).m_outc = (lenv)[(mlp).m_depth - 2];

	__ae2f_AnnMlpPredictStream_imp(
			(v_train), mlp, inp, out, lenv,
			weight, bias, outstream, act
			);

	if((actderiv)[(mlp).m_depth - 2]) {
		__ae2f_AnnSlpFetchDelta_imp(
				(v_train), (v_train), (out), (out_desired)
				, (actderiv)[(mlp).m_depth - 2]
				, (lossderiv)
				, (&((deltacache)[(mlp).m_outc * (((mlp).m_depth - 2) OPER_NONE)]))
				);
	} else {
		__ae2f_AnnSlpFetchDelta_imp(
				(v_train), (v_train), (out), (out_desired)
				, /** actderiv */
				, (lossderiv)
				, (&((deltacache)[(mlp).m_outc * (((mlp).m_depth - 2) OPER_NONE)]))
				);

	}

	__ae2f_AnnMlpFollowPrimal_imp(
			OPER_NEG, OPER_NONE
			, v_train, mlp, inp
			, (&((deltacache)[(mlp).m_outc * (((mlp).m_depth - 2) OPER_NONE)]))
			, lenv, outstream
			, deltacache, weight, bias
			, learningrate, learningrate_bias
			, actderiv
			);
}

#if ae2f_NEED_CLASS
ae2f_MAC(OPER_NEG, OPER_NONE, ) _ae2f_AnnMlpTrainPrimal(
		ae2f_err_t* const ae2f_opt	reterr
		, ae2f_AnnMlp* const		mlp
		, const ae2f_float_t* const	inp
		, ae2f_float_t* const		out
		, const ae2f_float_t* const	out_desired
		)
{
	if((reterr) && *(reterr));
	else unless((mlp) && (out) && (out_desired) && (inp)) {
		assert(0 && "nullref");
		(reterr) && (*(reterr) |= ae2f_errGlob_PTR_IS_NULL);
	} else {
		ae2f_AnnMlpTrain_t v_train;
		__ae2f_AnnMlpTrainPrimal_imp(
				OPER_NEG, OPER_NONE
				, v_train, *(mlp), inp
				, out, out_desired
				, (mlp)->m_sz, (mlp)->m_outcache
				, (mlp)->m_deltastream
				, (mlp)->m_weight, (mlp)->m_bias
				, (mlp)->m_learningrate, (mlp)->m_learningrate_bias
				, (mlp)->m_act, (mlp)->m_actderiv, (mlp)->m_lossderiv
				);
	}
}

ae2f_MAC(OPER_NEG, OPER_NONE, ) _ae2f_AnnMlpTrainAutoPrimal(
		ae2f_err_t* const ae2f_opt	reterr
		, ae2f_AnnMlp* const		mlp
		, const ae2f_float_t* const	inp
		, const ae2f_float_t* const	out_desired
		)
{
	if((reterr) && *(reterr));
	else unless((mlp) && (out_desired) && (inp)) {
		assert(0 && "nullref");
		(reterr) && (*(reterr) |= ae2f_errGlob_PTR_IS_NULL);
	} else {
		ae2f_AnnMlpTrain_t v_train;
		__ae2f_AnnMlpTrainPrimal_imp(
				OPER_NEG, OPER_NONE
				, v_train, *(mlp), inp
				, &(mlp)->m_outcache[((mlp)->m_outc) * ((mlp)->m_depth - 2)]
				, out_desired
				, (mlp)->m_sz, (mlp)->m_outcache
				, (mlp)->m_deltastream
				, (mlp)->m_weight, (mlp)->m_bias
				, (mlp)->m_learningrate, (mlp)->m_learningrate_bias
				, (mlp)->m_act, (mlp)->m_actderiv, (mlp)->m_lossderiv
				);
	}
}

#else
#define	__ae2f_AnnMlpTrainPrimal(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#define __ae2f_AnnMlpTrainAutoPrimal(...) \
	typedef char NO_ae2f_NEED_CLASS[-1]

#endif


#define	__ae2f_AnnMlpTrain_C(reterr, mlp, inp, out, out_desired) \
	__ae2f_AnnMlpTrainPrimal(&1 ? 0 : 1, &1, reterr, mlp, inp, out, out_desired)

#define	__ae2f_AnnMlpTrainStream_C(reterr, mlp, inp, out, out_desired) \
	__ae2f_AnnMlpTrainPrimal(&1 ? 0 : 1, ae2f_NONE, reterr, mlp, inp, out, out_desired)

#define	__ae2f_AnnMlpTrain_imp(...) \
	__ae2f_AnnMlpTrainPrimal_imp(&1 ? 0 : 1, &1, __VA_ARGS__) 

#define	__ae2f_AnnMlpTrainStream_imp(...) \
	__ae2f_AnnMlpTrainPrimal_imp(-1, ae2f_NONE, __VA_ARGS__)


/** @see __ae2f_AnnMlpTrainAutoPrimal */
#define	__ae2f_AnnMlpTrainAuto_C(reterr, mlp, inp, out_desired) \
	__ae2f_AnnMlpTrainAutoPrimal(&1 ? 0 : 1, &1, reterr, mlp, inp, out_desired)

/** @see __ae2f_AnnMlpTrainAutoPrimal */
#define	__ae2f_AnnMlpTrainAutoStream_C(reterr, mlp, inp, out_desired) \
	__ae2f_AnnMlpTrainAutoPrimal(-1, ae2f_NONE, reterr, mlp, inp, out_desired)

#endif


