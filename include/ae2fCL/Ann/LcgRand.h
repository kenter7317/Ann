#ifndef ae2fCL_Ann_LcgRand_h
#define ae2fCL_Ann_LcgRand_h

#define ae2fCL_AnnLcgRandA 1664525
#define ae2fCL_AnnLcgRandC 1013904223
#define ae2fCL_AnnLcgRandM 4294967296

#define ae2fCL_AnnLcgRand(seed) ((ae2fCL_AnnLcgRandA * (seed) + ae2fCL_AnnLcgRandC) % ae2fCL_AnnLcgRandM)

#include <ae2f/Float.h>
#define ae2fCL_AnnLcgRandDistribute(n) (((ae2f_float_t)(n)) / ((ae2f_float_t)ae2fCL_AnnLcgRandM))
#define ae2fCL_AnnLcgRandReal(seed) ae2fCL_AnnLcgRandDistribute(ae2fCL_AnnLcgRand(seed))

#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <ae2fCL/Loc.h>
#include <stdint.h>

#ifndef ae2fCL_LocAsCL

typedef union ae2fCL_ui32_64 {
    uint32_t u32[2];
    uint64_t u64;
} ae2fCL_ui32_64;

ae2f_extern ae2f_SHAREDCALL ae2fCL_ui32_64 
ae2fCL_AnnLcgRandSeed;

#define ae2fCL_AnnLcgRandG() ae2fCL_AnnLcgRand(ae2fCL_AnnLcgRandSeed.u64)
#define ae2fCL_AnnLcgRandRealG() ae2fCL_AnnLcgRandReal(ae2fCL_AnnLcgRandSeed.u64)

#endif
#endif