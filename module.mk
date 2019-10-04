SOURCES += rcbrowser.cpp
SOURCES += CDCmotor.cpp
SOURCES += pca9685Servo.cpp
SOURCES += hc_sr04.cpp
SOURCES += CManipulator.cpp
SOURCES += CRadar.cpp
SOURCES += CHttpCmdHandler.cpp
SOURCES += DMPmisc.cpp
SOURCES += CPower.cpp
SOURCES += joystick.cpp

CFLAGS += -I../rapidjson/include/

include ./libs/module.mk
