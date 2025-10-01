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
	size_t	m_outc;
	size_t	m_weightc;

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
	ae2fVK_AnnSlpMapRangedGeneric_t	m_map;
	ae2fVK_AnnSlpUnMap_t		m_unmap;

	const char*			m_clsrc;
	void*				m_clsrc_v;

	VkComputePipelineCreateInfo	m_vkcomputepipecreatinfo[ae2fVK_eAnnMlpPipes_LEN];
};

ae2f_structdef(union, ae2fVK_AnnMlpMkU2_t) {
	VkMemoryRequirements		m_vkmemreqs;
	VkDescriptorSetLayoutCreateInfo	m_vkdescsetlayoutcreatinfo;
	VkPipelineLayoutCreateInfo	m_vkpipelayoutcreatinfo;
	VkDeviceSize			m_i;
	VkShaderModuleCreateInfo	m_vkshadermodcreatinfo;
};

ae2f_structdef(union, ae2fVK_AnnMlpMkU3_t) {
	size_t	m_vkmemtypeidx;
	ae2f_float_t*	m_mapped;

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

#include <ae2f/Pack/End.h>

typedef ae2fVK_AnnSlpUnMap_t ae2fVK_AnnMlpUnMap_t;
typedef VkMappedMemoryRange		ae2fVK_AnnMlpUnMap_t, ae2fVK_AnnMlpMapRangedGeneric_t;
typedef ae2fVK_AnnSlpDescPoolCmdMk_t	ae2fVK_AnnMlpDescPoolCmdMk_t;


#define __ae2fVK_AnnMlpMdlSz(i_depth, ...) \
	((sizeof(uint32_t) * (i_depth)))

#define __ae2fVK_AnnMlpMdlOff(...) 0

#define __ae2fVK_AnnMlpOutStreamSz(i_depth, i_outc, i_weightc) \
	((sizeof(ae2f_float_t)) * (i_depth) * (i_outc)) /** outstream */ 

#define __ae2fVK_AnnMlpOutStreamOff(i_depth, i_outc, i_weightc) \
	(__ae2fVK_AnnMlpMdlOff(i_depth, i_outc, i_weightc) + __ae2fVK_AnnMlpMdlSz(i_depth, i_outc, i_weightc))

#define __ae2fVK_AnnMlpWeightSz(i_depth, i_outc, i_weightc) \
	((sizeof(ae2f_float_t)) * ((i_depth) - 1) * (i_weightc)) /** weight */

#define __ae2fVK_AnnMlpWeightOff(i_depth, i_outc, i_weightc) \
	(__ae2fVK_AnnMlpOutStreamOff(i_depth, i_outc, i_weightc) + __ae2fVK_AnnMlpOutStreamSz(i_depth, i_outc, i_weightc))

#define __ae2fVK_AnnMlpBiasSz(i_depth, i_outc, i_weightc) \
	(sizeof(ae2f_float_t)) * ((i_depth) - 1) * (i_outc) /** bias */

#define __ae2fVK_AnnMlpBiasOff(i_depth, i_outc, i_weightc) \
	(__ae2fVK_AnnMlpWeightOff(i_depth, i_outc, i_weightc) + __ae2fVK_AnnMlpWeightSz(i_depth, i_outc, i_weightc))

#define __ae2fVK_AnnMlpDeltaStreamSz(i_depth, i_outc, i_weightc) \
	(((sizeof(ae2f_float_t)) * ((i_depth) - 1) * (i_outc))) /** deltastream */

#define __ae2fVK_AnnMlpDeltaStreamOff(i_depth, i_outc, i_weightc) \
	(__ae2fVK_AnnMlpBiasSz(i_depth, i_outc, i_weightc) + __ae2fVK_AnnMlpBiasOff(i_depth, i_outc, i_weightc))

#define __ae2fVK_AnnMlpGoalSz(i_depth, i_outc, i_weightc) \
	(sizeof(ae2f_float_t)) * (i_outc) /** goal */

#define __ae2fVK_AnnMlpGoalOff(i_depth, i_outc, i_weightc) \
	(__ae2fVK_AnnMlpDeltaStreamSz(i_depth, i_outc, i_weightc) + __ae2fVK_AnnMlpDeltaStreamOff(i_depth, i_outc, i_weightc))

#define __ae2fVK_AnnMlpGlobMemSz(i_depth, i_outc, i_weightc) \
	(__ae2fVK_AnnMlpGoalOff(i_depth, i_outc, i_weightc) + __ae2fVK_AnnMlpGoalSz(i_depth, i_outc, i_weightc))


#define __ae2fVK_AnnMlpDescPoolMk_imp		__ae2fVK_AnnSlpDescPoolMk_imp
#define __ae2fVK_AnnMlpDescPoolClean_imp	__ae2fVK_AnnSlpDescPoolClean_imp

#define __ae2fVK_AnnMlpDescPoolCmdMk_imp	__ae2fVK_AnnSlpDescPoolCmdMk_imp
#define __ae2fVK_AnnMlpDescPoolCmdClean_imp	__ae2fVK_AnnSlpDescPoolCmdClean_imp

#define __ae2fVK_AnnMlpMapRangedGeneric_imp	__ae2fVK_AnnSlpMapRangedGeneric_imp
#define __ae2fVK_AnnMlpUnMapRanged_imp		__ae2fVK_AnnSlpUnMapRanged_imp

#endif
