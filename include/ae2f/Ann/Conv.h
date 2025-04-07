#ifndef ae2f_Ann_Conv_h
#define ae2f_Ann_Conv_h

#include <ae2f/MMap/MMap.h>
#include <ae2f/errGlob.h>
#include <ae2f/Call.h>
#include <ae2f/Cast.h>

#include <ae2f/Pack/Beg.h>

typedef ae2f_err_t
ae2f_AnnConv_t(
		const ae2f_mMMap* in_f,
		const ae2f_mMMap* in_g,
		ae2f_mMMap* out,
		size_t* opt_retoutszrequired,
		const size_t* stride,
		const size_t* pad
		) noexcept;

ae2f_extern ae2f_SHAREDEXPORT
ae2f_err_t
__ae2f_AnnConv1d(
		const ae2f_float_t* infv,
		size_t infc,
		const ae2f_float_t* ingv,
		size_t ingc,
		ae2f_float_t* outv,
		size_t* opt_outc,
		size_t stride,
		size_t pad
		) noexcept;

ae2f_extern ae2f_SHAREDEXPORT
ae2f_err_t
__ae2f_AnnConv(
		size_t dim, 
		const ae2f_float_t* infv, 
		const size_t* infc,
		size_t infcc,
		const ae2f_float_t* ingv, 
		const size_t* ingc,
		size_t ingcc,
		ae2f_float_t* outv,
		size_t* outc_opt,
		const size_t* stride_opt,
		const size_t* pad_opt
		) noexcept;

typedef struct ae2f_mAnnConv {
	size_t layerc;
} ae2f_mAnnConv;

/** @brief For function pointer */
#define ae2f_mAnnConvFpV(conv, ...) \
	ae2f_reinterpret_cast( \
			__VA_ARGS__ ae2f_AnnConvLayer_t**, \
			(conv) + 1 \
			)

#define ae2f_mAnnConvMMapV(conv, i, ...) \
	ae2f_reinterpret_cast( \
			__VA_ARGS__ ae2f_mMMap*, \
			ae2f_mAnnConvFpV(conv, __VA_ARGS__) \
			)

#include <ae2f/Pack/End.h>

#endif
