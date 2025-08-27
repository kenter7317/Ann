#ifndef ae2fVK_Ann_Mlp_core_h
#define ae2fVK_Ann_Mlp_core_h

#include <ae2f/Ann/Mlp.h>

#include "./Slp.core.h"

#include <ae2f/Cast.h>
#include <ae2f/errGlob.h>
#include <vulkan/vulkan.h>

#include <ae2f/Pack/Beg.h>

ae2f_enumdef(ae2fVK_eAnnMlpDescLayouts, {
		ae2fVK_eAnnMlpDescLayouts_ONLY,
		ae2fVK_eAnnMlpDescLayouts_LEN
		});

ae2f_enumdef(ae2fVK_eAnnMlpPipeLayouts, {
		ae2fVK_eAnnMlpPipeLayouts_kPredict,
		ae2fVK_eAnnMlpPipeLayouts_kTrain,
		ae2fVK_eAnnMlpPipeLayouts_LEN
		});

ae2f_enumdef(ae2fVK_eAnnMlpPipes, {
		ae2fVK_eAnnMlpPipes_kPredict,
		ae2fVK_eAnnMlpPipes_kPredictStream,
		ae2fVK_eAnnMlpPipes_kFollow,
		ae2fVK_eAnnMlpPipes_kTrainAuto,
		ae2fVK_eAnnMlpPipes_LEN
		});

ae2f_enumdef(ae2fVK_eAnnMlpDescPools, {
		ae2fVK_eAnnMlpDescPools_ONLY,
		ae2fVK_eAnnMlpDescPools_LEN
		});

ae2f_structdef(struct, ae2fVK_AnnMlp);

ae2f_structdef(struct, ae2fVK_AnnMlpMkU0Swap_t) {
	/**
	 * @brief
	 * Maximum output count among all layers.
	 */
	size_t m_outc;
	/**
	 * @brief
	 * Loop counter.
	 */
	size_t m_i;
	/**
	 * @brief
	 * Union for pointer manipulation during creation.
	 */
	ae2f_AnnMlpInitMkPtr_t	m_mkptr;
	/**
	 * @brief
	 * Base pointer to the created MLP.
	 */
	ae2fVK_AnnMlp* restrict	m_mkbase;
};

ae2f_structdef(union, ae2fVK_AnnMlpMkU0_t) {
	ae2f_AnnMlpMk_t		m_mk;
	ae2fVK_AnnMlpMkU0Swap_t	m_mkswap;
	/** END OF DEFINITION */
};

ae2f_structdef(union, ae2fVK_AnnMlpMkU1_t) {
	size_t				m_i;
	VkBufferCreateInfo		m_vkbufcreatinfo;
	VkMemoryAllocateInfo		m_vkmemallocinfo;
	VkDescriptorSetLayoutBinding	m_vkdescsetlayoutbind[ae2fVK_eAnnMlpDescLayouts_LEN];
	VkPushConstantRange		m_vkpushconstrange;
	ae2fVK_AnnSlpMap_t		m_map;
	ae2fVK_AnnSlpUnMap_t		m_unmap;

	const char*			m_clsrc;
	void*				m_clsrc_v;

	VkComputePipelineCreateInfo	m_vkcomputepipecreatinfo[ae2fVK_eAnnMlpPipes_LEN];
	VkDescriptorPoolCreateInfo	m_vkdescpoolcreatinfo;
};

ae2f_structdef(union, ae2fVK_AnnMlpMkU2_t) {
	VkMemoryRequirements		m_vkmemreqs;
	VkDescriptorSetLayoutCreateInfo	m_vkdescsetlayoutcreatinfo;
	VkPipelineLayoutCreateInfo	m_vkpipelayoutcreatinfo;
	VkDeviceSize			m_i;
	VkShaderModuleCreateInfo	m_vkshadermodcreatinfo;
	VkDescriptorPoolSize		m_vkdescpoolsz;
};

ae2f_structdef(union, ae2fVK_AnnMlpMkU3_t) {
	size_t	m_vkmemtypeidx;
	char*	m_cllog;
};

ae2f_structdef(struct, ae2fVK_AnnMlpMkU4_t) {
	ClspvError	m_clspverror;
};

ae2f_structdef(struct, ae2fVK_AnnMlpMkRet_t) {
	ae2f_err_t		m_err;
};
ae2f_structdef(struct, ae2fVK_AnnMlpMk_t) {
	ae2fVK_AnnMlpMkRet_t	m_ret;
	ae2fVK_AnnMlpMkU0_t	m_U0;
	ae2fVK_AnnMlpMkU1_t	m_U1;
	ae2fVK_AnnMlpMkU2_t	m_U2;
	ae2fVK_AnnMlpMkU3_t	m_U3;
	ae2fVK_AnnMlpMkU4_t	m_U4;
};

typedef ae2fVK_AnnSlpMap_t ae2fVK_AnnMlpMap_t;
typedef ae2fVK_AnnSlpUnMap_t ae2fVK_AnnMlpUnMap_t;

#include <ae2f/Pack/End.h>

#endif
