#ifndef ae2f_Ann_Conv_h
#define ae2f_Ann_Conv_h

#include <stddef.h>
#include <ae2f/Float.h>
#include <ae2f/Cast.h>
#include <ae2f/MMap/MMap.h>
#include <ae2f/errGlob.h>

#include <ae2f/Pack/Beg.h>

/**
 * @brief
 * # Convolution Layer
 *
 * @param outv
 * suggested initiated as 0.
 * or they are being added.
 * */
static ae2f_err_t
ae2f_AnnCnnConv1d(const ae2f_float_t *infv, size_t infc,
                  const ae2f_float_t *ingv, size_t ingc, ae2f_float_t *outv,
                  size_t *opt_outc, size_t stride, size_t pad) ae2f_noexcept;

/**
 * @brief
 * `dim` must be the dimension of mmaps, lengths of lists.
 * This function is meant to be recursive. For minimize the stack,
 *
 * @param infcc
 * All elements in [infc] timed.
 * If you set it to zero, it will calculate it for you.
 *
 * @param ingcc
 * All elemtns in [ingc] timed.
 * If you set it to zero, it will calculate it for you.
 * */
static ae2f_err_t ae2f_AnnCnnConv(
    size_t dim, const ae2f_float_t *infv, const size_t *infc, size_t infcc,
    const ae2f_float_t *ingv, const size_t *ingc, size_t ingcc,
    ae2f_float_t *outv, size_t *outc_opt, size_t outcc,
    const size_t *stride_opt, const size_t *pad_opt) ae2f_noexcept;

/**
 * @brief
 * Pooling type.
 * See @ref ae2f_AnnCnnPool
 * */
typedef enum ae2f_eAnnCnnPool {

  /** @brief Max */
  ae2f_eAnnCnnPool_MAX = 0b00,

  /** @brief Min */
  ae2f_eAnnCnnPool_MIN = 0b01,

  /** @brief Add */
  ae2f_eAnnCnnPool_ADD = 0b10,

  /** @brief Average */
  ae2f_eAnnCnnPool_AVG = 0b11,

  /**
   * @brief Middle. respect the output vector's value
   * */
  ae2f_eAnnCnnPool_MIDDLE = 0b100
} ae2f_eAnnCnnPool;

/**
 * @brief
 * # Pooling Layer
 *
 * @param outv
 * Vectors are suggested initiated as 0.
 * or they will affect the output itself.
 *
 * @param window size scanner
 * @param stride size jumper
 * */
static  ae2f_err_t
ae2f_AnnCnnPool1d(const ae2f_float_t *inv, const size_t inc, ae2f_float_t *outv,
                  size_t *opt_outc, const size_t window_opt,
                  const size_t stride_opt, ae2f_eAnnCnnPool) ae2f_noexcept;

/**
 * @brief
 * # Pooling layer
 *
 * @param incc		Pass 0 and will be calculated automatically
 * @param outcc		Pass 0 and will be calculated automatically
 * */
static  ae2f_err_t
ae2f_AnnCnnPool_imp(size_t dim, const ae2f_float_t *inv, const size_t *inc,
                    size_t incc, ae2f_float_t *outv, size_t *opt_outc,
                    size_t outcc, const size_t *window_opt,
                    const size_t *stride_opt, ae2f_eAnnCnnPool type) ae2f_noexcept;

static  ae2f_err_t
ae2f_AnnCnnPool(size_t dim, const ae2f_float_t *inv, const size_t *inc,
                size_t incc, ae2f_float_t *outv, size_t *opt_outc, size_t outcc,
                const size_t *window_opt, size_t windowcc,
                const size_t *stride_opt, ae2f_eAnnCnnPool type) ae2f_noexcept;

#include <ae2f/Pack/End.h>

#include "./Conv.auto.h"

#endif
