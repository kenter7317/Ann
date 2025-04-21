#include <ann-h/Slp.h>

ae2f_SHAREDEXPORT
size_t ae2f_mAnnSlpInit(
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
    ae2f_err_t* err
) {
	ae2f_err_t er = 0, ertmp = 0;
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
	_this->vClean = Clean;
	_this->vPredict = Predict;
	_this->vTrain = Train;

	/* Calculate maximum input size */
	for(size_t i = 0; i < outc; i++) {
		size_t 
			_inc =  incs_optA ? incs_optA[i] : ginc_optB
			, _pad = inpads_opt ? inpads_opt[i] : 0;

        	if(_this->inc < _pad + _inc) 
		{
			_this->inc = _pad + _inc;
		}
	}

	if((_this->pField = Field_opt)) {
		offset_opt -= 
			sizeof(ae2f_float_t)
			* _this->inc
			* _this->outc
			;
	}
	else {
		_this->pField = Field_opt = ae2f_mAnnSlpField(_this);
	}


	for(size_t i = 0; i < outc; i++) {
        	size_t 
		_inc =  incs_optA ? incs_optA[i] : ginc_optB,
        	_pad = inpads_opt ? inpads_opt[i] : 0;
        
        	if(!(ae2f_mAnnSlpPerVPad(_this)[i] 
					= calloc(
						ae2f_mAnnSpInitSz(
							sizeof(size_t)
							, _inc)
						, 1
						)
					)
				)
		{
            		er |= ae2f_errGlob_ALLOC_FAILED;
            		continue;
		}

        	ae2f_mAnnSpInit(
				ae2f_mAnnSlpPerV(_this, i),
				_inc, Field_opt + i * (_this->inc + 1),
				vAct, vActDeriv, vLossDeriv,
				&ertmp, 0
		);

        	er |= ertmp;
        	*(ae2f_mAnnSlpPerVPad(_this)[i]) = _pad;
	}



#undef return
DONE:
	if(err) *err = er;
	return ae2f_mAnnSlpInitSz(_this->inc, outc, offset_opt);
}

ae2f_SHAREDEXPORT
ae2f_AnnSlp* ae2f_AnnSlpMk(
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    ae2f_float_t* Field_opt,
    ae2f_fpAnnAct_t vAct, 
    ae2f_fpAnnAct_t vActDeriv, 
    ae2f_fpAnnLoss_t vLossDeriv,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err
) noexcept {
    ae2f_AnnSlp* _this = 0;

    size_t inc = 0;

    /* Calculate maximum input size */
    for(size_t i = 0; i < outc; i++) {
	    size_t 
		    _inc =  incs_optA ? incs_optA[i] : ginc_optB
		    , _pad = inpads_opt ? inpads_opt[i] : 0;
	    
	    if(inc < _pad + _inc) 
	    {
		    inc = _pad + _inc;
	    }
    }

	if(Field_opt) {
		offset_opt -= (inc + 1) * outc * sizeof(ae2f_float_t);
	}
    _this = calloc(ae2f_mAnnSlpInitSz(inc, outc, offset_opt), 1);
    if(!_this) {
	    if(err) *err |= ae2f_errGlob_ALLOC_FAILED;
	    return _this;
    }
    ae2f_mAnnSlpInit(
		&_this->Slp
		, incs_optA
		, ginc_optB
		, inpads_opt
		, Field_opt
		, vAct
		, vActDeriv
		, vLossDeriv
		, outc
		, offset_opt
		, err
	);
    if(err) *err &= ~ae2f_errGlob_DONE_HOWEV;
    return _this;
}

