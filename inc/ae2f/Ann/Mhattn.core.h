#ifndef ae2f_Ann_Mhattn_core_h
#define ae2f_Ann_Mhattn_core_h

#include <ae2f/Cast.h>
#include <ae2f/Float.h>
#include "./Util.h"

#include <ae2f/Pack/Beg.h>


ae2f_structdef(struct, ae2f_AnnMhattnFwdU0S0_t) {
	ae2f_float_t m_q, m_k, m_v;
};

ae2f_structdef(union, ae2f_AnnMhattnFwdU0_t) {
	ae2f_AnnMhattnFwdU0S0_t	m_S0;
	ae2f_float_t			m_one;
};

ae2f_structdef(struct, ae2f_AnnMhattnFwd_t) {
	size_t		m_i, m_j, m_k;
	ae2f_AnnMhattnFwdU0_t m_U0;
};

ae2f_structdef(struct, ae2f_AnnMhattnBwd_t) {
	int		m_unused;
};

#include <ae2f/Pack/End.h>

#endif
