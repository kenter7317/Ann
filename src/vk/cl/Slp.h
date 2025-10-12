#ifndef Slp_h
#define Slp_h

#define ae2f_NEED_CLASS 0

#include "mac.h"
#include <ae2f/Ann/Slp.core.h>
#include <ae2f/Ann/Slp.auto.h>


#if !__ae2f_MACRO_GENERATED
#define _clAtomAddF_tmpl(a, b, c, d, e)	clAtomAddF_tmpl(d, e)
#define host_float_uint_t	uint
#define _clAtomAddF(a, b, c, d)		clAtomAddF(b, c, d)
#endif

typedef struct clAtomAddF_t {
	union {
		host_float_t m_f; 
		uint32_t m_u[sizeof(host_float_t) >> 2];
	} m_atom[2];

	uint32_t	m_count;
} clAtomAddF_t;


ae2f_MAC(__global, ) clAtomAddF(clAtomAddF_t v_mem, __global volatile host_float_t* prm_dst, ae2f_float_t prm_val)
{
	typedef char clatomaddf_is_host_float_32bit_family[sizeof(host_float_t) & 3 ? -1 : 1];
	(v_mem).m_count = (sizeof(host_float_t) >> 2);
	((v_mem).m_atom)[0].m_f = *(prm_dst);
	((v_mem).m_atom)[1].m_f = ((v_mem).m_atom)[0].m_f + (host_float_t)(prm_val);

	while((v_mem).m_count--) {
		atom_cmpxchg(
				(__global volatile uint32_t* const)prm_dst
				, (v_mem).m_atom[0].m_u[(v_mem).m_count]
				, (v_mem).m_atom[1].m_u[(v_mem).m_count]
			    );
	}
}

#if 1

typedef clAtomAddF_t _clSlpPredict_t;

ae2f_MAC(__global, ) clSlpPredict(
		_clSlpPredict_t	v_mem,
		ae2f_float_t	ret,
		__local ae2f_float_t* const loc,
		const __global ae2f_float_t* const p_inp,
		const __global ae2f_float_t* const p_weight,
		const __global ae2f_float_t* const p_bias,
		const size_t iidx, const size_t isz, 
		const size_t oidx, const size_t osz,
		ae2f_AnnAct_t ACT
		)
{
	if((oidx) < (osz) && (iidx) < (isz)) {
		unless((iidx)) (loc)[oidx] = 0;
		ae2f_float_t v_tmp = (p_weight)[(oidx) * (isz) + (iidx)] * (p_inp)[iidx];
		_clAtomAddF(__global, v_mem, &(loc)[oidx], v_tmp);
		unless(iidx) {
			(loc)[oidx] += (p_bias)[oidx];
			ACT(&(ret), (loc), oidx, osz);
		}
	}

}

#endif

#define clSlpPredict	CL_Q_CVRT(_clSlpPredict)
#define clSlpPredict_t	CL_Q_CVRT(_clSlpPredict_t)

#endif
