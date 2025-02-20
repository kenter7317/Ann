#include <ae2f/Ann/Mlp.h>

static ae2f_err_t MlpTrain_Predict(
    ae2f_AnnMlp* _this,
    const ae2f_float_t* in,
    ae2f_float_t* outcache
) {
    ae2f_err_t ret = 0;

    if(!_this) return ae2f_errGlob_PTR_IS_NULL;
    if(!in) return ae2f_errGlob_PTR_IS_NULL;
    #define return(code) { ret |= code; goto RET; }
    const size_t BUFFCOUNT = *ae2f_AnnMlpLayerBuffCount(_this, const);
    const size_t SkipInput = BUFFCOUNT * _this->layerc;

    for(size_t i = 0; i < _this->layerc; i++) {
        ret |= ae2f_AnnSlpPredict(
            ae2f_AnnMlpLayerV(_this, i), 
            i ? outcache + ((i - 1) * BUFFCOUNT) : in,
            outcache + i * BUFFCOUNT
        );
    }

    RET:
    #undef return
    return ret;
}