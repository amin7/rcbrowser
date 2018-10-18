PROG = rcbrowser
#CXX = arm-linux-gnueabihf-g++.exe
MODULE_CFLAGS=-DMG_DISABLE_DAV_AUTH -DMG_ENABLE_FAKE_DAVLOCK
MONGOOSE_DIR = ../mongoose
SOURCES = rcbrowser.cpp
SOURCES += $(MONGOOSE_DIR)/mongoose.c
SOURCES += CDCmotor.cpp
SOURCES += pca9685Servo.cpp
SOURCES += hc_sr04.cpp
SOURCES += ./libs/pca9685.c

CFLAGS = -g -W -Wall -I$(MONGOOSE_DIR) -Wno-unused-function $(CFLAGS_EXTRA) $(MODULE_CFLAGS)  -std=c++11
CFLAGS += -pthread
CFLAGS += -I../rapidjson/include/
CFLAGS += -I../pca9685/src
CFLAGS += -I./libs

#CFLAGS +=  -D _SIMULATION_
CFLAGS +=-lwiringPi -lwiringPiDev -lpthread -lm -lwiringPiPca9685
#CFLAGS += --static
 
OBJ_DIR = ./obj/
MKDIR_P = mkdir -p

#@echo '$(OS)'

all: $(PROG)

$(OBJ_DIR):
	${MKDIR_P} ${OBJ_DIR}

$(PROG): $(OBJ_DIR) $(SOURCES)
	$(CXX) $(SOURCES) -o $(OBJ_DIR)$@ $(CFLAGS)

$(PROG).exe: $(OBJ_DIR) $(SOURCES)
	cl $(SOURCES) /I../.. /MD /Fe$(OBJ_DIR)$@

clean:
	rm -rf $(OBJ_DIR)


