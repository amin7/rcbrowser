/*
 * joystick.h
 *
 *  Created on: Sep 24, 2019
 *      Author: ominenko
 *
 *      https://github.com/torvalds/linux/blob/master/include/uapi/linux/joystick.h
 *      https://www.kernel.org/doc/Documentation/input/joystick-api.txt
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_
#include <stdint.h>
#include <string>
#include <iostream>
#include <linux/joystick.h>

class Joystick
{
private:
    int fd;

    protected:
    uint8_t getAxisCount() const;
    uint8_t getButtonCount() const;
    std::string getName() const;
    bool Connect();
    void Disonnect();

    public:
    Joystick();
    ~Joystick();

    bool isConnected() const
    {
        return (-1 != fd);
    }
    bool getEvent(struct js_event &event);
    friend std::ostream& operator<<(std::ostream& os, const Joystick& e);
};




#endif /* JOYSTICK_H_ */
