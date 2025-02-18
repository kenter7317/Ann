#include "../Sp.h"



#define ae2f_TMP ae2fCL_AnnSp::

constexprfun cl_mem* ae2f_TMP WCl() {
    return ae2fCL_AnnSpWCl(this);
}
constexprfun const cl_mem* ae2f_TMP WCl() const {
    return ae2fCL_AnnSpWCl(this, const);
}

constexprfun cl_mem* ae2f_TMP IOCl() {
    return ae2fCL_AnnSpIOCl(this);
}
constexprfun const cl_mem* ae2f_TMP IOCl() const {
    return ae2fCL_AnnSpIOCl(this, const);
}

template<typename T>
constexprfun const T* ae2f_TMP X() const {
    return ae2fCL_AnnSpX(this, T*, const);
}
 
template<typename T>
constexprfun T* ae2f_TMP X() {
    return ae2fCL_AnnSpX(this, T*);
}

#undef ae2f_TMP