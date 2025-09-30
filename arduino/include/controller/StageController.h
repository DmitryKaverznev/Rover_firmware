#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>
#include "Result.h"

class StageController {
public:
    explicit StageController(HoverRepo& hoverRepo, MPURepo& mpuRepo) :
            hoverRepo(hoverRepo),
            mpuRepo(mpuRepo)
            {}

    void run() {
        Log.infoln(F("Starting main program..."));



        while (1) {
            float yaw = (mpuRepo.getData())[0] * 180.0f / M_PI;
            Serial.println(yaw);
        }

        delay(10);

    }

    Result init() {
        Log.infoln(F("Starting init..."));

        hoverRepo.init();

        Result allInit = Result::Ok;

        allInit = resultAnd(allInit, mpuRepo.init());

        if (allInit == Result::Error) {
            Log.fatalln("Mail init is fatal!");
        } else {
            Log.infoln("Main init is success");
        }

        return allInit;
    }

private:
    HoverRepo& hoverRepo;
    MPURepo& mpuRepo;
};