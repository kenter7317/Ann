#include <ae2fCL/Ann/Slp.h>
#include <ae2fCL/Ann/Sp.h>
#include <ann-h/Slp.h>
#include <ae2fCL/Ann.h>

#include "CLCode/uf.h"


#define SLP_CL_CHUNK_SZ(in, out) \
	((in) * ((out) + 1) + (out))




ae2f_err_t TrainCL(
		ae2f_AnnSlp* _this,
		const ae2f_float_t* in,
		const ae2f_float_t* delta_optA,
		const ae2f_float_t* goal_optB,
		ae2f_float_t learningrate
		)
{
	if(!_this) 
		return ae2f_errGlob_PTR_IS_NULL;

	if(!in) 
		return ae2f_errGlob_PTR_IS_NULL;

    	if(learningrate == 0) 
		return ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_WRONG_OPERATION;

	if(delta_optA == goal_optB && goal_optB == 0) {
		return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_WRONG_OPERATION;
	}

#define return(k) { er = k; goto _OUT; }
	ae2f_err_t er = 0;
	cl_kernel K = ae2fCL_AnnKerns[ae2fCL_eAnnKernsSlpTrain];

	cl_int err = 0, err2 = 0;

	ae2fCL_AnnSlpMemX* memx = ae2fCL_AnnSlpAdd(_this);

	const size_t 
		WORKSZ[2] = { _this->inc, _this->outc },
		CHUNKSZ = SLP_CL_CHUNK_SZ(WORKSZ[0], WORKSZ[1]) * sizeof(ae2f_float_t);

#define IC WORKSZ[0]
#define OC WORKSZ[1]

	ae2f_float_t* PREDICTED_BUFF = ae2fCL_AnnSlpOutCache(_this);

	if(!(PREDICTED_BUFF = calloc(OC, sizeof(ae2f_float_t))))
		return(ae2f_errGlob_ALLOC_FAILED);


	if(delta_optA) {
		for(size_t i = 0; i < OC; i++)
			PREDICTED_BUFF[i] = delta_optA[i];

#define __X memx
		if(__X->Changed && (err2 = clEnqueueCopyBuffer(
						ae2fCL_Ann.Q
						,__X->In
						,__X->In
						,CHUNKSZ
						,0
						,CHUNKSZ
						,0, 0, 0
						))) 
		{
			err = err2;
			return(ae2f_errGlob_NFOUND);
		}

		err2 = clEnqueueWriteBuffer(
				ae2fCL_Ann.Q
				,memx->In
				,CL_TRUE
				,0
				,IC * sizeof(ae2f_float_t)
				,in
				,0, NULL, NULL
				); if(err2) err = err2;

		if(__X->Changed) 
		{
			for(size_t i = 0; i < _this->outc; i++) {
				const size_t 
					* _padv = ae2f_AnnSlpPerVPad(_this, const)[i],
					pad = *_padv;

					const ae2f_AnnSp* 
						perc = ae2f_reinterpret_cast(
								const ae2f_AnnSp*, 
								_padv + 1
								);
					
					err2 = clEnqueueWriteBuffer(
							ae2fCL_Ann.Q
							,__X->In
							,CL_TRUE
							,((IC * (1 + i)) + pad)* sizeof(ae2f_float_t)
							,perc->inc * sizeof(ae2f_float_t)
							,ae2f_AnnSpW(perc, const)
							, 0, NULL, 0
							); if(err2) 
					{
						err = err2;
						return(ae2f_errGlob_NFOUND);
					}
			}

			__X->Changed = 0;
		}

		goto _BUFFSET;
#undef __X

	}
	/* From here is the part for calculating delta from goal */
	if(!_this->vPredict)
		return(ae2f_errGlob_IMP_NOT_FOUND);

	if((er = _this->vPredict(
					_this
					, in
					, PREDICTED_BUFF
					))) goto _OUT;



_BUFFSET:
	for(size_t i = 0; i < OC; i++) {

		ae2f_AnnSp* perc = ae2f_AnnSlpPerV(_this, i);

		*ae2f_AnnSpB(perc) += PREDICTED_BUFF[i] = 
			delta_optA ?
			PREDICTED_BUFF[i] * learningrate :
			perc->CalDelta(PREDICTED_BUFF[i], goal_optB[i]) * learningrate;


	}

	if((err2 = clEnqueueWriteBuffer(
					ae2fCL_Ann.Q
					, memx->In
					, CL_TRUE
					, (IC * (OC + 1)) * sizeof(ae2f_float_t)
					, OC * sizeof(ae2f_float_t)
					, PREDICTED_BUFF
					, 0, NULL, NULL
					))) 
	{ 
		err = err2; 
		return(ae2f_errGlob_NFOUND); 
	}


#undef return

	if((err2 = clSetKernelArg(
					K
					, 0
					, sizeof(cl_mem)
					, &memx->In
					))) 
	{
		ae2fCL_Ann.LErr = err2;
		return(ae2f_errGlob_NFOUND);
	}


	if((err2 = clEnqueueNDRangeKernel(
					ae2fCL_Ann.Q
					, K
					, 2
					, 0
					, WORKSZ
					, 0
					, 0, NULL, NULL
					)))
	{
		ae2fCL_Ann.LErr = err2;
		return(ae2f_errGlob_NFOUND);
	}


#if 1
	for(size_t i = 0; i < _this->outc; i++) {
		size_t
			* padv = ae2f_AnnSlpPerVPad(_this)[i]
			, pad = *padv;

		ae2f_AnnSp* perc = ae2f_reinterpret_cast(ae2f_AnnSp*, padv + 1);

		if((err2 = clEnqueueReadBuffer(
						ae2fCL_Ann.Q
						, memx->In
						, CL_TRUE
						, (IC) * (1 + i) * sizeof(ae2f_float_t)
						, perc->inc * sizeof(ae2f_float_t)
						, ae2f_AnnSpW(perc)
						, 0, NULL, NULL
						))) 
		{
			ae2fCL_Ann.LErr = err2;
			return ae2f_errGlob_NFOUND;
		}
	}
#endif


#undef IC
#undef OC
_OUT:
	if(err) {
		ae2fCL_Ann.LErr = err;
		er |= ae2f_errGlob_NFOUND;
	}
	return er;
}



/**
 * OpenCL Code for Predicting SLP.
 * */
ae2f_err_t PredictCL(
	const ae2f_AnnSlp* _
	, const ae2f_float_t* in
	, ae2f_float_t* out
	) 
{
	if(!(_ && in))
		return ae2f_errGlob_PTR_IS_NULL;

	if(!out)
		return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_DONE_HOWEV;

	const size_t 
		WORKSZ[2] = { _->inc, _->outc },
		CHUNKSZ = SLP_CL_CHUNK_SZ(WORKSZ[0], WORKSZ[1]) * sizeof(ae2f_float_t);

#define IC WORKSZ[0]
#define OC WORKSZ[1]

	cl_int err = 0, err2 = 0;
	ae2f_err_t code = 0;
	ae2fCL_AnnSlpMemX* __X = ae2fCL_AnnSlpAdd(_);
	const cl_kernel KERNEL = ae2fCL_AnnKerns[ae2fCL_eAnnKernsSlpPredict];

	if(!__X->In) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_ALLOC_FAILED;

	#define event_mallocd 0

#define return(a) { code = a; goto _DONE; }


	if(__X->Changed && (err2 = clEnqueueCopyBuffer(
					ae2fCL_Ann.Q
					,__X->In
					,__X->In
					,CHUNKSZ
					,0
					,CHUNKSZ
					,0, 0, 0
					))) 
	{
		err = err2;
		return(ae2f_errGlob_NFOUND);
	}


	err2 = clEnqueueWriteBuffer(
			ae2fCL_Ann.Q
			,__X->In
			,CL_TRUE
			,0
			,IC * sizeof(ae2f_float_t)
			,in
			,0, NULL, NULL
			); if(err2) err = err2;

	if(__X->Changed) 
	{
		for(size_t i = 0; i < _->outc; i++) {
			const size_t 
				* _padv = ae2f_AnnSlpPerVPad(_, const)[i],
				pad = *_padv;

			const ae2f_AnnSp* 
				perc = ae2f_reinterpret_cast(
						const ae2f_AnnSp*, 
						_padv + 1
						);

			err2 = clEnqueueWriteBuffer(
					ae2fCL_Ann.Q
					,__X->In
					,CL_TRUE
					,((IC * (1 + i)) + pad)* sizeof(ae2f_float_t)
					,perc->inc * sizeof(ae2f_float_t)
					,ae2f_AnnSpW(perc, const)
					, 0, NULL, 0
					); if(err2) 
			{
				err = err2;
				return(ae2f_errGlob_NFOUND);
			}
		}

		__X->Changed = 0;
	}

	{
		if((err2 = clSetKernelArg(
				KERNEL
				, 0
				, sizeof(cl_mem)
				, &__X->In
				))) 
		{
			err = err2;
			return(ae2f_errGlob_NFOUND);
		}

		if((err2 = clSetKernelArg(
				KERNEL
				, 1
				, sizeof(ae2f_float_t) * _->inc * _->outc
				, 0
				))) 
		{
			err = err2;
			return(ae2f_errGlob_NFOUND);
		}

		if((err2 = clEnqueueNDRangeKernel(
				ae2fCL_Ann.Q
				, KERNEL
				, 2
				, 0
				, WORKSZ
				, WORKSZ
				, 0, NULL, NULL
				))) 
		{
			err = err2;
			return(ae2f_errGlob_NFOUND);
		}

		if((err2 = clEnqueueReadBuffer(
				ae2fCL_Ann.Q
				, __X->In
				, CL_TRUE
				, CHUNKSZ - sizeof(ae2f_float_t) * OC
				, OC * sizeof(ae2f_float_t)
				, out
				, 0, NULL, NULL
				)))
		{
			err = err2;
			return(ae2f_errGlob_NFOUND);
		}
	}

	for(size_t i = 0; i < OC; i++)
	{
		const ae2f_AnnSp* 
			perc = ae2f_AnnSlpPerV(_,i);

		if(!perc) return(ae2f_errGlob_IMP_NOT_FOUND);
		if(!perc->Act) return(ae2f_errGlob_IMP_NOT_FOUND);

		out[i] = perc->Act(out[i] + *ae2f_AnnSpB(perc));
	}

_DONE:
#undef return
#undef IC 
#undef OC
	if(err != CL_SUCCESS) {
		ae2fCL_Ann.LErr = err;
		code |= ae2f_errGlob_NFOUND;
	}

	return code;
}


static ae2f_AnnSlpClean_t CleanCL;

ae2f_SHAREDEXPORT
size_t ae2fCL_AnnSlpInit(
    ae2f_AnnSlp* _this,
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* errnfound_opt
) noexcept {
    ae2f_err_t er = 0, ertmp = 0;
    cl_int V = 0, v2 = 0;
    #define return(c) { er = c; goto DONE; }

    if(!_this) 
	    return(
		    ae2f_errGlob_PTR_IS_NULL | 
		    ae2f_errGlob_ALLOC_FAILED | 
		    ae2f_errGlob_DONE_HOWEV
    );

    _this->expected = 1;
    _this->inc = 0;
    _this->outc = _this->layerc = outc;
    _this->vClean = CleanCL;
    _this->vPredict = PredictCL;
    _this->vTrain = TrainCL;

    for(size_t i = 0; i < outc; i++) {
        size_t 
        _inc =  incs_optA ? incs_optA[i] : ginc_optB,
        _pad = inpads_opt ? inpads_opt[i] : 0;

        ae2f_AnnSlpPerVPad(_this)[i]
        = calloc(
		ae2f_AnnSpInitSz(
			sizeof(size_t), _inc
		), 1
	);

        ae2f_AnnSpInit(
            ae2f_AnnSlpPerV(_this, i),
            _inc, w_opt,
            Act, CalDelta,
            &ertmp, 0
        );

        if(v2 != CL_SUCCESS) V = v2;

        er |= ertmp;
        *ae2f_AnnSlpPerVPad(_this)[i] = _pad;

        w_opt && (w_opt += _inc);

        if(_this->inc < _pad + _inc) {
            _this->inc = _pad + _inc;
        }
    }

    ae2fCL_AnnSlpAdd(_this)->In = 
	    	clCreateBuffer(
				ae2fCL_Ann.Ctx, 
				CL_MEM_READ_WRITE, 
				(SLP_CL_CHUNK_SZ(_this->inc, outc) * sizeof(ae2f_float_t)) << 1, 
				NULL, &v2
		);

    ae2fCL_AnnSlpAdd(_this)->Changed = 1;

    if(CL_SUCCESS != v2) V = v2;

    #undef return
    DONE:
    if(err_opt) *err_opt = er;
    if(errnfound_opt) *errnfound_opt = V;
    return ae2fCL_AnnSlpInitSz(outc, offset_opt);
}

static ae2f_err_t CleanCL(ae2f_AnnSlp* _) {
	ae2f_err_t e = 0;
	if(_ && ae2fCL_AnnSlpAdd(_)->In) {
		if((ae2fCL_Ann.LErr = clReleaseMemObject(
				ae2fCL_AnnSlpAdd(_)->In
				)))
		{
			e = 
				ae2f_errGlob_FLUSH_FAILED | 
				ae2f_errGlob_NFOUND;
		}

	}
	return e | Clean(_);
}

ae2f_SHAREDEXPORT
ae2f_AnnSlp* ae2fCL_AnnSlpMk(
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* err_nfound_opt
) noexcept {
    ae2f_AnnSlp* rtn = calloc(ae2fCL_AnnSlpInitSz(outc, offset_opt), 1);
    ae2f_err_t err = 0;
    cl_int err2 = 0;

    ae2fCL_AnnSlpInit(rtn, incs_optA, ginc_optB, inpads_opt, w_opt, Act, CalDelta, outc, 0, &err, &err2);

    if(err_opt) *err_opt = err;
    if(err_nfound_opt) *err_nfound_opt = err2;
    return rtn;
}
