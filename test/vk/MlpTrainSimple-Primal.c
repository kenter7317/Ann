#include <ae2fVK/Ann/Mlp.h>
#include "../vk.h"

static ae2fVK_AnnMlpMk_t			s_mk;
static ae2fVK_AnnMlpMapRangedGeneric_t		s_mapranged;
static ae2f_float*				s_ptr0;
static ae2f_float*				s_ptr1;
static ae2fVK_AnnSlpGetCmd_t			s_getcmd;

static void	ActDummy(ae2f_float*, ae2f_float);
static void	LossDummy(ae2f_float* a, const ae2f_float* b, const ae2f_float* c, size_t d, size_t e)
{
	return;
}

static size_t	s_lpModelLen[] = {2, 4, 4, 1};

#define	NULL_GENERIC(T)	((T*)NULL)

int main() {
	Test_VkInit();

	// __ae2fVK_AnnMlpMk_imp(v_mk, iv_weight_opt, iv_bias_opt, iv_outstream_opt, iv_deltastream_opt, i_len_count, i_len, i_len_swap, i_prm_offset, i_extra, i_act, i_actderiv, i_lossderiv, i_learningrate, i_learningrate_bias, i_vkdev, i_vkmemprops, iv_vkalloccalls, i_vkcldeclaration, i_vkcldefinition)
	__ae2fVK_AnnMlpMk_imp(
			s_mk
			, NULL_GENERIC(ae2f_float)
			, NULL_GENERIC(ae2f_float)
			, NULL_GENERIC(ae2f_float)
			, NULL_GENERIC(ae2f_float)
			, 4, s_lpModelLen, NULL_GENERIC(size_t)
			, 0, 0
			, NULL_GENERIC(ae2f_AnnAct_t*), NULL_GENERIC(ae2f_AnnAct_t*)
			, LossDummy
			, 0.2, 0.2, vkdev, vkphydevmemprops 
			, NULL_GENERIC(VkAllocationCallbacks)
			, "", ""
			);

	{
		__ae2fVK_AnnMlpMapWB_imp(
				s_mapranged
				, *(s_mk).m_U0.m_mkswap.m_mkbase
				, (s_mk).m_ret.m_err
				, s_ptr0
				, s_ptr1
				);

		assert(s_ptr0);
		assert(s_ptr1);

		size_t i = (s_mk).m_U0.m_mkswap.m_mkbase->m_mlp.m_depth - 1;

		while(i--) {
			size_t j = (s_mk).m_U0.m_mkswap.m_mkbase->m_mlp.m_outc;
			while(j--) {
				const size_t k1 = (s_mk).m_U0.m_mkswap.m_mkbase->m_mlp.m_outc;
				size_t k = k1;
				s_ptr1[k1 * i + j] = 0.5;
				while(k--) {
					s_ptr0[k1 * k1 * i + k1 * k + j] = 0.5;
				}
			}
		}

		__ae2fVK_AnnMlpUnMapWB_imp(
				s_mapranged
				, *(s_mk).m_U0.m_mkswap.m_mkbase
				);
	}

	{
		__ae2fVK_AnnMlpMapGoal_imp(
				s_mapranged
				, (*(s_mk).m_U0.m_mkswap.m_mkbase)
				, (s_mk).m_ret.m_err
				, s_ptr0
				);

		assert(s_ptr0);
		s_ptr0[0] = 0.2862;
		printf("Goal has set: %f\n", s_ptr0[0]);

		__ae2fVK_AnnMlpUnMapGoal_imp(
				s_mapranged
				, *(s_mk).m_U0.m_mkswap.m_mkbase
				)
	}

	{
		__ae2fVK_AnnMlpMapOutStream_imp(
				s_mapranged
				, *(s_mk).m_U0.m_mkswap.m_mkbase
				, s_mk.m_ret.m_err
				, s_ptr0
				);

		s_ptr0[0] = 13;
		s_ptr0[1] = 11;

		__ae2fVK_AnnMlpUnMapOutStream_imp(s_mapranged, *(s_mk).m_U0.m_mkswap.m_mkbase);
	}

	VkCommandBuffer	vkcmdbuf = NULL;
	VkCommandPool	vkcmdpool = NULL;
	{
		VkCommandPoolCreateInfo	vkcmdpoolcreatinfo;
		vkcmdpoolcreatinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkcmdpoolcreatinfo.pNext = NULL;
		vkcmdpoolcreatinfo.queueFamilyIndex = 0;
		vkcmdpoolcreatinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

		if((((s_mk).m_U0.m_mkswap.m_mkbase->m_vkres) = vkCreateCommandPool(
						vkdev
						, &vkcmdpoolcreatinfo
						, NULL
						, &vkcmdpool
						)) != VK_SUCCESS)
		{
			printf("Errval: %d\n", (s_mk.m_U0.m_mkswap.m_mkbase)->m_vkres);
			assert(!"vkCreateCommandPool has failed.");
			return -1;
		}

		unless(vkcmdpool) {
			assert(!"vkCreateCommandPool went null");
			return -1;
		}
	}

	{
		VkCommandBufferAllocateInfo vkcmdallocinfo;
		vkcmdallocinfo.commandBufferCount = 1;
		vkcmdallocinfo.pNext = NULL;
		vkcmdallocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		vkcmdallocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		vkcmdallocinfo.commandPool = vkcmdpool;

		if((s_mk.m_U0.m_mkswap.m_mkbase->m_vkres = 
					vkAllocateCommandBuffers(
						(vkdev)
						, &vkcmdallocinfo
						, &vkcmdbuf
						)) != VK_SUCCESS)
		{
			printf("Errval: %d\n", (s_mk.m_U0.m_mkswap.m_mkbase)->m_vkres);
			assert(!"vkAllocateCommandBuffers has failed.");
			return -1;
		}

		unless((vkcmdbuf))
		{
			printf("Errval: %d\n", (s_mk.m_U0.m_mkswap.m_mkbase)->m_vkres);
			assert(!"vkAllocateCommandBuffers went null.");
			return -1;
		}
	}

	{
		ae2fVK_AnnSlpCmd_t	cmd;
		
		assert((s_mk).m_U0.m_mkswap.m_mkbase->m_vkdescpool[0]);
		assert(vkdev);


		(vkResetDescriptorPool((vkdev), s_mk.m_U0.m_mkswap.m_mkbase->m_vkdescpool[0], 0));

		__ae2fVK_AnnMlpTrainPerformed_imp(
				s_getcmd
				, cmd
				, vkcmdbuf
				, NULL
				, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
				, (*(s_mk).m_U0.m_mkswap.m_mkbase)
				, (s_mk).m_ret.m_err
				);

		VkSubmitInfo submit_info = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &vkcmdbuf,
		};
		size_t i;

		for(i = 0; i < 1000; i++)  {
			if (vkQueueSubmit(
						vkqueue
						, 1
						, &submit_info
						, VK_NULL_HANDLE
					 ) != VK_SUCCESS)
			{
				assert(!"vkQueueSubmit failed");
				return -1;
			}

			if (vkQueueWaitIdle(vkqueue) != VK_SUCCESS) {
				assert(!"vkQueueWaitIdle failed");
				return -1;
			}
		}

		__ae2fVK_AnnMlpPerformedFree_imp(
				(*(s_mk).m_U0.m_mkswap.m_mkbase)
				, cmd
				);
	}


	{
		__ae2fVK_AnnMlpMapOutStream_imp(
				s_mapranged
				, *(s_mk).m_U0.m_mkswap.m_mkbase
				, s_mk.m_ret.m_err
				, s_ptr0
				);

		const size_t
			a_depth = (s_mk).m_U0.m_mkswap.m_mkbase->m_mlp.m_depth
			, a_outc = (s_mk).m_U0.m_mkswap.m_mkbase->m_mlp.m_outc;

		printf("OUT: %f\n", s_ptr0[(a_depth) * a_outc]);
		printf("IN: %f %f\n", s_ptr0[0], s_ptr0[1]);


		__ae2fVK_AnnMlpUnMapOutStream_imp(s_mapranged, *(s_mk).m_U0.m_mkswap.m_mkbase);
	}

	vkFreeCommandBuffers(vkdev, vkcmdpool, 1, &vkcmdbuf);
	vkDestroyCommandPool(vkdev, vkcmdpool, NULL);

	__ae2fVK_AnnMlpClean_imp(*((s_mk).m_U0.m_mkswap.m_mkbase));
	free((s_mk).m_U0.m_mkswap.m_mkbase);
	Test_VkEnd();
	return 0;
}
