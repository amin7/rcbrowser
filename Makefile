PROG = rcbrowser
#SIMULATION = 1
ifeq ($(OS),Windows_NT)
SIMULATION = 1
CXX = arm-linux-gnueabihf-g++.exe
endif
SOURCES= 
CFLAGS = 

include module.mk

MODULE_CFLAGS=-DMG_DISABLE_DAV_AUTH -DMG_ENABLE_FAKE_DAVLOCK

CFLAGS += -g -W -Wall -Wno-unused-function $(CFLAGS_EXTRA) $(MODULE_CFLAGS)  -std=c++11
CFLAGS += -Wno-missing-field-initializers
CFLAGS += -pthread
CFLAGS += -ggdb

ifdef SIMULATION
CFLAGS +=  -D _SIMULATION_ -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-variable
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
$(SOURCES): ${OBJ_DIR}
	$(CXX) $(CFLAGS) -c  $@ -o $(OBJ_DIR)$(basename $(notdir $@)).o

$(PROG): $(OBJ_DIR) $(SOURCES)
	$(CXX) $(wildcard $(OBJ_DIR)*.o) -o $(OBJ_DIR)$@ $(CFLAGS)

$(PROG).exe: $(OBJ_DIR) $(SOURCES)
	cl /I../.. /MD /Fe$(OBJ_DIR)$@

clean:
	rm -rf $(OBJ_DIR)


