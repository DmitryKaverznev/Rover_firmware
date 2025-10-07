#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

#include "model/hover/HoverRepo.h"
#include "model/CameraRepo.h"
#include "usecase/LineUseCase.h"

#include "usecase/RotateUseCase.h"

#include "utilis/Result.h"
#include "utilis/RepeatRun.h"

class StageController {
public:
  
  StageController(HoverRepo& hoverRepo,
                                   MPURepo& mpuRepo,
                                   CameraRepo& cameraRepo,
                                   RotateUseCase& rotateUseCase,
                                   LineUseCase& lineUseCase)
                      : hoverRepo(hoverRepo),
                        mpuRepo(mpuRepo),
                        cameraRepo(cameraRepo),
                        rotateUseCase(rotateUseCase),
                        lineUseCase(lineUseCase)
  {
  }

  [[noreturn]] void run() const
    {
        Log.infoln("Starting main program...");


        while(true) {
            hoverRepo.set(10);
        }

        //rotateUseCase(90.0f, 20.0f, RotateUseCase::SpeedValue{60.0f, 2.0f});
    }

    Result init() const
    {
        Log.infoln("Starting init...");

        hoverRepo.init();
        hoverRepo.set(0);
        delay(1000);

        Result allInit = Ok;

        allInit = resultAnd(allInit, repeatRun::initMPU(mpuRepo));

        if (allInit == Error) {
            Log.fatalln("Mail init is fatal!");
        } else {
            Log.infoln("Main init is success");
        }

        return allInit;
    }

private:
    HoverRepo& hoverRepo;
    MPURepo& mpuRepo;
    CameraRepo& cameraRepo;

    RotateUseCase& rotateUseCase;
    LineUseCase& lineUseCase;
};