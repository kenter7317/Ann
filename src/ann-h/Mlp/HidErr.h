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
    for(size_t i = 0; i < layerNxt->outc; i++) {
        union {
            const size_t* pad;
            const ae2f_mAnnSp* slp;
        } perv = { .pad = ae2f_AnnSlpPerVPad(layerNxt)[i] + 1 };
        ret += ae2f_mAnnSpW(perv.slp, const)[idxThen] * deltasNxt[i];
    }

    return ret;
}