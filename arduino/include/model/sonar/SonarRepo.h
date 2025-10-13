#pragma once

#include "model/sonar/SonarUnit.h"

class SonarRepo
{
public:
    SonarRepo(
        const uint8_t sonarForward1_pinTrig, const uint8_t sonarForward1_pinEcho,
        const uint8_t sonarForward2_pinTrig, const uint8_t sonarForward2_pinEcho,
        const uint8_t sonarDown_pinTrig, const uint8_t sonarDown_pinEcho
    ) :
        _sonarForward1(sonarForward1_pinTrig, sonarForward1_pinEcho),
        _sonarForward2(sonarForward2_pinTrig, sonarForward2_pinEcho),
        _sonarDown(sonarDown_pinTrig, sonarDown_pinEcho)
    {}

    SonarUnit& getForward1() { return _sonarForward1; }
    SonarUnit& getForward2() { return _sonarForward2; }
    SonarUnit& getDown() { return _sonarDown; }

    uint16_t getMultyForward()
    {
        const uint16_t dist1 = _sonarForward1.readAverage();
        const uint16_t dist2 = _sonarForward2.readAverage();

        if (dist1 == 0)
            return dist2;
        if (dist2 == 0)
            return dist1;

        return (dist1 + dist2) / 2;
    }

private:
    SonarUnit _sonarForward1;
    SonarUnit _sonarForward2;
    SonarUnit _sonarDown;
};
