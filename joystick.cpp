/**
 * Author: Jason White
 *
 * Description:
 * Reads joystick/gamepad events and displays them.
 *
 * Compile:
 * gcc joystick.cpp -o -std=c++0x joystick
 * https://gist.github.com/jasonwhite/c5b2048c15993d285130
 *
 * Run:
 * ./joystick [/dev/input/jsX]
 *
 * See also:
 * https://github.com/drewnoakes/joystick/blob/master/joystick.cc
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>
#include "unistd.h"
#include "joystick.h"
using namespace std;

Joystick::Joystick() :
        fd(-1)
{
}

Joystick::~Joystick()
{
    Disonnect();
}

bool Joystick::Connect()
{
    if (-1 == fd)
    {
        fd = open("/dev/input/js0", O_NONBLOCK); //O_RDONLY
    }
    return (-1 != fd);
}

void Joystick::Disonnect()
{
    if (-1 != fd)
    {
        close(fd);
        fd = -1;
    }
}

uint8_t Joystick::getButtonCount() const
{
    uint8_t axes = 0;
    if (isConnected())
    {
        if (ioctl(fd, JSIOCGBUTTONS, &axes) == -1)
            return 0;
    }
    return axes;
}

uint8_t Joystick::getAxisCount() const
{
    uint8_t axes = 0;
    if (isConnected())
    {
    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;
    }
    return axes;
}

std::string Joystick::getName() const
{
    std::string name = "not connected";
    if (isConnected())
    {
        char tt[128];
        if (ioctl(fd, JSIOCGNAME(sizeof(tt)), tt) != -1)
        {
            name = tt;
        }
    }
    return name;
}

bool Joystick::getEvent(struct js_event &event)
{
    if (isConnected())
    {
        ssize_t bytes;
        bytes = read(fd, &event, sizeof(event));

        if (bytes == sizeof(event))
        {
            return true;
        }

        if (ENODEV == errno)
        { //disconnected
            event.type = JS_EVENT_INIT;
            event.number = 0;
            event.value = 0; //disconnect
            Disonnect();
            return true;
        }

    } else if (Connect())
    {
        event.type = JS_EVENT_INIT;
        event.number = 0;
        event.value = 1; //connect
        return true;
    }
    return false;
}


ostream& operator <<(ostream& os, const Joystick& j)
{
    os << "Joystick{Name=" << j.getName();
    os << ";Axis=" << static_cast<int>(j.getAxisCount());
    os << ";Buttons=" << static_cast<int>(j.getButtonCount());
    os << "}";
    return os;
}

ostream& operator <<(ostream& os, const struct js_event &event)
{
    os << "js_event{";
    if (event.type & JS_EVENT_INIT)
    {
        os << "init;";
    }
    os << "type=" << static_cast<int>(event.type);
    switch (event.type & (~JS_EVENT_INIT)) {
        case 0:
            os << "(connect)";
            break;
        case JS_EVENT_BUTTON:
            os << "(button)";
            break;
        case JS_EVENT_AXIS:
            os << "(axis)";
            break;
    }
    os << "number=" << static_cast<int>(event.number) << ";";
    os << "val=" << static_cast<int>(event.value) << "}";
    return os;
}

void Joystick_loop_test()
{
    Joystick joystick;
    while (1)
    {
        struct js_event event;
        if (joystick.getEvent(event))
        {
            std::cout << event << std::endl;
            if ((JS_EVENT_INIT == event.type) && (1 == event.value))
            {
                std::cout << joystick << std::endl;
            }
        }
    }
}



int main(int argc, char** argv)
{
    Joystick_loop_test();
    return 0;
}
