#ifndef ae2fVK_Ann_Slp_core_h
#define ae2fVK_Ann_Slp_core_h

#include <ae2f/Cast.h>
#include <ae2f/errGlob.h>
#include <vulkan/vulkan.h>
#include <ae2f/Ann/Slp.h>
#include <vulkan/vulkan_core.h>
#include <ae2f/Cmp.h>
#include <limits.h>
#include <ae2f/Float.h>

#define clspv_restrict ae2f_restrict
#include <clspv/Compiler.h>

#if VK_MAX_MEMORY_TYPES > UCHAR_MAX
#error "Sanity check: memory types will not be stored under unsigned char."
#endif


#include <ae2f/Pack/Beg.h>

/** Each of them matches for the pipeline */
typedef enum ae2fVK_eAnnSlpPipes {
	ae2fVK_eAnnSlpPipes_kPredict,
	ae2fVK_eAnnSlpPipes_kFollow,
	ae2fVK_eAnnSlpPipes_kTrain,
	ae2fVK_eAnnSlpPipes_kFit,

	/** @brief This is the end of the value, which typically means the count of actual kernel used. */
	ae2fVK_eAnnSlpPipes_LEN
} ae2fVK_eAnnSlpPipes;

typedef enum ae2fVK_eAnnSlpDescPools {
	ae2fVK_eAnnSlpDescPools_kPredict,
	ae2fVK_eAnnSlpDescPools_kTrain,
	ae2fVK_eAnnSlpDescPools_LEN
} ae2fVK_eAnnSlpDescPools;

typedef enum ae2fVK_eAnnSlpPipeLayouts {
	ae2fVK_eAnnSlpPipeLayouts_kPredict,
	ae2fVK_eAnnSlpPipeLayouts_kTrain,
	ae2fVK_eAnnSlpPipeLayouts_kFit,
	ae2fVK_eAnnSlpPipeLayouts_LEN
} ae2fVK_eAnnSlpPipeLayouts;

typedef enum ae2fVK_eAnnSlpDescLayouts {
	ae2fVK_eAnnSlpDescLayouts_kPredict,
	ae2fVK_eAnnSlpDescLayouts_kTrain,
	ae2fVK_eAnnSlpDescLayouts_LEN
} ae2fVK_eAnnSlpDescLayouts;


ae2f_structdef(struct, ae2fVK_AnnSlp);
ae2f_structdef(struct, ae2fVK_AnnSlpMkAlter_t) {

	/**
	 * @brief
	 * Stack size and allocation count.
	 */
	size_t m_stack, m_alloccount;

	/**
	 * @brief
	 * Pointer to the created SLP.
	 */
	ae2fVK_AnnSlp* ae2f_restrict m_ptr;

	unsigned char		m_i;

	/** ENDOFDEFINITION */
};

ae2f_structdef(union, ae2fVK_AnnSlpMkU0_t) {
	ae2fVK_AnnSlpMkAlter_t	m_alter;
	ae2f_AnnSlpMk_t		m_base;

	/***/
	size_t			m_spirv_len;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkU1_t) {
	VkMemoryAllocateInfo		m_vkmemallocinfo;
	VkBufferCreateInfo		m_vkbufcreatinfo;
	VkDescriptorSetLayoutCreateInfo	m_vkdescsetlayoutcreatinfo;
	VkPushConstantRange		m_vkpushconstrange;

	/***/
	void*				m_spirv;
	char*				m_spirv_c;
	uint32_t*			m_spirv_u32;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMkU2S0_t) {
	VkDescriptorSetLayoutBinding	m_glob, m_loc;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkU2_t) {
	VkMemoryRequirements		m_vkmemreqs;

	/** List dummy for ae2fVK_AnnSlpMkU2_t::m_S0. */
	VkDescriptorSetLayoutBinding	m_vkdescsetlayoutbind[2];

	ae2fVK_AnnSlpMkU2S0_t		m_S0;

	VkPipelineLayoutCreateInfo	m_vkpipelayoutcreatinfo;

	/***/
	char*				m_log;
	VkShaderModuleCreateInfo	m_vkshadermodcreatinfo;
	VkComputePipelineCreateInfo	m_vkcomputepipecreatinfo[ae2fVK_eAnnSlpPipes_LEN];
};

typedef char STATIC_ASSERT_ae2fVK_AnnSlpMkU2_vkdescsetlayoutbind_sz[
	sizeof(ae2fVK_AnnSlpMkU2S0_t) == sizeof(VkDescriptorSetLayoutBinding) * 2
	? 1 : -1
];

ae2f_structdef(union, ae2fVK_AnnSlpMkU3_t) {
	char*	m_openclsrc;
	void*	m_openclsrc_v;	
};

ae2f_structdef(struct, ae2fVK_AnnSlpMk_t) {
	ae2fVK_AnnSlpMkU0_t		m_U0;
	ae2fVK_AnnSlpMkU1_t		m_U1;
	ae2fVK_AnnSlpMkU2_t		m_U2;
	ae2fVK_AnnSlpMkU3_t		m_U3;


	/** @brief error */
	ae2f_err_t			m_reterr;
};

ae2f_structdef(struct, ae2fVK_AnnSlpPredictUnion0WrDescInfo_t) {
	VkDescriptorBufferInfo		m_buf[ae2fVK_eAnnSlpDescPools_LEN];
	VkWriteDescriptorSet		m_wrset;
};

ae2f_structdef(union, ae2fVK_AnnSlpPredictUnion0_t) {
	VkDescriptorSetAllocateInfo		m_vkdescsetallocinfo;
	VkDescriptorBufferInfo			m_vkdescbuffinfo;
	ae2fVK_AnnSlpPredictUnion0WrDescInfo_t	m_vkdescwrdescinfo;
	VkCommandBufferBeginInfo		m_vkcmdbufbeginfo;
};

ae2f_structdef(struct, ae2fVK_AnnSlpDescPoolCmdMk_t) {
	/** Temporary buffer 1 */
	ae2fVK_AnnSlpPredictUnion0_t	m_u0;
};

typedef VkMappedMemoryRange	ae2fVK_AnnSlpUnMap_t, ae2fVK_AnnSlpMapRangedGeneric_t;

ae2f_structdef(struct, ae2fVK_AnnSlpCreatDescPoolU0_t)
{
		VkDescriptorPoolCreateInfo		m_vkdescpoolcreatinfo;
		VkDescriptorPoolSize			m_vkdescpoolsz;
};

ae2f_structdef(struct, ae2fVK_AnnSlpCreatDescPool_t)
{
	ae2fVK_AnnSlpCreatDescPoolU0_t	m_U0;
};

#include <ae2f/Pack/End.h>

#define __ae2fVK_AnnSlpWeightSz_V(CLFLOAT, i_inp, i_out)	(sizeof(CLFLOAT) * (i_inp) * (i_out))
#define __ae2fVK_AnnSlpBiasSz_V(CLFLOAT, i_inp, i_out)		(sizeof(CLFLOAT) * (i_out))
#define __ae2fVK_AnnSlpInpSz_V(CLFLOAT, i_inp, i_out)		(sizeof(CLFLOAT) * (i_inp))
#define __ae2fVK_AnnSlpOutSz_V(CLFLOAT, i_inp, i_out)		(sizeof(CLFLOAT) * (i_out))
#define __ae2fVK_AnnSlpDeltaSz_V(CLFLOAT, i_inp, i_out)		(sizeof(CLFLOAT) * (i_out))
#define __ae2fVK_AnnSlpGoalSz_V(CLFLOAT, i_inp, i_out)		(sizeof(CLFLOAT) * (i_out))

#define __ae2fVK_AnnSlpWeightOff_V(CLFLOAT, i_inp, i_out)	0

#define __ae2fVK_AnnSlpBiasOff_V(CLFLOAT, i_inp, i_out)	\
	(__ae2fVK_AnnSlpWeightSz_V(CLFLOAT, i_inp, i_out) + __ae2fVK_AnnSlpWeightOff_V(CLFLOAT, i_inp, i_out))

#define __ae2fVK_AnnSlpInpOff_V(CLFLOAT, i_inp, i_out)	\
	(__ae2fVK_AnnSlpBiasSz_V(CLFLOAT, i_inp, i_out) + __ae2fVK_AnnSlpBiasOff_V(CLFLOAT, i_inp, i_out))

#define __ae2fVK_AnnSlpOutOff_V(CLFLOAT, i_inp, i_out)	\
	(__ae2fVK_AnnSlpInpSz_V(CLFLOAT, i_inp, i_out) + __ae2fVK_AnnSlpInpOff_V(CLFLOAT, i_inp, i_out))

#define __ae2fVK_AnnSlpDeltaOff_V(CLFLOAT, i_inp, i_out)	\
	(__ae2fVK_AnnSlpOutSz_V(CLFLOAT, i_inp, i_out) + __ae2fVK_AnnSlpOutOff_V(CLFLOAT, i_inp, i_out))
#define __ae2fVK_AnnSlpGoalOff_V(CLFLOAT, i_inp, i_out)	\
	(__ae2fVK_AnnSlpDeltaSz_V(CLFLOAT, i_inp, i_out) + __ae2fVK_AnnSlpDeltaOff_V(CLFLOAT, i_inp, i_out))

#define __ae2fVK_AnnSlpGlobSz_V(CLFLOAT, i_inp, i_out)	\
	(__ae2fVK_AnnSlpGoalSz_V(CLFLOAT, i_inp, i_out) + __ae2fVK_AnnSlpGoalOff_V(CLFLOAT, i_inp, i_out))

#define __ae2fVK_AnnSlpWeightSz(i_inp, i_out)	__ae2fVK_AnnSlpWeightSz_V(ae2f_float_t, i_inp, i_out)	
#define __ae2fVK_AnnSlpBiasSz(i_inp, i_out)	__ae2fVK_AnnSlpBiasSz_V(ae2f_float_t, i_inp, i_out) 
#define __ae2fVK_AnnSlpInpSz(i_inp, ...)	__ae2fVK_AnnSlpInpSz_V(ae2f_float_t, i_inp, i_out) 
#define __ae2fVK_AnnSlpOutSz(i_inp, i_out)	__ae2fVK_AnnSlpOutSz_V(ae2f_float_t, i_inp, i_out) 
#define __ae2fVK_AnnSlpDeltaSz(i_inp, i_out)	__ae2fVK_AnnSlpDeltaSz_V(ae2f_float_t, i_inp, i_out) 
#define __ae2fVK_AnnSlpGoalSz(i_inp, i_out)	__ae2fVK_AnnSlpGoalSz_V(ae2f_float_t, i_inp, i_out)

#define __ae2fVK_AnnSlpWeightOff(i_inp, i_out)	0
#define __ae2fVK_AnnSlpBiasOff(i_inp, i_out)	__ae2fVK_AnnSlpBiasOff_V(ae2f_float_t, i_inp, i_out)
#define __ae2fVK_AnnSlpInpOff(i_inp, i_out)	__ae2fVK_AnnSlpInpOff_V(ae2f_float_t, i_inp, i_out)
#define __ae2fVK_AnnSlpOutOff(i_inp, i_out)	__ae2fVK_AnnSlpOutOff_V(ae2f_float_t, i_inp, i_out) 
#define __ae2fVK_AnnSlpDeltaOff(i_inp, i_out)	__ae2fVK_AnnSlpDeltaOff_V(ae2f_float_t, i_inp, i_out)
#define __ae2fVK_AnnSlpGoalOff(i_inp, i_out)	__ae2fVK_AnnSlpGoalOff_V(ae2f_float_t, i_inp, i_out)

#define __ae2fVK_AnnSlpGlobSz(i_inp, i_out)	__ae2fVK_AnnSlpGlobSz_V(ae2f_float_t, i_inp, i_out)

#endif
