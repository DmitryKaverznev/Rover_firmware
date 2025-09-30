#pragma once

#include <ArduinoLog.h>
#include <Arduino_FreeRTOS.h>

#include "HoverUnit.h"

[[noreturn]] void vTaskHover(void *pvParameters);
class HoverRepo {
public:
    HoverRepo(HardwareSerial& serialUp, HardwareSerial& serialDown) :
            _serialUp(serialUp),
            _serialDown(serialDown),
            hoverUp(_serialUp),
            hoverDown(_serialDown) {
    }

    void init() {
        Log.infoln("HoverRepo is being initialized...");

        xTaskCreate(
                vTaskHover,
                "Hover",
                configMINIMAL_STACK_SIZE,
                this,
                1,
                nullptr
                );

        Log.infoln("HoverRepo initialized");
    }

    void set(int16_t speed, int16_t steer = 0) {
        Log.traceln("Hover set | %d %d", speed, steer);
        hoverUp.set(-speed, steer);
        hoverDown.set(speed, steer);
    }

    void update() {
        hoverUp.update();
        hoverDown.update();
    }

private:
    HardwareSerial& _serialUp;
    HardwareSerial& _serialDown;

    HoverUnit hoverUp;
    HoverUnit hoverDown;
};

