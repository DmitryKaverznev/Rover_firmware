#pragma once

#include <Arduino.h>
#include <AceSorting.h>

class SonarUnit
{
public:
    struct Pins
    {
        const uint8_t pinTrig;
        const uint8_t pinEcho;
    };

    explicit SonarUnit(const Pins pins) :
        _pinTrig(pins.pinTrig),
        _pinEcho(pins.pinEcho) {
        pinMode(_pinTrig, OUTPUT);
        pinMode(_pinEcho, INPUT);
        _timer = millis();
    }

    uint16_t read()
    {
        while(millis() - _timer < DELAY_BETWEEN) {}

        digitalWrite(_pinTrig, HIGH);
        delayMicroseconds(DELAY_TRIG);
        digitalWrite(_pinTrig, LOW);

        const uint16_t duration = pulseIn(_pinEcho, HIGH, MAX_DIST);
        _timer = millis();
        return duration / DIST_SOUND;
    }
    uint16_t readAverage()
    {
        for (int i = 0; i < SIZE_AVERAGE; i++)
        {
            _dists[i] = read();
        }

        ace_sorting::shellSortKnuth(_dists, SIZE_AVERAGE);
        return _dists[SIZE_AVERAGE / 2];
    }


private:
    uint8_t _pinTrig;
    uint8_t _pinEcho;

    uint8_t _timer;
    uint16_t _dists[5] = {};

    const int DELAY_BETWEEN = 10;
    const int DELAY_TRIG = 10;
    const int MAX_DIST = 20 * 1000;
    const int DIST_SOUND = 58;
    const int SIZE_AVERAGE = 5;

};