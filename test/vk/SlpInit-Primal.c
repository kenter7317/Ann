#include <ae2fVK/Ann/Slp.h>
#include "../vk.h"
#include "ae2f/Cast.h"


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

int main() {
	Test_VkInit();

	__ae2fVK_AnnSlpMk_imp(
			mk
			, 0, 0, 0
			, 3, 3, 0, 0
			, FakeAct, FakeAct, FakeLoss
			, 0, 0
			, vkdev	
			, vkphydevmemprops
			, NULL
			, (ae2f_reinterpret_cast(volatile const VkBufferCreateInfo* const, 0))
			, ae2f_reinterpret_cast(volatile const VkMemoryAllocateInfo* const, 0)
			);

	assert(mk.m_union.m_alter.m_ptr && "__ae2fVK_AnnSlpMk_imp has failed");

	__ae2fVK_AnnSlpMap(*(mk).m_union.m_alter.m_ptr, &map);
	__ae2fVK_AnnSlpUnMap(*(mk).m_union.m_alter.m_ptr);

	__ae2fVK_AnnSlpClean(*mk.m_union.m_alter.m_ptr);
	free(mk.m_union.m_alter.m_ptr);
	
	Test_VkEnd();
	mk.m_union.m_alter.m_ptr = 0;

	return 0;
}
