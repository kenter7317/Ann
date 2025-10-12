#ifndef ae2f_NEED_CLASS
#define ae2f_NEED_CLASS 1
#endif

/**
 * @file Mhattn.h
 * @brief
 * A multi-head attention.
 * */
#ifndef ae2f_Ann_Mhattn_h
#define ae2f_Ann_Mhattn_h

#include <ae2f/Cast.h>
#include <ae2f/Guide.h>
#include "./Act.h"

#include "./Mhattn.auto.h"
#include "./Mhattn.core.h"

#include "./Util.h"

#include <ae2f/Pack/Beg.h>

/** Metadata for Multihead Attention */
ae2f_structdef(struct, ae2f_AnnMhattn_t)
{
	size_t		m_mdldist, m_headc;
};


/** @brief m_headc * kdist == m_mdldist */
#define ae2f_AnnMhattnKDist(/** const ae2f_AnnMhattn_t */ prm_mhattn) \
	((prm_mhattn).m_mdldist / (prm_mhattn).m_headc)

/**
 * @brief
 * Index redirector from [m_headc, prm_seqlen, kdist] to [prm_seqlen, m_mdldist]
 *
 * @returns		{const size_t}
 * @param prm_mhattn	{const ae2f_AnnMhattn_t&}
 * @param prm_seqlen	{const size_t}
 * @param m_i2		{const size_t}	< m_headc
 * @param m_i1		{const size_t}	< prm_seqlen
 * @param m_i0		{const size_t}	< kdist
 * */
#define ae2f_AnnMhattnHeadSplit_imp(	\
		prm_mhattn,		\
		prm_seqlen,		\
		m_i2,			\
		m_i1,			\
		m_i0			\
		) ae2f_AnnUtilIdx2(						\
			m_i1, prm_seqlen					\
			, ((m_i0) + (m_i2) * ae2f_AnnMhattnKDist(prm_mhattn))	\
			, (prm_mhattn).m_mdldist)

/**
 * @brief
 * Index redirector from [prm_seqlen, m_mdldist] to [m_headc, prm_seqlen, kdist]
 *
 * @returns		{const size_t}
 * @param prm_mhattn	{const ae2f_AnnMhattn_t&}
 * @param prm_seqlen	{const size_t}
 * @param m_i1		{const size_t}	< prm_seqlen
 * @param m_i0		{const size_t}	< m_mdldist
 * */
#define ae2f_AnnMhattnHeadConcat_imp(	\
		prm_mhattn,		\
		prm_seqlen,		\
		m_i1,			\
		m_i0			\
		) ae2f_AnnUtilIdx3(					\
			(m_i0) / ae2f_AnnMhattnKDist(prm_mhattn)	\
			, (prm_mhattn).m_headc				\
			, m_i1, prm_seqlen				\
			, (m_i0) % ae2f_AnnMhattnKDist(prm_mhattn)	\
			, ae2f_AnnMhattnKDist(prm_mhattn)		\
			)

/**
 * @brief
 *
 * @details
 * Original implementation by @kenter7317.
 * Macrofied by @dalmurii.
 *
 * @param prm_seq	{const ae2f_float_t* const} (prm_seqlen, prm_mdldist)
 * @param prm_w		{const ae2f_float_t* const} (prm_mdldist, prm_mdldist)
 * @param prm_mdldist	{const size_t}
 * @param prm_seqlen	{const size_t}
 * @param prm_i		{const size_t} < prm_seqlen
 * @param prm_j		{const size_t} < prm_mdldist
 * @param prm_k		{const size_t} < prm_mdldist
 * */
#define ae2f_AnnMhattnFwdSeqConvOne_imp(		\
		prm_seq,				\
		prm_w,					\
		prm_mdldist,				\
		prm_seqlen,				\
		prm_i,					\
		prm_j,					\
		prm_k  					\
		)					\
		((prm_seq)[ae2f_AnnUtilIdx2(prm_i, prm_seqlen, prm_j, prm_mdldist)] *	\
		 (prm_w)[ae2f_AnnUtilIdx2(prm_j, prm_mdldist, prm_k, prm_mdldist)])


#if ae2f_NEED_CLASS

/** 
 * @brief
 * [M]ulti-[h]ead [att]e[n]tion.
 * */
ae2f_structdef(struct, ae2f_AnnMhattn) {

	/** Weights */
	ae2f_LP(m_mdldist, m_mdldist)	ae2f_float_t*	ae2f_restrict m_wqry;
	ae2f_LP(m_mdldist, m_mdldist)	ae2f_float_t*	ae2f_restrict m_wkey;
	ae2f_LP(m_mdldist, m_mdldist)	ae2f_float_t*	ae2f_restrict m_wval;
	ae2f_LP(m_mdldist, m_mdldist)	ae2f_float_t*	ae2f_restrict m_wout;

	/** @brief Model distance */
	size_t		m_mdldist;

	/** @brief Head count */
	size_t		m_headc;

	ae2f_AnnLoss_t*	m_loss;
};

#include <ae2f/Pack/End.h>

#endif

#endif
