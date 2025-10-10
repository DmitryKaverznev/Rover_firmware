#pragma once

#include "Config.h"

#include "model/hover/HoverRepo.h"
#include "model/MPURepo.h"
#include "model/CameraRepo.h"
#include "model/sonar/SonarRepo.h"

#include "usecase/RotateUseCase.h"

#include "controller/StageController.h"
#include "usecase/LineUseCase.h"

class DiContainer {
    HoverRepo* hoverRepo;
    MPURepo* mpuRepo;
    CameraRepo* cameraRepo;
    SonarRepo* sonarRepo;
    MotorRepo* motorRepo;
    ServoRepo* servoRepo;

    RotateUseCase* rotateUseCase;
    LineUseCase* lineUseCase;

    StageController* stageController;

public:
    DiContainer() {
        hoverRepo = new HoverRepo(pins::uart::hoverUp, pins::uart::hoverDown);
        mpuRepo = new MPURepo();
        cameraRepo = new CameraRepo(pins::uart::camera);
        rotateUseCase = new RotateUseCase(*mpuRepo, *hoverRepo);
        lineUseCase = new LineUseCase(*cameraRepo, *hoverRepo);
        sonarRepo = new SonarRepo(
                            {pins::sonar::trigF1, pins::sonar::echoF1},
                            {pins::sonar::trigF2, pins::sonar::echoF2},
                            {pins::sonar::trigBack, pins::sonar::echoBack});
        motorRepo = new MotorRepo(
                                    pins::motor::pwm,
                                    pins::motor::dir1,
                                    pins::motor::dir2,
                                    pins::motor::encDir,
                                    pins::motor::encInt,
                                    pins::motor::curr);
        servoRepo = new ServoRepo(pins::servoCap);


        stageController = new StageController(*hoverRepo,
                                                *mpuRepo,
                                                *cameraRepo,
                                                *sonarRepo,
                                                *motorRepo,
                                                *servoRepo,
                                                *rotateUseCase,
                                                *lineUseCase);
    }

    StageController* getController() const
    {
        return stageController;
    }

    ~DiContainer() {
        delete stageController;
        delete mpuRepo;
        delete cameraRepo;
        delete rotateUseCase;
        delete motorRepo;
        delete servoRepo;
        delete hoverRepo;
    }
};
