#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

#include "usecase/HoverSpeedUseCase.h"

class StageController {
public:
    HoverSpeedUseCase& hoverSpeedUseCase;

    explicit StageController(HoverSpeedUseCase& hoverSpeedUseCase) :
            hoverSpeedUseCase(hoverSpeedUseCase) {
    }

    void run() {
        Serial.begin(115200);

        Log.begin(LOG_LEVEL_INFO, &Serial, true);
        // Log.begin(LOG_LEVEL_ERROR, &Serial, true);

        Log.infoln("Starting StageController...");

        hoverSpeedUseCase.invoke(0);
        delay(2000);
        hoverSpeedUseCase.invoke(50);
        delay(3000);
        hoverSpeedUseCase.invoke(0);
    }
};