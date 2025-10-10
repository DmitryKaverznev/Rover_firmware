#pragma once

#include <Arduino.h>

class MotorRepo
{
public:
    explicit MotorRepo(const uint8_t pinPwm,
             const uint8_t pinDir1,
             const uint8_t pinDir2,
             const uint8_t pinEncDir,
             const uint8_t pinEncInterrupt,
             const uint8_t pinCurr) :
        _pinPwm(pinPwm),
        _pinDir1(pinDir1),
        _pinDir2(pinDir2),
        _pinEncDir(pinEncDir),
        _pinEncInterrupt(pinEncInterrupt),
        _pinCurr(pinCurr)
    {
        pinMode(_pinPwm, OUTPUT);
        pinMode(_pinDir1, OUTPUT);
        pinMode(_pinDir2, OUTPUT);

        pinMode(_pinEncDir, INPUT);
        pinMode(_pinEncInterrupt, INPUT);
        pinMode(_pinCurr, INPUT);
    }

    void set(int speed) const
    {
        speed = max(speedMin, min(speedMax, speed));

        digitalWrite(_pinDir1, speed > 0);
        digitalWrite(_pinDir2, speed < 0);
        analogWrite(_pinPwm, abs(speed));
    }

    void (* encoderInterrupt())()
    {
        if (digitalRead(_pinEncDir))
            _enc++;
        else
            _enc--;
        return nullptr;
    }

    int16_t getEnc() const
    {
        return _enc;
    }

    void encReset()
    {
        _enc = 0;
    }

    uint16_t getCurr()
    {
        _curr = analogRead(_pinCurr);
        return _curr;
    }

    void calibration()
    {
        encReset();
        set(calibrationSpeed);
        delay(500);
        set(-calibrationSpeed);
        delay(250);
        while (getCurr() < maxCurr) {}
        set(0);
        encReset();
    }
private:
    uint8_t _pinPwm;
    uint8_t _pinDir1;
    uint8_t _pinDir2;
    uint8_t _pinEncDir;
    uint8_t _pinEncInterrupt;
    uint8_t _pinCurr;

    int16_t _enc = 0;
    uint16_t _curr = 0;

    const int speedMax = 255;
    const int speedMin = -255;

    const int calibrationSpeed = 250;
    const int maxCurr = 25;
};