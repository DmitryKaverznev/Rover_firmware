#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>

#include "utilis/Dot.h"

class CameraRepo {
public:
    explicit CameraRepo(HardwareSerial& serial)
        : _serial(serial) {
        _serial.begin(115200);
    }

    struct Command {
        DotInt start;
        DotInt end;
    };

    void update() {
        _serialToCommand();
    }

    Command& getData() {
        return _commandNow;
    }

    void printData() const {
        Log.infoln("Camera command:");
        Log.infoln("  start: (%d, %d)", _commandNow.start.x, _commandNow.start.y);
        Log.infoln("  end:   (%d, %d)", _commandNow.end.x, _commandNow.end.y);
    }

private:
    Command _commandNow{};
    void _serialToCommand() {
        while (_serial.available() == 0) {
            // Log.traceln("Camera repo -> wait...")
        }

        JsonDocument doc;
        String jsonStr = _serial.readStringUntil('\n');
        const DeserializationError error = deserializeJson(doc, jsonStr);

        if (error) {
            Log.errorln("CameraRepo -> JSON deserialize error");
            return;
        }

        _commandNow.start.x = doc["x1"];
        _commandNow.start.y = doc["y1"];
        _commandNow.end.x = doc["x2"];
        _commandNow.end.y = doc["y2"];
    }

    HardwareSerial& _serial;
};
