#include <ae2f/Ann/Conv.h>

#define ae2f_AnnConv1dSz(in_f, in_g, pad, stride) \
	(((((pad << 1) + *ae2f_mMMapDimLen(in_f, const)) \
	- *ae2f_mMMapDimLen(in_g, const)) \
	/ stride) \
	+ 1)

inline static size_t getIndex(const ae2f_float_t* x, const size_t* xi,  const size_t* xc, size_t dim) {
    int idx = 0, i = 0;
    for (; i < dim; i++) {
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
	const size_t outc = ((infc - ingc + (pad << 1)) / (stride)) + 1;

#define return(n) { err = n; goto _return; }
	if(!stride) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_WRONG_OPERATION;
	if(!outv) return(0);
	if(outc <= pad << 1) return (ae2f_errGlob_OK);
	size_t i = 0, j;
	for(; i < outc; i++)
	{
		ae2f_float_t sum = 0;
		const size_t begi = i * stride;

		for(j = 0; j < ingc; j++)
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
		const size_t inc,
		ae2f_float_t* outv,
		size_t* opt_outc,
		const size_t window,
		const size_t stride,
		ae2f_eAnnCnnPool type
		) noexcept
{
	if(!stride) return ae2f_errGlob_WRONG_OPERATION;

	/** @brief Calculation of out vector's size. */
	size_t outc = 1 + ((inc - window) / stride), i, j; 

	if(inc < window)
		return ae2f_errGlob_IMP_NOT_FOUND;

	if(opt_outc) {
		*opt_outc = outc;
	}

	if(!inv)	return ae2f_errGlob_PTR_IS_NULL;
	if(!outv)	return ae2f_errGlob_OK;
	if(!stride)	return ae2f_errGlob_WRONG_OPERATION;
	if(!window)	return ae2f_errGlob_OK;

	for(i = 0; i < outc; i++)
	{
#define o (i * stride)
		ae2f_float_t a = 0;

		if(!(type & 0b10)) {
			a = inv[o]; /** is not adding, comparing? */
		}

		if(type & 0b100) {
			a = outv[i];	/** is outer thing */
		}


		for(j = 0; j < window; j++)
		{
			switch(type & 0b11)
			{
				case ae2f_eAnnCnnPool_ADD:
				case ae2f_eAnnCnnPool_AVG:
					a += inv[o + j];
					break;
#include <ae2f/Cmp.h>
				case ae2f_eAnnCnnPool_MAX:
					a = ae2f_CmpGetGt(a, inv[o + j]);
					break;

				case ae2f_eAnnCnnPool_MIN:
					a = ae2f_CmpGetLs(a, inv[o + j]);
					break;
			}
#undef o
		}

		if(type == ae2f_eAnnCnnPool_AVG)
			a /= window;

		outv[i] = a;
	}

	return 0;
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
	size_t stride, pad, opaddedlast, __onavg = 1;

	size_t i, j;

	if(!outv)	return ae2f_errGlob_PTR_IS_NULL;
	if(!infc)	return ae2f_errGlob_PTR_IS_NULL;
	if(!ingc)	return ae2f_errGlob_PTR_IS_NULL;

	/* 0D is not possible */
	if(!dim) return ae2f_errGlob_IMP_NOT_FOUND;

	/* When infcc or ingcc is zero, calculate both of them. */
	if((infcc && ingcc && outcc)) {
		for(i = 0; i < dim; i++)
		{
			pad = pad_opt ? pad_opt[i] : 0;
			stride = stride_opt ? stride_opt[i] : 1;

			/* 
			 * outc = ((infc - ingc + (pad << 1)) / (stride)) + 1;
			 * */
			opaddedlast = ((infc[i] - ingc[i] + (pad << 1)) / stride) + 1;
			if(outc_opt)	outc_opt[i] = opaddedlast;

		}
	}
	else {
		infcc = ingcc = outcc = 1;
		for(i = 0; i < dim; i++) {
			pad = pad_opt ? pad_opt[i] : 0;
			stride = stride_opt ? stride_opt[i] : 1;

			infcc *= infc[i], ingcc *= ingc[i];

			/* 
			 * outc = ((infc - ingc + (pad << 1)) / (stride)) + 1;
			 * */
			opaddedlast = ((infc[i] - ingc[i] + (pad << 1)) / stride) + 1;
			outcc *= opaddedlast;
			if(outc_opt)	outc_opt[i] = opaddedlast;
		}
	}

	/* When they went zero, the function is broken. */
	if(!(infcc && ingcc && outcc))
		return ae2f_errGlob_WRONG_OPERATION | ae2f_errGlob_PTR_IS_NULL;

	infcc /= infc[dim - 1];
	ingcc /= ingc[dim - 1];
	outcc /= opaddedlast;

	if(!outv) return 0;

	/* Lowkey for 1D (builtin) */
	if(dim == 1) {
		return ae2f_AnnCnnConv1d(
				infv
				, *infc
				, ingv
				, *ingc
				, outv
				, 0
				, stride
				, pad
				);
	}

	for(i = 0; i < opaddedlast; i++) {
		const size_t 
			begi = i * stride;

		for(j = 0; j < ingc[dim - 1]; j++) {
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
					, outv ? outv + i * stride * outcc : 0
					, 0, outcc
					, stride_opt, pad_opt
				      );

			if(ae2f_errGlobNormalised(e))
				return e;
		}
	}

	return e;
}



ae2f_extern ae2f_SHAREDEXPORT
ae2f_err_t ae2f_AnnCnnPool_imp(
		size_t dim,
		const ae2f_float_t* inv,
		const size_t* inc,
		size_t incc,
		ae2f_float_t* outv,
		size_t* opt_outc,
		size_t outcc,
		const size_t* window_opt,
		const size_t* stride_opt,
		ae2f_eAnnCnnPool type
		)
{
	if(!inv)	return ae2f_errGlob_PTR_IS_NULL;
	if(!inc)	return ae2f_errGlob_PTR_IS_NULL;
	if(!dim)	{
		return ae2f_errGlob_IMP_NOT_FOUND;
	}

	size_t outt, window, stride;
	size_t i, j;

	/* When infcc or ingcc is zero, calculate both of them. */
	if((incc && outcc)) {
		for(i = 0; i < dim; i++) {
			window = window_opt ? window_opt[i] : 1;
			stride = stride_opt ? stride_opt[i] : 0;

			/* 
			 * outc = ((infc - ingc + (pad << 1)) / (stride)) + 1;
			 * */
			outt = 1 + ((inc[i] - window) / stride);

			if(opt_outc) opt_outc[i] = outt;
		}
	}
	else {
		incc = outcc = 1;
		for(i = 0; i < dim; i++) {
			window = window_opt ? window_opt[i] : 1;
			stride = stride_opt ? stride_opt[i] : 0;
			
			incc *= inc[i];
			/* 
			 * outc = ((infc - ingc + (pad << 1)) / (stride)) + 1;
			 * */
			outt = 1 + ((inc[i] - window) / stride);

			if(opt_outc) opt_outc[i] = outt;
			outcc *= outt;
		}
	}

	if(!(incc && outcc))
		return ae2f_errGlob_ALLOC_FAILED;

	incc /= inc[dim - 1];
	outcc /= outt;

	if(dim == 1) {
		return ae2f_AnnCnnPool1d(
				inv
				, inc[dim - 1]
				, outv
				, 0
				, window
				, stride
				, type | 0b100
				);
	}

	ae2f_err_t e = 0;
	for(i = 0; i < outt; i++)
	{
		size_t _j;
		for(_j = 0; _j < window; _j++)
		{
#define j ((i * stride) + _j)
			e |= ae2f_AnnCnnPool_imp(
					dim - 1
					, inv + j * incc, inc, incc
					, outv + i * outcc, 0, outcc
					, window_opt, stride_opt, type 
					);
#undef j
		}
	}

	return e;
}

ae2f_extern ae2f_SHAREDEXPORT ae2f_err_t
ae2f_AnnCnnPool(size_t dim, const ae2f_float_t *inv, const size_t *inc,
                size_t incc, ae2f_float_t *outv, size_t *opt_outc, size_t outcc,
                const size_t *window_opt, size_t windowcc,
                const size_t *stride_opt, ae2f_eAnnCnnPool type)
{
	ae2f_err_t e = ae2f_AnnCnnPool_imp(
			dim
			, inv, inc, incc
			, outv, opt_outc, outcc
			, window_opt, stride_opt
			, type
			);

	size_t i, j;
	if(!windowcc) {
		windowcc = 1;
		for(i = 0; i < dim; i++)
		{
			windowcc *= window_opt ? window_opt[i] : 1;
		}
	}

	if(!outv) return ae2f_errGlob_OK;

	if(ae2f_eAnnCnnPool_AVG == (type & 0b11)) 
		for(i = 0; i < windowcc; i++)
		{
			outv[i] /= windowcc;
		}

	return e;
}
