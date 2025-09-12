#include "Device.h"

#include <Arduino.h>
#include "TimerOne.h"

HoverboardController dev::hoverUp(pins::uart::hoverUp);
HoverboardController dev::hoverDown(pins::uart::hoverDown);

Camera dev::camera(pins::uart::camera);

Sonar dev::sonarForward1(pins::sonar::trigF1, pins::sonar::echoF1);
Sonar dev::sonarForward2(pins::sonar::trigF2, pins::sonar::echoF2);
Sonar dev::sonarBack(pins::sonar::trigBack, pins::sonar::echoBack);

Motor dev::motor(pins::motor::pwm,
                 pins::motor::dir1,
                 pins::motor::dir2,
                 pins::motor::encDir,
                 pins::motor::encEnc,
                 pins::motor::curr);

Servo dev::servoCap;

void encoderInterrupt() {
    dev::motor.encoderInterrupt();
}

void dev::hoverBoardSet(int16_t steer, int16_t speed) {
    //Serial.println("Set: " +  String(speed));

    dev::hoverUp.set(steer, speed);
    dev::hoverDown.set(steer, speed);
}

void dev::hoverBoardSetSoft(int16_t steer, int16_t time, int16_t speedEnd) {
    dev::hoverUp.setSoft(steer, time, speedEnd);
    dev::hoverDown.setSoft(steer, time, speedEnd);
}

void dev::hoverBoardInit() {
    hoverUp.begin();
    hoverDown.begin();

    hoverBoardSet(0, 0);
    delay(1000);
}

void dev::cameraInit() {
    camera.begin();
}

void dev::servoInit()
{
    servoCap.attach(pins::servoCap);
}

void dev::motorInit()
{
    motor.init();
    
    attachInterrupt(digitalPinToInterrupt(pins::motor::encEnc), encoderInterrupt, RISING);
}

void dev::timersInit() {
    Timer1.initialize(settings::hoverboard::timeSendPeriod); // В микросекундах
    Timer1.attachInterrupt([](){
        digitalWrite(13, HIGH);
        dev::hoverUp.timerInterrupt();
        digitalWrite(13, LOW);

        //dev::hoverDown.timerInterrupt();

        //dev::camera.timerInterrupt();
    });
    Timer1.start();
}

void dev::sonarInit()
{
    sonarForward1.begin();
    sonarForward2.begin();
    sonarBack.begin();
}

void dev::buttonInit() {
    pinMode(PIN_BTN_START, INPUT_PULLUP);
}

bool dev::getButton() {
    return digitalRead(PIN_BTN_START);
}

void dev::waitButton() {
    while (! getButton());
    while (getButton());
}

int dev::getSonarForward() {
    int dist1 = dev::sonarForward1.readAverage();
    int dist2 = dev::sonarForward2.readAverage();

    if ((dist1 == 0 && dist2 != 0) || (dist1 != 0 && dist2 == 0)) {
        return max(dist1, dist2);
    } else if (dist1 == 0) {
        return 0;
    }
    
    
    return (dist1 + dist2) / 2;
}

void dev::waitSonar(Sonar& sonar)
{
    int dist = sonar.readAverage();
    while (dist == 0 || dist > 50);
}

void dev::openCap() // TODO: переписать
{
    for (int i = settings::servoCap::close; i < settings::servoCap::open; i++)
    {
        servoCap.write(i);
        delay(33);
    }
}

void dev::closeCap() // TODO: переписать
{
    for (int i = settings::servoCap::open; i >= settings::servoCap::close; i--)
    {
        servoCap.write(i);
        delay(33);
    }
}

void dev::upMotor()
{
    motorRun(settings::motor::speedOpen, settings::motor::timeOpen);
    motor.set(0);
}

void dev::waitMotorMaxCurr(int max)
{
    while (dev::motor.getCurr() < max);
}

void dev::motorRun(int16_t speed, int16_t time)
{
    while (abs(motor.getEnc()) < time) {
        motor.set(speed);
    }
}

void dev::motorCalibration()
{
    motor.encReset();
    motor.set(settings::motor::speedMotorCalib);
    delay(1000);
    motor.set(-settings::motor::speedMotorCalib);
    delay(250);
    waitMotorMaxCurr(settings::motor::motorMaxCurr);
    motor.set(0);
    motor.encReset();
}

void dev::hoverBoardRotate180() {
    hoverBoardSet(settings::hoverboard::rotate180Speed, 0);
    delay(settings::hoverboard::rotate180Time);
    hoverBoardSet(0, 0);
}

// uint dev::getSonarAverage(uint time) {
//     distsStep++;
//     distsStep %= SIZE_AVERAGE;
//     dists[distsStep] = getSonar();

//     ace_sorting::shellSortKnuth(dists, SIZE_AVERAGE);
//     uint average = dists[SIZE_AVERAGE / 2];

//     delay(time);

//     return average;
// }
// }