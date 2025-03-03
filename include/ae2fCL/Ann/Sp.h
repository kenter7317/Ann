/**
 * @file Sp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2025
 * 
 * I am adding @ref ae2f_AnnSp ability to 
 * make calculation parallelly with OpenCL. 
 * 
 * While keeping the basic structure of base class.
 * 
 */

#ifndef ae2fCL_Ann_Sp_h
#define ae2fCL_Ann_Sp_h

#include <ae2fCL/Loc.h>
#include <ae2f/Ann/Sp.h>
#include <CL/cl.h>

#if ae2f_WhenCXX(!) 0
struct ae2fCL_AnnSp : ae2f_AnnSp {
    #include "./Sp.h.cxx/Sp.hh"
};
#else

/// @class ae2fCL_AnnSp
/// @extends ae2f_AnnSp
/// @brief
/// Hello World
typedef ae2f_AnnSp ae2fCL_AnnSp;
#endif

#define ae2fCL_AnnSpPredict         ae2f_AnnSpPredict
#define ae2fCL_AnnSpTrain           ae2f_AnnSpTrain
#define ae2fCL_AnnSpTrainA          ae2f_AnnSpTrainA
#define ae2fCL_AnnSpTrainB          ae2f_AnnSpTrainB
#define ae2fCL_AnnSpClean           ae2f_AnnSpClean
#define ae2fCL_AnnSpB               ae2f_AnnSpB
#define ae2fCL_AnnSpW               ae2f_AnnSpW
#define ae2fCL_AnnSpDel             ae2f_AnnSpDel

#define ae2fCL_AnnSpInitSz(off, inc)    ae2f_AnnSpInitSz((off) + (cl_mem_SIZE << 2), inc)
#define ae2fCL_AnnSpWCl(per, ...)       ae2f_AnnSpX(per, cl_mem*, __VA_ARGS__)
#define ae2fCL_AnnSpIOCl(per, ...)      (ae2fCL_AnnSpWCl(per, __VA_ARGS__) + 1)
#define ae2fCL_AnnSpX(per, type, ...)   ae2f_reinterpret_cast(__VA_ARGS__ type, ae2fCL_AnnSpIOCl(per, __VA_ARGS__) + 1)

ae2f_extern ae2f_SHAREDCALL
size_t ae2fCL_AnnSpInit(
    ae2fCL_AnnSp* perc_opt,
    size_t icount,
    const ae2f_float_t* w_opt,
    ae2f_AnnAct_t Act,
    ae2f_AnnDelta_t CalDelta,
    ae2f_err_t* errret_opt,
    cl_int* erronnfound_opt,
    size_t off_opt
) noexcept;

ae2f_extern ae2f_SHAREDCALL
ae2fCL_AnnSp* ae2fCL_AnnSpMk(
    size_t icount,
    const ae2f_float_t* w_opt,
    ae2f_AnnAct_t Act,
    ae2f_AnnDelta_t CalDelta,
    ae2f_err_t* errret_opt,
    cl_int* erronnfound_opt,
    size_t off_opt
) noexcept;

#if ae2f_WhenCXX(!) 0
#include "Sp.h.cxx/imp.hh"
#endif

#endif