/// @file Slp.hpp
#ifndef ae2fCL_Ann_Slp_hpp
#define ae2fCL_Ann_Slp_hpp

#include "Slp.h"

namespace ae2fCL { namespace Ann {
    using rSlp = ae2fCL_AnnSlp;

    struct cSlp;
    class cSlpRefer : public rSlp {
        friend cSlp;
        constexpr cSlpRefer() : rSlp() {}
        
        public:
        constexpr cSlpRefer(const rSlp& slp) : rSlp(slp) {}
        constexpr cSlpRefer(const rSlp&& slp) : rSlp(slp) {}

        constexpr cSlpRefer(const rSlp& slp, const ae2fCL_fpAnnAct_t act, const ae2fCL_fpAnnSlpGetLoss_t loss) 
            : rSlp(slp) { this->mAct = act; this->mpGetLoss = loss; }
        constexpr cSlpRefer(const rSlp&& slp, const ae2fCL_fpAnnAct_t act, const ae2fCL_fpAnnSlpGetLoss_t loss) 
            : rSlp(slp) { this->mAct = act; this->mpGetLoss = loss; }

        inline cl_int WeightCheck(ae2f_float_t* buff, cl_command_queue queue) const {
            return ae2fCL_AnnSlpWeightCheck(this, buff, queue);
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
        ) const {
            return ae2fCL_AnnSlpPredictA(
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
        ) const {
            return ae2fCL_AnnSlpPredictB(
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
        ) const {
            return ae2fCL_AnnSlpPredictBuffAuto(
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
        ) {
            return ae2fCL_AnnSlpTrainA(
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
        ) {
            return ae2fCL_AnnSlpTrainB(
                this, in, in_idx, goal, learning_rate,
                diff_ret_optional, queue, blocking_read,
                num_events_in_wait_list, event_wait_list,
                event, context
            );
        }
    };

    struct cSlp : public cSlpRefer {
        inline ~cSlp() {
            ae2fCL_AnnSlpDel(this);
        }

        inline cSlp(
            ae2f_err_t* err_optional,
            const ae2f_float_t* inputs_optional,
            size_t inputsCount,
            ae2fCL_fpAnnAct_t mAct,
            ae2fCL_fpAnnSlpGetLoss_t fpGetLoss,
            cl_context ctx,
            cl_command_queue queue,
            cl_bool blocking_read,
            cl_uint num_events_in_wait_list,
            const cl_event *event_wait_list,
            cl_event *event
        ) {
            ae2f_err_t err = 0;
            if(!err_optional) err_optional = &err;

            *err_optional |= ae2fCL_AnnSlpMk(
                this, inputs_optional, inputsCount, 
                mAct, fpGetLoss, ctx, queue, 
                blocking_read, num_events_in_wait_list, 
                event_wait_list, event
            );
        }

        inline cSlp(
            ae2f_err_t* err_optional,
            size_t WeightsCount,
            ae2fCL_fpAnnAct_t mAct,
            ae2fCL_fpAnnSlpGetLoss_t fpGetLoss,
            cl_context ctx,
            cl_command_queue queue,
            cl_bool blocking_read,
            cl_uint num_events_in_wait_list,
            const cl_event* event_wait_list,
            cl_event* event
        ) : cSlp(
            err_optional, 0, WeightsCount, mAct, 
            fpGetLoss, ctx, queue, blocking_read, 
            num_events_in_wait_list, event_wait_list, event
        ) {}
    };
}}

#endif