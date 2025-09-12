#include "UARTStream.h"

UARTStream::UARTStream(HardwareSerial& serial, unsigned long bitrate) : serial(serial) {
    this->serial.begin(bitrate);
}

bool UARTStream::waitForStartByte(unsigned long timeout) {
    unsigned long startTime = millis();

    while (millis() - startTime < timeout) {
        if (serial.available() > 0) {
            uint8_t receivedByte = serial.read();
            if (receivedByte == BYTE_START) {
                return true;
            }
        }
        delay(1);
    }
    return false;
}

// Прием 16-битного числа (старший байт первый)
int16_t UARTStream::receiveInt16() {
    uint8_t highByte = serial.read();
    while (serial.available() == 0); // Ждем второй байт
    uint8_t lowByte = serial.read();

    return (int16_t)((highByte << 8) | lowByte);
}

// Основная функция приема данных
bool UARTStream::receiveData(int16_t* data, uint8_t dataCount) {
    // Ждем стартовый байт
    if (!waitForStartByte(1000)) {
        return false;
    }

    // Проверяем, достаточно ли данных в буфере
    if (serial.available() < (dataCount * 2)) {
        unsigned long startTime = millis();
        while (serial.available() < (dataCount * 2) && (millis() - startTime < 1000)) {
            delay(1);
        }

        if (serial.available() < (dataCount * 2)) {
            return false;
        }
    }

    // Читаем данные
    for (uint8_t i = 0; i < dataCount; i++) {
        data[i] = receiveInt16();
    }

    return true;
}