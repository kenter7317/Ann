#ifndef ae2f_MMap_imp_h
#define ae2f_MMap_imp_h

#include "ae2f/Call.h"
#include <ae2f/MMap/MMap.h>
#include <string.h>
#include <stdlib.h>

ae2f_SHAREDEXPORT void ae2f_mMMapSz(
		const size_t dim, const size_t* const lens
		, size_t* const ret) 
{ __ae2f_mMMapSz(dim, lens, ret); }

ae2f_SHAREDEXPORT void ae2f_mMMapFieldIdx(
		const ae2f_mMMap *mmap
		, size_t dim, const size_t *idxs
		, size_t* const ret
		)
{
	__ae2f_mMMapFieldIdx(mmap, dim, idxs, ret);
}

ae2f_SHAREDEXPORT void ae2f_mMMapInit(
		struct ae2f_mMMap* const mmap,
		const size_t dim,
		const size_t* const lens,
		ae2f_err_t* const ret_e,
		size_t* const opt_sz
		)
{
	__ae2f_mMMapInit(mmap, dim, lens, ret_e, opt_sz);
}

ae2f_SHAREDEXPORT 
void ae2f_mMMapMk(
		const size_t dim,
		const size_t* const lens,
		ae2f_err_t* const opt_ret_err,
		ae2f_mMMap** const ret_mmap
		) 
{
	__ae2f_mMMapMk(dim, lens, opt_ret_err, ret_mmap);
}

ae2f_SHAREDEXPORT
void ae2f_mMMapDel(ae2f_mMMap* del) {
	__ae2f_mMMapDel(del);
}

#endif
