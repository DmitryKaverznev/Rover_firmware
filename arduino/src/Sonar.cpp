#include "Sonar.h"

Sonar::Sonar(uint8_t pinTrig, uint8_t pinEcho) {
    _pinTrig = pinTrig;
    _pinEcho = pinEcho;
}

int16_t Sonar::read() {
    while(millis() - _timer < DELAY_BETWEEN);
    
    digitalWrite(_pinTrig, HIGH);
    delayMicroseconds(DELAY_TRIG);
    digitalWrite(_pinTrig, LOW);

    int16_t duration = pulseIn(_pinEcho, HIGH, MAX_DIST);
    _timer = millis();
    return duration / DIST_SOUND; 
}

int16_t Sonar::readAverage()
{
    for (int i = 0; i < SIZE_AVERAGE; i++)
    {
        _dists[i] = read();
    }

    ace_sorting::shellSortKnuth(_dists, SIZE_AVERAGE);
    return _dists[SIZE_AVERAGE / 2];
}

void Sonar::begin() {
    pinMode(_pinTrig, OUTPUT);
    pinMode(_pinEcho, INPUT);

    _timer = millis();
}