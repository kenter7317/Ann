#ifndef ae2fCL_Ann_LcgRand_h
#define ae2fCL_Ann_LcgRand_h

#include <stdint.h>

#define ae2fCL_AnnLcgRandA 1664525
#define ae2fCL_AnnLcgRandC 1013904223
#define ae2fCL_AnnLcgRandM 4294967296

/// @brief Generate a random number from 0 to [ae2fCL_AnnLcgRandM].
/// @param seed [uint64_t] Seed for random number calculating.
/// @return [uint64_t] Generated random number + possible next seed.
#define ae2fCL_AnnLcgRand(seed) ((ae2fCL_AnnLcgRandA * (seed) + ae2fCL_AnnLcgRandC) % ae2fCL_AnnLcgRandM)

#include <ae2f/Float.h>

/// @brief
/// Shrink the 64-bits unsigned integer to [0, 1] in float type. \n
/// 
/// @param n [uint64_t] A 64-bits unsigned integer.
/// @return [ae2f_float_t] [0 ~ 1]
#define ae2fCL_AnnLcgRandDistribute(n) (((ae2f_float_t)(n)) / ((ae2f_float_t)ae2fCL_AnnLcgRandM))

/// @brief
/// Generate a random number as float point from 0 to 1.
/// 
/// @param seed [uint64_t] 
/// @return [ae2f_float_t] Generated number from 0 to 1.
/// 
/// @see ae2fCL_AnnLcgRand
/// @see ae2fCL_AnnLcgRandDistribute
#define ae2fCL_AnnLcgRandReal(seed) ae2fCL_AnnLcgRandDistribute(ae2fCL_AnnLcgRand(seed))

#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <ae2fCL/Loc.h>

#ifndef ae2fCL_LocAsCL

/// @brief Pre-generated 64-bits seed type for separation. (under the hood).
typedef union ae2fCL_ui32_64 {
    uint32_t u32[2];
    uint64_t u64;
} ae2fCL_ui32_64;

/// @brief Pre-defined global seed. 
ae2f_extern ae2f_SHAREDCALL ae2fCL_ui32_64 
ae2fCL_AnnLcgRandSeed;

/// @see ae2fCL_AnnLcgRand
#define ae2fCL_AnnLcgRandG() ae2fCL_AnnLcgRand(ae2fCL_AnnLcgRandSeed.u64)

/// @see ae2fCL_AnnLcgRandReal
#define ae2fCL_AnnLcgRandRealG() ae2fCL_AnnLcgRandReal(ae2fCL_AnnLcgRandSeed.u64)

#endif
#endif