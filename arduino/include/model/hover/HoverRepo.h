#pragma once

#include <ArduinoLog.h>
#include <Arduino_FreeRTOS.h>

#include "GlobalConfig.h"
#include "Config.h"
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
                256,
                this,
                1,
                nullptr
                );

        Log.infoln("HoverRepo initialized");
    }

    void set(int16_t speed, int16_t steer = 0) {
        if (speed > 60) {
            Log.errorln("SPEED BIG - %d > %d", maxSpeed, speed);
            speed = 60;
        }

        taskENTER_CRITICAL();
        hoverUp.set(-speed, steer);
        hoverDown.set(speed, steer);
        taskEXIT_CRITICAL();
    }

    void update() const
    {
        hoverUp.update();
        hoverDown.update();
    }

private:
    HardwareSerial& _serialUp;
    HardwareSerial& _serialDown;

    HoverUnit hoverUp;
    HoverUnit hoverDown;

    const int maxSpeed = MAX_SPEED;
};

HoverRepo instanceOfHoverRepo(pins::uart::hoverUp, pins::uart::hoverDown);
inline HoverRepo* getImplementationOfHoverRepo() { return &instanceOfHoverRepo; }


[[noreturn]] inline void vTaskHover(void *pvParameters) {
    auto* repo = static_cast<HoverRepo*>(pvParameters);

    for(;;)
    {
        taskENTER_CRITICAL();
        repo->update();
        taskEXIT_CRITICAL();
        delay(100);
    }
}