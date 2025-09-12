#include "Motor.h"

Motor::Motor(uint8_t pinPwm,
             uint8_t pinDir1,
             uint8_t pinDir2,
             uint8_t pinEncDir,
             uint8_t pinEncInterrupt,
             uint8_t pinCurr)
{
    _pinPwm = pinPwm;
    _pinDir1 = pinDir1;
    _pinDir2 = pinDir2;
    _pinEncDir = pinEncDir;
    _pinEncInterrupt = pinEncInterrupt;
    _pinCurr = pinCurr;
}

void Motor::init()
{
    pinMode(_pinPwm, OUTPUT);
    pinMode(_pinDir1, OUTPUT);
    pinMode(_pinDir2, OUTPUT);
    pinMode(_pinEncDir, INPUT);
    pinMode(_pinEncInterrupt, INPUT);
}

void Motor::set(int speed)
{
    speed = max(SPEED_MIN, min(SPEED_MAX, speed));

    digitalWrite(_pinDir1, speed > 0);
    digitalWrite(_pinDir2, speed < 0);
    analogWrite(_pinPwm, abs(speed));
}

void Motor::encoderInterrupt()
{
    _updateInc();
}

int16_t Motor::getEnc()
{
    return _enc;
}

void Motor::encReset()
{
    _enc = 0;
}

uint16_t Motor::getCurr()
{
    _updateCurr();
    return _curr;
}

void Motor::_updateInc()
{
    if (digitalRead(_pinEncDir))
        _enc++;
    else
        _enc--;
}

void Motor::_updateCurr()
{
    _curr = analogRead(_pinCurr);
}
