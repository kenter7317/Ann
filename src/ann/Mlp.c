#include <ann-h/Mlp.h>

ae2f_SHAREDEXPORT
size_t ae2f_mAnnMlpInit(
    ae2f_mAnnMlp* _this,
    size_t layerc,
    size_t add_opt,
    const size_t* layerlenv,
    const size_t* layerpadv_opt_unused,
    const size_t* inpadv_opt,
    const ae2f_fpAnnAct_t* actv_opt,
    const ae2f_fpAnnAct_t* act_deriv_v_opt,
    const ae2f_fpAnnLoss_t* loss_v_opt,
    const ae2f_float_t* weights_opt,
    ae2f_err_t* errret_opt
) {
    ae2f_err_t err = 0;
    #define return(code) { err = code; goto EXIT; } 
    if(!_this) return(ae2f_errGlob_PTR_IS_NULL);
    if(!layerlenv) return(ae2f_errGlob_PTR_IS_NULL);
    if(layerc == 1) 
	    return(ae2f_errGlob_WRONG_OPERATION);

    _this->expected = 1;
    size_t max = _this->inc = *layerlenv;
    _this->outc = layerlenv[layerc - 1];
    _this->vClean = Clean;
    _this->vPredict = Predict;
    _this->vTrain = Train;
    _this->layerc = --layerc;

    for(size_t i = 0; i < layerc; i++) {
	    size_t 
		    LAYERSZ_L = layerlenv[i],
		    LAYERSZ_R = layerlenv[i + 1];

		    if(max < LAYERSZ_R)
			    max = LAYERSZ_R;

		    ae2f_err_t e = 0;

		    union {
			    size_t** unused;
			    union {
				    size_t* pad;
				    ae2f_mAnnSlp* slp;
			    }* u;
		    } perc = {
			    ae2f_mAnnMlpLayerVPad(_this) + i
		    };

		    perc.u->pad = calloc(
				    ae2f_mAnnSlpInitSz(
					    LAYERSZ_R, 
					    sizeof(size_t)
					    )
				    , 1
				    );

		    perc.u->pad++;
		    ae2f_mAnnSlpInitB(
				    perc.u->slp
				    , LAYERSZ_L
				    , 0, weights_opt
				    , actv_opt ? actv_opt[i] : 0

					, act_deriv_v_opt 
					? act_deriv_v_opt[i]
					: 0 

					, loss_v_opt ? loss_v_opt[i] : 0
				    , LAYERSZ_R, 0, &e
				    );

		    *(--perc.u->pad) 
			    = layerpadv_opt_unused 
			    ? layerpadv_opt_unused[i] 
			    : 0;

		    if(weights_opt) 
			    weights_opt += LAYERSZ_L * LAYERSZ_R; 

		    if(e)
			    err = err | e & ~ae2f_errGlob_DONE_HOWEV;


    }

    ae2f_mAnnMlpCache(_this,)[0] = calloc(
        (max * layerc) << 2,
        sizeof(ae2f_float_t)
    );
    *ae2f_mAnnMlpLayerBuffCount(_this) = max;

    EXIT:
    #undef return
    if(errret_opt) *errret_opt = err;
    return ae2f_mAnnMlpInitSz(++layerc, add_opt);
}

ae2f_SHAREDEXPORT
ae2f_AnnMlp* ae2f_AnnMlpMk(
		size_t layerc,
		size_t add_opt,
		const size_t* layerlenv,
		const size_t* layerpadv_opt,
		const size_t* inpadv_opt,
		const ae2f_fpAnnAct_t* actv_opt,
		const ae2f_fpAnnAct_t* act_deriv_v_opt,
		const ae2f_fpAnnLoss_t* loss_v_opt,
		const ae2f_float_t* weights_opt,
		ae2f_err_t* errret_opt
) noexcept {
    ae2f_AnnMlp* obj = calloc(ae2f_mAnnMlpInitSz(layerc, add_opt), 1);
    ae2f_mAnnMlpInit(
		    &obj->Mlp
		    , layerc
		    , add_opt
		    , layerlenv
		    , layerpadv_opt
		    , inpadv_opt
		    , actv_opt
		    , act_deriv_v_opt
		    , loss_v_opt
		    , weights_opt
		    , errret_opt
    );
    return obj;
}
