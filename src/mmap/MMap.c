#include <ae2f/MMap/MMap.h>
#include <memory.h>
#include <stdlib.h>

ae2f_SHAREDEXPORT
size_t ae2f_mMMapInit(
		struct ae2f_mMMap* mmap,
		size_t dim,
		const size_t* lens,
		ae2f_err_t* e
		)
{

	ae2f_err_t err = 0;
#define return(_) { err = _; goto RETURN; }

	if(!mmap)
		return(ae2f_errGlob_PTR_IS_NULL);
	if(!lens)
		return(ae2f_errGlob_PTR_IS_NULL);

	mmap->dim = dim;
	size_t* const s = ae2f_mMMapDimLen(mmap);

	for(size_t i = 0; i < dim; i++)
		s[i] = lens[i];

	memcpy(ae2f_mMMapDimLen(mmap), lens, dim * sizeof(size_t));

RETURN:
#undef return

	if(e) *e = err;
	return ae2f_mMMapSz(dim, lens);
}

ae2f_SHAREDEXPORT
ae2f_mMMap* ae2f_mMMapMk(
		size_t dim,
		const size_t* lens,
		ae2f_err_t* e
		)
{
#define return(e) { err = e; goto RETURN; }
	ae2f_mMMap* mmap = 0;
	ae2f_err_t err = 0;

	if(!lens) 
		return(ae2f_errGlob_PTR_IS_NULL);

	mmap = calloc(1, ae2f_mMMapSz(dim, lens));
	if(!mmap)
		return(ae2f_errGlob_ALLOC_FAILED);

	ae2f_mMMapInit(mmap, dim, lens, e);
RETURN:
#undef return
	if(e) *e = err;
	return mmap;
}
