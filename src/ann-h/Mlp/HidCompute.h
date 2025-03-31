#include <ae2f/Ann/Slp.h>

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
        retDeltaThen[i] = ae2f_mAnnSlpPerV(layerThen, i, const)->CalDelta(
            outThen[i],
            err + outThen[i]
        );
    }
}
