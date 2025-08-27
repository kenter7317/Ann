#ifndef mac_h
#define mac_h

#define ae2f_MAC()	void
#define	CAST(t, x)	((t)(x))


/** Quick Pattern */
#if CL_Q
#define CL_Q_CVRT(x)	x##_Q
#else
#define CL_Q_CVRT(x)	x
#endif

#endif
