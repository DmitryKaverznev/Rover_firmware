#pragma once

#include "io_di.h"
#include "model/sonar/SonarUnit.h"

class SonarRepo
{
public:
    SonarUnit& getForward1() const { return *_sonarForward1; }
    SonarUnit& getForward2() const { return *_sonarForward2; }
    SonarUnit& getDown() const { return *_sonarDown; }

    uint16_t getMultyForward() const
    {
        const uint16_t dist1 = _sonarForward1->readMedian();
        const uint16_t dist2 = _sonarForward2->readMedian();

        return min(dist1, dist2);
    }

private:
    SonarUnit* _sonarForward1 = IO_INJECT(SonarUnitUp1);
    SonarUnit* _sonarForward2 = IO_INJECT(SonarUnitUp2);
    SonarUnit* _sonarDown = IO_INJECT(SonarUnitDown);
};

SonarRepo instanceOfSonarRepo;
inline SonarRepo* getImplementationOfSonarRepo() { return &instanceOfSonarRepo; }