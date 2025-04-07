#include <ae2f/Ann/Conv.h>
#include <stdlib.h>
#include <memory.h>

#define ae2f_AnnConv1dSz(in_f, in_g, pad, stride) \
	(((((pad << 1) + *ae2f_mMMapDimLen(in_f, const)) \
	- *ae2f_mMMapDimLen(in_g, const)) \
	/ stride) \
	+ 1)

/// All vectors are suggested initiated as 0.
ae2f_SHAREDEXPORT
ae2f_err_t
ae2f_AnnConv1d(
		const ae2f_float_t* infv,
		size_t infc,
		const ae2f_float_t* ingv,
		size_t ingc,
		ae2f_float_t* outv,
		size_t* opt_outc,
		size_t stride,
		size_t pad
		)
{
	ae2f_err_t err = 0;

	const size_t outc = (((infc - ingc) / stride) + 1);
#define return(n) { err = n; goto _return; }

	if(!outv) return(ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_PTR_IS_NULL);
	if(outc <= pad << 1) return (ae2f_errGlob_OK);

	infv = infv - pad;

	for(size_t i = 0; i < outc; i++)
	{
		ae2f_float_t sum = 0;
		const size_t begi = i * stride;

		for(size_t j = 0; j < ingc; j++)
		{
			const size_t infi = begi + j;

			const ae2f_float_t infel = 
				infi < pad && infi > infc - pad
				? 0 : infv[infi];

			sum += infel * ingv[j];
		}

		outv[i] = sum;
	}

_return:
#undef return
	if(opt_outc) *opt_outc = outc;
	return err;
}

/**
 * @todo
 * Indexing
 *
 * @brief 
 * `dim` must be the dimension of mmaps, lengths of lists.
 * This function is meant to be recursive. For minimize the stack,
 *
 * @param prm
 * Meant to be a vector.
 * Size... unknown.
 * */
ae2f_SHAREDEXPORT
ae2f_err_t
__ae2f_AnnConv(
		const ae2f_mMMap* inf, 
		const ae2f_mMMap* ing, 
		ae2f_mMMap* out,
		size_t* opt_outc,
		size_t* stride_opt,
		size_t* pad_opt
		)
{
	ae2f_err_t e = 0;
	if(!inf)	return ae2f_errGlob_PTR_IS_NULL;
	if(!ing)	return ae2f_errGlob_PTR_IS_NULL;
	if(!out)	return ae2f_errGlob_PTR_IS_NULL;

	if(inf->dim != ing->dim || ing->dim != out->dim || out->dim != inf->dim)
		return ae2f_errGlob_WRONG_OPERATION;

	const size_t 
		stride = stride_opt ? stride_opt[ing->dim - 1] : 1
		, pad = pad_opt ? pad_opt[ing->dim - 1] : 0
		, opaddedlast = (
				ae2f_mMMapDimLen(inf, const)[ing->dim - 1] 
				+ (pad << 1) 
				- ae2f_mMMapDimLen(ing, const)[ing->dim - 1]
				) / (stride + 1);

	if(opt_outc) 
		opt_outc[ing->dim - 1] = opaddedlast;

	for(size_t i = 0; i < ae2f_mMMapDimLen(out, const)[inf->dim - 1]; i++) {
		for(size_t j = 0; j < ae2f_mMMapDimLen(ing, const)[inf->dim - 1]; j++) {

		}
	}

	return e;
}
