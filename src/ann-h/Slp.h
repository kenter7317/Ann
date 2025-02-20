#include <ae2f/Ann/Slp.h>
#include <stdlib.h>

static ae2f_AnnSlpPredict_t Predict;
static ae2f_AnnSlpTrain_t Train;
static ae2f_AnnSlpClean_t Clean;

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