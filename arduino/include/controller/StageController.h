#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

#include "utilis/Result.h"
#include "utilis/SoftMode.h"
#include "utilis/RepeatRun.h"

class StageController {
public:
    explicit StageController(HoverRepo& hoverRepo,
                             MPURepo& mpuRepo,
                             RotateUseCase& rotateUseCase) :
            hoverRepo(hoverRepo),
            mpuRepo(mpuRepo),
            rotateUseCase(rotateUseCase) {
    }

    void run() {
        Log.infoln(F("Starting main program..."));

        rotateUseCase(90, 20, 60);
    }

    Result init() {
        Log.infoln(F("Starting init..."));

        hoverRepo.init();

        Result allInit = Result::Ok;

        allInit = resultAnd(allInit, repeatRun::initMPU(mpuRepo));

        if (allInit == Result::Error) {
            Log.fatalln(F("Mail init is fatal!"));
        } else {
            Log.infoln(F("Main init is success"));
        }

        return allInit;
    }

private:
    HoverRepo& hoverRepo;
    MPURepo& mpuRepo;

    RotateUseCase& rotateUseCase;
};