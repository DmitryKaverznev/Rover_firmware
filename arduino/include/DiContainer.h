#pragma once

#include "Config.h"
#include "model/hover/HoverRepo.h"
#include "usecase/HoverSpeedUseCase.h"
#include "controller/StageController.h"

class DiContainer {
private:
    // repo
    HoverRepo* hoverRepo;

    // use case
    HoverSpeedUseCase* hoverSpeedUseCase;

    // controller
    StageController* stageController;

    DiContainer() {
        hoverRepo = new HoverRepo(pins::uart::hoverUp, pins::uart::hoverDown);
        hoverSpeedUseCase = new HoverSpeedUseCase(*hoverRepo);
        stageController = new StageController(*hoverSpeedUseCase);
    }

    ~DiContainer() {
        delete hoverRepo;
        delete hoverSpeedUseCase;
    }

public:
    static DiContainer& getInstance() {
        static DiContainer _instance; // NOLINT(*-dynamic-static-initializers)
        return _instance;
    }

    StageController* getController() {
        return stageController;
    }

    HoverRepo* getHoverRepo() {
        return hoverRepo;
    }

    DiContainer(const DiContainer&) = delete;
    DiContainer& operator=(const DiContainer&) = delete;

};