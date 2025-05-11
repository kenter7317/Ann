#include <ae2fCL/Ann/Slp.h>
#include <ae2fCL/Ann/Sp.h>
#include <ann-h/Slp.h>
#include <ae2fCL/Ann.h>
#include "CLCode/uf.h"


#define SLP_CL_CHUNK_SZ(in, out) \
	((in) + (in + 2) * (out))

ae2f_err_t TrainCL(
		ae2f_mAnnSlp* _this,
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

	ae2fCL_mAnnSlpMemX* memx = ae2fCL_mAnnSlpAdd(_this);

#define __X memx
	const size_t 
		WORKSZ[2] = { _this->inc, _this->outc },
		CHUNKSZ = SLP_CL_CHUNK_SZ(WORKSZ[0], WORKSZ[1]) * sizeof(ae2f_float_t);

#define IC WORKSZ[0]
#define OC WORKSZ[1]
	ae2f_float_t* PREDICTED_BUFF = ae2f_mAnnSlpOutCache(_this);
	
	if(__X->Changed) {
		const size_t* const *pad = ae2f_mAnnSlpPerVPad(_this, const);
		ae2f_float_t* field = _this->pField;
		for(size_t i = 0; i < OC; i++) 
			for(size_t j = 0; j < pad[i][0]; j++)
				j[field + i * (IC + 1) + 1] = 0;
	}

	err2 = clEnqueueWriteBuffer(
			ae2fCL_Ann.Q
			,memx->In
			,CL_TRUE
			,0
			,IC * sizeof(ae2f_float_t)
			,in
			,0, NULL, NULL
			); 
	if(err2) err = err2;
		
	if(__X->Changed)
	{
		if((ae2fCL_Ann.LErr = clEnqueueWriteBuffer(
						ae2fCL_Ann.Q
						, __X->In
						, CL_TRUE
						, IC * sizeof(ae2f_float_t)
						, (IC + 1) * OC * sizeof(ae2f_float_t)
						, _this->pField
						, 0, 0, 0
						)))
				return(ae2f_errGlob_NFOUND);
		__X->Changed = 0;
	}

	if(delta_optA) {
		for(size_t i = 0; i < OC; i++)
			PREDICTED_BUFF[i] = delta_optA[i];

		goto _BUFFSET;
	}

#undef __X

	/* From here is the part for calculating delta from goal */
	if(!_this->vPredict)
		return(ae2f_errGlob_IMP_NOT_FOUND);

	if((er = _this->vPredict(
					_this
					, in
					, PREDICTED_BUFF
					))) goto _OUT;


	for(size_t i = 0; i < OC; i++) {
		ae2f_mAnnSp* perc = ae2f_mAnnSlpPerV(_this, i);
		PREDICTED_BUFF[i] = 
			perc->vLossDeriv(PREDICTED_BUFF, goal_optB, i, OC)
			* perc->vActDeriv(PREDICTED_BUFF[i]) 
			* learningrate; /**/
	}
_BUFFSET:

	if((err2 = clEnqueueWriteBuffer(
					ae2fCL_Ann.Q
					, memx->In
					, CL_TRUE
					, (IC + (IC + 1) * (OC)) * sizeof(ae2f_float_t)
					, OC * sizeof(ae2f_float_t)
					, PREDICTED_BUFF
					, 0, NULL, NULL
					))) 
	{ 
		err = err2; 
		return(ae2f_errGlob_NFOUND); 
	}

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

	if((ae2fCL_Ann.LErr = clEnqueueNDRangeKernel(
					ae2fCL_Ann.Q
					, K
					, 2
					, 0
					, WORKSZ
					, 0
					, 0, NULL, NULL
					)))
		return(ae2f_errGlob_NFOUND);

#if 1
	if((ae2fCL_Ann.LErr = clEnqueueReadBuffer(
					ae2fCL_Ann.Q
					, memx->In
					, CL_TRUE
					, (IC) * sizeof(ae2f_float_t)
					, (IC + 1) * OC * sizeof(ae2f_float_t)
					, _this->pField
					, 0, 0, 0
					)))
		return(ae2f_errGlob_NFOUND);
#endif

#undef return
#undef IC
#undef OC
_OUT:
	if(err) {
		ae2fCL_Ann.LErr = err;
		er |= ae2f_errGlob_NFOUND;
	}



	return er;
}
#undef ev_outs


/**
 * OpenCL Code for Predicting SLP.
 * */
ae2f_err_t PredictCL(
	const ae2f_mAnnSlp* _
	, const ae2f_float_t* in
	, ae2f_float_t* out
	)
{
	if(!(_ && in))
		return ae2f_errGlob_PTR_IS_NULL;

	if(!out)
		return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_DONE_HOWEV;

	if(ae2fCL_Ann.LErr)
		return ae2f_errGlob_NFOUND;

	const size_t 
		WORKSZ[2] = { _->inc, _->outc },
		CHUNKSZ = SLP_CL_CHUNK_SZ(
				WORKSZ[0]
				, WORKSZ[1]
				)
			* sizeof(ae2f_float_t);
	/**/

	cl_event 
		events[3] = {0, 0, 0};

#define IC WORKSZ[0]
#define OC WORKSZ[1]

	ae2f_err_t code = 0;
	ae2fCL_mAnnSlpMemX* __X = ae2fCL_mAnnSlpAdd(_);
	const cl_kernel KERNEL = ae2fCL_AnnKerns[ae2fCL_eAnnKernsSlpPredict];

	if(!__X->In) return ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_ALLOC_FAILED;

#define return(a) { code = a; goto _DONE; }

	if(__X->Changed) {
		const size_t* const *pad = ae2f_mAnnSlpPerVPad(_, const);
		ae2f_float_t* field = _->pField;
		for(size_t i = 0; i < OC; i++) 
			for(size_t j = 0; j < pad[i][0]; j++)
				j[field + i * (IC + 1) + 1] = 0;
	}

	/* Inputs */
	ae2fCL_Ann.LErr = clEnqueueWriteBuffer(
			ae2fCL_Ann.Q
			,__X->In
			,CL_FALSE
			,0
			,IC * sizeof(ae2f_float_t)
			,in
			, 0
			, NULL
			, events
			); 
	if(ae2fCL_Ann.LErr) return(ae2f_errGlob_NFOUND);
	if(!events[0]) return(ae2f_errGlob_ALLOC_FAILED);

	if(__X->Changed) 
	{
		ae2fCL_Ann.LErr = clEnqueueWriteBuffer(
				ae2fCL_Ann.Q
				, __X->In
				, CL_TRUE
				, IC * sizeof(ae2f_float_t)
				, (IC + 1) * OC * sizeof(ae2f_float_t)
				, _->pField
				, 0, 0
				, events + 1
				);
		if(ae2fCL_Ann.LErr) return(ae2f_errGlob_NFOUND);
		if(!events[1]) return(ae2f_errGlob_ALLOC_FAILED);
	}

	{
		if((ae2fCL_Ann.LErr = clSetKernelArg(
				KERNEL
				, 0
				, sizeof(cl_mem)
				, &__X->In
				))) return(ae2f_errGlob_NFOUND);

		if((ae2fCL_Ann.LErr = clSetKernelArg(
				KERNEL
				, 1
				, sizeof(ae2f_float_t) * _->inc * _->outc
				, 0
				))) return(ae2f_errGlob_NFOUND);

		if((ae2fCL_Ann.LErr = clEnqueueNDRangeKernel(
				ae2fCL_Ann.Q
				, KERNEL
				, 2
				, 0
				, WORKSZ
				, WORKSZ
				, 1 + !!__X->Changed
				, events
				, events + 2
				))) return(ae2f_errGlob_NFOUND);

		if(events[0] && (ae2fCL_Ann.LErr = clReleaseEvent(events[0])))
			return(ae2f_errGlob_NFOUND);
		events[0] = 0;
		if(events[1] && __X->Changed && (ae2fCL_Ann.LErr = clReleaseEvent(events[1])))
			return(ae2f_errGlob_NFOUND);
		events[1] = 0;

		if(!events[2]) return(ae2f_errGlob_ALLOC_FAILED);

		if((ae2fCL_Ann.LErr = clEnqueueReadBuffer(
				ae2fCL_Ann.Q
				, __X->In
				, CL_FALSE
				, (IC + (IC + 1) * OC) * sizeof(ae2f_float_t)
				, OC * sizeof(ae2f_float_t)
				, out
				, 1, events + 2, events
				))) return(ae2f_errGlob_NFOUND);
		if(!events[0]) return(ae2f_errGlob_ALLOC_FAILED);
		if((ae2fCL_Ann.LErr = clReleaseEvent(events[2])))
			return(ae2f_errGlob_NFOUND);
		events[2] = 0;

		if((ae2fCL_Ann.LErr = clWaitForEvents(1, events)))
			return(ae2f_errGlob_NFOUND);

		if((ae2fCL_Ann.LErr = clReleaseEvent(events[0])))
			return(ae2f_errGlob_NFOUND);
		events[0] = 0;
	}

	for(size_t i = 0; i < OC; i++)
	{
		const ae2f_mAnnSp* 
			perc = ae2f_mAnnSlpPerV(_,i);

		if(!perc) return(ae2f_errGlob_IMP_NOT_FOUND);
		if(!perc->vAct) return(ae2f_errGlob_IMP_NOT_FOUND);

		out[i] = perc->vAct(out[i] + *ae2f_mAnnSpB(perc));
	}

_DONE:
#undef return
#undef IC 
#undef OC
	__X->Changed = 0;

#define WaitAndKill(_index) \
	if(events[_index]) { \
		if ((ae2fCL_Ann.LErr = clWaitForEvents(1, events + _index))) \
		return code | ae2f_errGlob_NFOUND & ~ae2f_errGlob_DONE_HOWEV; \
		\
		if ((ae2fCL_Ann.LErr = clReleaseEvent(events[_index]))) \
		return code | ae2f_errGlob_NFOUND & ~ae2f_errGlob_DONE_HOWEV; \
	}

	WaitAndKill(0);
	WaitAndKill(1);
	WaitAndKill(2);

	return code;
}
#undef ev_outs


static ae2f_mAnnSlpClean_t CleanCL;

ae2f_SHAREDEXPORT
size_t ae2fCL_mAnnSlpInit(
    ae2f_mAnnSlp* _this,
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    ae2f_float_t* Field_opt,
    ae2f_fpAnnAct_t vAct, 
    ae2f_fpAnnAct_t vActDeriv, 
    ae2f_fpAnnLoss_t vLossDeriv,
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
		    _inc =  incs_optA ? incs_optA[i] : ginc_optB
		    , _pad = inpads_opt ? inpads_opt[i] : 0;

	    if(_this->inc < _pad + _inc) {
		    _this->inc = _pad + _inc;
	    }
    }

    if(Field_opt) {
	    _this->pField = Field_opt;
	    offset_opt -= (_this->inc + 1) * outc;
    } else {
	    _this->pField = Field_opt 
		    = ae2f_mAnnSlpField(_this);
    }

    for(size_t i = 0; i < outc; i++) {
        size_t 
        _inc =  incs_optA ? incs_optA[i] : ginc_optB,
        _pad = inpads_opt ? inpads_opt[i] : 0;

        ae2f_mAnnSlpPerVPad(_this)[i]
        = calloc(
		ae2f_mAnnSpInitSz(
			sizeof(size_t), _inc
		), 1
	);

        ae2f_mAnnSpInit(
            ae2f_mAnnSlpPerV(_this, i)
	    , _inc

	    , Field_opt 
	    ? Field_opt + i * (_this->inc + 1) 
	    : 0

	    , vAct
	    , vActDeriv
	    , vLossDeriv
	    , &ertmp, 0
        );

        if(v2 != CL_SUCCESS) V = v2;

        er |= ertmp;
        *ae2f_mAnnSlpPerVPad(_this)[i] = _pad;
    }

    ae2fCL_mAnnSlpAdd(_this)->In = 
	    	clCreateBuffer(
				ae2fCL_Ann.Ctx
				, CL_MEM_READ_WRITE 
				, ((_this->inc + (_this->inc + 2) * outc) * sizeof(ae2f_float_t)) 
				, NULL, &v2
		);

    ae2fCL_mAnnSlpAdd(_this)->Changed = 1;

    if(CL_SUCCESS != v2) V = v2;

    #undef return
    DONE:
    if(err_opt) *err_opt = er;
    if(errnfound_opt) *errnfound_opt = V;
    return ae2fCL_mAnnSlpInitSz(_this->inc, outc, offset_opt);
}

static ae2f_err_t CleanCL(ae2f_mAnnSlp* _) {
	ae2f_err_t e = 0;
	if(_ && ae2fCL_mAnnSlpAdd(_)->In) {
		if((ae2fCL_Ann.LErr = clReleaseMemObject(
				ae2fCL_mAnnSlpAdd(_)->In
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
ae2fCL_AnnSlp* ae2fCL_AnnSlpMk(
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    ae2f_float_t* Field_opt,
    ae2f_fpAnnAct_t vAct, 
    ae2f_fpAnnAct_t vActDeriv, 
    ae2f_fpAnnLoss_t vLossDeriv,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err_opt,
    cl_int* err_nfound_opt
) noexcept {
	size_t inc = 0;
	for(size_t i = 0; i < outc; i++) {
        size_t 
        _inc =  incs_optA ? incs_optA[i] : ginc_optB,
        _pad = inpads_opt ? inpads_opt[i] : 0;

		if(inc < _pad + _inc) {
			inc = _pad + _inc;
		}
	}

    if(Field_opt) {
	    offset_opt -= (inc + 1) * outc;
    }

    ae2fCL_AnnSlp* rtn = calloc(ae2fCL_mAnnSlpInitSz(inc, outc, offset_opt), 1);
    ae2f_err_t err = 0;
    cl_int err2 = 0;

    ae2fCL_mAnnSlpInit(
		&rtn->CL_Slp
		, incs_optA
		, ginc_optB
		, inpads_opt
		, Field_opt
		, vAct
		, vActDeriv
		, vLossDeriv
		, outc
		, 0
		, &err
		, &err2
	);

    if(err_opt) *err_opt = err;
    if(err_nfound_opt) *err_nfound_opt = err2;
    return rtn;
}
