#ifndef ae2f_Ann_Mhattn_auto_h
#define ae2f_Ann_Mhattn_auto_h

#include <ae2f/Ann/Mhattn.h>

#if !ae2f_MAC_BUILD || !__ae2f_MACRO_GENERATED
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#endif

#include <ae2f/Macro.h>

/**
 * @brief
 * 
 * @param ret_attnw	(m_headc, prm_seqlen, prm_seqlen)
 * @param ret_attno	(m_headc, prm_seqlen, kdist)
 * @param ret_out	(prm_seqlen, m_mdldist)
 *
 * @param ref_qcache	(prm_seqlen, m_mdldist)
 * @param ref_kcache	(prm_seqlen, m_mdldist)
 * @param ref_vcache	(prm_seqlen, m_mdldist)
 *
 * @param prm_qry	(prm_seqlen, m_mdldist)
 * @param prm_key	(prm_seqlen, m_mdldist)
 * @param prm_val	(prm_seqlen, m_mdldist)
 *
 * @param prm_wqry	(prm_mdldist, prm_mdldist)
 * @param prm_wkey	(prm_mdldist, prm_mdldist)
 * @param prm_wval	(prm_mdldist, prm_mdldist)
 * @param prm_wout	(prm_mdldist, prm_mdldist)
 * */
ae2f_MAC() ae2f_AnnMhattnFwd_imp(
		ae2f_AnnMhattnFwd_t			ref_mem,

		ae2f_float_t* const			ref_qcache,
		ae2f_float_t* const			ref_kcache,
		ae2f_float_t* const			ref_vcache,
		ae2f_float_t* const			ref_attnw_cache,

		const ae2f_AnnMhattn_t			prm_mhattn,
		const ae2f_float_t* const		prm_wqry, 
		const ae2f_float_t* const		prm_wkey,
		const ae2f_float_t* const		prm_wval,
		const ae2f_float_t* const		prm_wout,
		const ae2f_float_t* const		prm_qry,
		const ae2f_float_t* const		prm_key,
		const ae2f_float_t* const		prm_val,
		ae2f_opt const ae2f_float_t* const	prm_mask_opt,

		const size_t				prm_seqlen,

		ae2f_AnnActFwdMHATTN_t			prm_act,

		ae2f_float_t* const			ret_out,
		ae2f_float_t* const			ret_attnw,
		ae2f_float_t* const			ret_attno
)
{
	{
		assert(ref_kcache && "ae2f_AnnMhattnFwd_imp");
		assert(ref_qcache && "ae2f_AnnMhattnFwd_imp");
		assert(ref_vcache && "ae2f_AnnMhattnFwd_imp");
		assert(ref_attnw_cache && "ae2f_AnnMhattnFwd_imp");

		assert(prm_wqry && "ae2f_AnnMhattnFwd_imp");
		assert(prm_wkey && "ae2f_AnnMhattnFwd_imp");
		assert(prm_wval && "ae2f_AnnMhattnFwd_imp");
		assert(prm_wout && "ae2f_AnnMhattnFwd_imp");

		assert(prm_qry && "ae2f_AnnMhattnFwd_imp");
		assert(prm_key && "ae2f_AnnMhattnFwd_imp");
		assert(prm_val && "ae2f_AnnMhattnFwd_imp");

		assert(ret_out && "ae2f_AnnMhattnFwd_imp");
		assert(ret_attnw && "ae2f_AnnMhattnFwd_imp");
		assert(ret_attno && "ae2f_AnnMhattnFwd_imp");
	}

	(ref_mem).m_i = (prm_seqlen) /* i */ * ((prm_mhattn).m_mdldist) /* k */; 

	/** 
	 * Matrix multiplication.
	 * */
	while((ref_mem).m_i--) {
		(ref_mem).m_U0.m_S0.m_k = 0;
		(ref_mem).m_U0.m_S0.m_q = 0;
		(ref_mem).m_U0.m_S0.m_v = 0;

		for((ref_mem).m_j = (prm_mhattn).m_mdldist /* j */; (ref_mem).m_j--;) {
			(ref_mem).m_U0.m_S0.m_q
				+= ae2f_AnnMhattnFwdSeqConvOne_imp(
						prm_qry
						, prm_wqry
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_i % (prm_mhattn).m_mdldist
						);

			(ref_mem).m_U0.m_S0.m_k
				+= ae2f_AnnMhattnFwdSeqConvOne_imp(
						prm_key
						, prm_wkey
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_i % (prm_mhattn).m_mdldist
						);

			(ref_mem).m_U0.m_S0.m_v
				+= ae2f_AnnMhattnFwdSeqConvOne_imp(
						prm_val
						, prm_wval
						, (prm_mhattn).m_mdldist
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						, (ref_mem).m_i % (prm_mhattn).m_mdldist
						);
		}

		(ref_qcache)[(ref_mem).m_i] = (ref_mem).m_U0.m_S0.m_q;
		(ref_kcache)[(ref_mem).m_i] = (ref_mem).m_U0.m_S0.m_k;
		(ref_vcache)[(ref_mem).m_i] = (ref_mem).m_U0.m_S0.m_v;
	}

	/**
	 * @brief
	 * cache could be read as (m_headc, prm_seqlen, kdist)
	 * */
	for((ref_mem).m_i = (prm_mhattn).m_headc; (ref_mem).m_i--;) {
		(ref_mem).m_j = (prm_seqlen) /* i */ * (prm_seqlen) /* k */;
		/** 
		 * q * Transpose(k) goes to (prm_seqlen, prm_seqlen)
		 * */
		while((ref_mem).m_j--) {
			(ref_mem).m_U0.m_one = 0;
			(ref_mem).m_k = ae2f_AnnMhattnKDist(prm_mhattn) /* j */;

			while((ref_mem).m_k--) {
				/** Since it is transposing, index must be (i, j) * (k, j) */
				(ref_mem).m_U0.m_one
					+= (ref_qcache)[ae2f_AnnMhattnHeadSplit_imp(
							prm_mhattn
							, prm_seqlen
							, (ref_mem).m_i
							, (ref_mem).m_j / (prm_seqlen)
							, (ref_mem).m_k)
					]
					* (ref_kcache)[ae2f_AnnMhattnHeadSplit_imp( /**/
							prm_mhattn
							, prm_seqlen
							, (ref_mem).m_i
							, (ref_mem).m_j % (prm_seqlen)
							, (ref_mem).m_k)
					];
			}

			(ref_attnw_cache)[ae2f_AnnUtilIdx3(
					(ref_mem).m_i
					, (prm_mhattn).m_headc
					, 0, (prm_seqlen)
					, 0, (prm_seqlen)
					) + (ref_mem).m_j]

				= 
				((ref_mem).m_U0.m_one / sqrt(
					ae2f_static_cast(
						ae2f_float_t
						,  ae2f_AnnMhattnKDist(prm_mhattn)))
				)
				+ ((prm_mask_opt) ? (prm_mask_opt)[(ref_mem).m_j] : 0.);
		}

		(ref_mem).m_j = (prm_seqlen);
		while((ref_mem).m_j--) {
			(ref_mem).m_k = (prm_seqlen);
			while((ref_mem).m_k--) {
				prm_act(
						/* ret */
						&(ret_attnw)[ae2f_AnnUtilIdx3(
							(ref_mem).m_i
							, (prm_mhattn).m_headc
							, (ref_mem).m_j, (prm_seqlen)
							, (ref_mem).m_k, (prm_seqlen)
							)],
						/* prm_retidx */
						(ref_mem).m_k,
						/* prm_inp */
						&(ref_attnw_cache)[ae2f_AnnUtilIdx3(
							(ref_mem).m_i, (prm_mhattn).m_headc
							, (ref_mem).m_j, (prm_seqlen)
							, 0, prm_seqlen
							)],
						/* prm_len */
						(prm_seqlen)
				       );
			}
		}

		(ref_mem).m_j = (prm_seqlen) /* i */ * ae2f_AnnMhattnKDist(prm_mhattn) /* k */;
		while((ref_mem).m_j--)
		{
			(ref_mem).m_U0.m_one = 0;
			for((ref_mem).m_k = (prm_seqlen) /* j */; (ref_mem).m_k--;)
			{
				(ref_mem).m_U0.m_one
					+= (ret_attnw)[ae2f_AnnUtilIdx3(
							(ref_mem).m_i, (prm_mhattn).m_headc
							, (ref_mem).m_j / ae2f_AnnMhattnKDist(prm_mhattn), prm_seqlen
							, (ref_mem).m_k, prm_seqlen
							)] * /**/
					(ref_vcache)[ae2f_AnnMhattnHeadSplit_imp(
							prm_mhattn
							, prm_seqlen
							, (ref_mem).m_i
							, (ref_mem).m_k
							, (ref_mem).m_j % ae2f_AnnMhattnKDist(prm_mhattn)
							)];
			}

			(ret_attno)[
				ae2f_AnnUtilIdx3(
						(ref_mem).m_i, (prm_mhattn).m_headc
						, 0, prm_seqlen
						, 0, ae2f_AnnMhattnKDist(prm_mhattn)
						) + (ref_mem).m_j
			] = (ref_mem).m_U0.m_one;
		}
	}

	(ref_mem).m_i = (prm_seqlen) /* i */ * (prm_mhattn).m_mdldist /* k */;
	while((ref_mem).m_i--) {
		(ref_mem).m_j = ((prm_mhattn).m_mdldist) /* j */;
		(ref_mem).m_U0.m_one = 0;

		while((ref_mem).m_j--) {
			(ref_mem).m_U0.m_one +=
				(ret_attno)[
				ae2f_AnnMhattnHeadConcat_imp(
						prm_mhattn
						, prm_seqlen
						, (ref_mem).m_i / (prm_mhattn).m_mdldist
						, (ref_mem).m_j
						)] * /**/
					(prm_wout)[ae2f_AnnUtilIdx2(
							(ref_mem).m_j, (prm_mhattn).m_mdldist
							, (ref_mem).m_i % (prm_mhattn).m_mdldist
							, (prm_mhattn).m_mdldist)];
		}

		(ret_out)[(ref_mem).m_i] = (ref_mem).m_U0.m_one;
	}
}

/**
 * @brief
 *
 * @param ref_grad_heads	(m_headc, prm_seqlen, kdist)
 * @param ref_grad_scores	(prm_seqlen, prm_seqlen)
 *
 * @param ref_qcache	(prm_seqlen, m_mdldist)
 * @param ref_kcache	(prm_seqlen, m_mdldist)
 * @param ref_vcache	(prm_seqlen, m_mdldist)
 * 
 * @param prm_attnw	(m_headc, prm_seqlen, prm_seqlen)
 * @param prm_attno	(m_headc, prm_seqlen, kdist)
 *
 * @param prm_grad_out	(prm_seqlen, m_mdldist)
 *
 * @param prm_qry	(prm_seqlen, m_mdldist)
 * @param prm_key	(prm_seqlen, m_mdldist)
 * @param prm_val	(prm_seqlen, m_mdldist)
 *
 * @param prm_wqry	(prm_mdldist, prm_mdldist)
 * @param prm_wkey	(prm_mdldist, prm_mdldist)
 * @param prm_wval	(prm_mdldist, prm_mdldist)
 * @param prm_wout	(prm_mdldist, prm_mdldist)
 *
 * @param ret_grad_wout	(prm_mdldist, prm_mdldist)
 * */
ae2f_MAC() ae2f_AnnMhattnBwd_imp(
		ae2f_float_t* const			ref_qcache,
		ae2f_float_t* const			ref_kcache,
		ae2f_float_t* const			ref_vcache,
		ae2f_float_t* const			ref_grad_heads,
		ae2f_float_t* const			ref_grad_scores,

		const ae2f_AnnMhattn_t			prm_mhattn,

		const ae2f_float_t* const		prm_grad_out,

		const ae2f_float_t* const		prm_wqry, 
		const ae2f_float_t* const		prm_wkey,
		const ae2f_float_t* const		prm_wval,
		const ae2f_float_t* const		prm_wout,

		const ae2f_float_t* const		prm_qry,
		const ae2f_float_t* const		prm_key,
		const ae2f_float_t* const		prm_val,

		const size_t				prm_seqlen,
		const ae2f_float_t			prm_lr,

		const ae2f_float_t* const		prm_attnw,
		const ae2f_float_t* const		prm_attno,

		ae2f_AnnActBwdMHATTN_t				prm_actderiv,

		ae2f_float_t* const			ret_grad_wqry, 
		ae2f_float_t* const			ret_grad_wkey,
		ae2f_float_t* const			ret_grad_wval,
		ae2f_float_t* const			ret_grad_wout
		)
{
	ae2f_structdef(struct, ae2f_AnnMhattnBwdREG_t) {
		size_t m_i, m_j, m_hidx;

		union ae2f_AnnMhattnBwdU0REG_t {
			struct ae2f_AnnMhattnBwdU0S0REG_t {
				ae2f_float_t	m_q;
				ae2f_float_t	m_k;
				ae2f_float_t	m_v;
			} m_S0;
			ae2f_float_t m_f;
		} m_U0;
	};

	ae2f_structdef(struct, ae2f_AnnMhattnBwdRAM_t) {
		union ae2f_AnnMhattnBwdU0_t {
			ae2f_float_t	m_f;
			ae2f_float_t	m_fa[1];
		} m_U0;
	};

	ae2f_reg ae2f_AnnMhattnBwdREG_t	reg_bwd;
	ae2f_reg ae2f_AnnMhattnBwdRAM_t	ref_bwd;

	/** ret_grad_wout is done here. */
	(reg_bwd).m_i = (prm_mhattn).m_mdldist /* i */ * (prm_mhattn).m_mdldist /* k */;
	while((reg_bwd).m_i--) {
		(reg_bwd).m_j = (prm_seqlen) /* j */;
		(reg_bwd).m_U0.m_f = 0;

		while((reg_bwd).m_j--) {
			(reg_bwd).m_U0.m_f
				+= (prm_attno)[ae2f_AnnMhattnHeadConcat_imp(
						prm_mhattn, prm_seqlen
						, (reg_bwd).m_j
						, (reg_bwd).m_i / (prm_mhattn).m_mdldist) /* i */]
				* (prm_grad_out)[ae2f_AnnUtilIdx2( /**/
						(reg_bwd).m_j, (prm_seqlen)
						, (reg_bwd).m_i % (prm_mhattn).m_mdldist, (prm_mhattn).m_mdldist
						)];
		}

		(ret_grad_wout)[(reg_bwd).m_i] = (reg_bwd).m_U0.m_f;
	}

	(reg_bwd).m_i = (prm_seqlen) /* i */ * (prm_mhattn).m_mdldist /* k */;
	while((reg_bwd).m_i--) {
		(reg_bwd).m_j = (prm_mhattn).m_mdldist /* j */;
		(reg_bwd).m_U0.m_f = 0;
		while((reg_bwd).m_j--) {
			/** i j * k j */
			(reg_bwd).m_U0.m_f
				+= (prm_grad_out)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_i / (prm_mhattn).m_mdldist, prm_seqlen
						, (reg_bwd).m_j, (prm_mhattn).m_mdldist
						)]
				* (prm_wout)[ae2f_AnnUtilIdx2(/**/
						(reg_bwd).m_i % (prm_mhattn).m_mdldist, (prm_mhattn).m_mdldist /**/
						, (reg_bwd).m_j, (prm_mhattn).m_mdldist
						)]
				;
		}

		(ref_grad_heads)[
			ae2f_AnnMhattnHeadConcat_imp(prm_mhattn, prm_seqlen
					, (reg_bwd).m_i / (prm_mhattn).m_mdldist
					, (reg_bwd).m_i % (prm_mhattn).m_mdldist
					)
		] = (reg_bwd).m_U0.m_f;
	}

	(reg_bwd).m_hidx = (prm_mhattn).m_headc;
	while((reg_bwd).m_hidx--) {
		(reg_bwd).m_i = (prm_seqlen) /* i */ * ae2f_AnnMhattnKDist(prm_mhattn) /* k */;
		while((reg_bwd).m_j--) {
			(reg_bwd).m_U0.m_f = 0;
			(reg_bwd).m_j = (prm_seqlen) /* j */;

			while((reg_bwd).m_j--) {
				(reg_bwd).m_U0.m_f
					+= (prm_attnw)[ae2f_AnnUtilIdx2(
							(reg_bwd).m_i / ae2f_AnnMhattnKDist(prm_mhattn), (prm_seqlen)
							, (reg_bwd).m_j, (prm_seqlen))
					] * /**/
					(ref_grad_heads)[ae2f_AnnUtilIdx3(
							(reg_bwd).m_hidx, (prm_mhattn).m_headc
							, (reg_bwd).m_j
							, (prm_seqlen)
							, (reg_bwd).m_i % ae2f_AnnMhattnKDist(prm_mhattn)
							, ae2f_AnnMhattnKDist(prm_mhattn)
							)
					];
			}

			(ref_vcache)[
				ae2f_AnnMhattnHeadSplit_imp(
						prm_mhattn, prm_seqlen
						, (reg_bwd).m_hidx
						, (reg_bwd).m_i / ae2f_AnnMhattnKDist(prm_mhattn)
						, (reg_bwd).m_i % ae2f_AnnMhattnKDist(prm_mhattn)
						)
			] = (reg_bwd).m_U0.m_f;
		}

		(reg_bwd).m_i = (prm_seqlen) /* i */ * (prm_seqlen) /* k */;
		while((reg_bwd).m_i--) {
			(reg_bwd).m_j = ae2f_AnnMhattnKDist(prm_mhattn);
			(reg_bwd).m_U0.m_f = 0;
			while((reg_bwd).m_j--) {
				(reg_bwd).m_U0.m_f +=
					(ref_grad_heads)[ae2f_AnnUtilIdx3(
							(reg_bwd).m_hidx, (prm_mhattn).m_headc
							, (reg_bwd).m_i / (prm_seqlen), prm_seqlen
							, (reg_bwd).m_j, ae2f_AnnMhattnKDist(prm_mhattn)
							)] * /**/
					(prm_val)[ae2f_AnnMhattnHeadSplit_imp(
							prm_mhattn, prm_seqlen
							, (reg_bwd).m_hidx
							, (reg_bwd).m_i % (prm_seqlen)
							, (reg_bwd).m_j
							)];
			}

			(ref_grad_scores)[ae2f_AnnUtilIdx2(
					(reg_bwd).m_i / (prm_seqlen), prm_seqlen
					, (reg_bwd).m_i % (prm_seqlen), prm_seqlen)
			] = (reg_bwd).m_U0.m_f;
		}

		(reg_bwd).m_i = (prm_seqlen);
		while((reg_bwd).m_i--) {
			(reg_bwd).m_j = (prm_seqlen);
			while((reg_bwd).m_j--) {
				prm_actderiv(
					/* ret */
					&(ref_grad_scores)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_i, prm_seqlen,
						(reg_bwd).m_j, prm_seqlen
						)],
					/* prm_retidx */
					(reg_bwd).m_j,
					/* prm_grad_in */
					&(ref_grad_scores)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_i, prm_seqlen,
						0, prm_seqlen
						)],
					/* prm_softmax_out */
					&(prm_attnw)[ae2f_AnnUtilIdx3(
						(reg_bwd).m_hidx, (prm_mhattn).m_headc,
						(reg_bwd).m_i, prm_seqlen,
						0, prm_seqlen
						)],
					/* prm_len */
					prm_seqlen
				);
			} /** SEQLEN */
		} /** SEQLEN (ACTDERIV) */

		(reg_bwd).m_i = (prm_seqlen) /* i */ * ae2f_AnnMhattnKDist(prm_mhattn) /* k */;
		while((reg_bwd).m_i--) {
			(reg_bwd).m_j = (prm_seqlen);
			(reg_bwd).m_U0.m_S0.m_k = 0;
			(reg_bwd).m_U0.m_S0.m_q = 0;

			while((reg_bwd).m_j--) {
				(reg_bwd).m_U0.m_S0.m_q += 
					(ref_grad_scores)[ae2f_AnnUtilIdx2(
							(reg_bwd).m_i / ae2f_AnnMhattnKDist(prm_mhattn)
							, (prm_seqlen)
							, (reg_bwd).m_j, prm_seqlen)
					] * /**/
					(prm_key)[ae2f_AnnMhattnHeadSplit_imp(prm_mhattn, prm_seqlen
							, (reg_bwd).m_hidx
							, (reg_bwd).m_j
							, (reg_bwd).m_i % ae2f_AnnMhattnKDist(prm_mhattn))];

				/** key(kdist, seqlen), scores(seqlen, seqlen) */
				(reg_bwd).m_U0.m_S0.m_k +=
					(prm_qry)[ae2f_AnnMhattnHeadSplit_imp(
							prm_mhattn, prm_seqlen
							, (reg_bwd).m_hidx
							, (reg_bwd).m_j
							, (reg_bwd).m_i / (prm_seqlen)
							)] * /**/
					(ref_grad_scores)[ae2f_AnnUtilIdx2(
							(reg_bwd).m_j, (prm_seqlen)
							, (reg_bwd).m_i % (prm_seqlen), prm_seqlen
							)];
			}

			(ref_kcache)[ae2f_AnnMhattnHeadSplit_imp(
					prm_mhattn, prm_seqlen
					, (reg_bwd).m_hidx
					, (reg_bwd).m_i / (prm_seqlen)
					, (reg_bwd).m_i % (prm_seqlen)
					)] = (reg_bwd).m_U0.m_S0.m_k;

			(ref_qcache)[ae2f_AnnMhattnHeadSplit_imp(
					prm_mhattn, prm_seqlen
					, (reg_bwd).m_hidx
					, (reg_bwd).m_i / (prm_seqlen)
					, (reg_bwd).m_i % (prm_seqlen)
					)] = (reg_bwd).m_U0.m_S0.m_q;
		} /* CONV (GRAD_Q_HEAD, GRAD_K_HEAD) */	
	} /** HEADCOUNT */

	(reg_bwd).m_i = (prm_mhattn).m_mdldist /* i */ * (prm_mhattn).m_mdldist /* k */;
	while((reg_bwd).m_i--) {
		(reg_bwd).m_U0.m_S0.m_k = 0;
		(reg_bwd).m_U0.m_S0.m_q = 0;
		(reg_bwd).m_U0.m_S0.m_v = 0;

		(reg_bwd).m_j = (prm_seqlen);
		while((reg_bwd).m_j--) {
			(reg_bwd).m_U0.m_S0.m_q += 
				(prm_qry)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_i / (prm_mhattn).m_mdldist, prm_seqlen
						, (reg_bwd).m_j, (prm_mhattn).m_mdldist
						)] * /**/
				(ref_qcache)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_j, (prm_mhattn).m_mdldist
						, (reg_bwd).m_i % (prm_mhattn).m_mdldist, (prm_mhattn).m_mdldist
						)];

			(reg_bwd).m_U0.m_S0.m_k += 
				(prm_key)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_i / (prm_mhattn).m_mdldist, prm_seqlen
						, (reg_bwd).m_j, (prm_mhattn).m_mdldist
						)] * /**/
				(ref_kcache)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_j, (prm_mhattn).m_mdldist
						, (reg_bwd).m_i % (prm_mhattn).m_mdldist, (prm_mhattn).m_mdldist
						)];

			(reg_bwd).m_U0.m_S0.m_v += 
				(prm_val)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_i / (prm_mhattn).m_mdldist, prm_seqlen
						, (reg_bwd).m_j, (prm_mhattn).m_mdldist
						)] * /**/
				(ref_vcache)[ae2f_AnnUtilIdx2(
						(reg_bwd).m_j, (prm_mhattn).m_mdldist
						, (reg_bwd).m_i % (prm_mhattn).m_mdldist, (prm_mhattn).m_mdldist
						)];
		}

		(ret_grad_wqry)[(reg_bwd).m_i] = (reg_bwd).m_U0.m_S0.m_q;
		(ret_grad_wkey)[(reg_bwd).m_i] = (reg_bwd).m_U0.m_S0.m_k;
		(ret_grad_wval)[(reg_bwd).m_i] = (reg_bwd).m_U0.m_S0.m_v;
	}
}

#endif
