/**
 * @file Sp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-11
 * 
 * @copyright Copyright (c) 2025
 * 
 * Inline function for separated friends
 * This file is hidden.
 * 
 */
#include <ae2f/Ann/Sp.h>

#define ae2f_AnnSpPredictI(_this, in, i, ...) (ae2f_AnnSpW(_this, __VA_ARGS__ const)[i] * (in)[i])
#define ae2f_AnnSpTrainI(_this, in, i, ...) (ae2f_AnnSpW(_this, __VA_ARGS__)[i] += _delta * (in)[i])