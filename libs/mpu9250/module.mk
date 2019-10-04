SOURCES += ./libs/mpu9250/SparkFunMPU9250-DMP.cpp
SOURCES += ./libs/mpu9250/util/inv_mpu.cpp  
SOURCES += ./libs/mpu9250/util/inv_mpu_dmp_motion_driver.cpp
SOURCES += ./libs/mpu9250/util/rasp_mpu9250_log.cpp
SOURCES += ./libs/mpu9250/util/rasp_mpu9250_clk.cpp
SOURCES += ./libs/mpu9250/util/rasp_mpu9250_i2c.cpp

CFLAGS += -I./libs/mpu9250
CFLAGS += -I./libs/mpu9250/util
