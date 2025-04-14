#include "../test.h"
#include <ae2f/Ann/Sp.h>
#include <stdio.h>


#define gThreshold 0.0001
static ae2f_float_t 
Sigmoid(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

#define SigmoidDeriv 0

ae2f_float_t Sub(ae2f_float_t out, ae2f_float_t goal) {
    return out - goal;
}

ae2f_extern int mainc() {
    ae2f_err_t err = 0;
    ae2f_float_t outfloat = 0;
    ae2f_float_t Buff[] = {
        0.3, 0.2, 0.4, 0.6, 0.1
    };

    ae2f_mAnnSp* Perc;
    ae2f_float_t out_checksum = 0;

    Perc = (ae2f_mAnnSp*)ae2f_AnnSpMk(
        sizeof(Buff)/sizeof(ae2f_float_t), 0 /*Buff*/, 
        Sigmoid, SigmoidDeriv, Sub, &err, 0
    );
    CHECK_ERR(err, 0, __failure);

    for(size_t i = 0; i < Perc->inc; i++) {
        printf("Weight: %f\n", (Buff[i] = ae2f_mAnnSpW(Perc)[i]));
    }
    puts("");

    err = ae2f_mAnnSpPredict(
        Perc, Buff, &outfloat
    );
    
    if(err) goto __failure;
    printf("out: %f\n", outfloat);
    printf(
        "Bias global: %f, with bias: %f\n", 
        *ae2f_mAnnSpB(Perc), 
        (*ae2f_mAnnSpB(Perc)) * sizeof(Buff)/ sizeof(ae2f_float_t) + outfloat
    );

    for(size_t i = 0; i < Perc->inc; i++) 
    {
        ae2f_float_t got = Buff[i] * Buff[i];
        out_checksum += got;
        printf("Check-got: %f\n", got);
    }
    out_checksum += *ae2f_mAnnSpB(Perc);
    out_checksum = Perc->Act(out_checksum);
    printf("Checking two values match...: %f %f\n", out_checksum, outfloat);
    if((out_checksum - outfloat) * (out_checksum - outfloat) > gThreshold) {
        printf("Check failed\n");
        return ae2f_errGlob_WRONG_OPERATION;
    }

    __failure:
    ae2f_mAnnSpDel(Perc);
    return err;
}

int maincc() {
    ae2f_err_t err = 0;
    ae2f_float_t outfloat = 0;
    ae2f_float_t Buff[] = {
        0.3, 0.2, 0.4, 0.6, 0.1
    };

    ae2f_AnnSp* Perc;
    ae2f_float_t out_checksum = 0;

    Perc = ae2f_AnnSpMk(
        sizeof(Buff)/sizeof(ae2f_float_t), Buff, 
        Sigmoid, SigmoidDeriv, Sub, &err, 0
    );
    CHECK_ERR(err, 0, __failure);

    err = Perc->Sp.Predict(
        Buff, &outfloat
    );
    
    if(err) goto __failure;
    printf("out: %f\n", outfloat);
    printf(
        "Bias global: %f, with bias: %f\n", 
        *Perc->Sp.B(), 
        (*Perc->Sp.B()) * sizeof(Buff)/ sizeof(ae2f_float_t) + outfloat
    );

    for(size_t i = 0; i < Perc->Sp.inc; i++) 
    {
        ae2f_float_t got = Buff[i] * Buff[i];
        out_checksum += got;
        printf("Check-got: %f\n", got);
    }
    out_checksum += *Perc->Sp.B();
    out_checksum = Perc->Sp.Act(out_checksum);
    printf("Checking two values match...: %f %f\n", out_checksum, outfloat);
    if((out_checksum - outfloat) * (out_checksum - outfloat) > gThreshold) {
        printf("Check failed\n");
        return ae2f_errGlob_WRONG_OPERATION;
    }

    __failure:
    if(Perc) delete (Perc);
    return err;
}

int main() {
    return mainc() | maincc();
}