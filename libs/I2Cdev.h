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

#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#ifndef __cplusplus
#error A C++ compiler is required!
#endif 
#include <stdint.h>
#include <math.h> // required for BMP180
#include <stdlib.h> // required for MPU6060
#include <string.h> // required for MPU6060


class I2Cdev {
  int fd_ = -1;
public:

  int initialize(uint8_t i2cAddress);

  bool readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data);
  bool readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);

  bool readByte(uint8_t regAddr, uint8_t *data);
  uint16_t readWord(uint8_t regAddr);
  void writeWord(uint8_t regAddr, uint16_t data);

  bool readBytes(uint8_t regAddr, uint8_t length, uint8_t *data);

  bool writeBit(uint8_t regAddr, uint8_t bitNum, bool data);
  bool writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
  bool writeByte(uint8_t regAddr, uint8_t data);

  bool writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data);
};

#endif /* _I2CDEV_H_ */
