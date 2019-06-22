#include <app/Scene.hpp>
#include <app/RackScrollWidget.hpp>
#include <settings.hpp>
#include "FrankBuss.hpp"

struct FrankBussShakerModule;

struct FrankBussShakerModule : Module {
	enum InputIds {
		TENSION_INPUT,
		OPACITY_INPUT,
		ZOOM_INPUT,
		X_POS_INPUT,
		Y_POS_INPUT,
		NUM_INPUTS
	};

	FrankBussShakerModule() {
		config(0, NUM_INPUTS, 0, 0);
	}

	/*
	void process(const ProcessArgs &args) override {
	}
	*/
};

struct FrankBussShakerWidget : ModuleWidget {
	FrankBussShakerWidget(FrankBussShakerModule *module) {
		setModule(module);

		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/shaker.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInput<PJ301MPort>(Vec(20, 140), module, FrankBussShakerModule::TENSION_INPUT));
		addInput(createInput<PJ301MPort>(Vec(20, 180), module, FrankBussShakerModule::OPACITY_INPUT));
		addInput(createInput<PJ301MPort>(Vec(20, 220), module, FrankBussShakerModule::ZOOM_INPUT));
		addInput(createInput<PJ301MPort>(Vec(20, 260), module, FrankBussShakerModule::X_POS_INPUT));
		addInput(createInput<PJ301MPort>(Vec(20, 300), module, FrankBussShakerModule::Y_POS_INPUT));
	}

	math::Vec lastOffset;
	math::Vec offsetOrg;
	float lastTension = 0;
	float lastOpacity = 0;
	float lastZoom = 0;
	float lastXPos = 0;
	float lastYPos = 0;
	int initialized = 0;
	
	void step() override {
		ModuleWidget::step();
		if (!module) return;
		if (APP->engine->isPaused()) {
			// reset initialized position when paused
			offsetOrg = APP->scene->rackScroll->offset;
			return;
		}

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
				settings::zoom = math::clamp(zoom / 5.0f, -2.0f, 2.0f);
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
			if (initialized < 10) {
				offsetOrg = APP->scene->rackScroll->offset;
				lastOffset = offsetOrg;
				initialized++;
				return;
			}

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
	}
	
};

Model *modelFrankBussShaker = createModel<FrankBussShakerModule, FrankBussShakerWidget>("Shaker");
