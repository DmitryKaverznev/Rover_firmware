#include "Config.h"
#include "DiContainer.h"

HardwareSerial& pins::uart::hoverUp = Serial3;
HardwareSerial& pins::uart::hoverDown = Serial2;
HardwareSerial& pins::uart::camera = Serial1;

bool DMPReady = false;
void DMPDataReady() {
    DMPReady = true;
}