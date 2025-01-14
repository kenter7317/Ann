#define ae2fCL_AnnLcgRandA 1664525
#define ae2fCL_AnnLcgRandC 1013904223
#define ae2fCL_AnnLcgRandM 4294967296
#define ae2fCL_AnnLcgRand(seed) ((ae2fCL_AnnLcgRandA * (seed) + ae2fCL_AnnLcgRandC) % ae2fCL_AnnLcgRandM)

#include <ae2f/Float.h>
#define ae2fCL_AnnLcgRandDistribute(n) (((ae2f_float_t)(n)) / ((ae2f_float_t)ae2fCL_AnnLcgRandM))
#define ae2fCL_AnnLcgRandReal(seed) ae2fCL_AnnLcgRandDistribute(ae2fCL_AnnLcgRand(seed))