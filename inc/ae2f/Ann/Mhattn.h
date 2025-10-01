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
#include ""

ae2f_structdef(struct, ae2f_AnnMhattn_t)
{
	size_t		m_mdldis, m_numheads;
};

#if ae2f_NEED_CLASS

/** 
 * @brief
 * [M]ulti-[h]ead [att]e[n]tion.
 * */
ae2f_structdef(struct, ae2f_AnnMhattn) {

	/** @brief Query */
	ae2f_float_t*	restrict m_qry;
	ae2f_float_t*	restrict m_key;
	ae2f_float_t*	restrict m_val;
	ae2f_float_t*	restrict m_out;

	/** @brief Model distance */
	size_t		m_mdldis;

	/** @brief Head count */
	size_t		m_numheads;

	ae2f_AnnLoss_t*	m_loss;
};

#endif

#endif
