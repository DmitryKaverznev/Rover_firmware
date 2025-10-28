#pragma once
#include "ArduinoLog.h"
#include "model/sonar/SonarRepo.h"

class SonarBackWaitUseCase
{
public:
    void run() const
    {
        uint16_t distDown = sonarRepo->getDown().readMedian();
        while (distDown > 20)
        {
            distDown = sonarRepo->getDown().readMedian();
            Log.infoln("SonarWaitUseCase -> getDown: %d", distDown);
        }
    }

private:
    SonarRepo* sonarRepo = IO_INJECT(SonarRepo);
};

SonarBackWaitUseCase instanceOfSonarBackWaitUseCase;
inline SonarBackWaitUseCase* getImplementationOfSonarBackWaitUseCase() { return &instanceOfSonarBackWaitUseCase; }