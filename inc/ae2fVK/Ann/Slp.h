#ifndef ae2fVK_Ann_Slp_h
#define ae2fVK_Ann_Slp_h

#include "./Slp.core.h"
#include "./Slp.auto.h"

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

	VkAllocationCallbacks* restrict	m_vkalloccalls;

	VkDescriptorSetLayout	m_vkdescsetlayout[ae2fVK_eAnnSlpDescLayouts_LEN];
	VkDescriptorPool	m_vkdescpool[2];

	VkPipelineLayout	m_vkpipelayout[ae2fVK_eAnnSlpPipeLayouts_LEN];
	VkShaderModule		m_vkshadermodule;
	VkPipeline		m_vkpipeline[ae2fVK_eAnnSlpPipes_LEN];
};


#endif
