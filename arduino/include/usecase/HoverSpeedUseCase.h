#pragma once

#include <ArduinoLog.h>

class HoverSpeedUseCase {
public:
    explicit HoverSpeedUseCase(HoverRepo& hoverRepo) :
        hoverRepo(hoverRepo) {
        hoverRepo.init();
    }

    void invoke(int16_t speed, int16_t steer = 0) {
        hoverRepo.set(speed, steer);
    }

private:
    HoverRepo &hoverRepo;
};