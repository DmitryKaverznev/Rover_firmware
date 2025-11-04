#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include <Arduino_FreeRTOS.h>
#include "Config.h"

#pragma once

class CameraRepo {
public:
    explicit CameraRepo(HardwareSerial& serial)
        : _serial(serial) {
        _serial.begin(115200);
    }

    struct Command {
        int32_t  data;
    };

    void update() {
        _serialToCommand();
    }

    void clear() {
        _commandNow = {};
        while (_serial.available()) {
            _serial.read();
        }
    }

    Command& getData() {
        return _commandNow;
    }

private:
    Command _commandNow{};
    void _serialToCommand() {
        if (_serial.available() == 0) {
            return;
        }

        if (_serial.read() == 0xC) {
            _commandNow.data = 2;
        }
    }

    HardwareSerial& _serial;
};

CameraRepo instanceOfCameraRepo(pins::uart::camera);
inline CameraRepo* getImplementationOfCameraRepo() { return &instanceOfCameraRepo; }