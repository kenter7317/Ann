#include <ae2f/Ann/Slp.h>
#include <stdlib.h>

ae2f_SHAREDEXPORT void ae2f_AnnSlpInit(
		ae2f_AnnSlp_t* const _this,
		const size_t* const incs_optA,
		const size_t ginc_optB,
		const size_t outc,
		const size_t offset_opt,
		ae2f_err_t* const err_opt,
		size_t* const initsz_opt
		) noexcept
__ae2f_AnnSlpInit(
		_this
		, incs_optA, ginc_optB
		, outc
		, offset_opt, err_opt, initsz_opt
		);


ae2f_SHAREDEXPORT void ae2f_AnnSlpMk(
		const size_t* const incs_optA,
		const size_t ginc_optB,
		ae2f_float_t* const Field_opt,
		const size_t outc,
		const size_t offset_opt,
		ae2f_AnnSlp** const slp,
		ae2f_err_t* const err_opt
		) noexcept 
__ae2f_AnnSlpMk(
		incs_optA, ginc_optB
		, Field_opt
		, outc, offset_opt
		, slp, err_opt
	       );


ae2f_SHAREDEXPORT void ae2f_AnnSlpDel(ae2f_AnnSlp* const slp) 
	noexcept
{ __ae2f_AnnSlpDel(slp); }

ae2f_SHAREDEXPORT void ae2f_AnnSlpPredict(
		ae2f_err_t* const err_opt
		, const ae2f_AnnSlp* const _this
		, const ae2f_float_t* const prm_in
		, ae2f_float_t* const out
		, ae2f_AnnAct_t* const act_opt
		) noexcept
{
	if(act_opt) {
		__ae2f_AnnSlpPredict(
				err_opt, _this->m_Slp
				, prm_in, out
				, _this->m_field
				, (_this->m_field + (_this->m_Slp->m_inc * _this->m_Slp->m_outc))
				, act_opt
				);
	}
	else {
		__ae2f_AnnSlpPredict(
				err_opt
				, _this->m_Slp
				, prm_in, out
				, _this->m_field
				, _this->m_field + (_this->m_Slp[0].m_inc * _this->m_Slp[0].m_outc)
				, 
				);
	}
}


ae2f_SHAREDEXPORT void ae2f_AnnSlpFollow(
		ae2f_err_t* const reterr_opt

		, const ae2f_AnnSlp* const _this

		, ae2f_LP(_this::inc)
		const ae2f_float_t* const prm_in

		, ae2f_LP(_this::outc) 
		const ae2f_float_t* const delta

		, ae2f_float_t learningrate
		, ae2f_float_t learningrate_bias
		) noexcept
__ae2f_AnnSlpFollow(
		reterr_opt, _this->m_Slp
		, prm_in, delta, _this->m_field, _this->m_field + (_this->m_Slp[0].m_inc * _this->m_Slp[0].m_outc)
		, learningrate, learningrate_bias
		);
