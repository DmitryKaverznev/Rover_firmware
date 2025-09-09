#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

#define SPEED_MAX 255
#define SPEED_MIN (-255)


class Motor {
public:
    Motor(uint8_t pinPwm,
             uint8_t pinDir1,
             uint8_t pinDir2,
             uint8_t pinEncDir,
             uint8_t pinEncInterrupt,
             uint8_t pinCurr);
    void init();
    void set(int speed);
    void encoderInterrupt();

    int16_t getEnc();
    void encReset();
    uint16_t getCurr();
private:
    uint8_t _pinPwm;
    uint8_t _pinDir1;
    uint8_t _pinDir2;
    uint8_t _pinEncDir;
    uint8_t _pinEncInterrupt;
    uint8_t _pinCurr;

    int16_t _enc = 0;
    uint16_t _curr = 0;

    void _updateInc();
    void _updateCurr();
};

#endif