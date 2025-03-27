#include <ae2f/Ann/Conv.h>
#include <stdlib.h>
#include <memory.h>

#define ae2f_AnnConv1dSz(in_f, in_g, pad, stride) \
	(((((pad << 1) + *ae2f_mMMapDimLen(in_f, const)) \
	- *ae2f_mMMapDimLen(in_g, const)) \
	/ stride) \
	+ 1)

ae2f_SHAREDEXPORT
ae2f_err_t
ae2f_AnnConv1d(
		const ae2f_mMMap* in_f, 
		const ae2f_mMMap* in_g,
		ae2f_mMMap* out,
		size_t* opt_retoutszrequired,
		size_t stride,
		size_t pad
		)
{
	ae2f_err_t err = 0;

	if(!in_f) return ae2f_errGlob_PTR_IS_NULL;
	if(!in_g) return ae2f_errGlob_PTR_IS_NULL;

	const size_t 
		infc = (pad << 1) + *ae2f_mMMapDimLen(in_f, const)
		, ingc = *ae2f_mMMapDimLen(in_g, const)
		, outc = ((infc - ingc) / stride) + 1;

#define return(n) { err = n; goto _return; }

	if(!out) return(ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_PTR_IS_NULL);
	if(*ae2f_mMMapDimLen(out) < outc) return(ae2f_errGlob_WRONG_OPERATION);

	const ae2f_float_t
		* const		inf_field = ae2f_mMMapField(in_f, const) - pad
		, * const	ing_field = ae2f_mMMapField(in_g, const);

	ae2f_float_t
		* const		out_field = ae2f_mMMapField(out);

	for(size_t i = 0; i < outc; i++)
	{
		ae2f_float_t sum = 0;
		const size_t begi = i * stride;

		for(size_t j = 0; j < ingc; j++)
		{
			const size_t infi = begi + j;

			const ae2f_float_t infel = 
				infi < pad && infi > infc - pad
				? 0 : inf_field[infi];

			sum += infel * ing_field[j];
		}

		out_field[i] = sum;
	}

_return:
#undef return
	if(opt_retoutszrequired) *opt_retoutszrequired = outc;
	return err;
}

ae2f_SHAREDEXPORT
ae2f_err_t
ae2f_AnnConv2d(
		const ae2f_mMMap* in_f, 
		const ae2f_mMMap* in_g,
		ae2f_mMMap* out,
		size_t* opt_retoutszrequired,
		size_t stride,
		size_t pad
		)
{
	ae2f_err_t err = 0;



_return:
#undef return
	if(opt_retoutszrequired) {}
	return err;
}
