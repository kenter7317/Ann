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

#define clspv_restrict restrict
#include <clspv/Compiler.h>

#if VK_MAX_MEMORY_TYPES > UCHAR_MAX
#error "Sanity check: memory types will not be stored under unsigned char."
#endif


#include <ae2f/Pack/Beg.h>

/** Each of them matches for the pipeline */
typedef enum ae2fVK_eAnnSlpPipes {
	ae2fVK_eAnnSlpPipes_kPredict = 0,
	ae2fVK_eAnnSlpPipes_kFollow,
	ae2fVK_eAnnSlpPipes_kTrain,
	ae2fVK_eAnnSlpPipes_kFit,

	/** @brief This is the end of the value, which typically means the count of actual kernel used. */
	ae2fVK_eAnnSlpPipes_LEN
} ae2fVK_eAnnSlpPipes;

typedef enum ae2fVK_eAnnSlpDescPools {
	ae2fVK_eAnnSlpDescPools_kPredict,
	ae2fVK_eAnnSlpDescPools_kFollow = 1,
	ae2fVK_eAnnSlpDescPools_kTrain = 1,
	ae2fVK_eAnnSlpDescPools_kFit = 1,
	ae2fVK_eAnnSlpDescPools_LEN
} ae2fVK_eAnnSlpDescPools;

typedef enum ae2fVK_eAnnSlpPipeLayouts {
	ae2fVK_eAnnSlpPipeLayouts_kPredict = 0,
	ae2fVK_eAnnSlpPipeLayouts_kFollow = 1,
	ae2fVK_eAnnSlpPipeLayouts_kTrain = 1,
	ae2fVK_eAnnSlpPipeLayouts_kFit = 2,
	ae2fVK_eAnnSlpPipeLayouts_LEN
} ae2fVK_eAnnSlpPipeLayouts;

typedef enum ae2fVK_eAnnSlpDescLayouts {
	ae2fVK_eAnnSlpDescLayouts_kPredict = 0,
	ae2fVK_eAnnSlpDescLayouts_kFollow = 1,
	ae2fVK_eAnnSlpDescLayouts_kTrain = 1,
	ae2fVK_eAnnSlpDescLayouts_kFit = 1,
	ae2fVK_eAnnSlpDescLayouts_LEN
} ae2fVK_eAnnSlpDescLayouts;

/**
 * @brief
 * A command, descriptor set for one task.
 * */
ae2f_structdef_n(struct, ae2fVK_AnnSlpCmd_t, ae2fVK_AnnSlpPredictCmd_t) {
	VkDescriptorSet		m_lpvkdescset;
};


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
	ae2fVK_AnnSlp* restrict m_ptr;

	unsigned char		m_i;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkUnion_t) {
	ae2fVK_AnnSlpMkAlter_t	m_alter;
	ae2f_AnnSlpMk_t		m_base;
};

ae2f_structdef(union /*union*/, ae2fVK_AnnSlpMkVKInfos_t) {
	VkBufferCreateInfo		m_buf;
	VkMemoryAllocateInfo		m_alloc;
	VkPipelineLayoutCreateInfo	m_pipelayoutcreat;
	VkShaderModuleCreateInfo	m_shadermodulecreat;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkVK_t) {
	VkMemoryRequirements	m_req;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMkStackLayoutPredict_t) {
	VkDescriptorSetLayoutCreateInfo	m_creatinfo;
	VkDescriptorSetLayoutBinding	m_bind[2];
	VkPushConstantRange		m_pushconstant;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMkStackCreatDescPool) {
	VkDescriptorPoolCreateInfo		m_creatinfo;
	VkDescriptorPoolSize			m_sz;

};

ae2f_structdef(union /*union*/, ae2fVK_AnnSlpMkVKStack_t) {
	VkMemoryRequirements			m_memreq;
	ae2fVK_AnnSlpMkStackLayoutPredict_t	m_layout;
	ClspvError				m_isgood;
	ae2fVK_AnnSlpMkStackCreatDescPool	m_creatdescpool;
};

ae2f_structdef(union, ae2fVK_AnnSlpMkSPtr_t) {
	char* 			m_char;
	char* 			m_char_r;
	void* 			m_void;
	uint32_t* 		m_wrds;
	size_t* 		m_sz;
	ae2f_float_t*		m_float;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMk_t) {
	ae2fVK_AnnSlpMkUnion_t		m_union;
	ae2fVK_AnnSlpMkVKInfos_t		m_vkinfo;
	ae2fVK_AnnSlpMkVKStack_t		m_vkstack;

	ae2fVK_AnnSlpMkSPtr_t		m_clsrc;
	size_t				m_clout_len;
	char*				m_unused;
	ae2fVK_AnnSlpMkSPtr_t		m_clout;
	VkComputePipelineCreateInfo	m_pipecreat[4];

	/** @brief error */
	ae2f_err_t				m_reterr;
};

ae2f_structdef(struct, ae2fVK_AnnSlpPredictUnion0WrDescInfo_t) {
	VkDescriptorBufferInfo		m_buf[2];
	VkWriteDescriptorSet		m_wrset;
};

ae2f_structdef(union, ae2fVK_AnnSlpPredictUnion0_t) {
	VkDescriptorSetAllocateInfo		m_vkdescsetallocinfo;
	VkDescriptorBufferInfo			m_vkdescbuffinfo;
	ae2fVK_AnnSlpPredictUnion0WrDescInfo_t	m_vkdescwrdescinfo;
	VkCommandBufferBeginInfo		m_vkcmdbufbeginfo;
};

ae2f_structdef(struct, ae2fVK_AnnSlpGetCmd_t) {
	/** Temporary buffer 1 */
	ae2fVK_AnnSlpPredictUnion0_t	m_u0;
};


ae2f_structdef(union, ae2fVK_AnnSlpMapPtr_t) {
	ae2f_float_t*	m_f;
	void*		m_v;
};

ae2f_structdef(struct, ae2fVK_AnnSlpMap_t) {
	ae2fVK_AnnSlpMapPtr_t	m_map;
	VkMappedMemoryRange		m_vkmmemr;
};

typedef VkMappedMemoryRange	
ae2fVK_AnnSlpUnMap_t;

#include <ae2f/Pack/End.h>

#define __ae2fVK_AnnSlpMkCLSPV_imp(r_handle, ir_err, i_first, i_third) \
	__ae2fVK_AnnSlpMkCLSPVVerbose_imp(r_handle, ir_err, i_first, ae2fVK_AnnSlpSHADER, i_third)

#endif
