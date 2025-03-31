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

typedef struct {
	size_t dimi;
	size_t i, j, outlen, inglen;
} ConvPrm_t;

struct {
	size_t 
		pad,
		dim,
		* opt_retoutszrequired;

	const size_t
		* stride,
		* lpinfc,
		* lpingc,
		* lpoutc;

	const ae2f_float_t
		* infv,
		* ingv;

	ae2f_float_t
		* outv;
	
	// Down here is no parameters
	ae2f_err_t err;

}* ConvRet;

/**
 * @brief 
 * `dim` must be the dimension of mmaps, lengths of lists.
 * This function is meant to be recursive. For minimize the stack, 
 * */
ae2f_SHAREDEXPORT
void
__ae2f_AnnConv(ConvPrm_t* prm)
{
	if(!ConvRet) return;
	if(!prm) {
		// no good, prm is null
		ConvRet->err |= ae2f_errGlob_PTR_IS_NULL;
		ConvRet = 0;
		return;
	}

	ConvRet->err = ae2f_errGlobNormalised(ConvRet->err);
	if(ConvRet->err) return;

#define return(e) { ConvRet->err |= e; return; }

	if(!ConvRet->stride)		return(ae2f_errGlob_PTR_IS_NULL);

	if(!ConvRet->lpinfc)		return(ae2f_errGlob_PTR_IS_NULL);
	if(!ConvRet->lpingc)		return(ae2f_errGlob_PTR_IS_NULL);
	if(!ConvRet->lpoutc)		return(ae2f_errGlob_PTR_IS_NULL);

	if(!ConvRet->infv)		return(ae2f_errGlob_PTR_IS_NULL);
	if(!ConvRet->ingv)		return(ae2f_errGlob_PTR_IS_NULL);
	if(!ConvRet->outv)		return(ae2f_errGlob_PTR_IS_NULL);

	if(ConvRet->dim - prm->dimi == 1)
	{
		// It is just single layer.
		// Ending by calling 1d.
		ConvRet->err = ae2f_AnnConv1d(
				ConvRet->infv,		*ConvRet->lpinfc
				, ConvRet->ingv,	*ConvRet->lpingc
				, ConvRet->outv,	ConvRet->opt_retoutszrequired
				, *ConvRet->stride,	ConvRet->pad
				);

		// operation is over
		ConvRet->prm = 0;
		return;
	}

	if(prm->out.dim < prm->dim) return (ae2f_errGlob_WRONG_OPERATION);

	prm->outlen = ConvRet->pad - prm->out.c[prm->dim - 1];
	prm->inglen = ConvRet->pad - prm->ing.c[prm->dim - 1];

	for(prm->i = 0; prm->i < prm->outlen; prm->i++)
	{

		for(prm->j = 0; prm->j < prm->inglen; prm->j++)
		{

		}
	}
}
