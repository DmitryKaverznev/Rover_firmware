#pragma once
#include "model/hover/HoverRepo.h"
#include "model/sonar/SonarRepo.h"

class HoverGoSonarUseCase
{
public:
    void run() const
    {
        uint16_t distForward = _sonarRepo->getMultyForward();
        while (distForward > 30 || distForward == 0)
        {
            distForward = _sonarRepo->getMultyForward();
            Log.infoln("HoverGoSonarUseCase -> distForward =: %d", distForward);
        }
    }

private:
    HoverRepo* _hoverRepo = IO_INJECT(HoverRepo);
    SonarRepo* _sonarRepo = IO_INJECT(SonarRepo);
};

HoverGoSonarUseCase instanceOfHoverGoSonarUseCase;
inline HoverGoSonarUseCase* getImplementationOfHoverGoSonarUseCase() { return &instanceOfHoverGoSonarUseCase; }