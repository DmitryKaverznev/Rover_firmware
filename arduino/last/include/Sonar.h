#ifndef SONAR_H
#define SONAR_H

#include <Arduino.h>
#include "AceSorting.h"
#include "Config.h"

#define SIZE_AVERAGE 5

#define DIST_SOUND 58
#define DELAY_TRIG 10
#define MAX_DIST 20 * 1000
#define DELAY_BETWEEN 10

class Sonar {
public:
    Sonar(uint8_t pinTrig, uint8_t pinEcho);
    void begin();
    int16_t read();
    int16_t readAverage();

    
private:
    uint8_t _pinTrig;
    uint8_t _pinEcho;

    uint8_t _timer;
    uint8_t _dists[SIZE_AVERAGE];

};

#endif