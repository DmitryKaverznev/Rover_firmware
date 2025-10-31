#pragma once
#include "model/CameraRepo.h"
#include "model/hover/HoverRepo.h"
#include "model/sonar/SonarRepo.h"

class HoverGoSonarUseCase
{
public:
    enum StatusReturn {
        CAMERA,
        SONAR
    };

    StatusReturn run() const
    {
        CameraRepo::Command command;
        command.data = 0;
        _cameraRepo->clear();

        while (true)
        {
            _cameraRepo->update();
            command = _cameraRepo->getData();
            const uint16_t distForward = _sonarRepo->getMultyForward();
            Log.infoln("HoverGoSonarUseCase -> distForward =: %d", distForward);

            if (distForward < 30 && distForward != 0) {
                return SONAR;
            }
            if (command.data == 1 || command.data == 2) {
                return CAMERA;
            }
        }
    }

private:
    HoverRepo* _hoverRepo = IO_INJECT(HoverRepo);
    SonarRepo* _sonarRepo = IO_INJECT(SonarRepo);
    CameraRepo* _cameraRepo = IO_INJECT(CameraRepo);
};

HoverGoSonarUseCase instanceOfHoverGoSonarUseCase;
inline HoverGoSonarUseCase* getImplementationOfHoverGoSonarUseCase() { return &instanceOfHoverGoSonarUseCase; }