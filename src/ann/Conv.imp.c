#include <ae2f/Cmp.h>
#include <ae2f/Ann/Conv.h>

#include <stdlib.h>
#include <string.h>

#define ae2f_AnnConv1dSz(in_f, in_g, pad, stride) \
	(((((pad << 1) + *ae2f_mMMapDimLen(in_f, const)) \
	   - *ae2f_mMMapDimLen(in_g, const)) \
	   / stride) \
	   + 1)

static size_t iter_indices(
		size_t dim, size_t* indices, const size_t* max_indices) {
	size_t i;
	for (i = dim; i--;) {
		indices[i]++;
		if (indices[i] < max_indices[i]) {
			return 1;
		}
		indices[i] = 0;
	}
	return 0;
}

static void get_conv_indices(
		size_t dim,
		const size_t* out_indices, const size_t* filter_indices,
		const size_t* in_dims, const size_t* filter_dims,
		const size_t* stride, const size_t* pad,
		size_t* in_idx, size_t* filter_idx, size_t* out_idx,
		int* is_padded) {
	size_t in_stride = 1, filter_stride = 1, out_stride = 1;
	size_t i;

	*in_idx = *filter_idx = *out_idx = 0;
	*is_padded = 0;

	for (i = dim; i--;) {
		assert(dim != i);

		size_t cur_in_idx =
			out_indices[i] * (stride ? stride[i] : 1) + filter_indices[i];
		if (cur_in_idx < (pad ? pad[i] : 0) ||
				cur_in_idx >= (in_dims[i] + (pad ? pad[i] : 0))) {
			*is_padded = 1;
			return;
		}
		cur_in_idx -= (pad ? pad[i] : 0);

		*in_idx += cur_in_idx * in_stride;
		*filter_idx += filter_indices[i] * filter_stride;
		*out_idx += out_indices[i] * out_stride;

		in_stride *= in_dims[i];
		filter_stride *= filter_dims[i];
		out_stride *=
			((in_dims[i] - filter_dims[i] + 2 * (pad ? pad[i] : 0)) /
			 (stride ? stride[i] : 1) +
			 1);
	}
}

/** 
 * @brief
 * all vectors are suggested initiated as 0. 
 * */
	ae2f_SHAREDEXPORT ae2f_err_t
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
	size_t i, j;
	for(i = 0; i < outc; i++)
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
ae2f_SHAREDEXPORT ae2f_err_t ae2f_AnnCnnConv(
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
	if (!outv || !infc || !ingc) return ae2f_errGlob_PTR_IS_NULL;
	if (!dim) return ae2f_errGlob_IMP_NOT_FOUND;

	size_t* out_dims = (size_t*)malloc(dim * sizeof(size_t));
	size_t total_out_size = 1;

	size_t i;
	for (i = 0; i < dim; i++) {
		out_dims[i] = ((infc[i] - ingc[i] + 2 * (pad_opt ? pad_opt[i] : 0)) /
				(stride_opt ? stride_opt[i] : 1) +
				1);
		total_out_size *= out_dims[i];
		if (outc_opt) outc_opt[i] = out_dims[i];
	}

	memset(outv, 0, total_out_size * sizeof(ae2f_float_t));

	size_t* out_indices = (size_t*)calloc(dim, sizeof(size_t));
	size_t* filter_indices = (size_t*)calloc(dim, sizeof(size_t));

	do {
		do {
			size_t in_idx, filter_idx, out_idx;
			int is_padded;
			get_conv_indices(dim, out_indices, filter_indices, infc, ingc,
					stride_opt, pad_opt, &in_idx, &filter_idx,
					&out_idx, &is_padded);
			if (!is_padded) {
				outv[out_idx] += infv[in_idx] * ingv[filter_idx];
			}
		} while (iter_indices(dim, filter_indices, ingc));
	} while (iter_indices(dim, out_indices, out_dims));

	free(out_dims);
	free(out_indices);
	free(filter_indices);

	return 0;
}


ae2f_extern ae2f_SHAREDEXPORT
ae2f_err_t ae2f_AnnCnnPool_imp(
		size_t dim, const ae2f_float_t* inv, const size_t* inc, size_t incc,
		ae2f_float_t* outv, size_t* opt_outc, size_t outcc,
		const size_t* window_opt, const size_t* stride_opt, ae2f_eAnnCnnPool type) {
	if (!inv || !inc) return ae2f_errGlob_PTR_IS_NULL;
	if (!dim) return ae2f_errGlob_IMP_NOT_FOUND;

	size_t* out_dims = (size_t*)malloc(dim * sizeof(size_t));
	size_t total_out_size = 1;
	size_t i;

	for (i = 0; i < dim; i++) {
		out_dims[i] =
			(inc[i] - window_opt[i]) / (stride_opt ? stride_opt[i] : 1) + 1;
		total_out_size *= out_dims[i];
		if (opt_outc) opt_outc[i] = out_dims[i];
	}

	size_t* out_indices = (size_t*)calloc(dim, sizeof(size_t));
	assert(out_indices);

	do {
		size_t out_idx = 0;
		size_t out_stride = 1;

		for (i = dim; i--; ) {


			out_idx += out_indices[i] * out_stride;
			out_stride *= out_dims[i];
		}

		ae2f_float_t res = 0;
		switch (type & 0b11) {
			case ae2f_eAnnCnnPool_MAX:
				res = -__FLT_MAX__;
				break;
			case ae2f_eAnnCnnPool_MIN:
				res = __FLT_MAX__;
				break;
		}

		size_t* window_indices = (size_t*)calloc(dim, sizeof(size_t));
		assert(window_indices);

		do {
			size_t in_idx = 0;
			size_t in_stride = 1;

			for (i = dim; i--; ) {
				assert((((intptr_t)i) >= 0) && "Fishy index");
				assert(i != dim);
				in_idx += (out_indices[i] * (stride_opt ? stride_opt[i] : 1) +
						window_indices[i]) *
					in_stride;
				in_stride *= inc[i];
			}

			switch (type & 0b11) {
				case ae2f_eAnnCnnPool_ADD:
				case ae2f_eAnnCnnPool_AVG:
					res += inv[in_idx];
					break;
				case ae2f_eAnnCnnPool_MAX:
					if (inv[in_idx] > res) res = inv[in_idx];
					break;
				case ae2f_eAnnCnnPool_MIN:
					if (inv[in_idx] < res) res = inv[in_idx];
					break;
			}
		} while (iter_indices(dim, window_indices, window_opt));

		outv[out_idx] = res;
		free(window_indices);
	} while (iter_indices(dim, out_indices, out_dims));

	free(out_dims);
	free(out_indices);

	return 0;
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
