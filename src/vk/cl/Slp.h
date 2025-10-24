#ifndef Slp_h
#define Slp_h

#define ae2f_NEED_CLASS 0

#include "mac.h"
#include <ae2f/Ann/Slp.core.h>
#include <ae2f/Ann/Slp.auto.h>


#if !__ae2f_MACRO_GENERATED
#define _clAtomAddF_tmpl(a, b, c, d, e)	clAtomAddF_tmpl(d, e)
#define _clAtomAddF(a, b, c, d)		clAtomAddF(b, c, d)
#endif

#define _clAtomAddF_t(__global, host_float_t) struct { \
	union { \
		host_float_t	m_f;  \
		host_float_t	m_fa[MAX(1, 4 / sizeof(host_float_t))]; \
		uint		m_u[(MAX(1, sizeof(host_float_t) >> 2))]; \
	} m_atom[2]; \
	uint			m_count; \
	union { \
		__global volatile uint* 	m_pchg; \
		__global volatile host_float_t* m_fp; \
		intptr_t			m_ip; \
	} m_U0; \
}

#if !__ae2f_MACRO_GENERATED
typedef _clAtomAddF_t(__global, host_float_t) clAtomAddF_t;
#endif

ae2f_MAC(__global, ) clAtomAddF(clAtomAddF_t v_mem, __global volatile host_float_t* prm_dst, ae2f_float_t prm_val)
{
	if(sizeof((v_mem).m_atom[0].m_f) < 4) {
		(v_mem).m_count = 4 / sizeof((v_mem).m_atom[0].m_f);
		while((v_mem).m_count--) {
			(v_mem).m_atom[0].m_fa[(v_mem).m_count]
				= (prm_dst)[(v_mem).m_count]
				;
		}
	} else {
		((v_mem).m_atom)[0].m_f = ((v_mem).m_atom)[1].m_f = *(prm_dst);
	}

	((v_mem).m_atom)[1].m_f += (prm_val);

	(v_mem).m_count = MAX(1, (sizeof(((v_mem).m_atom[0].m_f)) >> 2));
	while((v_mem).m_count--) {
		(v_mem).m_U0.m_fp = (prm_dst);
		(v_mem).m_U0.m_pchg += (v_mem).m_count;
		atom_xchg_u(
				(v_mem).m_U0.m_pchg
				, (v_mem).m_atom[1].m_u[(v_mem).m_count]
			   );
	}
}

#if !__ae2f_MACRO_GENERATED
#define _clSlpPredict_t	_clAtomAddF_t
typedef clAtomAddF_t clSlpPredict_t;
#endif 

ae2f_MAC(__global, ) clSlpPredict(
		clSlpPredict_t	v_mem,
		ae2f_float_t	ret,
		__local ae2f_float_t* const loc,
		const __global ae2f_float_t* const p_inp,
		const __global ae2f_float_t* const p_weight,
		const __global ae2f_float_t* const p_bias,
		const size_t iidx, const size_t isz, 
		const size_t oidx, const size_t osz,
		ae2f_AnnActFFN_t ACT
		)
{
	if((oidx) < (osz) && (iidx) < (isz)) {
		unless((iidx)) (loc)[oidx] = 0;

		if(sizeof((v_mem).m_atom[0].m_f) >= 4) {
			_clAtomAddF(__global
					, v_mem, &(loc)[oidx]
					, (p_weight)[(oidx) * (isz) + (iidx)] * (p_inp)[iidx]
				   );
		} else {
			unless((oidx) & 1) {
				(v_mem).m_atom[0].m_u[0] = 
					CAST(__global uint*, loc)[0];

				(v_mem).m_atom[1].m_fa[0]
					= (v_mem).m_atom[1].m_fa[0] 
					+ (p_weight)[(oidx) * (isz) + (iidx)] * (p_inp)[iidx]
					;

				if((oidx) + 1 < (osz))
					(v_mem).m_atom[1].m_fa[1]
						= (v_mem).m_atom[1].m_fa[1] 
						+ (p_weight)[((oidx) + 1) * (isz) + (iidx)] * (p_inp)[iidx]
						;

				atom_xchg_u(CAST(__global uint*, loc), (v_mem).m_atom[1].m_u[0]);
			}
		}

		unless(iidx) {
			(loc)[oidx] += (p_bias)[oidx];
			ACT(&(ret), (loc), oidx, osz);
		}
	}

}


#endif
