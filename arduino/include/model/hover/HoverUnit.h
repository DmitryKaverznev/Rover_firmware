#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

#include "SerialCommand.h"

class HoverUnit {
public:
    explicit HoverUnit(HardwareSerial& hoverSerial) :
            _route{0, 0},
            _serial(hoverSerial) {
        _serial.begin(SERIAL_RATE);
    }

    void set(int16_t speed, int16_t steer) {
        _route.speed = speed;
        _route.steer = steer;

        Log.traceln("SET %d | %d - command", _route.steer, _route.speed);

    }

    void update() {
        _send();
    }

private:
    HoverRoute _route;

    static const uint16_t START_FRAME = 0xABCD;
    static const uint32_t SERIAL_RATE = 115200;

    HardwareSerial& _serial;

    void _send() {
        SerialCommand command;

        command.start    = (uint16_t)START_FRAME;
        command.steer    = (int16_t)_route.steer;
        command.speed    = (int16_t)_route.speed;
        command.checksum = (uint16_t)(command.start ^ command.steer ^ command.speed);

        _serial.write((uint8_t *) &command, sizeof(command));
    }
};