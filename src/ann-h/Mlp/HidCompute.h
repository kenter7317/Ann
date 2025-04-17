#include <ae2f/Ann/Slp.h>
#include <stdio.h>

static ae2f_float_t MlpTrain_HidErr(
    const ae2f_mAnnSlp* layerNxt,
    const ae2f_float_t* deltasNxt,
    size_t idxThen
);

static void MlpTrain_HidCompute(
    const ae2f_mAnnSlp* layerThen,
    const ae2f_mAnnSlp* layerNxt,
    ae2f_float_t* retDeltaThen,
    const ae2f_float_t* deltasNxt,
    const ae2f_float_t* outThen
) {
    for(size_t i = 0; i < layerThen->outc; i++) {
	const ae2f_float_t err = MlpTrain_HidErr(layerNxt, deltasNxt, i);
    
    const ae2f_fpAnnAct_t act_p = ae2f_mAnnSlpPerV(
        layerThen
        , i, const
        )->vActDeriv;

    if(act_p)
        retDeltaThen[i] = act_p(outThen[i]) * err;
    else  {
        retDeltaThen[i] = outThen[i] * err;
    }
    } 
}
