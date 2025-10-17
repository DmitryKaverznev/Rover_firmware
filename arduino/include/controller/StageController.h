#pragma once

#include <ArduinoLog.h>
#include <io_di.h>

#include "Servo.h"
#include "model/hover/HoverRepo.h"
#include "model/CameraRepo.h"
#include "model/ServoRepo.h"
#include "model/sonar/SonarRepo.h"

#include "usecase/LineUseCase.h"
#include "usecase/RotateUseCase.h"

#include "utilis/Result.h"
#include "utilis/RepeatRun.h"

class StageController {
public:

    void run() const
  {
      Log.infoln("Starting main program...");

      uint16_t distDown = sonarRepo->getDown().readAverage();
      while (distDown > 20 || distDown == 0)
      {
          distDown = sonarRepo->getDown().readAverage();
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

      rotateUseCase->run(180, 20, {30, 5});

      delay(1000);

      hoverRepo->set(40);
      delay(2000);

      hoverRepo->set(0);

        servoRepo->move(ServoRepo::OPEN, 2000);
        delay(2000);


    }

    Result init() const
    {
        Log.infoln("Starting init...");

        servoRepo->init();

        hoverRepo->init();
        hoverRepo->set(0);
        delay(1000);

        Result allInit = Ok;

        allInit = resultAnd(allInit, repeatRun::initMPU(*mpuRepo));

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

    RotateUseCase* rotateUseCase = IO_INJECT(RotateUseCase);
    LineUseCase* lineUseCase = IO_INJECT(LineUseCase);
};

StageController instanceOfStageController;
inline StageController* getImplementationOfStageController() { return &instanceOfStageController; }