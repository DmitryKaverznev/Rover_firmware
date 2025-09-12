#pragma once

#include <Arduino.h>

typedef struct{
    uint16_t start;
    int16_t  steer;
    int16_t  speed;
    uint16_t checksum;
} SerialCommand;

typedef struct{
    int16_t  steer;
    int16_t  speed;
} HoverRoute;