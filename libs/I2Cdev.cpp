// I2Cdev library collection - Main I2C device class
// Abstracts bit and byte I2C R/W functions into a convenient class
// RaspberryPi bcm2835 library port: bcm2835 library available at http://www.airspayce.com/mikem/bcm2835/index.html
// Based on Arduino's I2Cdev by Jeff Rowberg <jeff@rowberg.net>
//

/* ============================================
I2Cdev device library code is placed under the MIT license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "I2Cdev.h"
#include <stdio.h>

#ifndef _SIMULATION_
#include <wiringPi.h>
#include <wiringPiI2C.h>
int I2Cdev::initialize(uint8_t i2cAddress) {
  return wiringPiI2CSetup(i2cAddress);
}
/** Read a single bit from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @return Status of read operation (true = success)
 */
bool I2Cdev::readBit(int fd, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
  const uint8_t res = wiringPiI2CReadReg8(fd, regAddr);
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
bool I2Cdev::readBits(int fd, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
  uint8_t res = wiringPiI2CReadReg8(fd, regAddr);
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
bool I2Cdev::readByte(int fd, uint8_t regAddr, uint8_t *data) {
  const uint8_t res = wiringPiI2CReadReg8(fd, regAddr);
  data[0] = res;
  return true;
}

/** Read multiple bytes from an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr First register regAddr to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @return I2C_TransferReturn_TypeDef http://downloads.energymicro.com/documentation/doxygen/group__I2C.html
 */
bool I2Cdev::readBytes(int fd, uint8_t regAddr, uint8_t length, uint8_t *data) {
  while (length--) {
    const uint8_t res = wiringPiI2CReadReg8(fd, regAddr);
    *data = res;
    data++;
    regAddr++;
  }
  return true;
}

/** write a single bit in an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeBit(int fd, uint8_t regAddr, uint8_t bitNum, bool data) {
  //first reading registery value
  uint8_t res = wiringPiI2CReadReg8(fd, regAddr);
  res = (data) ? (res | (1 << bitNum)) : (res & ~(1 << bitNum));
  wiringPiI2CWriteReg8(fd, regAddr, res);
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
bool I2Cdev::writeBits(int fd, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
  uint8_t b = wiringPiI2CReadReg8(fd, regAddr);
  uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
  data <<= (bitStart - length + 1); // shift data into correct position
  data &= mask; // zero all non-important bits in data
  b &= ~(mask); // zero all important bits in existing byte
  b |= data; // combine data with existing byte
  wiringPiI2CWriteReg8(fd, regAddr, b);
  return true;
}

/** Write single byte to an 8-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 */
bool I2Cdev::writeByte(int fd, uint8_t regAddr, uint8_t data) {
  wiringPiI2CWriteReg8(fd, regAddr, data);
  return true;
}

/** Read single word from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for word value read from device
 * @return Status of read operation (true = success)
 */
bool I2Cdev::readWord(int fd, uint8_t regAddr, uint16_t *data) {
  *data = wiringPiI2CReadReg16(fd, regAddr);
  return true;
}

bool I2Cdev::writeWord(int fd, uint8_t regAddr, uint16_t data) {
  wiringPiI2CWriteReg16(fd, regAddr, data);
  return true;
}

bool I2Cdev::writeBytes(int fd, uint8_t regAddr, uint8_t length, uint8_t *data) {
  while (length--) {
    wiringPiI2CWriteReg8(fd, regAddr, *data);
    regAddr++;
    data++;
  }
  return true;
}
#else
int I2Cdev::initialize(uint8_t i2cAddress) {
  return -1;
}
bool I2Cdev::readBit(int fd, uint8_t regAddr, uint8_t bitNum, uint8_t *data) {return false;}
bool I2Cdev::readBits(int fd, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {return false;}

bool I2Cdev::readByte(int fd, uint8_t regAddr, uint8_t *data) {return false;}
bool I2Cdev::readWord(int fd, uint8_t regAddr, uint16_t *data) {return false;}
bool I2Cdev::readBytes(int fd, uint8_t regAddr, uint8_t length, uint8_t *data) {return false;}

bool I2Cdev::writeBit(int fd, uint8_t regAddr, uint8_t bitNum, bool data) {return false;}
bool I2Cdev::writeBits(int fd, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {return false;}

bool I2Cdev::writeByte(int fd, uint8_t regAddr, uint8_t data) {return false;}
bool I2Cdev::writeWord(int fd, uint8_t regAddr, uint16_t data) {return false;}
bool I2Cdev::writeBytes(int fd, uint8_t regAddr, uint8_t length, uint8_t *data) {return false;}
#endif
