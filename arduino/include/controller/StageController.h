#pragma once

#include <ArduinoLog.h>
#include <io_di.h>

#include "Servo.h"
#include "model/hover/HoverRepo.h"
#include "model/CameraRepo.h"
#include "model/MotorRepo.h"
#include "model/ServoRepo.h"
#include "model/sonar/SonarRepo.h"

#include "usecase/RotateUseCase.h"

#include "utilis/Result.h"
#include "utilis/RepeatRun.h"

class StageController {
public:

    void run() const
  {
      Log.infoln("Starting main program...");

        hoverRepo->set(50);
/*
        motorRepo->calibration();
        motorRepo->run(250, 1000);
*/
/*
        uint16_t distDown = sonarRepo->getDown().readMedian();
        while (distDown > 20)
        {
            distDown = sonarRepo->getDown().readMedian();
            Log.infoln("StageController -> getDown: %d", distDown);
        }
        hoverRepo->set(60);

        uint16_t distForward = sonarRepo->getMultyForward();
        while (distForward > 30 || distForward == 0)
        {
          distForward = sonarRepo->getMultyForward();
          Log.infoln("StageController -> distForward =: %d", distForward);
        }
        hoverRepo->set(0);
        delay(1000);

        hoverRepo->set(-50);
        delay(1000);
        hoverRepo->set(0);

        rotateUseCase->run(180, 20, {30, 5});

        delay(1000);

        hoverRepo->set(40);
        delay(2000);

        hoverRepo->set(0);

        servoRepo->init();

        servoRepo->move(ServoRepo::OPEN, 2000);
        delay(2000);

        motorRepo->set(250);
        delay(3000);
        motorRepo->set(0);*/
    }

    Result init() const
    {
        Log.infoln("Starting init...");


        hoverRepo->init();
        hoverRepo->set(0);
        delay(1000);

        Result allInit = Ok;

        allInit = resultAnd(allInit, repeatRun::initMPU(*mpuRepo));

        //motorRepo->motorCalibration();

        if (allInit == Error) {
            Log.fatalln("Mail init is fatal!");
        } else {
            Log.infoln("Main init is success");
        }

        return allInit;
    }

private:
    HoverRepo* hoverRepo = IO_INJECT(HoverRepo);
    MPURepo* mpuRepo = IO_INJECT(MPURepo);
    CameraRepo* cameraRepo = IO_INJECT(CameraRepo);
    SonarRepo* sonarRepo = IO_INJECT(SonarRepo);
    ServoRepo* servoRepo = IO_INJECT(ServoRepo);
    MotorRepo* motorRepo = IO_INJECT(MotorRepo);

    RotateUseCase* rotateUseCase = IO_INJECT(RotateUseCase);
};

StageController instanceOfStageController;
inline StageController* getImplementationOfStageController() { return &instanceOfStageController; }