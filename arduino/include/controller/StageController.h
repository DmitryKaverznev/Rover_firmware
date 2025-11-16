#pragma once

#include <ArduinoLog.h>
#include <io_di.h>

#include "model/hover/HoverRepo.h"
#include "model/CameraRepo.h"
#include "model/MotorRepo.h"
#include "model/ServoRepo.h"
#include "model/sonar/SonarRepo.h"
#include "usecase/HoverGoSonarUseCase.h"
#include "usecase/HoverSoftMoveUseCase.h"

#include "usecase/RotateUseCase.h"
#include "usecase/SonarWaitUseCase.h"

#include "util/Result.h"
#include "util/RepeatRun.h"

class StageController {
public:
    void run() const
    {
        Log.infoln("Starting main program...");


        sonarBackWaitUseCase->run();
        hoverSoftMoveUseCase->run(1000, {0, 50}); // ждём человека

        const HoverGoSonarUseCase::StatusReturn status1 = hoverGoSonarUseCase->run(); // едем до домика
        if (status1 == HoverGoSonarUseCase::CAMERA) {
            hoverRepo->set(50);
            hoverSoftMoveUseCase->run(900, {50, 0});
            hoverRepo->set(0);
        }
        hoverRepo->set(0);

        delay(1000);

        hoverSoftMoveUseCase->run(250, {0, -50});
        hoverSoftMoveUseCase->run(250, {-50, 0});

        rotateUseCase->run(180, 20, {50, 7}); // разворачиваемся
        delay(500);

        hoverSoftMoveUseCase->run(5000, {0, 50});

        const HoverGoSonarUseCase::StatusReturn status2 = hoverGoSonarUseCase->run(); // едем до домика
        if (status2 == HoverGoSonarUseCase::CAMERA) {
            hoverRepo->set(50);
            hoverSoftMoveUseCase->run(1000, {50, 0});
            hoverRepo->set(0);
        }

        hoverSoftMoveUseCase->run(500, {0, -50});
        delay(1500);
        hoverRepo->set(0);

        delay(500);
        servoRepo->init();

        servoRepo->move(ServoRepo::OPEN, 4000);
        delay(4500);
        motorRepo->up();

        //*/
    }

    Result init() const
    {
        Log.infoln("Starting init...");


        hoverRepo->init();
        hoverRepo->set(0);
        delay(1000);

        motorRepo->calibration();

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
    MotorRepo* motorRepo = IO_INJECT(MotorRepo);

    RotateUseCase* rotateUseCase = IO_INJECT(RotateUseCase);
    HoverGoSonarUseCase* hoverGoSonarUseCase = IO_INJECT(HoverGoSonarUseCase);
    SonarBackWaitUseCase* sonarBackWaitUseCase = IO_INJECT(SonarBackWaitUseCase);
    HoverSoftMoveUseCase* hoverSoftMoveUseCase = IO_INJECT(HoverSoftMoveUseCase);
};

StageController instanceOfStageController;
inline StageController* getImplementationOfStageController() { return &instanceOfStageController; }