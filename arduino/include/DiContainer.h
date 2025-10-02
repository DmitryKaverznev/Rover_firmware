#pragma once

#include "Config.h"

#include "model/hover/HoverRepo.h"
#include "model/MPURepo.h"

#include "usecase/RotateUseCase.h"

#include "controller/StageController.h"

class DiContainer {
private:
    HoverRepo* hoverRepo;
    MPURepo* mpuRepo;

    RotateUseCase* rotateUseCase;

    StageController* stageController;

public:
    DiContainer() {
        hoverRepo = new HoverRepo(pins::uart::hoverUp, pins::uart::hoverDown);
        mpuRepo = new MPURepo();
        rotateUseCase = new RotateUseCase(*mpuRepo, *hoverRepo);
        stageController = new StageController(*hoverRepo, *mpuRepo, *rotateUseCase);
    }

    StageController* getController() {
        return stageController;
    }

    ~DiContainer() {
        delete stageController;
        delete mpuRepo;
        delete rotateUseCase;
        delete hoverRepo;
    }
};
