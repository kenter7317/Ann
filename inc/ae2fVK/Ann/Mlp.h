#ifndef ae2fVK_Ann_Mlp_h
#define ae2fVK_Ann_Mlp_h

#include <ae2f/Ann/Mlp.h>
#include <vulkan/vulkan.h>

#include "./Mlp.core.h"
#include "./Mlp.auto.h"

ae2f_structdef(struct, ae2fVK_AnnMlp) {
	ae2f_AnnMlp	m_mlp;

	VkDevice	m_vkdev;
	VkResult	m_vkres;
	VkBuffer	m_vkglobbuf, m_vklocbuf;
	VkDeviceMemory	m_vkglobdevmem, m_vklocdevmem;

	VkAllocationCallbacks* restrict	m_vkalloccalls;

	VkDescriptorSetLayout	m_vkdescsetlayout[ae2fVK_eAnnMlpDescLayouts_LEN];
	VkDescriptorPool	m_vkdescpool[ae2fVK_eAnnMlpDescPools_LEN];

	VkPipelineLayout	m_vkpipelayout[ae2fVK_eAnnMlpPipeLayouts_LEN];
	VkShaderModule		m_vkshadermodule;
	VkPipeline		m_vkpipeline[ae2fVK_eAnnMlpPipes_LEN];
};

#endif
