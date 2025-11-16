#pragma once
#include "ArduinoLog.h"
#include "io_di.h"
#include "model/hover/HoverRepo.h"
#include "usecase/HoverGoSonarUseCase.h"
#include "usecase/HoverSoftMoveUseCase.h"
#include "usecase/RotateUseCase.h"
#include "util/RepeatRun.h"
#include "util/Result.h"

class SuperTeamController {
public:
    void run() const {
        Log.infoln("Starting main program...");

        hoverSoftMoveUseCase->run(2000, {0, 50});

        const HoverGoSonarUseCase::StatusReturn status1 = hoverGoSonarUseCase->run(); // едем до домика
        if (status1 == HoverGoSonarUseCase::CAMERA) {
            hoverRepo->set(50, 45);
            const float startAngel = getYaw();
            while (true) {
                if (startAngel + 90 < getYaw()) {
                    break;
                }
            }
        } else {
            rotateUseCase->run(90, 40, {30, 15}); // разворачиваемся
        }

        hoverRepo->set(50, 0);

        const HoverGoSonarUseCase::StatusReturn status2 = hoverGoSonarUseCase->run(); // едем до домика
        if (status2 == HoverGoSonarUseCase::CAMERA) {
            hoverRepo->set(50);
            hoverSoftMoveUseCase->run(1500, {50, 0});
        }
        hoverRepo->set(0);

        //*/
    }

    float getYaw() const
    {
        return mpuRepo->getData()[0] * 180 / M_PI;
    }

    Result init() const
    {
        Log.infoln("Starting init...");


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

    RotateUseCase* rotateUseCase = IO_INJECT(RotateUseCase);
    HoverGoSonarUseCase* hoverGoSonarUseCase = IO_INJECT(HoverGoSonarUseCase);
    HoverSoftMoveUseCase* hoverSoftMoveUseCase = IO_INJECT(HoverSoftMoveUseCase);
};

SuperTeamController instanceOfSuperTeamController;
inline SuperTeamController* getImplementationOfSuperTeamController() { return &instanceOfSuperTeamController; }