#pragma once

#include <ArduinoLog.h>
#include <io_di.h>

#include "Servo.h"
#include "model/hover/HoverRepo.h"
#include "model/CameraRepo.h"
#include "model/MotorRepo.h"
#include "model/ServoRepo.h"
#include "model/sonar/SonarRepo.h"
#include "usecase/HoverGoSonarUseCase.h"

#include "usecase/RotateUseCase.h"
#include "usecase/SonarWaitUseCase.h"

#include "utilis/Result.h"
#include "utilis/RepeatRun.h"

class StageController {
public:
    [[noreturn]] void run() const
  {
      Log.infoln("Starting main program...");

        while (true) {
            const CameraRepo::Command command = cameraRepo->getData();
            Log.infoln("%d %d %d %d", command.start.x, command.start.y, command.end.x, command.end.y);
        }
/*
        while (true)
        {
            rotateUseCase->run(180, 20, {50, 5});
            hoverRepo->set(0);
            delay(3000);
        }
/*
        sonarBackWaitUseCase->run();
        hoverRepo->set(60);


        hoverGoSonarUseCase->run();
        hoverRepo->set(0);
        delay(1000);
        hoverRepo->set(-50);
        delay(1000);
        hoverRepo->set(0);
        rotateUseCase->run(180, 20, {50, 5});
        delay(1000);
        hoverRepo->set(50);
        hoverGoSonarUseCase->run();
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
        delay(2500);
        motorRepo->up();*/
    }

    Result init() const
    {
        Log.infoln("Starting init...");


        hoverRepo->init();
        hoverRepo->set(0);
        delay(1000);

        Result allInit = Ok;

        allInit = resultAnd(allInit, repeatRun::initMPU(*mpuRepo));

#ifdef GLOBAL_CONFIG__ENABLE_LIFT
        motorRepo->calibration();
#endif

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
    HoverGoSonarUseCase* hoverGoSonarUseCase = IO_INJECT(HoverGoSonarUseCase);
    SonarBackWaitUseCase* sonarBackWaitUseCase = IO_INJECT(SonarBackWaitUseCase);
};

StageController instanceOfStageController;
inline StageController* getImplementationOfStageController() { return &instanceOfStageController; }