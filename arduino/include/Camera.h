#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>

#define START_BITE 0xAB

typedef struct {
    uint16_t id;
    uint16_t cx;
    uint16_t cy;
    uint16_t w;
} ReciveData;

class Camera {
public:
    Camera(HardwareSerial& cameraSerial);
    void timerInterrupt();
    void begin();
    ReciveData getRecive();
private:
    HardwareSerial& _cameraSerial;
    ReciveData _reciveData;
    
    void _receive();
    
};

#endif