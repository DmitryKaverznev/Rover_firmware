#pragma once

#include "Config.h"
#include "model/hover/HoverRepo.h"
#include "controller/StageController.h"

class DiContainer {
private:
    // repo
    HoverRepo* hoverRepo;

    // controller
    StageController* stageController;

    DiContainer() {
        hoverRepo = new HoverRepo(pins::uart::hoverUp, pins::uart::hoverDown);
        stageController = new StageController(*hoverRepo);
    }

    ~DiContainer() {
        delete hoverRepo;
        delete stageController;
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