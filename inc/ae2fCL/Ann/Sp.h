/**
 * @file Sp.h
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2025
 * 
 * I am adding @ref ae2f_mAnnSp ability to 
 * make calculation parallelly with OpenCL. 
 * 
 * While keeping the basic structure of base class.
 * 
 */

#ifndef ae2fCL_Ann_Sp_h
#define ae2fCL_Ann_Sp_h

typedef struct 
ae2f_WhenC(ae2f_mAnnSp)
ae2f_WhenCXX(ae2fCL_mAnnSp) 
ae2fCL_mAnnSp;

#include <ae2f/Ann/Sp.h>
#include <ae2fCL/Ann.h>

#if ae2f_WhenCXX(!) 0
struct ae2fCL_mAnnSp {
#include "./Sp.h.cxx/mSp.hh"
};
#endif

#define ae2fCL_mAnnSpClean           ae2f_mAnnSpClean
#define ae2fCL_mAnnSpDel             ae2f_mAnnSpDel

#define ae2fCL_mAnnSpInitSz(off, inc)    ae2f_mAnnSpInitSz((off) + (cl_mem_SIZE << 2), inc)
#define ae2fCL_mAnnSpWCl(per, ...)       ae2f_reinterpret_cast(__VA_ARGS__ cl_mem*, ae2f_reinterpret_cast(__VA_ARGS__ ae2f_AnnSp*, per) + 1)
#define ae2fCL_mAnnSpIOCl(per, ...)      (ae2fCL_mAnnSpWCl(per, __VA_ARGS__) + 1)

ae2f_extern ae2f_SHAREDCALL
size_t ae2fCL_mAnnSpInit(
    ae2fCL_mAnnSp* perc_opt,
    size_t icount,
    ae2f_float_t* Field_opt,
    ae2f_AnnAct_t vAct,
    ae2f_AnnAct_t vActDeriv,
    ae2f_AnnLoss_t vLossDeriv,
    ae2f_err_t* errret_opt,
    cl_int* erronnfound_opt,
    size_t off_opt
) noexcept;

typedef union ae2fCL_AnnSp {
    ae2fCL_mAnnSp CL_Sp;
    ae2f_mAnnSp Sp;

    #if ae2f_WhenCXX(!)0
    #include "Sp.h.cxx/Sp.hh"
    #endif
} ae2fCL_AnnSp;

#define ae2fCL_AnnSpClean   ae2f_AnnSpClean
#define ae2fCL_AnnSpDel     ae2f_AnnSpDel

ae2f_extern ae2f_SHAREDCALL
ae2fCL_AnnSp* ae2fCL_AnnSpMk(
    size_t icount,
    ae2f_float_t* Field_opt,
    ae2f_AnnAct_t vAct,
    ae2f_AnnAct_t vActDeriv,
    ae2f_AnnLoss_t vLossDeriv,
    ae2f_err_t* errret_opt,
    cl_int* erronnfound_opt,
    size_t off_opt
) noexcept;

#if ae2f_WhenCXX(!) 0
#include "Sp.h.cxx/imp.hh"
#endif

#endif