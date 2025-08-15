#include <ae2fVK/Ann/Slp.h>
#include <vulkan/vulkan_core.h>
#include "../vk.h"
#include "ae2f/Cast.h"
#include "ae2f/errGlob.h"

#include <string.h>


#if 1

ae2fVK_AnnSlpMk_t	mk;

size_t			mapinp;

ae2f_float_t FakeAct(ae2f_float_t v) { return 0; }

ae2f_float_t FakeLoss (
		const ae2f_float_t* out, 
		const ae2f_float_t* goal,
		size_t index,
		size_t count
		) { return 0; }

ae2f_float_t*	map;

// #define fgetc(...)

int main() {
	Test_VkInit();
        memset(&(mk), 0, sizeof(mk));

	__ae2fVK_AnnSlpMk_imp(
                        mk,
			0, 0, 0
			, 3, 3, 0, 0
			, FakeAct, FakeAct, FakeLoss
			, 0, 0
			, vkdev
			, vkphydevmemprops
			, NULL

			, 
			"#define LOSS_DERIV(y, y_desired, i, c) 0\n"
			"#define ACT(x) 999\n"
			, "/** This is also a comment */\n"
			);

	puts("__ae2fVK_AnnSlpMk_imp is done");
	fgetc(stdin);

	assert(mk.m_union.m_alter.m_ptr->m_slp.m_Slp[0].m_inc == 3);
	assert(mk.m_union.m_alter.m_ptr->m_slp.m_Slp[0].m_outc == 3);

	printf("Stateval: %d\n", (mk).m_union.m_alter.m_ptr->m_vkres);

	assert(mk.m_union.m_alter.m_ptr && "__ae2fVK_AnnSlpMk_imp has failed");
	assert(mk.m_reterr == ae2f_errGlob_OK);

	assert(mk.m_union.m_alter.m_ptr->m_vkres == VK_SUCCESS);

	{
		ae2f_err_t err = 0;
		ae2fVK_AnnSlpPredictUnMapOutput_t unmapoutput;
		ae2f_float_t*	OutputMapped = NULL;;

	__ae2fVK_AnnSlpPredictMapOutput_imp(unmapoutput, err, mk.m_union.m_alter.m_ptr[0], &OutputMapped, vkqueue);
		puts("Before OutputMapped written");
		fgetc(stdin);

		assert(OutputMapped);
		assert((mk.m_union.m_alter.m_ptr[0].m_vkres) == VK_SUCCESS);
		OutputMapped[0] = 3;
		OutputMapped[1] = 3;
		OutputMapped[2] = 3;
		printf("Outputmapped before predict: %f %f %f\n", OutputMapped[0], OutputMapped[1], OutputMapped[2]);
		fgetc(stdin);
		__ae2fVK_AnnSlpPredictUnMapOutput_imp(unmapoutput, mk.m_union.m_alter.m_ptr[0], vkqueue);

		puts("After __ae2fVK_AnnSlpPredictUnMapOutput_imp.");
		fgetc(stdin);
	}

	VkCommandBuffer	vkcmdbuf = NULL;
	VkCommandPool	vkcmdpool = NULL;

	{
		VkCommandPoolCreateInfo	vkcmdpoolcreatinfo;
		vkcmdpoolcreatinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkcmdpoolcreatinfo.pNext = NULL;
		vkcmdpoolcreatinfo.queueFamilyIndex = 0;

		if((((mk).m_union.m_alter.m_ptr->m_vkres) = vkCreateCommandPool(
						vkdev
						, &vkcmdpoolcreatinfo
						, NULL
						, &vkcmdpool
						)) != VK_SUCCESS)
		{
			printf("Errval: %d\n", (mk).m_union.m_alter.m_ptr->m_vkres);
			assert(!"vkCreateCommandPool has failed.");
			return -1;
		}

		puts("After vkCreateCommandPool.");
		fgetc(stdin);

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

		if((mk.m_union.m_alter.m_ptr->m_vkres = 
					vkAllocateCommandBuffers(
						(vkdev)
						, &vkcmdallocinfo
						, &vkcmdbuf
						)) != VK_SUCCESS)
		{
			printf("Errval: %d\n", (mk).m_union.m_alter.m_ptr->m_vkres);
			assert(!"vkAllocateCommandBuffers has failed.");
			return -1;
		}


		puts("After vkAllocateCommandBuffers.");
		fgetc(stdin);

		unless((vkcmdbuf)) 
		{
			printf("Errval: %d\n", (mk).m_union.m_alter.m_ptr->m_vkres);
			assert(!"vkAllocateCommandBuffers went null.");
			return -1;
		}
	}

	{
		ae2fVK_AnnSlpPredict_t v_predict;
		ae2f_float_t	inp[] = {1, 2, 3};
		ae2fVK_AnnSlpPredictCmd_t	v_cmd;
		ae2f_err_t err = 0;

		puts("Before __ae2fVK_AnnSlpPredictPerformed_imp");

		__ae2fVK_AnnSlpPredictPerformed_imp(
				v_predict
				, v_cmd
				, vkcmdbuf
				, inp
				, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
				, 1
				, (mk.m_union.m_alter.m_ptr[0])
				, err
				);

		puts("After __ae2fVK_AnnSlpPredictPerformed_imp");
		fgetc(stdin);

		if(err != ae2f_errGlob_OK) {
			if (err & ae2f_errGlob_NFOUND)
				printf(
						"Vulkan state number %d: \n"
						, mk.m_union.m_alter.m_ptr->m_vkres
				      );

			printf("Error value %x, \n", err);
			assert(!"__ae2fVK_AnnSlpPredictPerformed_imp has failed.");
			return -1;
		}

		unless((v_cmd).m_lpvkdescset) {
			assert(!"__ae2fVK_AnnSlpPredictPerformed_imp went null.");
			return -1;
		}

		{
			VkSubmitInfo submit_info = {
				.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
				.commandBufferCount = 1,
				.pCommandBuffers = &vkcmdbuf,
			};

			ae2fVK_AnnSlpPredictUnMapOutput_t unmapoutput;

			ae2f_float_t*	OutputMapped = NULL;

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

			puts("After vkQueueSubmit");
			fgetc(stdin);

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

			puts("After vkQueueSubmit 2");
			fgetc(stdin);

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

			puts("After vkQueueSubmit 3");
			fgetc(stdin);

			if (vkQueueWaitIdle(vkqueue) != VK_SUCCESS) {
				assert(!"vkQueueWaitIdle failed");
				return -1;
			}

			puts("After vkQueueWaitIdle");
			fgetc(stdin);

			/** Check is required, but not planned to write for no */
			OutputMapped = NULL;
			__ae2fVK_AnnSlpPredictMapOutput_imp(unmapoutput, err, mk.m_union.m_alter.m_ptr[0], &OutputMapped, vkqueue);
			assert(OutputMapped);
			assert((mk.m_union.m_alter.m_ptr[0].m_vkres) == VK_SUCCESS);
			printf("Outputmapped after prediction %f %f %f\n", OutputMapped[0], OutputMapped[1], OutputMapped[2]);
			__ae2fVK_AnnSlpPredictUnMapOutput_imp(unmapoutput, mk.m_union.m_alter.m_ptr[0], vkqueue);
		}

		__ae2fVK_AnnSlpPredictFree_imp(
				mk.m_union.m_alter.m_ptr[0]
				, v_cmd
				);
	}

	__ae2fVK_AnnSlpClean_imp((*mk.m_union.m_alter.m_ptr));
	vkFreeCommandBuffers(vkdev, vkcmdpool, 1, &vkcmdbuf);
	vkDestroyCommandPool(vkdev, vkcmdpool, NULL);

	memset(mk.m_union.m_alter.m_ptr, 0, sizeof(*mk.m_union.m_alter.m_ptr));
	free(mk.m_union.m_alter.m_ptr);

	memset(&mk, 0, sizeof(mk));
	Test_VkEnd();
	return 0;
}

#else

int main() {
	return 0;
}

#endif
