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
    return ae2f_mAnnMlpLayerV(this->_this, a);
}
constexprfun const ae2f_mAnnMlpEl* ae2f_TMP Perc(size_t a) const {
    return ae2f_mAnnMlpLayerV(this->_this, a, const);
}

constexprfun size_t* ae2f_TMP PercPad(size_t a) {
    return ae2f_mAnnMlpLayerVPad(this->_this)[a];
}
constexprfun const size_t* ae2f_TMP PercPad(size_t a) const {
    return ae2f_mAnnMlpLayerVPad(this->_this, const)[a];
}

#undef ae2f_TMP


#define ae2f_TMP ae2f_AnnMlp::
inline ae2f_TMP ~ae2f_AnnMlp() {
    ae2f_AnnMlpClean(this);
}
#undef ae2f_TMP