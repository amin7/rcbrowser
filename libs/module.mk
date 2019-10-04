SOURCES += ./libs/mongoose.c
ifndef SIMULATION
SOURCES += ./libs/pca9685.c
endif

CFLAGS += -I./libs

include ./libs/mpu9250/module.mk
