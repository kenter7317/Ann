#include <ae2f/Ann/Mlp.h>
#include <stdio.h>
static ae2f_err_t MlpTrain_Predict(
    const ae2f_mAnnMlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* outcache
) {
    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;

    size_t A = *ae2f_mAnnMlpLayerBuffCount(_this, const);

    union {
        const ae2f_float_t* CF;
        ae2f_float_t* F;
        uintptr_t P;
    }
    tmpVi = { in }, 
    tmpVo = { outcache };

    if(!tmpVo.P) return ae2f_errGlob_ALLOC_FAILED | ae2f_errGlob_PTR_IS_NULL;

    ae2f_err_t err = 0;

    for(size_t i = 0; i < _this->layerc; i++) {
        const ae2f_AnnSlp* slp = ae2f_mAnnMlpLayerV(_this, i, const);

        err |= ae2f_AnnSlpPredict(
            slp,
            tmpVi.CF, tmpVo.F
        );

        tmpVi.F = tmpVo.F;  tmpVo.F += A;
    }

    return err;
}