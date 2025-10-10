#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

#include "Config.h"
#include "model/hover/HoverRepo.h"
#include "model/CameraRepo.h"
#include "model/MotorRepo.h"
#include "model/ServoRepo.h"
#include "model/sonar/SonarRepo.h"

#include "usecase/LineUseCase.h"
#include "usecase/RotateUseCase.h"

#include "utilis/Result.h"
#include "utilis/RepeatRun.h"

class StageController {
public:
  
  StageController(
                   HoverRepo& hoverRepo,
                   MPURepo& mpuRepo,
                   CameraRepo& cameraRepo,
                   SonarRepo& sonarRepo,
                   MotorRepo& motorRepo,
                   ServoRepo& servoRepo,
                   RotateUseCase& rotateUseCase,
                   LineUseCase& lineUseCase) :
                hoverRepo(hoverRepo),
                mpuRepo(mpuRepo),
                cameraRepo(cameraRepo),
                sonarRepo(sonarRepo),
                motorRepo(motorRepo),
                servoRepo(servoRepo),
                rotateUseCase(rotateUseCase),
                lineUseCase(lineUseCase)
  {
  }

    [[noreturn]] void run() const
    {
        Log.infoln("Starting main program...");
        servoRepo.getServo().attach(45);
        while(true) {/*
            const uint16_t distForward = sonarRepo.getDistSonarsUp();
            const uint16_t distBack = sonarRepo.getSonarBack().readAverage();

            Log.infoln("StageController -> %d %d", distForward, distBack);*/

            servoRepo.getServo().write(0);
            Log.infoln("open");
            delay(3000);
            servoRepo.getServo().write(90);
            Log.infoln("close");
            delay(3000);
        }
    }


    void runLast() const
    {
      //attachInterrupt(digitalPinToInterrupt(pins::motor::encInt), motorRepo.encoderInterrupt(), RISING);

      //motorRepo.calibration();


      uint8_t dist = sonarRepo.getSonarBack().readAverage();
      while (dist == 0 || dist > 50) {
          dist = sonarRepo.getSonarBack().readAverage();
      }
      Log.infoln("1");
      hoverRepo.set(50);
      delay(3000);
      motorRepo.set(0);
      Log.infoln("2");
    }

    void goMeters(const uint8_t meters) const
    {
      const uint8_t time = abs(meters * 1500);

      hoverRepo.set(50);
      delay(time);
      hoverRepo.set(0, 0);
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
    SonarRepo& sonarRepo;
    MotorRepo& motorRepo;
    ServoRepo& servoRepo;

    RotateUseCase& rotateUseCase;
    LineUseCase& lineUseCase;
};