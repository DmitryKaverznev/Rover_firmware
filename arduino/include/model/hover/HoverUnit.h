#pragma once

#include <Arduino.h>

#include "SerialCommand.h"

class HoverUnit {
public:
    explicit HoverUnit(HardwareSerial& hoverSerial) :
            _route{0, 0},
            _serial(hoverSerial) {
        _serial.begin(SERIAL_RATE);
    }

    void set(const int16_t speed, const int16_t steer) {
        _route.speed = speed;
        _route.steer = steer;
    }

    void update() const
    {
        _send();
    }

private:
    HoverRoute _route;

    static constexpr uint16_t START_FRAME = 0xABCD;
    static constexpr uint32_t SERIAL_RATE = 115200;

    HardwareSerial& _serial;

    void _send() const
    {
        SerialCommand command;

        command.start    = static_cast<uint16_t>(START_FRAME);
        command.steer    = static_cast<int16_t>(_route.steer);
        command.speed    = static_cast<int16_t>(_route.speed);
        command.checksum = static_cast<uint16_t>(command.start ^ command.steer ^ command.speed);

        _serial.write(reinterpret_cast<uint8_t*>(&command), sizeof(command));
    }
};