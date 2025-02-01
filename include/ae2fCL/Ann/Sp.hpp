/// @file Sp.hpp
#ifndef ae2fCL_Ann_Slp_hpp
#define ae2fCL_Ann_Slp_hpp

#include "Sp.h"

namespace ae2fCL { namespace Ann {
    using Sp = ae2fCL_AnnSp;

    struct cSp;
    class cSpRefer : public Sp {
        friend cSp;
        constexpr cSpRefer() noexcept : Sp() {}
        
        public:
        constexpr cSpRefer(const Sp& slp) noexcept : Sp(slp) {}
        constexpr cSpRefer(const Sp&& slp) noexcept : Sp(slp) {}

        constexpr cSpRefer(const Sp& slp, const ae2fCL_fpAnnAct_t act, const ae2fCL_fpAnnSpGetLoss_t loss) 
            noexcept : Sp(slp) { this->mAct = act; this->mpGetLoss = loss; }
        constexpr cSpRefer(const Sp&& slp, const ae2fCL_fpAnnAct_t act, const ae2fCL_fpAnnSpGetLoss_t loss) 
            noexcept : Sp(slp) { this->mAct = act; this->mpGetLoss = loss; }
    };

    struct cSp : public Sp {
        inline ~cSp() noexcept {
            ae2fCL_AnnSpDel(this);
        }

        inline cSp(
            ae2f_err_t* err_optional,
            const ae2f_float_t* inputs_optional,
            size_t inputsCount,
            ae2fCL_fpAnnAct_t mAct,
            ae2fCL_fpAnnSpGetLoss_t fpGetLoss,
            cl_context ctx,
            cl_command_queue queue,
            cl_bool blocking_read,
            cl_uint num_events_in_wait_list,
            const cl_event *event_wait_list,
            cl_event *event
        ) noexcept {
            ae2f_err_t err = 0;
            if(!err_optional) err_optional = &err;

            *err_optional |= ae2fCL_AnnSpMk(
                this, inputs_optional, inputsCount, 
                mAct, fpGetLoss, ctx, queue, 
                blocking_read, num_events_in_wait_list, 
                event_wait_list, event
            );
        }

        inline cSp(
            ae2f_err_t* err_optional,
            size_t WeightsCount,
            ae2fCL_fpAnnAct_t mAct,
            ae2fCL_fpAnnSpGetLoss_t fpGetLoss,
            cl_context ctx,
            cl_command_queue queue,
            cl_bool blocking_read,
            cl_uint num_events_in_wait_list,
            const cl_event* event_wait_list,
            cl_event* event
        ) noexcept : cSp(
            err_optional, 0, WeightsCount, mAct, 
            fpGetLoss, ctx, queue, blocking_read, 
            num_events_in_wait_list, event_wait_list, event
        ) {}
    };
}}

#endif