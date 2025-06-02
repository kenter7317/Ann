#include "../test.h"
#include <ae2fCL/Ann.h>
#include <ae2fCL/Ann/Slp.h>
#include <stdio.h>

#define gThreshold 0.0001
static ae2f_float_t Sigmoid(ae2f_float_t x) {
  printf("THIS IS SIGMOID %f\n", x);
  return 1.0 / (1.0 + exp(-x));
}

static ae2f_float_t SigmoidPrime(ae2f_float_t x) { return (x) * (1.0f - x); }

static ae2f_AnnLoss_t Sub;

static ae2f_float_t Sub(const ae2f_float_t *output, const ae2f_float_t *target,
                        size_t i, size_t c) {
  return (output[i] - target[i]) / c;
}

int mainc() {
  ae2f_err_t err = 0;
  cl_int errcl[1] = {0};
  ae2f_float_t outfloat = 0;
  ae2f_float_t Buff[] = {0.3, 0.2, 0.4, 0.6, 0.1};

  ae2f_float_t Buff2[] = {0, 0.3, 0.2, 0.4, 0.6, 0.1};

  ae2f_mAnnSp *Perc = 0;
  ae2fCL_AnnSlp *SLP = 0;
  ae2f_float_t out_checksum = 0;

  err = ae2fCL_AnnMkEasy(errcl);
  CHECK_ERR(err, CL_SUCCESS, __failure);

  SLP = ae2fCL_AnnSlpMkB(sizeof(Buff) / sizeof(ae2f_float_t), NULL, Buff2,
                         Sigmoid, SigmoidPrime, Sub, 1, 0, &err, errcl);

  Perc = ae2f_mAnnSlpPerV(&SLP->Slp, 0);

  CHECK_ERR(err, CL_SUCCESS, __failure);
  err = ae2f_mAnnSlpPredict(&SLP->Slp, Buff, &outfloat);

  if (err)
    goto __failure;
  printf("out: %f\n", outfloat);
  printf("Bias global: %f, with bias: %f\n", *ae2f_mAnnSpB(Perc),
         (*ae2f_mAnnSpB(Perc)) * sizeof(Buff) / sizeof(ae2f_float_t) +
             outfloat);

  for (size_t i = 0; i < Perc->inc; i++) {
    ae2f_float_t got = Buff[i] * Buff[i];
    out_checksum += got;
    printf("Check-got: %f\n", got);
  }

  out_checksum += *Perc->B();
  out_checksum = Perc->vAct(out_checksum);
  printf("Checking two values match...: %f %f\n", out_checksum, outfloat);
  if ((out_checksum - outfloat) * (out_checksum - outfloat) > gThreshold) {
    printf("Check failed\n");
    return ae2f_errGlob_WRONG_OPERATION;
  }

__failure:
  if (SLP)
    ae2fCL_AnnSlpDel(SLP);
  ae2fCL_AnnDel();
  if (ae2fCL_Ann.Q)
    clReleaseCommandQueue(ae2fCL_Ann.Q);
  ae2fCL_Ann.Q = 0;
  if (ae2fCL_Ann.Ctx)
    clReleaseContext(ae2fCL_Ann.Ctx);
  ae2fCL_Ann.Ctx = 0;
  return err;
}

int maincc() {
  ae2f_err_t err = 0;
  cl_int errcl[1] = {0};
  ae2f_float_t outfloat = 0;
  ae2f_float_t Buff[] = {0.3, 0.2, 0.4, 0.6, 0.1};

  ae2f_float_t Buff2[] = {0, 0.3, 0.2, 0.4, 0.6, 0.1};

  ae2f_mAnnSp *Perc = 0;
  ae2fCL_AnnSlp *SLP = 0;
  ae2f_float_t out_checksum = 0;

  err = ae2fCL_AnnMkEasy(errcl);
  CHECK_ERR(err, CL_SUCCESS, __failure);

  SLP = ae2fCL_AnnSlpMkB(sizeof(Buff) / sizeof(ae2f_float_t), NULL, Buff2,
                         Sigmoid, SigmoidPrime, Sub, 1, 0, &err, errcl);

  Perc = SLP->Slp.Perc(0);

  CHECK_ERR(err, CL_SUCCESS, __failure);
  err = SLP->Slp.Predict(Buff, &outfloat);

  printf("BIAS %f %p\n", SLP->Slp.Perc(0)->pField[0], SLP->Slp.Perc(0)->pField);
  printf("W[0] %f %p\n", SLP->Slp.Perc(0)->W()[0], SLP->Slp.Perc(0)->W());

  printf("PREDICT: %d\n", err);

  if (err)
    goto __failure;
  printf("out: %f\n", outfloat);
  printf("Bias global: %f, with bias: %f\n", *ae2f_mAnnSpB(Perc),
         (*ae2f_mAnnSpB(Perc)) * sizeof(Buff) / sizeof(ae2f_float_t) +
             outfloat);

  for (size_t i = 0; i < Perc->inc; i++) {
    ae2f_float_t got = Buff[i] * Buff[i];
    out_checksum += got;
    printf("Check-got: %f\n", got);
  }

  out_checksum += *Perc->B();
  out_checksum = Perc->vAct(out_checksum);
  printf("Checking two values match...: %f %f\n", out_checksum, outfloat);
  if ((out_checksum - outfloat) * (out_checksum - outfloat) > gThreshold) {
    printf("Check failed\n");
    return ae2f_errGlob_WRONG_OPERATION;
  }

__failure:
  if (SLP) {
    delete SLP; // ae2fCL_AnnSlpDel(SLP);
    SLP = 0;
  }
  ae2fCL_AnnDel();
  if (ae2fCL_Ann.Q)
    clReleaseCommandQueue(ae2fCL_Ann.Q);
  ae2fCL_Ann.Q = 0;
  if (ae2fCL_Ann.Ctx)
    clReleaseContext(ae2fCL_Ann.Ctx);
  ae2fCL_Ann.Ctx = 0;

  printf("is it good? %d\n", err);
  return err;
}

int main() {
  int d = ((maincc()) || (mainc()));
  return d;
}
