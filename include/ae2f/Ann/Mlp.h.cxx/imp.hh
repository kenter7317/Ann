/**
 * @file imp.hh
 * @author ae2f
 * @brief 
 * @version 0.1
 * @date 2025-02-19
 * 
 * @copyright Copyright (c) 2025
 * 
 * Additional implementations for ae2f/Ann/Mlp.h as C++
 * 
 */

#define ae2f_TMP ae2f_mAnnMlp::

constexprfun ae2f_mAnnMlpEl* ae2f_TMP Perc(size_t a) {
    return ae2f_mAnnMlpLayerV(this, a);
}
constexprfun const ae2f_mAnnMlpEl* ae2f_TMP Perc(size_t a) const {
    return ae2f_mAnnMlpLayerV(this, a, const);
}

#undef ae2f_TMP