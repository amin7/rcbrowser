// I2Cdev library collection - Main I2C device class
// Abstracts bit and byte I2C R/W functions into a convenient class
// RaspberryPi bcm2835 library port: bcm2835 library available at http://www.airspayce.com/mikem/bcm2835/index.html
// Based on Arduino's I2Cdev by Jeff Rowberg <jeff@rowberg.net>
//

#include "I2Cdev.h"
#include <stdio.h>

#ifndef _SIMULATION_
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/i2c-dev.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define I2C_SMBUS 0x0720  /* SMBus-level access */
#define I2C_SMBUS_READ  1
#define I2C_SMBUS_WRITE 0

int I2Cdev::initialize(uint8_t i2cAddress) {
  fd_ = wiringPiI2CSetup(i2cAddress);
  return fd_;
}
/** Read a single bit from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @return Status of read operation (true = success)
 */
bool I2Cdev::readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
  const uint8_t res = wiringPiI2CReadReg8(fd_, regAddr);
  *data = res & (1 << bitNum);
  return true;
}

/** Read multiple bits from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @return Status of read operation (true = success)
 */
bool I2Cdev::readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
  uint8_t res = wiringPiI2CReadReg8(fd_, regAddr);
  uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  res &= mask;
  res >>= (bitStart - length + 1);
  *data = res;
  return true;
}

/** Read single byte from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for byte value read from device
 * @return Status of read operation (true = success)
 */
bool I2Cdev::readByte(uint8_t regAddr, uint8_t *data) {
  *data = wiringPiI2CReadReg8(fd_, regAddr);
  return true;
}
uint8_t I2Cdev::readByte(uint8_t regAddr) {
  return wiringPiI2CReadReg8(fd_, regAddr);
}

/** Read multiple bytes from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @return I2C_TransferReturn_TypeDef http://downloads.energymicro.com/documentation/doxygen/group__I2C.html
 */
bool I2Cdev::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
  const auto res = i2c_smbus_read_i2c_block_data(fd_, regAddr, length, data);
  return (res < 0) ? false : true;
}

bool I2Cdev::writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
  const auto res = i2c_smbus_write_i2c_block_data(fd_, regAddr, length, data);
  return (res < 0) ? false : true;
}
/** write a single bit in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBit(uint8_t regAddr, uint8_t bitNum, bool data) {
  //first reading registery value
  uint8_t res = wiringPiI2CReadReg8(fd_, regAddr);
  res = (data) ? (res | (1 << bitNum)) : (res & ~(1 << bitNum));
  wiringPiI2CWriteReg8(fd_, regAddr, res);
  return true;
}

/** Write multiple bits in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
  uint8_t b = wiringPiI2CReadReg8(fd_, regAddr);
  uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  data <<= (bitStart - length + 1); // shift data into correct position
  data &= mask; // zero all non-important bits in data
  b &= ~(mask); // zero all important bits in existing byte
  b |= data; // combine data with existing byte
  wiringPiI2CWriteReg8(fd_, regAddr, b);
  return true;
}

/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeByte(uint8_t regAddr, uint8_t data) {
  wiringPiI2CWriteReg8(fd_, regAddr, data);
  return true;
}

/** Read single word from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for word value read from device
 * @return Status of read operation (true = success)
 */
uint16_t I2Cdev::readWord(uint8_t regAddr) {
  auto val = wiringPiI2CReadReg16(fd_, regAddr);
  return (val << 8) | (val >> 8);
}

void I2Cdev::writeWord(uint8_t regAddr, uint16_t data) {
  wiringPiI2CWriteReg16(fd_, regAddr, (data << 8) | (data >> 8));
}

#else
int I2Cdev::initialize(uint8_t i2cAddress) {
  return -1;
}
bool I2Cdev::readBit( uint8_t regAddr, uint8_t bitNum, uint8_t *data) {return false;}
bool I2Cdev::readBits( uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {return false;}
bool I2Cdev::readByte(uint8_t regAddr, uint8_t *data) {
  return false;
}
uint8_t I2Cdev::readByte(uint8_t regAddr) {
  return 0;
}
uint16_t I2Cdev::readWord(uint8_t regAddr) {
  return 0;
}
bool I2Cdev::readBytes( uint8_t regAddr, uint8_t length, uint8_t *data) {return false;}

bool I2Cdev::writeBit( uint8_t regAddr, uint8_t bitNum, bool data) {return false;}
bool I2Cdev::writeBits( uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {return false;}

bool I2Cdev::writeByte( uint8_t regAddr, uint8_t data) {return false;}
void I2Cdev::writeWord( uint8_t regAddr, uint16_t data) {}
bool I2Cdev::writeBytes( uint8_t regAddr, uint8_t length, uint8_t *data) {return false;}
#endif
