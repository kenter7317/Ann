#include <ae2f/Ann/Slp.h>

/// @todo
/// Error handeling is needed
/// @brief 
/// # Error Calculating Funtion for Hidden layer
/// 
/// Sum for multiplying Delta and Weight 
static ae2f_float_t MlpTrain_HidErr(
    const ae2f_AnnSlp* layerNxt,
    const ae2f_float_t* deltasNxt,
    size_t idxThen
) {
    ae2f_float_t ret = 0;
    for(size_t i = 0; i < layerNxt->layerc; i++)
        ret += *ae2f_AnnSpW(ae2f_AnnSlpPerV(layerNxt, i, const), const) * deltasNxt[i];

    return ret;
}