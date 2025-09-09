#include "Camera.h"


Camera::Camera(HardwareSerial& cameraSerial) : _cameraSerial(cameraSerial) {}

void Camera::begin() {
  _cameraSerial.begin(115200);
}

void Camera::timerInterrupt() {
    _receive();
}

void Camera::_receive() {
        const uint8_t packetSize = sizeof(ReciveData) + sizeof(uint8_t) * 2;
    
    if (_cameraSerial.available() >= packetSize) {

        uint8_t bites[packetSize];
        _cameraSerial.readBytes(bites, packetSize);
        
        /*
        for (uint8_t i = 0; i < packetSize; ++i) {
            Serial.print(bites[i], HEX);
            Serial.print(' ');
        }
        Serial.print('\n');
        */

        if (bites[0] != START_BITE) {
            return;
        }
        
        uint8_t checksum = bites[0];
        for (uint8_t i = 1; i < 9; ++i) {
            checksum ^= bites[i];
        }
        
        if (checksum != bites[9]) {
            Serial.println("checksum error");
            return;
        }
        
        _reciveData.id = (bites[1] << 8) | bites[2];
        _reciveData.cx = (bites[3] << 8) | bites[4];
        _reciveData.cy = (bites[5] << 8) | bites[6];
        _reciveData.w  = (bites[7] << 8) | bites[8];
    }
}


ReciveData Camera::getRecive() {
    return _reciveData;
}