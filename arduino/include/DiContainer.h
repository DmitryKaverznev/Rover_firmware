#pragma once

#include "Config.h"

#include "model/hover/HoverRepo.h"
#include "model/MPURepo.h"
#include "model/CameraRepo.h"

#include "usecase/RotateUseCase.h"

#include "controller/StageController.h"
#include "usecase/LineUseCase.h"

class DiContainer {
    HoverRepo* hoverRepo;
    MPURepo* mpuRepo;
    CameraRepo* cameraRepo;

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


        stageController = new StageController(*hoverRepo,
                                                *mpuRepo,
                                                *cameraRepo,
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
        delete hoverRepo;
    }
};
