#ifndef ae2fVK_Ann_Slp_h
#define ae2fVK_Ann_Slp_h

#include <vulkan/vulkan.h>
#include <ae2f/Ann/Slp.h>
#include <vulkan/vulkan_core.h>

ae2f_structdef(struct, ae2fVK_AnnSlp) 
{
	/* @brief 
	 * input count
	 */
	size_t m_inc;

	/* @brief 
	 * output count
	 */
	size_t m_outc;

	/**
	 * @brief
	 * Fields allocated as 2nd array.
	 * */
	ae2f_float_t* restrict m_weight;
	ae2f_float_t* restrict m_bias;
	ae2f_float_t* restrict m_cache;

	ae2f_AnnSlp_t m_Slp[1];

	ae2f_AnnAct_t * m_act, * m_actderiv;
	ae2f_AnnLoss_t* m_lossderiv;

	ae2f_float_t m_learningrate, m_learningrate_bias;

	VkDevice	m_vkdev;
	VkResult	m_vkres;
	VkBuffer	m_vkbuf;
};


#endif

#ifndef ae2fVK_Ann_Slp_c

#if !__ae2f_MACRO_GENERATED
#include <ae2fVK/Ann/Slp.h>
#endif

#define ae2fVK_Ann_Slp_c


#endif
