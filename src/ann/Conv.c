#include <ae2f/Ann/Conv.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>

#define ae2f_AnnConv1dSz(in_f, in_g, pad, stride) \
	(((((pad << 1) + *ae2f_mMMapDimLen(in_f, const)) \
	- *ae2f_mMMapDimLen(in_g, const)) \
	/ stride) \
	+ 1)

inline static size_t getIndex(const ae2f_float_t* x, const size_t* xi,  const size_t* xc, size_t dim) {
    int idx = 0;
    for (int i = 0; i < dim; i++) {
        idx += xi[i] * xc[i];
    }
    return idx;
}

/** 
 * @brief
 * all vectors are suggested initiated as 0. 
 * */
ae2f_SHAREDEXPORT
ae2f_err_t
ae2f_AnnCnnConv1d(
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

	if(!stride) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_WRONG_OPERATION;

	const size_t outc = ((infc - ingc + (pad << 1)) / (stride)) + 1;
#define return(n) { err = n; goto _return; }

	if(!outv) return(ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_PTR_IS_NULL);
	if(outc <= pad << 1) return (ae2f_errGlob_OK);

	for(size_t i = 0; i < outc; i++)
	{
		ae2f_float_t sum = 0;
		const size_t begi = i * stride;

		for(size_t j = 0; j < ingc; j++)
		{
			const size_t infi = begi + j;

			/* If infv is padded so it went to zero. */
			if(infi < pad || infi >= infc + pad) {
				continue;
			}

			/* If one of them are zero for nullptr reason */
			if(!(infv && ingv)) {
				continue;
			}

			sum += 
				infv[(infi - pad)] * ingv[j];
		}

		/**
		 * It adds the sum value, expecting that outv is initialised with 0,
		 * ...or it is called on loop and needs to added somehow.
		 * */
		outv[i] += sum;
	}
_return:
#undef return
	if(opt_outc) *opt_outc = outc;
	return err;
} 


ae2f_extern ae2f_SHAREDEXPORT
ae2f_err_t ae2f_AnnCnnPool1d(
		const ae2f_float_t* inv,
		const size_t* inc,
		ae2f_float_t* outv,
		const size_t* outc,
		const size_t pool_size,
		const size_t stride_opt
		) noexcept
{
	
}

/**
 * @brief 
 * `dim` must be the dimension of mmaps, lengths of lists.
 * This function is meant to be recursive. For minimize the stack,
 *
 * @param infcc
 * All elements in [infc] timed.
 * If you set it to zero, it will calculate it for you.
 *
 * @param ingcc
 * All elemtns in [ingc] timed.
 * If you set it to zero, it will calculate it for you.
 *
 * @param outcc
 * All elemtns in [outc] timed.
 * You need to set it to zero. it will calculate it for you.
 * */
ae2f_SHAREDEXPORT
ae2f_err_t
ae2f_AnnCnnConv(
		size_t dim, 
		const ae2f_float_t* infv, 
		const size_t* infc,
		size_t infcc,
		const ae2f_float_t* ingv, 
		const size_t* ingc,
		size_t ingcc,
		ae2f_float_t* outv,
		size_t* outc_opt,
		size_t outcc,
		const size_t* stride_opt,
		const size_t* pad_opt
		)
{
	ae2f_err_t e = 0;

	if(!outv)	return ae2f_errGlob_PTR_IS_NULL;
	if(!infc)	return ae2f_errGlob_PTR_IS_NULL;
	if(!ingc)	return ae2f_errGlob_PTR_IS_NULL;

	/* When infcc or ingcc is zero, calculate both of them. */
	if(!(infcc && ingcc && outcc) && (infcc = 1) && (ingcc = 1) && (outcc = 1))
		for(size_t i = 0; i < dim; i++) {
			const size_t 
				pad = pad_opt ? pad_opt[i] : 0
				, stride = stride_opt ? stride_opt[i] : 1;

			infcc *= infc[i], ingcc *= ingc[i];

			/* 
			 * outc = ((infc - ingc + (pad << 1)) / (stride)) + 1;
			 * */
			outcc *= ((infc[i] - ingc[i] + (pad << 1)) / stride) + 1;
		}

	/* When they went zero, the function is broken. */
	if(!(infcc && ingcc && outcc))
		return ae2f_errGlob_WRONG_OPERATION | ae2f_errGlob_PTR_IS_NULL;

	else infcc /= infc[dim - 1], ingcc /= ingc[dim - 1];

	const size_t 
		stride = stride_opt ? stride_opt[dim - 1] : 1
		, pad = pad_opt ? pad_opt[dim - 1] : 0
		, opaddedlast = ((
				infc[dim - 1]
				+ (pad << 1) 
				- ingc[dim - 1]
				) / (stride)) + 1;

	outcc /= opaddedlast;


	/* 0D is not possible */
	if(!dim) return ae2f_errGlob_IMP_NOT_FOUND;
	/* Lowkey for 1D (builtin) */
	else if(dim == 1)
		return ae2f_AnnCnnConv1d(
				infv
				, *infc
				, ingv
				, *ingc
				, outv
				, outc_opt
				, stride
				, pad
				);

	if(outc_opt)
		outc_opt[dim - 1] = opaddedlast;

	for(size_t i = 0; i < opaddedlast; i++) {
		const size_t 
			begi = i * stride;

		for(size_t j = 0; j < ingc[dim - 1]; j++) {
			const size_t infi = begi + j;

			/* If infv is padded so it went to zero. */
			if(infi < pad || infi >= infc[dim - 1] + pad)
				continue;

			/* If one of them are zero for nullptr reason */
			if(!(infv && ingv))
				continue;

			/* 
			 * Call it recursively, but dim must be shortened. 
			 * sum += infv[infi + pad] * ingv[j]; 
			 *
			 *
			 * */
			e |= ae2f_AnnCnnConv(
					dim - 1
					, infv + (infi) * infcc, infc, infcc
					, ingv + j * ingcc, ingc, ingcc
					, outv ? outv + i * outcc : 0, outc_opt, outcc
					, stride_opt, pad_opt
				      );

			if(ae2f_errGlobNormalised(e))
				return e;
		}
	}

	return e;
}
