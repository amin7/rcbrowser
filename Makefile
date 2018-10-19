PROG = rcbrowser

SIMULATION = 1

ifeq ($(OS),Windows_NT)
CXX = arm-linux-gnueabihf-g++.exe
endif

MODULE_CFLAGS=-DMG_DISABLE_DAV_AUTH -DMG_ENABLE_FAKE_DAVLOCK

MONGOOSE_DIR = ../mongoose
SOURCES = rcbrowser.cpp
SOURCES += $(MONGOOSE_DIR)/mongoose.c
SOURCES += CDCmotor.cpp
SOURCES += pca9685Servo.cpp
SOURCES += hc_sr04.cpp
ifndef SIMULATION
SOURCES += ./libs/pca9685.c
endif

CFLAGS = -g -W -Wall -I$(MONGOOSE_DIR) -Wno-unused-function $(CFLAGS_EXTRA) $(MODULE_CFLAGS)  -std=c++11
CFLAGS += -pthread
CFLAGS += -I../rapidjson/include/
CFLAGS += -I../pca9685/src
CFLAGS += -I./libs
CFLAGS += -ggdb

ifdef SIMULATION
CFLAGS +=  -D _SIMULATION_
else
CFLAGS += -lwiringPi -lwiringPiDev -lpthread -lm
endif
ifeq ($(OS),Windows_NT)
CFLAGS += -static
endif
 
OBJ_DIR = ./obj/
MKDIR_P = mkdir -p


all: $(PROG)

$(OBJ_DIR):
	${MKDIR_P} ${OBJ_DIR}

$(PROG): $(OBJ_DIR) $(SOURCES)
	$(CXX) $(SOURCES) -o $(OBJ_DIR)$@ $(CFLAGS)

$(PROG).exe: $(OBJ_DIR) $(SOURCES)
	cl $(SOURCES) /I../.. /MD /Fe$(OBJ_DIR)$@

clean:
	rm -rf $(OBJ_DIR)


