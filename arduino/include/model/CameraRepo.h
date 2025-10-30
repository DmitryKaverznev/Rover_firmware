#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <Arduino_FreeRTOS.h>
#include "Config.h"
#include "utilis/Dot.h"
#pragma once

[[noreturn]] inline void vTaskCamera(void *pvParameters);
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

        JsonDocument doc;
        String jsonStr = _serial.readStringUntil('\n');
        const DeserializationError error = deserializeJson(doc, jsonStr);

        if (error) {
            Log.errorln("CameraRepo -> JSON deserialize error");
            return;
        }

        _commandNow.data = doc["data"];
    }

    HardwareSerial& _serial;
};

CameraRepo instanceOfCameraRepo(pins::uart::camera);
inline CameraRepo* getImplementationOfCameraRepo() { return &instanceOfCameraRepo; }


[[noreturn]] inline void vTaskCamera(void *pvParameters) {
    auto* repo = static_cast<CameraRepo*>(pvParameters);

    for (;;) {
        taskENTER_CRITICAL();
        repo->update();
        taskEXIT_CRITICAL();

        delay(1);
    }
}