#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>


class StageController {
public:
    HoverRepo& hoverRepo;

    explicit StageController(HoverRepo& hoverRepo) :
            hoverRepo(hoverRepo) {
    }

    void run() {
        Serial.begin(115200);

        Log.begin(LOG_LEVEL_INFO, &Serial, true);
        // Log.begin(LOG_LEVEL_ERROR, &Serial, true);

        Log.infoln("Starting StageController...");

        hoverRepo.init();

        hoverRepo.set(0);
        delay(2000);

        hoverRepo.set(0);
    }
};