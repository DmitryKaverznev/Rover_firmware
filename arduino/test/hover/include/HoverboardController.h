#ifndef HOVERBOARDCONTROLLER_H
#define HOVERBOARDCONTROLLER_H

#include <Arduino.h>

#define HOVER_SERIAL_BAUD   115200
#define START_FRAME         0xABCD

typedef struct {
    uint16_t start;
    int16_t  steer;
    int16_t  speed;
    uint16_t checksum;
} SerialCommand;

typedef struct {
    uint16_t start;
    int16_t  cmd1;
    int16_t  cmd2;
    int16_t  speedR_meas;
    int16_t  speedL_meas;
    int16_t  batVoltage;
    int16_t  boardTemp;
    uint16_t cmdLed;
    uint16_t checksum;
} SerialFeedback;

typedef struct {
    int16_t speedStart = 0;
    int16_t speedEnd = 0;
    uint32_t timeStart = 0;
    uint32_t timeEnd = 0;
} SoftMove;

enum class DebugMode {
    DISABLE,
    ENABLE
};

class HoverboardController {
public:
    HoverboardController(USARTClass& hoverSerial);
    void begin();
    void set(int16_t steer, int16_t speed);
    void setSoft(int16_t steer, int16_t time, int16_t speedEnd);
    void timerInterrupt();
    void updateReceive();
    void setDebug(DebugMode debugMode);

private:
    USARTClass& _hoverSerial;
    SerialCommand _command;
    SoftMove softMove;
    DebugMode _debugMode;


    void _updateSoftMove();
    void _send();
    void _softMoveDebug();
};

#endif // HOVERBOARDCONTROLLER_H
