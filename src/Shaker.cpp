#include "FrankBuss.hpp"

struct FrankBussShakerModule : Module {
	enum ParamIds {
		ON_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		TENSION_INPUT,
		OPACITY_INPUT,
		ZOOM_INPUT,
		X_POS_INPUT,
		Y_POS_INPUT,
		NUM_INPUTS
	};

	FrankBussShakerModule() {
		config(NUM_PARAMS, NUM_INPUTS, 0, 0);
		configSwitch(ON_PARAM, 0, 1, 1, "Shaker", {"Off", "On"});
		configInput(TENSION_INPUT, "Tension");
		configInput(OPACITY_INPUT, "Opacity");
		configInput(ZOOM_INPUT, "Zoom");
		configInput(X_POS_INPUT, "X-pos");
		configInput(Y_POS_INPUT, "Y-pos");
	}
};

struct FrankBussShakerWidget : ModuleWidget {
	FrankBussShakerWidget(FrankBussShakerModule *module) {
		setModule(module);

		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/shaker.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParam<CKSS>(Vec(15, 60), module, FrankBussShakerModule::ON_PARAM));
		int y0 = 130;
		int ofs = 33;
		addInput(createInput<PJ301MPort>(Vec(15, y0), module, FrankBussShakerModule::TENSION_INPUT));
		y0 += ofs;
		addInput(createInput<PJ301MPort>(Vec(15, y0), module, FrankBussShakerModule::OPACITY_INPUT));
		y0 += ofs;
		addInput(createInput<PJ301MPort>(Vec(15, y0), module, FrankBussShakerModule::ZOOM_INPUT));
		y0 += ofs;
		addInput(createInput<PJ301MPort>(Vec(15, y0), module, FrankBussShakerModule::X_POS_INPUT));
		y0 += ofs;
		addInput(createInput<PJ301MPort>(Vec(15, y0), module, FrankBussShakerModule::Y_POS_INPUT));
		
		// always turn it off on load
		if (module) module->params[FrankBussShakerModule::ON_PARAM].setValue(0);
	}

	math::Vec lastOffset;
	math::Vec exitOffset;
	math::Vec offsetOrg;
	float lastTension = 0;
	float lastOpacity = 0;
	float lastZoom = 0;
	float lastXPos = 0;
	float lastYPos = 0;
	bool lastOn = false;
	int initialized = 0;
	float exitZoom = 0;
	
	void step() override {
		ModuleWidget::step();
		if (!module) return;

		// change original position, if user moved it manually
		if (exitOffset.x != APP->scene->rackScroll->offset.x) {
			offsetOrg.x += APP->scene->rackScroll->offset.x - exitOffset.x;
		}
		if (exitOffset.y != APP->scene->rackScroll->offset.y) {
			offsetOrg.y += APP->scene->rackScroll->offset.y - exitOffset.y;
		}

		// reset initialized position when engine is paused, off, or zoom changed (when a cable is connected to it), and stop it
		bool on = module->params[FrankBussShakerModule::ON_PARAM].getValue() > 0.f;
		bool zoomChanged = exitZoom != APP->scene->rackScroll->getZoom();
		bool zoomCable = module->inputs[FrankBussShakerModule::ZOOM_INPUT].active;
		if (/*APP->engine->isPaused() ||*/ !on || (zoomChanged && zoomCable)) {
			offsetOrg = APP->scene->rackScroll->offset;
			exitOffset = offsetOrg;
			exitZoom = APP->scene->rackScroll->getZoom();
			module->params[FrankBussShakerModule::ON_PARAM].setValue(0);
			return;
		}
		
		// reset initialized position when turned on
		if (on && !lastOn) {
			offsetOrg = APP->scene->rackScroll->offset;
			exitOffset = offsetOrg;
		}
		lastOn = on;
		
		// test tension changes
		float tension = module->inputs[FrankBussShakerModule::TENSION_INPUT].getVoltage(0);
		if (module->inputs[FrankBussShakerModule::TENSION_INPUT].active) {
			if (tension != lastTension) {
				settings::cableTension = math::clamp(tension / 10.0f, 0.0f, 1.0f);
				lastTension = tension;
			}
		}
		
		// test opacity changes
		float opacity = module->inputs[FrankBussShakerModule::OPACITY_INPUT].getVoltage(0);
		if (module->inputs[FrankBussShakerModule::OPACITY_INPUT].active) {
			if (opacity != lastOpacity) {
				settings::cableOpacity = math::clamp(opacity / 10.0f, 0.0f, 1.0f);
				lastOpacity = opacity;
			}
		}
		
		// test zoom changes
		float zoom = module->inputs[FrankBussShakerModule::ZOOM_INPUT].getVoltage(0);
		if (module->inputs[FrankBussShakerModule::ZOOM_INPUT].active) {
			if (zoom != lastZoom) {
				APP->scene->rackScroll->setZoom(math::clamp(zoom / 5.0f, -2.0f, 2.0f));
				lastZoom = zoom;
			}
		}
		
		// test position changes
		float x = module->inputs[FrankBussShakerModule::X_POS_INPUT].getVoltage(0);
		if (!module->inputs[FrankBussShakerModule::X_POS_INPUT].active) x = lastXPos;
		float y = module->inputs[FrankBussShakerModule::Y_POS_INPUT].getVoltage(0);
		if (!module->inputs[FrankBussShakerModule::Y_POS_INPUT].active) y = lastYPos;
		if (x != lastXPos || y != lastYPos) {
			// init it once after some time
			/*
			if (initialized < 10) {
				offsetOrg = APP->scene->rackScroll->offset;
				lastOffset = offsetOrg;
				initialized++;
				return;
			}*/

			// calculate and set new position
			math::Vec newOffset = offsetOrg;
			newOffset.x += math::clamp(x * 10.0f, -100.0f, 100.0f);
			newOffset.y += math::clamp(y * 10.0f, -100.0f, 100.0f);
			if (newOffset.x != lastOffset.x || newOffset.y != lastOffset.y) {
				APP->scene->rackScroll->offset = newOffset;
				lastOffset = newOffset;
			}
			
			lastXPos = x;
			lastYPos = y;
		}
		
		exitZoom = APP->scene->rackScroll->getZoom();
		exitOffset = APP->scene->rackScroll->offset;
	}
	
};

Model *modelFrankBussShaker = createModel<FrankBussShakerModule, FrankBussShakerWidget>("Shaker");
