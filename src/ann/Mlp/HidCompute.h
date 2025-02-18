#include <ae2f/Ann/Slp.h>

static ae2f_float_t MlpTrain_HidErr(
    const ae2f_AnnSlp* layerNxt,
    const ae2f_float_t* deltasNxt,
    size_t idxThen
);

static void MlpTrain_HidCompute(
    const ae2f_AnnSlp* layerThen,
    const ae2f_AnnSlp* layerNxt,
    ae2f_float_t* retDeltaThen,
    const ae2f_float_t* deltasNxt,
    const ae2f_float_t* outThen
) {
    for(size_t i = 0; i < layerThen->layerc; i++) {

        const ae2f_float_t err = MlpTrain_HidErr(layerNxt, deltasNxt, i);        

        retDeltaThen[i] = ae2f_AnnSlpPerV(layerThen, i, const)->CalDelta(
            outThen[i],
            err + outThen[i]
        );
    }
}