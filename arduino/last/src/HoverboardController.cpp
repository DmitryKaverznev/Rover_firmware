#include "HoverboardController.h"

HoverboardController::HoverboardController(HardwareSerial& hoverSerial) : _hoverSerial(hoverSerial) {
    _command.start = 0;
    _command.steer = 0;
    _command.speed = 0;
    _command.checksum = 0;
}

void HoverboardController::begin() {
    _hoverSerial.begin(HOVER_SERIAL_BAUD);
}

void HoverboardController::set(int16_t steer, int16_t speed) {
    softMove.speedStart = speed;
    _command.steer = steer;
    _command.speed = speed;
}

void HoverboardController::setSoft(int16_t steer, int16_t time, int16_t speedEnd) {
    softMove.speedStart = _command.speed;
    softMove.speedEnd = speedEnd;
    softMove.timeStart = millis();
    softMove.timeEnd = softMove.timeStart + time;
    _command.steer = steer;
}

void HoverboardController::_updateSoftMove() {
    if (softMove.timeEnd == 0) return;

    uint32_t currentTime = millis();

    if (currentTime >= softMove.timeEnd) {
        _command.speed = softMove.speedEnd;
        softMove = {};
        return;
    }

    uint32_t elapsed = currentTime - softMove.timeStart;
    uint32_t duration = softMove.timeEnd - softMove.timeStart;
    if (duration == 0) return;

    int32_t speedDiff = softMove.speedEnd - softMove.speedStart;
    int16_t newSpeed = softMove.speedStart + (int16_t)(((long long) speedDiff * elapsed) / duration);
    
    _command.speed = newSpeed;
}


void HoverboardController::_send() {
    SerialCommand command = _command;
    command.speed = command.speed * -1;

    command.start = (uint16_t)START_FRAME;
    command.checksum = (uint16_t)(command.start ^ command.steer ^ command.speed);

    _hoverSerial.write((uint8_t *) &command, sizeof(command));

    /*
    for (int i = 0; i < 24; ++i) {
        _hoverSerial.write(0x00);
    }*/

}

void HoverboardController::setDebug(DebugMode debugMode) { 
    _debugMode = debugMode;
}

void HoverboardController::_softMoveDebug() {
    if (_debugMode == DebugMode::ENABLE)
    {
        uint32_t currentTime = millis();

        if (softMove.timeEnd == 0) return;
        
        uint32_t elapsed = currentTime - softMove.timeStart;
        uint32_t duration = softMove.timeEnd - softMove.timeStart;
        if (duration == 0) return;

        int32_t speedDiff = softMove.speedEnd - softMove.speedStart;
        int16_t newSpeed = softMove.speedStart + (int16_t)(((long long)speedDiff * elapsed) / duration);
        

        Serial.println(newSpeed);
    }
    
}

void HoverboardController::timerInterrupt() {
    _updateSoftMove();
    _softMoveDebug();
    _send();
}