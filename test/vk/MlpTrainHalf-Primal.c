#include <ae2fVK/Ann/Mlp.h>
#include "../vk.h"
#include <CL/cl_platform.h>

static ae2fVK_AnnMlpMk_t			s_mk;
static ae2fVK_AnnMlpMapRangedGeneric_t		s_mapranged;
static ae2f_float*				s_ptr0;
static ae2f_float*				s_ptr1;

static void	ActDummy(ae2f_float*, ae2f_float);
static void	LossDummy(ae2f_float* a, const ae2f_float* b, const ae2f_float* c, size_t d, size_t e)
{
	return;
}

static size_t	s_lpModelLen[] = {2, 14, 3, 2, 1};

#define	NULL_GENERIC(T)	((T*)NULL)

int main() {
	Test_VkInit();

	__ae2fVK_AnnMlpMk_imp_V(
			double
			, s_mk
			, NULL_GENERIC(ae2f_float)
			, NULL_GENERIC(ae2f_float)
			, NULL_GENERIC(ae2f_float)
			, NULL_GENERIC(ae2f_float)
			, sizeof(s_lpModelLen) / sizeof(s_lpModelLen[0])
			, s_lpModelLen
			, NULL_GENERIC(size_t)
			, 0, 0
			, NULL_GENERIC(ae2f_AnnAct_t*), NULL_GENERIC(ae2f_AnnAct_t*)
			, LossDummy
			, 0.1, 0.1, vkdev, vkphydevmemprops 
			, NULL_GENERIC(VkAllocationCallbacks)
			, ""
			"#define CL_Q 0\n"
			"#define ACT_DERIV(lidx, r, x, i, c) { *(r) = (((x)[i] + 1e-7) * (1.0 - (x)[i] - 1e-7)); } \n"
			"#define ACT(lidx, r, x, i, c) { *(r) = (1.0 / (1.0 + exp(-(x[i])))); } \n"
			"#define LOSS_DERIV(r, o, t, i, c) { *(r) = ((o)[i] - (t)[i]) / (c); } \n"
			"#pragma OPENCL EXTENSION cl_khr_fp16 : enable\n"
			"#define ae2f_float_t half\n"
			""
			, "\n"
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
			const size_t k1 = j;
			while(j--) {
				size_t k = k1;
				s_ptr1[k1 * i + j] = 0.2;
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
		s_ptr0[0] = 0.281236;
		printf("Goal has set: %f\n", s_ptr0[0]);

		__ae2fVK_AnnMlpUnMapGoal_imp(
				s_mapranged
				, *(s_mk).m_U0.m_mkswap.m_mkbase
				);
	}

	{
		__ae2fVK_AnnMlpMapOutStream_imp(
				s_mapranged
				, *(s_mk).m_U0.m_mkswap.m_mkbase
				, s_mk.m_ret.m_err
				, s_ptr0
				);

		s_ptr0[0] = 12;
		s_ptr0[1] = 13;

		const size_t
			a_depth = (s_mk).m_U0.m_mkswap.m_mkbase->m_mlp.m_depth
			, a_outc = (s_mk).m_U0.m_mkswap.m_mkbase->m_mlp.m_outc;


		printf("OUT INIT FOR TEST: %f\n", (s_ptr0[(a_depth - 1) * a_outc] = 4));

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
		ae2fVK_AnnMlpCreatDescPool_t	v_poolmk;
		ae2fVK_AnnMlpDescPoolCmdMk_t	v_cmdmk;

		ae2fVK_AnnMlpDescPool		v_descpool;
		ae2fVK_AnnMlpDescPoolCmd	v_descpoolcmd;

		/** Getting pool */
		__ae2fVK_AnnMlpDescPoolMk_imp(
				v_poolmk
				, v_descpool
				, s_mk.m_ret.m_err
				, (s_mk.m_U0.m_mkswap.m_mkbase[0])
				, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
				, 1
				);

		/** Getting predict command */
		__ae2fVK_AnnMlpDescPoolCmdMkTrain_imp(
				v_cmdmk
				, (s_mk.m_U0.m_mkswap.m_mkbase[0])
				, s_mk.m_ret.m_err
				, v_descpool
				, v_descpoolcmd
				, vkcmdbuf
				);

		assert(s_mk.m_ret.m_err == ae2f_errGlob_OK);


		VkSubmitInfo submit_info = {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &vkcmdbuf,
		};
		size_t i;

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

			printf("OUT: %f\n", s_ptr0[(a_depth - 1) * (a_outc)]);
			printf("IN: %f %f\n", s_ptr0[0], s_ptr0[1]);


			__ae2fVK_AnnMlpUnMapOutStream_imp(s_mapranged, *(s_mk).m_U0.m_mkswap.m_mkbase);

		}


		for(i = 0; i < 10000; i++)  {
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

		__ae2fVK_AnnMlpDescPoolCmdClean_imp(
				s_mk.m_U0.m_mkswap.m_mkbase[0]
				, v_descpool
				, v_descpoolcmd
				);

		__ae2fVK_AnnMlpDescPoolClean_imp(
				s_mk.m_U0.m_mkswap.m_mkbase[0]
				, v_descpool
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

		printf("OUT: %f\n", s_ptr0[(a_depth - 1) * (a_outc)]);
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
