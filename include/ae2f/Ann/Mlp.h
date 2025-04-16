/**
 * @file Mlp.h
 * @author ae2f
 * @brief 
 * @version 0.2
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef ae2f_Ann_Mlp_h
#define ae2f_Ann_Mlp_h

#include "./Slp.h"
#include <ae2f/Pack/Beg.h>

/// @brief 
/// Element of @ref ae2f_mAnnMlp
typedef ae2f_mAnnSlp ae2f_mAnnMlpEl;

#if ae2f_WhenCXX(!) 0

struct ae2f_mAnnMlp {
#include "Mlp.h.cxx/mMlp.hh"
};

#else

/// @class ae2f_mAnnMlp
/// @extends ae2f_mAnnSlp
/// @brief 
/// # Multi Layered Perceptron
/// 
/// 
typedef ae2f_mAnnSlp ae2f_mAnnMlp;

#endif

typedef union ae2f_AnnMlp {
    ae2f_mAnnSlp Slp;
ae2f_mAnnMlp Mlp;

    #if ae2f_WhenCXX(!)0
    #include "Mlp.h.cxx/Mlp.hh"
    #endif
} ae2f_AnnMlp;

/// @memberof ae2f_mAnnMlp
/// @details ae2f_mAnnSlpPerVPad
#define ae2f_mAnnMlpLayerVPad ae2f_mAnnSlpPerVPad

/// @memberof ae2f_mAnnMlp
#define ae2f_mAnnMlpLayerV(mlp, i, ...) \
ae2f_reinterpret_cast(__VA_ARGS__ ae2f_mAnnMlpEl*, ae2f_mAnnMlpLayerVPad(mlp, __VA_ARGS__)[i] + 1)

/// @memberof ae2f_mAnnMlp
#define ae2f_mAnnMlpInitSz(layerc, add) \
(sizeof(ae2f_mAnnMlp) + (sizeof(void*) * ((layerc))) + (sizeof(size_t) * 1) + (add))

/// @brief 
/// Predict function api. \n
/// For details see @ref ae2f_mAnnMlpPredict_t.
typedef ae2f_mAnnSlpPredict_t ae2f_mAnnMlpPredict_t;

/// @brief
/// Training function api. \n
/// For details see @ref ae2f_mAnnMlpTrain_t
typedef ae2f_mAnnSlpTrain_t ae2f_mAnnMlpTrain_t;

/// @brief 
/// Cleaning function api.
typedef ae2f_mAnnSlpClean_t ae2f_mAnnMlpClean_t;

/// @memberof ae2f_mAnnMlp
/// @brief
/// The predicted max buffer count among all perceptron's possible length of I/O.
#define ae2f_mAnnMlpLayerBuffCount(mlp, ...) \
ae2f_mAnnSlpX(mlp, __VA_ARGS__ size_t*, __VA_ARGS__)

/// @memberof ae2f_mAnnMlp
/// @brief
/// Its length is @ref (*ae2f_mAnnMlpLayerBuffCount(mlp) * mlp->layerc, 3 * sizeof(ae2f_float_t)).
#define ae2f_mAnnMlpCache(mlp, ...) \
ae2f_reinterpret_cast(__VA_ARGS__ ae2f_float_t**, ae2f_mAnnMlpLayerBuffCount(mlp, __VA_ARGS__) + 1)

/// @memberof ae2f_mAnnMlp
/// @brief
#define ae2f_mAnnMlpX(mlp,type,...) \
ae2f_reinterpret_cast(__VA_ARGS__ type, ae2f_mAnnMlpCache(mlp, __VA_ARGS__) + 1)

/**
 * @brief
 * Initialise, and make additional allocations for the model. \n
 * It could be cleaned with @ref ae2f_AnnMlpClean
 * 
 * @param layerc
 * The length + 1 for almost any kind of vectors.
 *
 * @param act_deriv_v_opt
 * Its length is layerc - 1.
 *
 * It will be for each layer of perceptron's 
 * delta-to-delta calculation.
 *
 * @param delta_opt
 * This one function pointer is for delta calculatoin from goal,
 * for last layer, which could get the actual goal(expected output) 
 * as it seem.
 *
 * */
ae2f_extern ae2f_SHAREDCALL
size_t ae2f_mAnnMlpInit(
    ae2f_mAnnMlp* _this,
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    const ae2f_fpAnnAct_t* actv_opt,
    const ae2f_fpAnnAct_t* act_deriv_v_opt,
    const ae2f_fpAnnLoss_t* lossderiv_v_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt
) noexcept;

/**
 * @brief
 * Makes an actual instance of MLP. \n
 * It could be released with
 * @ref ae2f_AnnMlpDel
 * or simply `delete` in a context of C++.
 *
 * @param layerc
 * The length for almost any kind of vectors.
 *
 * @param act_deriv_v_opt
 * Its length is layerc - 1,
 * 
 * It will be for each layer of perceptron's 
 * delta-to-delta calculation.
 *
 * @param delta_opt
 * This one function pointer is for delta calculatoin from goal,
 * for last layer, which could get the actual goal(expected output) 
 * as it seem.
 * */
ae2f_extern ae2f_SHAREDCALL
ae2f_AnnMlp* ae2f_AnnMlpMk(
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    const ae2f_fpAnnAct_t* actv_opt,
    const ae2f_fpAnnAct_t* act_deriv_v_opt,
    const ae2f_fpAnnLoss_t* lossderiv_v_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt
) noexcept;


#define ae2f_AnnMlpClean ae2f_AnnSlpClean
#define ae2f_AnnMlpDel ae2f_AnnSlpDel

#if ae2f_WhenCXX(!) 0
#include "Mlp.h.cxx/imp.hh"
#endif

#include <ae2f/Pack/End.h>

#endif
