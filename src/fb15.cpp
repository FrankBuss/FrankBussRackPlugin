#include "FrankBuss.hpp"
#include "fb15.svg.hpp"

struct FB15 : Module {
	enum ParamIds {
		PITCH_PARAM,
		EXTERNAL_TRIGGER_LEVEL_PARAM,
		EXTERNAL_EXT_NOISE_PARAM,
		PITCH_TUNE_PARAM,
		PITCH_VCO_2_DETUNE_PARAM,

		VCO_1_FEET_PARAM,
		VCO_1_WAVE_PARAM,
		VCO_1_LFO_MOD_PARAM,
		VCO_1_PW_PARAM,
		VCO_1_PWM_PARAM,
		MIXER_1_EXT_NOISE_PARAM,
		MIXER_1_VCO_1_PARAM,
		VCF_1_CUT_OFF_FREQ_PARAM,
		VCF_1_RESONANCE_PARAM,
		VCF_1_HPF_BPF_LPF_PARAM,
		VCF_1_LFO_MOD_PARAM,
		VCF_1_EG_DEPTH_PARAM,
		VCF_1_EG_PARAM,
		VCA_1_INITIAL_LEVEL_PARAM,
		VCA_1_LFO_MOD_PARAM,
		VCA_1_EG_DEPTH_PARAM,
		VCA_1_EG_PARAM,

		VCO_2_FEET_PARAM,
		VCO_2_WAVE_PARAM,
		VCO_2_LFO_MOD_PARAM,
		VCO_2_PW_PARAM,
		VCO_2_PWM_PARAM,
		MIXER_2_VCO_1_PARAM,
		MIXER_2_VCO_2_PARAM,
		VCF_2_CUT_OFF_FREQ_PARAM,
		VCF_2_RESONANCE_PARAM,
		VCF_2_HPF_BPF_LPF_PARAM,
		VCF_2_LFO_MOD_PARAM,
		VCF_2_EG_DEPTH_PARAM,
		VCF_2_EG_PARAM,
		VCA_2_INITIAL_LEVEL_PARAM,
		VCA_2_LFO_MOD_PARAM,
		VCA_2_EG_DEPTH_PARAM,
		VCA_2_EG_PARAM,
		
		EG_1_EXT_KBD_PARAM,
		EG_1_NORMAL_TIMEX5_PARAM,
		EG_1_A_PARAM,
		EG_1_D_PARAM,
		EG_1_S_PARAM,
		EG_1_R_PARAM,

		EG_2_EXT_KBD_PARAM,
		EG_2_NORMAL_TIMEX5_PARAM,
		EG_2_A_PARAM,
		EG_2_D_PARAM,
		EG_2_S_PARAM,
		EG_2_R_PARAM,

		LFO_SPEED_PARAM,
		LFO_FUNCTION_PARAM,
		GLIDE_TIME_PARAM,
		GLIDE_VCO_1_DEPTH_PARAM,
		GLIDE_VCO_2_DEPTH_PARAM,
		
		PORTAMENTO_SLIDER_PARAM,
		BRILLIANCE_VCF_PARAM,
		BRILLIANCE_SLIDER_PARAM,
		PITCH_BEND_LIMITER_PARAM,
		PITCH_BEND_SLIDER_PARAM,

		OUTPUT_VOLUME_PARAM,
		
		NUM_PARAMS
	};
	enum InputIds {
		GATE_INPUT,
		V_OCT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO_OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		BLINK_LIGHT,
		NUM_LIGHTS
	};

	float phase = 0.0;
	float blinkPhase = 0.0;

	FB15() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		
		configParam(FB15::EXTERNAL_TRIGGER_LEVEL_PARAM, 0.0, 10.0, 0.0, "");                                                                                                               
		configParam(FB15::EXTERNAL_EXT_NOISE_PARAM, 0.0, 1.0, 0.0, "");
		configParam(FB15::PITCH_TUNE_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::PITCH_VCO_2_DETUNE_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCO_1_FEET_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCO_1_WAVE_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCO_1_LFO_MOD_PARAM, 0, 10.0, 0.0, "");
		configParam(FB15::VCO_1_PW_PARAM, 50.0, 90.0, 50.0, "");
		configParam(FB15::VCO_1_PWM_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::MIXER_1_EXT_NOISE_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::MIXER_1_VCO_1_PARAM, 0.0, 10.0, 10.0, "");
		configParam(FB15::VCF_1_CUT_OFF_FREQ_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCF_1_RESONANCE_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCF_1_HPF_BPF_LPF_PARAM, 0.0, 2.0, 0.0, "");
		configParam(FB15::VCF_1_LFO_MOD_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCF_1_EG_DEPTH_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCF_1_EG_PARAM, 0.0, 2.0, 0.0, "");
		configParam(FB15::VCA_1_INITIAL_LEVEL_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::VCA_1_LFO_MOD_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::VCA_1_EG_DEPTH_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::VCA_1_EG_PARAM, 0.0, 1.0, 0.0, "");
		configParam(FB15::VCO_2_FEET_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCO_2_WAVE_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCO_2_LFO_MOD_PARAM, 0, 10.0, 0.0, "");
		configParam(FB15::VCO_2_PW_PARAM, 50.0, 90.0, 50.0, "");
		configParam(FB15::VCO_2_PWM_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::MIXER_2_VCO_1_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::MIXER_2_VCO_2_PARAM, 0.0, 10.0, 10.0, "");
		configParam(FB15::VCF_2_CUT_OFF_FREQ_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCF_2_RESONANCE_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCF_2_HPF_BPF_LPF_PARAM, 0.0, 2.0, 0.0, "");
		configParam(FB15::VCF_2_LFO_MOD_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCF_2_EG_DEPTH_PARAM, -1.0, 1.0, 0.0, "");
		configParam(FB15::VCF_2_EG_PARAM, 0.0, 2.0, 0.0, "");
		configParam(FB15::VCA_2_INITIAL_LEVEL_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::VCA_2_LFO_MOD_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::VCA_2_EG_DEPTH_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::VCA_2_EG_PARAM, 0.0, 1.0, 0.0, "");
		configParam(FB15::EG_1_EXT_KBD_PARAM, 0.0, 1.0, 0.0, "");
		configParam(FB15::EG_1_NORMAL_TIMEX5_PARAM, 0.0, 1.0, 0.0, "");
		configParam(FB15::EG_1_A_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::EG_1_D_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::EG_1_S_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::EG_1_R_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::EG_2_EXT_KBD_PARAM, 0.0, 1.0, 0.0, "");
		configParam(FB15::EG_2_NORMAL_TIMEX5_PARAM, 0.0, 1.0, 0.0, "");
		configParam(FB15::EG_2_A_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::EG_2_D_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::EG_2_S_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::EG_2_R_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::LFO_SPEED_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::LFO_FUNCTION_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::GLIDE_TIME_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::GLIDE_VCO_1_DEPTH_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::GLIDE_VCO_2_DEPTH_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::PORTAMENTO_SLIDER_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::BRILLIANCE_VCF_PARAM, 0.0, 2.0, 0.0, "");
		configParam(FB15::BRILLIANCE_SLIDER_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::PITCH_BEND_LIMITER_PARAM, 0.0, 10.0, 0.0, "");
		configParam(FB15::PITCH_BEND_SLIDER_PARAM, 0.0, 10.00, 1.0, "");
		configParam(FB15::OUTPUT_VOLUME_PARAM, 0.0, 10.0, 10.0, "");
		
		
	}
	void process(const ProcessArgs &args) override;
};


void FB15::process(const ProcessArgs &args) {
#if 0
	// Implement a simple sine oscillator
	float deltaTime = args.sampleTime;

	// Compute the frequency from the pitch parameter and input
	float pitch = params[PITCH_PARAM].getValue();
	pitch += inputs[PITCH_INPUT].getVoltage();
	pitch = clamp(pitch, -4.0f, 4.0f);
	// The default pitch is C4
	float freq = 261.626f * powf(2.0f, pitch);

	// Accumulate the phase
	phase += freq * deltaTime;
	if (phase >= 1.0f)
		phase -= 1.0f;

	// Compute the sine output
	float sine = sinf(2.0f * M_PI * phase);
	outputs[SINE_OUTPUT].setVoltage(5.0f * sine);

	// Blink light at 1Hz
	blinkPhase += deltaTime;
	if (blinkPhase >= 1.0f)
		blinkPhase -= 1.0f;
	lights[BLINK_LIGHT].value = (blinkPhase < 0.5f) ? 1.0f : 0.0f;
#endif
}


struct FB15Widget : ModuleWidget {
	FB15Widget(FB15 *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/fb15.svg")));

		addParam(createParam<Davies1900hBlackKnob>(EXTERNAL_TRIGGER_LEVEL_POS, module, FB15::EXTERNAL_TRIGGER_LEVEL_PARAM));
		addParam(createParam<CKSS>(EXTERNAL_EXT_NOISE_POS, module, FB15::EXTERNAL_EXT_NOISE_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(PITCH_TUNE_POS, module, FB15::PITCH_TUNE_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(PITCH_VCO_2_DETUNE_POS, module, FB15::PITCH_VCO_2_DETUNE_PARAM));

		addParam(createParam<Davies1900hBlackKnob>(VCO_1_FEET_POS, module, FB15::VCO_1_FEET_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCO_1_WAVE_POS, module, FB15::VCO_1_WAVE_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCO_1_LFO_MOD_POS, module, FB15::VCO_1_LFO_MOD_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCO_1_PW_POS, module, FB15::VCO_1_PW_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCO_1_PWM_POS, module, FB15::VCO_1_PWM_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(MIXER_1_EXT_NOISE_POS, module, FB15::MIXER_1_EXT_NOISE_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(MIXER_1_VCO_1_POS, module, FB15::MIXER_1_VCO_1_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCF_1_CUT_OFF_FREQ_POS, module, FB15::VCF_1_CUT_OFF_FREQ_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCF_1_RESONANCE_POS, module, FB15::VCF_1_RESONANCE_PARAM));
		addParam(createParam<CKSSThree>(VCF_1_HPF_BPF_LPF_POS, module, FB15::VCF_1_HPF_BPF_LPF_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCF_1_LFO_MOD_POS, module, FB15::VCF_1_LFO_MOD_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCF_1_EG_DEPTH_POS, module, FB15::VCF_1_EG_DEPTH_PARAM));
		addParam(createParam<CKSSThree>(VCF_1_EG_POS, module, FB15::VCF_1_EG_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCA_1_INITIAL_LEVEL_POS, module, FB15::VCA_1_INITIAL_LEVEL_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCA_1_LFO_MOD_POS, module, FB15::VCA_1_LFO_MOD_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCA_1_EG_DEPTH_POS, module, FB15::VCA_1_EG_DEPTH_PARAM));
		addParam(createParam<CKSS>(VCA_1_EG_POS, module, FB15::VCA_1_EG_PARAM));

		addParam(createParam<Davies1900hBlackKnob>(VCO_2_FEET_POS, module, FB15::VCO_2_FEET_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCO_2_WAVE_POS, module, FB15::VCO_2_WAVE_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCO_2_LFO_MOD_POS, module, FB15::VCO_2_LFO_MOD_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCO_2_PW_POS, module, FB15::VCO_2_PW_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCO_2_PWM_POS, module, FB15::VCO_2_PWM_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(MIXER_2_VCO_1_POS, module, FB15::MIXER_2_VCO_1_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(MIXER_2_VCO_2_POS, module, FB15::MIXER_2_VCO_2_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCF_2_CUT_OFF_FREQ_POS, module, FB15::VCF_2_CUT_OFF_FREQ_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCF_2_RESONANCE_POS, module, FB15::VCF_2_RESONANCE_PARAM));
		addParam(createParam<CKSSThree>(VCF_2_HPF_BPF_LPF_POS, module, FB15::VCF_2_HPF_BPF_LPF_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCF_2_LFO_MOD_POS, module, FB15::VCF_2_LFO_MOD_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCF_2_EG_DEPTH_POS, module, FB15::VCF_2_EG_DEPTH_PARAM));
		addParam(createParam<CKSSThree>(VCF_2_EG_POS, module, FB15::VCF_2_EG_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCA_2_INITIAL_LEVEL_POS, module, FB15::VCA_2_INITIAL_LEVEL_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCA_2_LFO_MOD_POS, module, FB15::VCA_2_LFO_MOD_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(VCA_2_EG_DEPTH_POS, module, FB15::VCA_2_EG_DEPTH_PARAM));
		addParam(createParam<CKSS>(VCA_2_EG_POS, module, FB15::VCA_2_EG_PARAM));
		
		addParam(createParam<CKSS>(EG_1_EXT_KBD_POS, module, FB15::EG_1_EXT_KBD_PARAM));
		addParam(createParam<CKSS>(EG_1_NORMAL_TIMEX5_POS, module, FB15::EG_1_NORMAL_TIMEX5_PARAM));
		addParam(createParam<BefacoSlidePot>(EG_1_A_POS, module, FB15::EG_1_A_PARAM));
		addParam(createParam<BefacoSlidePot>(EG_1_D_POS, module, FB15::EG_1_D_PARAM));
		addParam(createParam<BefacoSlidePot>(EG_1_S_POS, module, FB15::EG_1_S_PARAM));
		addParam(createParam<BefacoSlidePot>(EG_1_R_POS, module, FB15::EG_1_R_PARAM));

		addParam(createParam<CKSS>(EG_2_EXT_KBD_POS, module, FB15::EG_2_EXT_KBD_PARAM));
		addParam(createParam<CKSS>(EG_2_NORMAL_TIMEX5_POS, module, FB15::EG_2_NORMAL_TIMEX5_PARAM));
		addParam(createParam<BefacoSlidePot>(EG_2_A_POS, module, FB15::EG_2_A_PARAM));
		addParam(createParam<BefacoSlidePot>(EG_2_D_POS, module, FB15::EG_2_D_PARAM));
		addParam(createParam<BefacoSlidePot>(EG_2_S_POS, module, FB15::EG_2_S_PARAM));
		addParam(createParam<BefacoSlidePot>(EG_2_R_POS, module, FB15::EG_2_R_PARAM));

		addParam(createParam<Davies1900hBlackKnob>(LFO_SPEED_POS, module, FB15::LFO_SPEED_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(LFO_FUNCTION_POS, module, FB15::LFO_FUNCTION_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(GLIDE_TIME_POS, module, FB15::GLIDE_TIME_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(GLIDE_VCO_1_DEPTH_POS, module, FB15::GLIDE_VCO_1_DEPTH_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(GLIDE_VCO_2_DEPTH_POS, module, FB15::GLIDE_VCO_2_DEPTH_PARAM));
		
		addParam(createParam<BefacoSlidePot>(PORTAMENTO_SLIDER_POS, module, FB15::PORTAMENTO_SLIDER_PARAM));
		addParam(createParam<CKSSThree>(BRILLIANCE_VCF_POS, module, FB15::BRILLIANCE_VCF_PARAM));
		addParam(createParam<BefacoSlidePot>(BRILLIANCE_SLIDER_POS, module, FB15::BRILLIANCE_SLIDER_PARAM));
		addParam(createParam<Davies1900hBlackKnob>(PITCH_BEND_LIMITER_POS, module, FB15::PITCH_BEND_LIMITER_PARAM));
		addParam(createParam<BefacoSlidePot>(PITCH_BEND_SLIDER_POS, module, FB15::PITCH_BEND_SLIDER_PARAM));
		
		addParam(createParam<Davies1900hBlackKnob>(OUTPUT_VOLUME_POS, module, FB15::OUTPUT_VOLUME_PARAM));
		

		addInput(createInput<PJ301MPort>(GATE_POS, module, FB15::GATE_INPUT));
		addInput(createInput<PJ301MPort>(V_OCT_POS, module, FB15::V_OCT_INPUT));
		addOutput(createOutput<PJ301MPort>(AUDIO_OUT_POS, module, FB15::AUDIO_OUT_OUTPUT));

/*
		addChild(createLight<MediumLight<RedLight>>(Vec(41, 59), module, FB15::BLINK_LIGHT));
		*/
	}
};


Model *modelFB15 = createModel<FB15, FB15Widget>("FB15");
