#include "Config.h"
#include "DiContainer.h"

HardwareSerial& pins::uart::hoverUp = Serial3;
HardwareSerial& pins::uart::hoverDown = Serial1;
HardwareSerial& pins::uart::camera = Serial1;


ISR(TIMER5_A) {

    DiContainer::getInstance().getHoverRepo()->update();
}