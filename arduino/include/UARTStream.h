#ifndef UARTSTREAM_H
#define UARTSTREAM_H

#include <Arduino.h>

class UARTStream : public Stream {
private:
    HardwareSerial& serial;
    static const uint8_t BYTE_START = 0xAB;

public:
    UARTStream(HardwareSerial& serial, unsigned long bitrate);

    // Функции для приема данных
    bool receiveData(int16_t* data, uint8_t dataCount);
    bool waitForStartByte(unsigned long timeout = 1000);
    int16_t receiveInt16();
};

#endif //UARTSTREAM_H