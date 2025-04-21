#include "../MMap.h"

#define ae2f_TMP ae2f_mMMap::

constexprfun size_t* ae2f_TMP DimLen() 
{
	return ae2f_mMMapDimLen(this);
}
constexprfun const size_t* ae2f_TMP DimLen() const 
{
	return ae2f_mMMapDimLen(this, const);
}

constexprfun ae2f_float_t* ae2f_TMP Field()
{
	return ae2f_mMMapField(this);
}
constexprfun const ae2f_float_t* ae2f_TMP Field() const 
{
	return ae2f_mMMapField(this, const);
}

#undef ae2f_TMP
