#include <ae2f/Ann/Slp.h>
#include <stdlib.h>

static ae2f_AnnSlpPredict_t Predict;
static ae2f_AnnSlpTrain_t Train;
static ae2f_AnnSlpClean_t Clean;

ae2f_SHAREDEXPORT
size_t ae2f_AnnSlpInit(
    ae2f_AnnSlp* _this,
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err
) {
    ae2f_err_t er = 0, ertmp = 0;
    #define return(c) { er = c; goto DONE; }

    if(!_this) return(ae2f_errGlob_PTR_IS_NULL | ae2f_errGlob_ALLOC_FAILED | ae2f_errGlob_DONE_HOWEV);

    _this->inc = 0;
    _this->outc = _this->layerc = outc;
    _this->vClean = Clean;
    _this->vPredict = Predict;
    _this->vTrain = Train;

    for(size_t i = 0; i < _this->outc; i++) {
        size_t 
        _inc =  incs_optA ? incs_optA[i] : ginc_optB,
        _pad = inpads_opt ? inpads_opt[i] : 0;

        ae2f_AnnSlpPerVPad(_this)[i]
        = calloc(ae2f_AnnSpInitSz(sizeof(size_t), _inc), 1);
        
        ae2f_AnnSpInit(
            ae2f_AnnSlpPerV(_this, i),
            _inc, w_opt,
            Act, CalDelta,
            &ertmp, sizeof(size_t)
        );

        er |= ertmp;
        *ae2f_AnnSlpPerVPad(_this)[i] = _pad;

        w_opt && (w_opt += _inc);

        if(_this->inc < _pad + _inc) {
            _this->inc = _pad + _inc;
        }
    }

    #undef return
    DONE:
    if(err) *err = er;
    return ae2f_AnnSlpInitSz(outc, offset_opt);
}

ae2f_SHAREDEXPORT
ae2f_AnnSlp* ae2f_AnnSlpMk(
    const size_t* incs_optA,
    size_t ginc_optB,
    const size_t* inpads_opt,
    const ae2f_float_t* w_opt,
    ae2f_fpAnnAct_t Act, 
    ae2f_fpAnnDelta_t CalDelta,
    size_t outc,
    size_t offset_opt,
    ae2f_err_t* err
) noexcept {
    ae2f_AnnSlp* _this = 0;
    _this = calloc(ae2f_AnnSlpInitSz(outc, offset_opt), 1);
    ae2f_AnnSlpInit(_this, incs_optA, ginc_optB, inpads_opt, w_opt, Act, CalDelta, outc, offset_opt, err);
    if(err) *err &= ~ae2f_errGlob_DONE_HOWEV;
    return _this;
}

static ae2f_err_t Clean(
    ae2f_AnnSlp* _this
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;

    for(size_t i = 0; i < _this->outc; i++) {
        ae2f_AnnSpDel(ae2f_AnnSlpPerV(_this, i));
        ae2f_AnnSlpPerVPad(_this, )[i] = 0;
    }

    _this->inc = 0;
    _this->outc = 0;
    _this->layerc = 0;
    _this->vClean = 0;
    _this->vPredict = 0;
    _this->vTrain = 0;

    return ae2f_errGlob_OK;
}

static ae2f_err_t Predict(
    const ae2f_AnnSlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* out_opt
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!out_opt) ae2f_errGlob_DONE_HOWEV | ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;

    ae2f_err_t err = 0;

    for(size_t i = 0; i < _this->outc; i++) {
        const size_t* _per = ae2f_AnnSlpPerVPad(_this, const)[i];

        ae2f_err_t er = 
        ae2f_AnnSpPredict(
            (const ae2f_AnnSp*)(_per + 1),
            in + *_per,
            out_opt + i
        );

        err |= er;
    }

    return err;
}

static ae2f_err_t Train(
    ae2f_AnnSlp* _this,
    const ae2f_float_t* in,
    const ae2f_float_t* delta_optA,
    const ae2f_float_t* goal_optB,
    ae2f_float_t learningrate
) noexcept {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    if(learningrate == 0) return ae2f_errGlob_OK;
    if(delta_optA) goto _DELTA;
    if(!goal_optB) return ae2f_errGlob_WRONG_OPERATION | ae2f_errGlob_PTR_IS_NULL;

    ae2f_err_t _er = 0;

    for(size_t i = 0; i < _this->outc; i++) {
        size_t* pr = ae2f_AnnSlpPerVPad(_this)[i];

        _er |= ae2f_AnnSpTrainB(
            (ae2f_AnnSp*)(pr + 1), 
            in + *pr, 
            goal_optB[0], 
            learningrate
        );
    }

    return _er;

    _DELTA:
    for(size_t i = 0; i < _this->outc; i++) {
        size_t* pr = ae2f_AnnSlpPerVPad(_this)[i];

        _er |= ae2f_AnnSpTrainA(
            (ae2f_AnnSp*)(pr + 1), 
            in + *pr, 
            delta_optA, 
            learningrate
        );
    }

    return _er;
}