#ifndef ae2f_MMap_h
#define ae2f_MMap_h

#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <ae2f/Float.h>
#include <ae2f/errGlob.h>
#include <ae2f/Cmp.h>

#include <ae2f/Pack/Beg.h>
#include <ae2f/Macro.h>
#include <ae2f/Guide.h>

#include <assert.h>

/** @brief
 * Multilinear map, or tensor.
 *
 * 
 * */
ae2f_structdef_v(struct, ae2f_mMMap, ae2f_mMMap)
{
	/** @brief aka dimension count */
	size_t m_dim;
};

ae2f_structdef(struct, ae2f_MMap) {
	ae2f_mMMap	mmap;
};

ae2f_extern ae2f_SHAREDCALL
void ae2f_mMMapInit(
		ae2f_mMMap* const mmap
		, const size_t dim
		, const size_t* const lens
		, ae2f_err_t* const opt_ret_err
		, size_t* const opt_ret_sz
		) ae2f_noexcept;


ae2f_extern ae2f_SHAREDCALL
void ae2f_mMMapMk(
		const size_t dim,
		const size_t* const lens,
		ae2f_err_t* const opt_ret_err,
		ae2f_FREE(free, ae2f_mMMapDel) ae2f_mMMap** const ret_mmap
		) ae2f_noexcept;

ae2f_extern ae2f_SHAREDCALL 
void ae2f_mMMapFieldIdx(
		const ae2f_mMMap* const mmap,
		const size_t dim,
		const size_t* const idxs,
		size_t* const ret
		);

ae2f_extern ae2f_SHAREDCALL void ae2f_mMMapSz(
		const size_t dim, const size_t* const lens
		, size_t* const ret);

ae2f_extern ae2f_SHAREDCALL void ae2f_mMMapDel(
		ae2f_mMMap*	mmap
		);

#if !(ae2f_MAC_BUILD && __ae2f_MACRO_GENERATED)

#define ae2f_mMMapInit		__ae2f_mMMapInit
#define ae2f_mMMapMk		__ae2f_mMMapMk
#define ae2f_mMMapDel		__ae2f_mMMapDel
#define ae2f_mMMapSz		__ae2f_mMMapSz
#define ae2f_mMMapFieldIdx	__ae2f_mMMapFieldIdx

#include <string.h>
#include <stdlib.h>

#endif /** Function declarations */


/** @brief Length vector for every dimension index. */
#define ae2f_mMMapDimLen(mmap, ...) \
	ae2f_reinterpret_cast(__VA_ARGS__ size_t*, ((mmap) + 1))

/** @breif Memory field. Indexer needed. */
#define ae2f_mMMapField(mmap, ...) \
	ae2f_reinterpret_cast(__VA_ARGS__ ae2f_float_t*, ae2f_mMMapDimLen(mmap, __VA_ARGS__) + (mmap)->m_dim)


ae2f_structdef(struct, ae2f_mMMapSz_t) {
	/** @brief output */
	size_t m_ret;
	size_t m_dim;
};

ae2f_structdef(struct, ae2f_mMMapFieldIdx_t) {
	size_t m_ret;
	size_t m_i;
	size_t m_dim;

	size_t m_L, m_I;
};

ae2f_structdef(struct, ae2f_mMMapInit_t) {
	size_t m_i, m_ret;
};

ae2f_structdef(struct, ae2f_mMMapMk_t) {
	ae2f_mMMap* m_ptr;
	ae2f_mMMapInit_t m_initstack;
};

#include <ae2f/Pack/End.h>

#endif


#ifndef ae2f_MMap_c

#if !__ae2f_MACRO_GENERATED
#include <ae2f/MMap/MMap.h>
#endif

#define ae2f_MMap_c


ae2f_MAC() _ae2f_mMMapSz_imp(
		ae2f_mMMapSz_t v_mapsz
		, const size_t dim
		, const size_t* const lens
		)
{
	assert(dim);
	assert(lens);

	v_mapsz.m_ret = 1;
	v_mapsz.m_dim = (dim);

	while(v_mapsz.m_dim--) {
		v_mapsz.m_ret *= (lens)[(v_mapsz).m_dim];
	}

	assert(v_mapsz.m_dim == -1);

	v_mapsz.m_ret = 
		v_mapsz.m_ret * sizeof(ae2f_float_t)
		+	((dim)) * sizeof(size_t)
		+	sizeof(ae2f_MMap);


}

ae2f_MAC() _ae2f_mMMapSz(const size_t dim, const size_t* const lens
		, size_t* const ret)
{
	unless(lens) {
		assert(0 && "lens is not good");
		(ret) && (*(ret) = -1);
	} else {
		ae2f_mMMapSz_t v_sz;
		__ae2f_mMMapSz_imp(v_sz, dim, lens);
		(ret) && (*(ret) = v_sz.m_ret);
	}
}

#if ae2f_WhenCXX(!)0
/**
 * @brief
 * Calculates the memory length needed in bytes.
 *
 * @param dim
 * Dimension, aka length of lens.
 *
 * @param lens
 * Length vector.
 *
 * */
constextendedfun size_t _ae2f_mMMapSz(
		size_t dim, 
		const size_t* lens
		) ae2f_noexcept
{
	if(!(lens)) {
		return -1;
	}

	ae2f_mMMapSz_t v_sz;
	__ae2f_mMMapSz_imp(v_sz, dim, lens);
	return v_sz.m_ret;
}

#endif


ae2f_MAC() _ae2f_mMMapFieldIdx_imp(
		ae2f_mMMapFieldIdx_t v_fidx,
		const ae2f_mMMap* const mmap,
		const size_t dim,
		const size_t* const idxs
		)
{
	assert((mmap) && "mmap is null");
	assert((idxs));

	v_fidx.m_L = 1;
	v_fidx.m_dim = ae2f_CmpGetLs(dim, (mmap)->m_dim);
	v_fidx.m_ret = 0;

	for(v_fidx.m_i = 0; v_fidx.m_i < v_fidx.m_dim - 1 && v_fidx.m_L; v_fidx.m_i++) {
		v_fidx.m_L = ae2f_mMMapDimLen(mmap)[v_fidx.m_dim - v_fidx.m_i - 1];
		v_fidx.m_I = (idxs)[v_fidx.m_dim - 1 - v_fidx.m_i] % v_fidx.m_L;
		v_fidx.m_ret += v_fidx.m_I;
		v_fidx.m_ret *= v_fidx.m_L;
	}

	if(v_fidx.m_i < v_fidx.m_dim - 1) 
	{
		assert(0 && "who are you");
		v_fidx.m_ret = -1;
	}

	else {
		v_fidx.m_ret += (idxs)[(dim) - 1 - v_fidx.m_i] % ae2f_mMMapDimLen(mmap)[
			v_fidx.m_dim 
				- v_fidx.m_i 
				- 1
		];

		v_fidx.m_dim = (mmap)->m_dim;

		for(; v_fidx.m_i < v_fidx.m_dim - 1; v_fidx.m_i++) {
			v_fidx.m_ret *= ae2f_mMMapDimLen(mmap)[v_fidx.m_dim - 1 - v_fidx.m_i];
		}
	}
}

ae2f_MAC() _ae2f_mMMapFieldIdx(
		const ae2f_mMMap* const mmap,
		const size_t dim,
		const size_t* const idxs,
		size_t* const ret
		)
{
	unless(ret)
		;
	unless((idxs) && (mmap)) {
		assert(0 && "nullptr from __ae2f_mMMapFieldIdx");
		*(ret) = -1;
	}
	else {
		ae2f_mMMapFieldIdx_t v_idx;
		__ae2f_mMMapFieldIdx_imp(v_idx, mmap, dim, idxs);
		*(ret) = v_idx.m_ret;
	}
}


#if ae2f_WhenCXX(!)0

// 1, 2, ..... n
constextendedfun size_t _ae2f_mMMapFieldIdx(
		const ae2f_mMMap* mmap,
		size_t dim,
		const size_t* idxs
		) ae2f_noexcept
{
	if(!idxs) return -1;
	if(!mmap) return -1;

	ae2f_mMMapFieldIdx_t v_idx;
	__ae2f_mMMapFieldIdx_imp(v_idx, mmap, dim, idxs);

	return v_idx.m_ret; // / lens[i - 1];
}

#endif

ae2f_MAC() _ae2f_mMMapInit_imp(
		ae2f_mMMapInit_t v_init,
		struct ae2f_mMMap* const mmap,
		const size_t dim,
		const size_t* const lens
		)
{
	(mmap)->m_dim = (dim);
	for(v_init.m_i = 0; v_init.m_i < (dim); v_init.m_i++)
		ae2f_mMMapDimLen(mmap)[v_init.m_i] = (lens)[v_init.m_i];

	memcpy(ae2f_mMMapDimLen(mmap), lens, (dim) * sizeof(size_t));
	__ae2f_mMMapSz((dim), lens, &v_init.m_ret);
}

ae2f_MAC() _ae2f_mMMapInit(
		struct ae2f_mMMap* const mmap,
		const size_t dim,
		const size_t* const lens,
		ae2f_err_t* const ret_e,
		size_t* const opt_sz
		)
{
	if((ret_e) && *(ret_e))
		;
	else unless((mmap) && (lens))
		(ret_e) && (*(ret_e) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		ae2f_mMMapInit_t v_init;
		__ae2f_mMMapInit_imp(v_init, mmap, dim, lens);
		(opt_sz) && (*(opt_sz) = v_init.m_ret);
	}
}

ae2f_MAC() _ae2f_mMMapMk_imp(ae2f_mMMapMk_t v_mk, const size_t dim, const size_t* const lens) {
	assert((lens) && "lens is null");
	__ae2f_mMMapSz((dim), (lens), (&v_mk.m_initstack.m_ret));

	assert((v_mk.m_initstack.m_ret != -1) && "Sz has failed.");
	assert((v_mk.m_initstack.m_ret > sizeof(ae2f_mMMap)) && "Sz is fishy.");


	if((v_mk.m_ptr = calloc(1, v_mk.m_initstack.m_ret))) {
		__ae2f_mMMapInit_imp(v_mk.m_initstack, (v_mk.m_ptr), dim, lens);
	} else {
		assert(0 && "allocation has failed");
	}
}

ae2f_MAC() _ae2f_mMMapMk(
		const size_t dim,
		const size_t* const lens,
		ae2f_err_t* const opt_ret_err,
		ae2f_mMMap** const ret_mmap
		)
{
	if(opt_ret_err && *(opt_ret_err));
	else unless((lens) && (ret_mmap))
		(opt_ret_err) && (*(opt_ret_err) |= ae2f_errGlob_PTR_IS_NULL);
	else {
		ae2f_mMMapMk_t v_mk;
		__ae2f_mMMapMk_imp(v_mk, dim, lens);

		assert(v_mk.m_ptr);
		unless(v_mk.m_ptr) {
			(opt_ret_err) && (*(opt_ret_err) |= ae2f_errGlob_ALLOC_FAILED);
		}

		*(ret_mmap) = v_mk.m_ptr;
	}
}

ae2f_MAC() _ae2f_mMMapDel(void* a) {
	free(a);
}

#endif
