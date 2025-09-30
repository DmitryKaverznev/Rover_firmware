#pragma once

#include "Config.h"

#include "model/hover/HoverRepo.h"
#include "model/MPURepo.h"

#include "controller/StageController.h"

class DiContainer {
private:
    HoverRepo* hoverRepo;
    MPURepo* mpuRepo;

    StageController* stageController;

public:
    DiContainer() {
        hoverRepo = new HoverRepo(pins::uart::hoverUp, pins::uart::hoverDown);
        mpuRepo = new MPURepo();
        stageController = new StageController(*hoverRepo, *mpuRepo);
    }

    StageController* getController() {
        return stageController;
    }

    MPURepo* getMPURepo() {
        return mpuRepo;
    }

    ~DiContainer() {
        delete stageController;
        delete mpuRepo;
        delete hoverRepo;
    }
};
