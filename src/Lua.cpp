#include "FrankBuss.hpp"
#include "lua.hpp"

struct LuaModule;

extern "C" {
	static int l_addLabel(lua_State *L);
	static int l_addInput(lua_State *L);
	static int l_addOutput(lua_State *L);
	static int l_getVoltage(lua_State *L);
	static int l_setVoltage(lua_State *L);
}

struct LuaModule : Module {
	lua_State *L;
	bool run = true;
	
	LuaModule() {
		config(8, 8, 8, 8);
	}
	
	void process(const ProcessArgs &args) override {
		if (run) {
			lua_pushnumber(L, args.sampleRate);
			lua_setglobal(L, "sampleRate");
			lua_getglobal(L, "process");
			if (lua_pcall(L, 0, 0, 0)) {
				printf("error calling 'process': %s", lua_tostring(L, -1));
				lua_pop(L,1);
				run = false;
			}
		}
	}
	
	// functions called from Lua
	float onGetVoltage(int id) {
		return inputs[id].getVoltage();
	}
	void onSetVoltage(int id, float voltage) {
		outputs[id].setVoltage(voltage);
	}
};

struct LuaWidget : ModuleWidget {
	lua_State *L;
	int lastInput;
	int lastOutput;
	LuaModule *luaModule;

	LuaWidget(LuaModule *module) {
		setModule(module);
		luaModule = module;
		
		lastInput = 0;
		lastOutput = 0;

		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/lua.svg")));

		// init Lua environment
		L = luaL_newstate();
		if (module) module->L = L;
		luaL_openlibs(L);
		
		// set pointer to this widget for calling C functions from Lua
		lua_pushlightuserdata(L, this);
		lua_setglobal(L, "LuaWidget");

		// add C functions
		lua_pushcfunction(L, l_addLabel);
		lua_setglobal(L, "addLabel");
		lua_pushcfunction(L, l_addInput);
		lua_setglobal(L, "addInput");
		lua_pushcfunction(L, l_addOutput);
		lua_setglobal(L, "addOutput");
		lua_pushcfunction(L, l_getVoltage);
		lua_setglobal(L, "getVoltage");
		lua_pushcfunction(L, l_setVoltage);
		lua_setglobal(L, "setVoltage");
		
		std::string scriptFile = asset::plugin(pluginInstance, "script.lua");
		if (luaL_loadfile(L, scriptFile.c_str())) {
			printf("script load error: %s\n", lua_tostring(L, -1));
			lua_pop(L,1);
		} else {
			printf("script loaded: %s\n", scriptFile.c_str());
			if (lua_pcall(L,0, LUA_MULTRET, 0)) {
				printf("script execute error: %s\n", lua_tostring(L, -1));
				lua_pop(L,1);
			}
		}
		
	}

	// functions called from Lua
	void onAddLabel(float x, float y, const char* text, float fontSize) {
		Label* label = new Label();
		label->box.pos = Vec(x, y);
		label->text = text;
		label->fontSize = fontSize;
		label->color = color::BLACK;
		addChild(label);
	}
	int onAddInput(float x, float y) {
		addInput(createInput<PJ301MPort>(Vec(x, y), module, lastInput));
		return lastInput++;
	}
	int onAddOutput(float x, float y) {
		addOutput(createOutput<PJ301MPort>(Vec(x, y), module, lastOutput));
		return lastOutput++;
	}

};


Model *modelLua = createModel<LuaModule, LuaWidget>("Lua");

extern "C" {
	static int l_addLabel(lua_State *L) {
		lua_getglobal(L, "LuaWidget");
		LuaWidget* widget = (LuaWidget*) lua_touserdata(L, -1);
		lua_Number x = lua_tonumber(L, 1);
		lua_Number y = lua_tonumber(L, 2);
		const char* text = lua_tostring(L, 3);
		lua_Number fontSize = lua_tonumber(L, 4);
		widget->onAddLabel(x, y, text, fontSize);
		return 0;
	}

	static int l_addInput(lua_State *L) {
		lua_getglobal(L, "LuaWidget");
		LuaWidget* widget = (LuaWidget*) lua_touserdata(L, -1);
		lua_Number x = lua_tonumber(L, 1);
		lua_Number y = lua_tonumber(L, 2);
		int id = widget->onAddInput(x, y);
		lua_pushnumber(L, id);
		return 1;
	}

	static int l_addOutput(lua_State *L) {
		lua_getglobal(L, "LuaWidget");
		LuaWidget* widget = (LuaWidget*) lua_touserdata(L, -1);
		lua_Number x = lua_tonumber(L, 1);
		lua_Number y = lua_tonumber(L, 2);
		int id = widget->onAddOutput(x, y);
		lua_pushnumber(L, id);
		return 1;
	}
	
	static int l_getVoltage(lua_State *L) {
		lua_getglobal(L, "LuaWidget");
		LuaWidget* widget = (LuaWidget*) lua_touserdata(L, -1);
		lua_Number id = lua_tonumber(L, 1);
		float voltage = widget->luaModule->onGetVoltage(id);
		lua_pushnumber(L, voltage);
		return 1;
	}

	static int l_setVoltage(lua_State *L) {
		lua_getglobal(L, "LuaWidget");
		LuaWidget* widget = (LuaWidget*) lua_touserdata(L, -1);
		lua_Number id = lua_tonumber(L, 1);
		lua_Number voltage = lua_tonumber(L, 2);
		widget->luaModule->onSetVoltage(id, voltage);
		return 0;
	}
}
