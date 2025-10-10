#pragma once

#include <model/sonar/SonarUnit.h>

class SonarRepo
{
public:
    SonarRepo(const SonarUnit::Pins pinsSonarUp1,
                const SonarUnit::Pins pinsSonarUp2,
                const SonarUnit::Pins pinsSonarBack) :
                sonarForward1(pinsSonarUp1),
                sonarForward2(pinsSonarUp2),
                sonarBack(pinsSonarBack) {
    }

    SonarUnit& getSonarForward1()
    {
        return sonarForward1;
    }

    SonarUnit& getSonarForward2()
    {
        return sonarForward2;
    }

    SonarUnit& getSonarBack()
    {
        return sonarBack;
    }

    uint16_t getDistSonarsUp()
    {
        const uint16_t dist1 = sonarForward1.readAverage();
        const uint16_t dist2 = sonarForward2.readAverage();

        if (dist1 == 0)
            return dist2;
        if (dist2 == 0)
            return dist1;

        return (sonarForward1.readAverage() + sonarForward2.readAverage()) / 2;
    }

private:
    SonarUnit sonarForward1;
    SonarUnit sonarForward2;
    SonarUnit sonarBack;
};