#ifndef ae2fVK_Ann_Slp_h
#define ae2fVK_Ann_Slp_h

#include "./Slp.core.h"
#include "./Slp.auto.h"
#include "./SlpPool.auto.h"

/**
 * @brief
 * Vulkan memory layout
 * [WeightBuffer] [BiasBuffer] [DeltaCacheBuffer]
 *
 * 
 * */
ae2f_structdef(struct, ae2fVK_AnnSlp)
{
	ae2f_AnnSlp	m_slp;

	VkDevice	m_vkdev;
	VkResult	m_vkres;
	VkBuffer	m_vkglobbuf, m_vklocbuf;
	VkDeviceMemory	m_vkglobdevmem, m_vklocdevmem;

	VkAllocationCallbacks* ae2f_restrict	m_vkalloccalls;

	VkDescriptorSetLayout	m_vkdescsetlayout[ae2fVK_eAnnSlpDescLayouts_LEN];

	VkPipelineLayout	m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_LEN];
	VkShaderModule		m_vkshadermodule;
	VkPipeline		m_vkpipeline[ae2fVK_eAnnSlpPipes_LEN];
};

/**
 * @brief
 * Descriptor Pool for ae2fVK_AnnSlp.
 *
 * @details
 * Note that pool has maximum amount of ae2fVK_AnnSLpDescPoolCmd.
 * */
ae2f_structdef(struct, ae2fVK_AnnSlpDescPool)
{
	VkDescriptorPool	m_vkdescpool;
};
/**
 * @brief
 * A command, descriptor set for one task.
 * */
ae2f_structdef(struct, ae2fVK_AnnSlpDescPoolCmd)
{
	VkDescriptorSet		m_vkdescset;
};

#endif
