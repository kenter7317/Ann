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

        inline cl_int WeightCheck(ae2f_float_t* buff, cl_command_queue queue) const noexcept {
            return ae2fCL_AnnSpWeightCheck(this, buff, queue);
        }

        inline ae2f_err_t Predict(
            ae2fCL_HostPtr(__global, ae2f_float_t) in,
            ae2fCL_HostPtr(__global, ae2f_float_t) out,
            uint32_t in_idx,
            uint32_t out_idx,
            ae2f_float_t* outbuff_optional,
            cl_command_queue queue,
            cl_bool blocking_event,
            cl_uint num_events_in_wait_list,
            const cl_event *event_wait_list,
            cl_event *event
        ) const noexcept {
            return ae2fCL_AnnSpPredictA(
                this, in, out, in_idx, out_idx, 
                outbuff_optional, queue, blocking_event, 
                num_events_in_wait_list, event_wait_list, event
            );
        }

        inline ae2f_err_t Predict(
            ae2fCL_HostPtr(__global, ae2f_float_t) in,
            uint32_t in_idx,
            ae2f_float_t* outbuff_optional,
            cl_command_queue queue,
            cl_bool blocking_event,
            cl_uint num_events_in_wait_list,
            const cl_event *event_wait_list,
            cl_event *event,
            cl_context context
        ) const noexcept {
            return ae2fCL_AnnSpPredictB(
                this, in, in_idx,
                outbuff_optional, context,
                queue, blocking_event, num_events_in_wait_list,
                event_wait_list, event  
            );
        }

        inline ae2f_err_t Predict(
            const ae2f_float_t* in,
            ae2f_float_t* out,
            cl_command_queue queue,
            cl_bool blocking_read,
            cl_uint num_events_in_wait_list,
            const cl_event *event_wait_list,
            cl_event *event,
            cl_context context
        ) const noexcept {
            return ae2fCL_AnnSpPredictBuffAuto(
                this, in, out, queue,
                blocking_read, num_events_in_wait_list,
                event_wait_list, event, context
            );
        }

        inline ae2f_err_t Train(
            ae2fCL_HostPtr(__global, ae2f_float_t) in,
            ae2fCL_HostPtr(__global, ae2f_float_t) out,
            uint32_t in_idx,
            uint32_t out_idx,

            ae2f_float_t goal,
            ae2f_float_t learning_rate,
            ae2f_float_t* diff_ret_optional,

            cl_command_queue queue,
            cl_bool blocking_read,
            cl_uint num_events_in_wait_list,
            const cl_event *event_wait_list,
            cl_event *event
        ) noexcept {
            return ae2fCL_AnnSpTrainA(
                this, in, out, in_idx, out_idx,
                goal, learning_rate, diff_ret_optional,
                queue, blocking_read, num_events_in_wait_list,
                event_wait_list, event
            );
        }

        inline ae2f_err_t Train(
            ae2fCL_HostPtr(__global, ae2f_float_t) in,
            uint32_t in_idx,

            ae2f_float_t goal,
            ae2f_float_t learning_rate,
            ae2f_float_t* diff_ret_optional,

            cl_command_queue queue,
            cl_bool blocking_read,
            cl_uint num_events_in_wait_list,
            const cl_event *event_wait_list,
            cl_event *event,

            cl_context context
        ) noexcept {
            return ae2fCL_AnnSpTrainB(
                this, in, in_idx, goal, learning_rate,
                diff_ret_optional, queue, blocking_read,
                num_events_in_wait_list, event_wait_list,
                event, context
            );
        }

        inline ae2f_err_t Train(
            ae2fCL_HostPtr(__global, ae2f_float_t) in,
            uint32_t in_idx,

            ae2f_float_t learning_rate,
            ae2f_float_t* diff_ret_optional,
            const ae2f_float_t* delta_precalculated,

            cl_command_queue queue,
            cl_bool blocking_read,
            cl_uint num_events_in_wait_list,
            const cl_event *event_wait_list,
            cl_event *event
        ) {
            return ae2fCL_AnnSpTrainC(
                this, in, in_idx, learning_rate, diff_ret_optional,
                delta_precalculated, queue, blocking_read, 
                num_events_in_wait_list, event_wait_list, event
            );
        }
    };

    struct cSp : public cSpRefer {
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