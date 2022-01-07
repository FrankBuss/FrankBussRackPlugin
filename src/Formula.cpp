#include "FrankBuss.hpp"
#include "formula/Formula.h"

struct FrankBussFormulaModule : Module {
	enum ParamIds {
		X_PARAM,
		Y_PARAM,
		Z_PARAM,
		KNOB_PARAM,
		CLAMP_PARAM,
		B_MINUS_1_PARAM,
		B_0_PARAM,
		B_1_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		X_INPUT,
		Y_INPUT,
		Z_INPUT,
		W_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		FORMULA_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ERROR_LIGHT,
		OK_LIGHT,
		CLAMP_LIGHT,
		B_MINUS_1_LIGHT,
		B_0_LIGHT,
		B_1_LIGHT,
		NUM_LIGHTS
	};

	std::string textField;
	std::string freqField;
	float blinkPhase = 0.0f;

	Formula formula;
	Formula freqFormula;
	bool compiled = false;
	bool textDirty = false;
	bool freqDirty = false;
	bool doclamp = true;
	bool freqFormulaEnabled = false;
	float radiobutton = 0.0f;
	float phase[PORT_MAX_CHANNELS];

	dsp::SchmittTrigger clampTrigger;
	dsp::SchmittTrigger bMinus1Trigger;
	dsp::SchmittTrigger b0Trigger;
	dsp::SchmittTrigger b1Trigger;

	float* formulaP = NULL;
	float* formulaK = NULL;
	float* formulaB = NULL;
	float* formulaW = NULL;
	float* formulaX = NULL;
	float* formulaY = NULL;
	float* formulaZ = NULL;

	float* freqFormulaP = NULL;
	float* freqFormulaK = NULL;
	float* freqFormulaB = NULL;
	float* freqFormulaW = NULL;
	float* freqFormulaX = NULL;
	float* freqFormulaY = NULL;
	float* freqFormulaZ = NULL;

	FrankBussFormulaModule() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		// configParam(FrankBussFormulaModule::B_MINUS_1_PARAM, 0.0f, 1.0f, 0.0f, "");
		// configParam(FrankBussFormulaModule::B_0_PARAM, 0.0f, 1.0f, 0.0f, "");
		// configParam(FrankBussFormulaModule::B_1_PARAM, 0.0f, 1.0f, 0.0f, "");
		configButton(B_MINUS_1_PARAM, "Variable 'b': -1");
		configButton(B_0_PARAM, "Variable 'b': 0");
		configButton(B_1_PARAM, "Variable 'b': 1");
		configParam(FrankBussFormulaModule::KNOB_PARAM, -1.0f, 1.0f, 0.0f, "Variable 'k'");
		// configParam(FrankBussFormulaModule::CLAMP_PARAM, 0.0f, 1.0f, 0.0f, "");
		configButton(CLAMP_PARAM, "Clamp to -5V/+5V");
		
		configLight(ERROR_LIGHT, "Status:\n  green light: ok\n  red blinking light: error\n  -------------------------------\n ");
		
		configInput(X_INPUT, "Variable 'x'");
		configInput(Y_INPUT, "Variable 'y'");
		configInput(Z_INPUT, "Variable 'z'");
		configInput(W_INPUT, "Variable 'w'");
		configOutput(FORMULA_OUTPUT, "Result");
	}
	
	int getMaxChannels() {
		int channels = inputs[W_INPUT].getChannels();
		channels = max(channels, inputs[X_INPUT].getChannels());
		channels = max(channels, inputs[Y_INPUT].getChannels());
		channels = max(channels, inputs[Z_INPUT].getChannels());
		if (channels == 0) channels = 1;
		return channels;
	}

	void process(const ProcessArgs &args) override {
		if (clampTrigger.process(params[CLAMP_PARAM].getValue())) {
			doclamp = !doclamp;
		}
		if (bMinus1Trigger.process(params[B_MINUS_1_PARAM].getValue())) {
			radiobutton = -1.0f;
		}	
		if (b0Trigger.process(params[B_0_PARAM].getValue())) {
			radiobutton = 0.0f;
		}
		if (b1Trigger.process(params[B_1_PARAM].getValue())) {
			radiobutton = 1.0f;
		}
		
		float deltaTime = args.sampleTime;

		// evaluate frequency and output formula
		int channels = getMaxChannels();
		if (compiled) {
			for (int c = 0; c < channels; c++) {
				try {
					// get inputs
					float w = inputs[W_INPUT].getVoltage(c);
					float x = inputs[X_INPUT].getVoltage(c);
					float y = inputs[Y_INPUT].getVoltage(c);
					float z = inputs[Z_INPUT].getVoltage(c);
					
					// knob
					float k = params[KNOB_PARAM].getValue();

					// set all variables
					*formulaP = phase[c];
					*formulaK = k;
					*formulaB = radiobutton;
					*formulaW = w;
					*formulaX = x;
					*formulaY = y;
					*formulaZ = z;

					if (freqFormulaEnabled) {
						*freqFormulaP = phase[c];
						*freqFormulaK = k;
						*freqFormulaB = radiobutton;
						*freqFormulaW = w;
						*freqFormulaX = x;
						*freqFormulaY = y;
						*freqFormulaZ = z;
						float freq = evalFormula(freqFormula);
						phase[c] += freq * args.sampleTime;
						if (phase[c] > 1.0f) phase[c] -= 1.0f;
					}
					float val = evalFormula(formula);
					if (doclamp) val = clamp(val, -5.0f, 5.0f);
					outputs[FORMULA_OUTPUT].setVoltage(val, c);
				} catch (MathError&) {
					// ignore math errors, e.g. division by zero
					outputs[FORMULA_OUTPUT].setVoltage(0, c);
				} catch (exception&) {
					// for all other exceptions, set compiled to false, e.g. VariableNotFound
					compiled = false;
				}
			}
		} else {
			for (int c = 0; c < channels; c++) {
				outputs[FORMULA_OUTPUT].setVoltage(0, c);
			}
		}
		outputs[FORMULA_OUTPUT].setChannels(channels);


		// Blink light at 1Hz
		blinkPhase += deltaTime;
		if (blinkPhase >= 1.0f)
			blinkPhase -= 1.0f;
		if (compiled) {
			lights[OK_LIGHT].value = 0;
			lights[ERROR_LIGHT].value = 1;
		} else {
			lights[OK_LIGHT].value = (blinkPhase < 0.5f) ? 1.0f : 0.0f;
			lights[ERROR_LIGHT].value = 0;
		}

		lights[CLAMP_LIGHT].value = (doclamp);
		lights[B_MINUS_1_LIGHT].value = (radiobutton == -1.0f);
		lights[B_0_LIGHT].value = (radiobutton == -0.0f);
		lights[B_1_LIGHT].value = (radiobutton == 1.0f);
	}

	void parseFormula(Formula& formula, std::string expr) {
		formula.setVariable("pi", M_PI);
		formula.setVariable("e", M_E);
		
		formula.setVariable("p", 0);
		formula.setVariable("k", 0);
		formula.setVariable("b", 0);
		formula.setVariable("w", 0);
		formula.setVariable("x", 0);
		formula.setVariable("y", 0);
		formula.setVariable("z", 0);

		formula.setExpression(expr);
	}

	float evalFormula(Formula& formula) {
		// eval
		float val = formula.eval();
		if (!isfinite(val) || isnan(val)) val = 0.0f;
		return val;
	}

	void compile()
	{
		compiled = false;
		for (int c = 0; c < PORT_MAX_CHANNELS; c++) phase[c] = 0;
		if (textField.size() > 0) {
			try {
				parseFormula(formula, textField);
				freqFormulaEnabled = false;
				if (freqField.size() > 0) {
					parseFormula(freqFormula, freqField);
					freqFormulaEnabled = true;
				}
				
				formulaP = formula.getVariableAddress("p");
				formulaK = formula.getVariableAddress("k");
				formulaB = formula.getVariableAddress("b");
				formulaW = formula.getVariableAddress("w");
				formulaX = formula.getVariableAddress("x");
				formulaY = formula.getVariableAddress("y");
				formulaZ = formula.getVariableAddress("z");

				if (freqFormulaEnabled) {
					freqFormulaP = freqFormula.getVariableAddress("p");
					freqFormulaK = freqFormula.getVariableAddress("k");
					freqFormulaB = freqFormula.getVariableAddress("b");
					freqFormulaW = freqFormula.getVariableAddress("w");
					freqFormulaX = freqFormula.getVariableAddress("x");
					freqFormulaY = freqFormula.getVariableAddress("y");
					freqFormulaZ = freqFormula.getVariableAddress("z");
				}
				
				compiled = true;
			} catch (exception& e) {
				printf("formula exception: %s\n", e.what());
			}
		}
	}

	// void onAdd(const AddEvent &e) override
	// {
		// textField = "";
		// freqField = "";
		// compile();
		// dirty = true;
	// }

	void onReset (const ResetEvent &e) override
	{
		textField = "";
		freqField = "";
		compile();
		textDirty = true;
		freqDirty = true;
	}

	json_t *dataToJson() override {
		json_t *rootJ = json_object();

		json_object_set_new(rootJ, "text", json_string(textField.c_str()));
		json_object_set_new(rootJ, "freq", json_string(freqField.c_str()));
		json_object_set_new(rootJ, "clamp", json_boolean(doclamp));
		json_object_set_new(rootJ, "button", json_real(radiobutton));
		return rootJ;
	}

	void dataFromJson(json_t *rootJ) override {
		json_t *textJ = json_object_get(rootJ, "text");
		if (textJ) textField = json_string_value(textJ);

		json_t *freqJ = json_object_get(rootJ, "freq");
		if (freqJ) freqField = json_string_value(freqJ);

		json_t *clampJ = json_object_get(rootJ, "clamp");
		if (clampJ) doclamp = json_is_true(clampJ);
		
		json_t *buttonJ = json_object_get(rootJ, "button");
		if (buttonJ) radiobutton = json_real_value(buttonJ);

		compile();
		textDirty = true;
		freqDirty = true;
	}

};

enum TextFieldType {
	TEXT,
	FREQ
};

struct FormulaTextField : LedDisplayTextField {
	FrankBussFormulaModule* module;
	TextFieldType type;
	
	FormulaTextField() : LedDisplayTextField() {
		bgColor = nvgRGB(0x00, 0x00, 0x00);
	}
	
	void setModule(FrankBussFormulaModule* module) {
		this->module = module;
	}
	
	void setTextFieldType(TextFieldType type) {
		this->type = type;
	}
	
    void onChange(const event::Change &e) override {
		if (module) {
			if (type == TEXT) {
				module->textField = getText();
			} else if (type == FREQ) {
				module->freqField = getText();
			}
			module->compile();
		}
	}
	
	void step() override {
		LedDisplayTextField::step();
		if (module && (module->textDirty || module->freqDirty)) {
			if (type == TEXT) {
				setText(module->textField);
				module->textDirty = false;
			} else if (type == FREQ) {
				setText(module->freqField);
				module->freqDirty = false;
			}
		}
	}
};

struct FrankBussFormulaWidget : ModuleWidget {
	FormulaTextField* textField;
	FormulaTextField* freqField;
	
	FrankBussFormulaWidget(FrankBussFormulaModule *module) {
		setModule(module);

		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/formula.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		LedDisplay* textDisplay = createWidget<LedDisplay>(mm2px(Vec(3, 13)));
		textDisplay->box.size = mm2px(Vec(85, 51));
		addChild(textDisplay);
		textField = createWidget<FormulaTextField>(mm2px(Vec(3, 13)));
		textField->setModule(module);
		textField->setTextFieldType(TEXT);
		textField->box.size = mm2px(Vec(85, 51));
		textField->multiline = true;
		addChild(textField);

		LedDisplay* freqDisplay = createWidget<LedDisplay>(mm2px(Vec(16, 67.5)));
		freqDisplay->box.size = mm2px(Vec(72, 10));
		addChild(freqDisplay);
		freqField = createWidget<FormulaTextField>(mm2px(Vec(16, 67.5)));
		freqField->setModule(module);
		freqField->setTextFieldType(FREQ);
		freqField->box.size = mm2px(Vec(72, 10));
		freqField->multiline = false;
		addChild(freqField);

		addParam(createParam<LEDButton>(Vec(30, 260), module, FrankBussFormulaModule::B_MINUS_1_PARAM));
		addChild(createLight<MediumLight<GreenLight>>(Vec(34.4f, 264.4f), module, FrankBussFormulaModule::B_MINUS_1_LIGHT));
		
		addParam(createParam<LEDButton>(Vec(75, 260), module, FrankBussFormulaModule::B_0_PARAM));
		addChild(createLight<MediumLight<GreenLight>>(Vec(79.4f, 264.4f), module, FrankBussFormulaModule::B_0_LIGHT));

		addParam(createParam<LEDButton>(Vec(120, 260), module, FrankBussFormulaModule::B_1_PARAM));
		addChild(createLight<MediumLight<GreenLight>>(Vec(124.4f, 264.4f), module, FrankBussFormulaModule::B_1_LIGHT));

		addParam(createParam<RoundLargeBlackKnob>(Vec(170, 240), module, FrankBussFormulaModule::KNOB_PARAM));

		addChild(createLight<MediumLight<GreenRedLight>>(Vec(240, 240), module, FrankBussFormulaModule::ERROR_LIGHT));

		addInput(createInput<PJ301MPort>(Vec(20, 310), module, FrankBussFormulaModule::W_INPUT));
		addInput(createInput<PJ301MPort>(Vec(60, 310), module, FrankBussFormulaModule::X_INPUT));
		addInput(createInput<PJ301MPort>(Vec(100, 310), module, FrankBussFormulaModule::Y_INPUT));
		addInput(createInput<PJ301MPort>(Vec(140, 310), module, FrankBussFormulaModule::Z_INPUT));

		addParam(createParam<LEDButton>(Vec(190, 314), module, FrankBussFormulaModule::CLAMP_PARAM));
		addChild(createLight<MediumLight<GreenLight>>(Vec(194.4f, 318.4f), module, FrankBussFormulaModule::CLAMP_LIGHT));

		addOutput(createOutput<PJ301MPort>(Vec(220, 310), module, FrankBussFormulaModule::FORMULA_OUTPUT));
	}
};

Model *modelFrankBussFormula = createModel<FrankBussFormulaModule, FrankBussFormulaWidget>("Formula");
