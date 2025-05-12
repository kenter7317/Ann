#include <ae2fCL/Ann.h>
#include "../test.h"
#include <ae2fCL/Ann/Sp.h>
#include <stdio.h>


#define gThreshold 0.0001
static ae2f_float_t 
Sigmoid(ae2f_float_t x) {
    return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t 
SigmoidPrime(ae2f_float_t x) {
    return (x) * (1.0f - x);
}

static ae2f_AnnLoss_t Sub;

static ae2f_float_t
Sub(const ae2f_float_t* output, const ae2f_float_t* target, size_t i, size_t c) {
    return (output[i] - target[i]) / c;
}

int mainc() {
    ae2f_err_t err = 0;
    cl_int errcl[1] = {0};
    ae2f_float_t outfloat = 0;
    ae2f_float_t Buff[] = {
        0.3, 0.2, 0.4, 0.6, 0.1
    };

    ae2f_float_t Buff2[] = {
        0, 0.3, 0.2, 0.4, 0.6, 0.1
    };

    ae2fCL_AnnSp* Perc;
    ae2f_float_t out_checksum = 0;
    
    err = ae2fCL_AnnMkEasy(errcl);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    printf("Buff2: %f\n", Buff2[0]);

    Perc = ae2fCL_AnnSpMk(
        sizeof(Buff)/sizeof(ae2f_float_t), Buff2, 
        Sigmoid, SigmoidPrime, Sub, &err, errcl, 0
    );
    CHECK_ERR(err, CL_SUCCESS, __failure);
    if(ae2f_mAnnSpB(&Perc->Sp) != Buff2)
    {
        puts("Link failed");
        return 1;
    }

    printf("Buff2: %f\n", Buff2[0]);
    ae2fCL_Ann.LErr = 0;
    err = ae2f_mAnnSpPredict(
        &Perc->Sp, Buff, &outfloat
    );

    printf("buff2: %f\n", *Buff2);
    if(err) goto __failure;
    if(ae2fCL_Ann.LErr) goto __failure;
    printf("out: %f\n", outfloat);
    printf(
        "Bias global: %f, with bias: %f\n", 
        *ae2f_mAnnSpB(&Perc->Sp), 
        (*ae2f_mAnnSpB(&Perc->Sp)) * sizeof(Buff)/ sizeof(ae2f_float_t) + outfloat
    );

    printf("Bias checked on C++: %f\n", Perc->Sp.B()[0]);

    for(size_t i = 0; i < Perc->Sp.inc; i++) 
    {
        ae2f_float_t got = Buff[i] * Buff[i];
        out_checksum += got;
        printf("Check-got: %f\n", got);
        printf("Buff2: %f\n", Buff2[i + 1]);
    }
    out_checksum += *ae2f_mAnnSpB(&Perc->Sp);
    out_checksum = Perc->Sp.vAct(out_checksum);

    printf("Checking two values match...: %f %f\n", out_checksum, outfloat);
    if((out_checksum - outfloat) * (out_checksum - outfloat) > gThreshold) {
        printf("Check failed\n");
        return ae2f_errGlob_WRONG_OPERATION;
    }

    __failure:
    printf("Done. in whatever reason. %d\n", err);
    printf("ErrCL: %d\n", ae2fCL_Ann.LErr);
    if(Perc) ae2fCL_AnnSpDel(Perc);
    ae2fCL_AnnDel();
    if(ae2fCL_Ann.Q) clReleaseCommandQueue(ae2fCL_Ann.Q);
    if(ae2fCL_Ann.Ctx) clReleaseContext(ae2fCL_Ann.Ctx);
    return err;
}

int maincc() {
    ae2f_err_t err = 0;
    cl_int errcl[1] = {0};
    ae2f_float_t outfloat = 0;
    ae2f_float_t Buff[] = {
        0.3, 0.2, 0.4, 0.6, 0.1
    };

    ae2f_float_t Buff2[] = {
        0, 0.3, 0.2, 0.4, 0.6, 0.1
    };

    ae2fCL_AnnSp* Perc = 0;
    ae2f_float_t out_checksum = 0;
    
    err = ae2fCL_AnnMkEasy(errcl);
    CHECK_ERR(err, CL_SUCCESS, __failure);

    Perc = ae2fCL_AnnSpMk(
        sizeof(Buff)/sizeof(ae2f_float_t), Buff2, 
        Sigmoid, SigmoidPrime, Sub, &err, errcl, 0
    );
    CHECK_ERR(err, CL_SUCCESS, __failure);

    puts("PREDICT START: MAINCC");
    err = Perc->Sp.Predict(
        Buff, &outfloat
    );
    if(err) goto __failure;
    puts("PREDICT SUCCEED: MAINCC");
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
    out_checksum = Perc->Sp.vAct(out_checksum);
    printf("Checking two values match...: %f %f\n", out_checksum, outfloat);
    if((out_checksum - outfloat) * (out_checksum - outfloat) > gThreshold) {
        printf("Check failed\n");
        return ae2f_errGlob_WRONG_OPERATION;
    }

    
    __failure:
    if(Perc) delete (Perc);
    ae2fCL_AnnDel();
    if(ae2fCL_Ann.Q) clReleaseCommandQueue(ae2fCL_Ann.Q);
    if(ae2fCL_Ann.Ctx) clReleaseContext(ae2fCL_Ann.Ctx);
    return err | errcl[0];
}

int main() {
    return ((mainc()) || maincc() );//|| (mainc()));
}