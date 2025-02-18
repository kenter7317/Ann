/**
 * @file LcgRand.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2f_Ann_LcgRand_h
#define ae2f_Ann_LcgRand_h

#include <stdint.h>

#define ae2f_AnnLcgRandA 1664525
#define ae2f_AnnLcgRandC 1013904223
#define ae2f_AnnLcgRandM 4294967296

/// @brief Generate a random number from 0 to [ae2f_AnnLcgRandM].
/// @param seed [uint64_t] Seed for random number calculating.
/// @return [uint64_t] Generated random number + possible next seed.
#define ae2f_AnnLcgRand(seed) ((ae2f_AnnLcgRandA * (seed) + ae2f_AnnLcgRandC) % ae2f_AnnLcgRandM)

#include <ae2f/Float.h>

/// @brief
/// Shrink the 64-bits unsigned integer to [0, 1] in float type. \n
/// 
/// @param n [uint64_t] A 64-bits unsigned integer.
/// @return [ae2f_float_t] [0 ~ 1]
#define ae2f_AnnLcgRandDistribute(n) (((ae2f_float_t)(n)) / ((ae2f_float_t)ae2f_AnnLcgRandM))

/// @brief
/// Generate a random number as float point from 0 to 1.
/// 
/// @param seed [uint64_t] 
/// @return [ae2f_float_t] Generated number from 0 to 1.
/// 
/// See
/// @ref ae2f_AnnLcgRand
/// @ref ae2f_AnnLcgRandDistribute
#define ae2f_AnnLcgRandReal(seed) ae2f_AnnLcgRandDistribute(ae2f_AnnLcgRand(seed))

#include <ae2f/Call.h>
#include <ae2f/Cast.h>
#include <ae2fCL/Loc.h>

#ifndef ae2fCL_LocAsCL

/// @brief Pre-generated 64-bits seed type for separation. (under the hood).
typedef union ae2fCL_LcgRandSeed {
    uint32_t u32[2];
    uint64_t u64;
} ae2fCL_LcgRandSeed;

/// @brief 
/// Pre-defined global seed. \n
/// Simply change this would change the global seed for lcgG.
///
/// @see ae2f_AnnLcgRandG
/// @see ae2f_AnnLcgRandRealG
ae2f_extern ae2f_SHAREDCALL ae2fCL_LcgRandSeed 
ae2f_AnnLcgRandSeed;

/// @see ae2f_AnnLcgRand
#define ae2f_AnnLcgRandG() ae2f_AnnLcgRand(ae2f_AnnLcgRandSeed.u64)

/// @see ae2f_AnnLcgRandReal
#define ae2f_AnnLcgRandRealG() ae2f_AnnLcgRandReal(ae2f_AnnLcgRandSeed.u64)

#endif
#endif