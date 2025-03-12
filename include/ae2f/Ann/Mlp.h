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
typedef ae2f_AnnSlp ae2f_mAnnMlpEl;

#if ae2f_WhenCXX(!) 0

struct ae2f_mAnnMlp : private ae2f_AnnSlp {
#include "Mlp.h.cxx/Mlp.hh"
};

#else

/// @class ae2f_mAnnMlp
/// @extends ae2f_AnnSlp
/// @brief 
/// # Multi Layered Perceptron
/// 
/// 
typedef ae2f_AnnSlp ae2f_mAnnMlp;

#endif

typedef union ae2f_AnnMlp {
    ae2f_AnnSlp Slp;
    ae2f_mAnnMlp Mlp;
} ae2f_AnnMlp;

/// @memberof ae2f_mAnnMlp
/// @details ae2f_AnnSlpPerVPad
#define ae2f_mAnnMlpLayerVPad ae2f_AnnSlpPerVPad

/// @memberof ae2f_mAnnMlp
#define ae2f_mAnnMlpLayerV(mlp, i, ...) \
ae2f_reinterpret_cast(__VA_ARGS__ ae2f_mAnnMlpEl*, ae2f_mAnnMlpLayerVPad(mlp, __VA_ARGS__)[i] + 1)

/// @memberof ae2f_mAnnMlp
#define ae2f_mAnnMlpInitSz(layerc, add) \
(sizeof(ae2f_mAnnMlp) + (sizeof(void*) * ((layerc))) + (sizeof(size_t) * 1) + (add))

/// @brief 
/// Predict function api. \n
/// For details see @ref ae2f_mAnnMlpPredict_t.
typedef ae2f_AnnSlpPredict_t ae2f_mAnnMlpPredict_t;

/// @brief
/// Training function api. \n
/// For details see @ref ae2f_mAnnMlpTrain_t
typedef ae2f_AnnSlpTrain_t ae2f_mAnnMlpTrain_t;

/// @brief 
/// Cleaning function api.
typedef ae2f_AnnSlpClean_t ae2f_mAnnMlpClean_t;

/// @memberof ae2f_mAnnMlp
/// @brief
/// The predicted max buffer count among all perceptron's possible length of I/O.
#define ae2f_mAnnMlpLayerBuffCount(mlp, ...) \
ae2f_AnnSlpX(mlp, __VA_ARGS__ size_t*, __VA_ARGS__)

/// @memberof ae2f_mAnnMlp
/// @brief
/// Its length is @ref (*ae2f_mAnnMlpLayerBuffCount(mlp) * mlp->layerc, 3 * sizeof(ae2f_float_t)).
#define ae2f_mAnnMlpCache(mlp, ...) \
ae2f_reinterpret_cast(__VA_ARGS__ ae2f_float_t**, ae2f_mAnnMlpLayerBuffCount(mlp, __VA_ARGS__) + 1)

/// @memberof ae2f_mAnnMlp
/// @brief
#define ae2f_mAnnMlpX(mlp,type,...) \
ae2f_reinterpret_cast(__VA_ARGS__ type, ae2f_mAnnMlpCache(mlp, __VA_ARGS__) + 1)

/// @memberof ae2f_mAnnMlp
/// @brief 
/// d
/// @param _this 
/// @param layerc 
/// @param add_opt 
/// @param layerlenv 
/// @param layerpadv_opt 
/// @param inpadv_opt 
/// @param actglob_opt 
/// @param deltaglob_opt 
/// @param weights_opt 
/// @param errret_opt 
/// @return 
ae2f_extern ae2f_SHAREDCALL
size_t ae2f_mAnnMlpInit(
    ae2f_mAnnMlp* _this,
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    ae2f_AnnAct_t* actglob_opt,
    ae2f_AnnDelta_t* deltaglob_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt
) noexcept;

ae2f_extern ae2f_SHAREDCALL
ae2f_AnnMlp* ae2f_AnnMlpMk(
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt,
    const size_t* inpadv_opt,
    ae2f_AnnAct_t* actglob_opt,
    ae2f_AnnDelta_t* deltaglob_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt
) noexcept;

#if ae2f_WhenCXX(!) 0
#include "Mlp.h.cxx/imp.hh"
#endif

#include <ae2f/Pack/End.h>

#endif