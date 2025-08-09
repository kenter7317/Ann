#ifndef Slp_h
#define Slp_h
#include "./mac.h"

typedef struct Predict_t {
        size_t          m_i;
        ae2f_float_t    m_ret;
} Predict_t;

ae2f_MAC() Predict(
		Predict_t			v_predict
                , const size_t                  isz
		, const size_t			osz
		, const size_t			oidx
                , const ae2f_float* const       inp
                , const ae2f_float* const       weight
                , const ae2f_float              bias
                )
{
        (v_predict).m_ret = 0;
        for((v_predict).m_i = isz; (v_predict).m_i--; )
        {
                (v_predict).m_ret += ((weight) + (v_predict).m_i * (osz))[oidx];
        }

        (v_predict).m_ret = ACT((v_predict).m_ret + (bias));
}

typedef struct Follow_t {
        size_t          m_i;
        ae2f_float_t    m_ret;
} Follow_t;

ae2f_MAC() Follow(

                )
{

}

#endif
