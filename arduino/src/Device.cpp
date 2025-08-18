#include "Device.h"

#include <Arduino.h>
#include <DueTimer.h>

HoverboardController dev::hoverUp(UART_HOVER_UP);
HoverboardController dev::hoverDown(UART_HOVER_DOWN);

Camera dev::camera(UART_CAMERA);

Sonar dev::sonarForward1(PIN_SONAR_TRIK_F1, PIN_SONAR_ECHO_F1);
Sonar dev::sonarForward2(PIN_SONAR_TRIK_F2, PIN_SONAR_ECHO_F2);
Sonar dev::sonarBack(PIN_SONAR_TRIK_BACK, PIN_SONAR_ECHO_BACK);

Motor dev::motor(PIN_MOTOR_PWM,
                 PIN_MOTOR_DIR_1,
                 PIN_MOTOR_DIR_2,
                 PIN_MOTOR_ENC_DIR,
                 PIN_MOTOR_ENC_INT,
                 PIN_MOTOR_CURR);

Servo dev::servoCap;

void timerInterrupt() {
    dev::hoverUp.timerInterrupt();
    dev::hoverDown.timerInterrupt();

    dev::camera.timerInterrupt();
}

void encoderInterrupt() {
    dev::motor.encoderInterrupt();
}

void dev::hoverBoardSet(int16_t steer, int16_t speed) {
    dev::hoverUp.set(steer, speed);
    dev::hoverDown.set(steer, speed);
}

void dev::hoverBoardSetSoft(int16_t steer, int16_t time, int16_t speedEnd) {
    dev::hoverUp.setSoft(time, steer, speedEnd);
    dev::hoverDown.setSoft(time, steer, speedEnd);
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
    servoCap.attach(PIN_SERVOCAP);
}

void dev::motorInit()
{
    motor.init();
    
    attachInterrupt(digitalPinToInterrupt(PIN_MOTOR_ENC_INT), encoderInterrupt, RISING);
}

void dev::timersInit() {
    Timer1.attachInterrupt(timerInterrupt);
    Timer1.start(TIME_SEND);
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

uint dev::getSonarForward() {
    uint dist1 = dev::sonarForward1.readAverage();
    uint dist2 = dev::sonarForward2.readAverage();

    if ((dist1 == 0 && dist2 != 0) || (dist1 != 0 && dist2 == 0)) {
        return max(dist1, dist2);
    } else if (dist1 == 0 || dist2 == 0) {
        return 0;
    }
    
    
    return (dist1 + dist2) / 2;
}

void dev::waitSonar(Sonar& sonar)
{
    uint dist = sonar.readAverage();
    while (dist == 0 || dist > 50);
}

void dev::openCap()
{
    for (int i = SERVOCAP_CLOSE; i < SERVOCAP_OPEN; i++)
    {
        servoCap.write(i);
        delay(33);
    }
}

void dev::closeCap()
{
    for (int i = SERVOCAP_OPEN; i >= SERVOCAP_CLOSE; i--)
    {
        servoCap.write(i);
        delay(33);
    }
}

void dev::upMotor()
{
    motorRun(SPEED_MOTOR_OPEN, TIME_MOTOR_OPEN);
    motor.set(0);
}

void dev::waitMotorMaxCurr(uint max)
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
    motor.set(SPEED_MOTOR_CALIB);
    delay(1000);
    motor.set(-SPEED_MOTOR_CALIB);
    delay(250);
    waitMotorMaxCurr(MOTOR_MAX_CURR);
    motor.set(0);
    motor.encReset();
}

void dev::hoverBoardRotate180() {
    hoverBoardSet(HOVERBOARD_ROTATE_180_SPEED, 0);
    delay(HOVERBOARD_ROTATE_180_TIME);
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