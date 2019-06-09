SOURCES += $(wildcard src/*.cpp src/formula/*.cpp)
LDFLAGS += -L/usr/local/lib -lluajit-5.1
FLAGS += -I/usr/local/include/luajit-2.0
DISTRIBUTABLES += $(wildcard LICENSE*) res

RACK_DIR ?= ../..
include $(RACK_DIR)/plugin.mk
