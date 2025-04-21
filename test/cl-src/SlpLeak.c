#include <ae2fCL/Ann/Slp.h>

ae2f_float_t a(ae2f_float_t) { return 0; }
ae2f_float_t b(ae2f_float_t, ae2f_float_t) { return 0; }

static ae2f_float_t
Backward(const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
    return (output[i] - target[i]) / c;
}

int main() {
    ae2fCL_AnnSlp* slp;

    slp = ae2fCL_AnnSlpMkB(
        1, 0, 0, a, a, Backward, 1, 0, 0, 0 
    );
    ae2f_float_t in, out;

    ae2f_mAnnSlpPredict(&slp->Slp, &in, &out);
    ae2f_mAnnSlpTrainB(&slp->Slp, &in, &out, 0.1);
    ae2fCL_AnnSlpDel(slp);

    return 0;
}