#include <ae2fVK/Ann/Mlp.h>
#include "../vk.h"

static ae2fVK_AnnMlpMk_t s_mk;
static void	LossDummy(ae2f_float* a, const ae2f_float* b, const ae2f_float* c, size_t d, size_t e)
{
	return;
}

static size_t	s_lpModelLen[] = {2, 4, 4, 1};

#define	NULL_GENERIC(T)	((T*)NULL)

int main() {
	Test_VkInit();

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
			, 0.1, 0.1, vkdev, vkphydevmemprops 
			, NULL_GENERIC(VkAllocationCallbacks)
			, "", ""
			);

	__ae2fVK_AnnMlpClean_imp(*((s_mk).m_U0.m_mkswap.m_mkbase));
	free((s_mk).m_U0.m_mkswap.m_mkbase);
	Test_VkEnd();
	return 0;
}
