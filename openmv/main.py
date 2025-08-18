import sensor
import pyb
import time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()


uart = pyb.UART(3, 115200)
uart.init(115200, bits=8)

BITE_START = 0xAB
DELAY = 100

red_led = pyb.LED(1)
green_led = pyb.LED(2)

class NumSendByte():
    def __init__(self, num):
        self.num = num

    def getNum(self):
        return self.num

    def getByte(self):
        high_byte = (self.num >> 8) & 0xFF
        low_byte = self.num & 0xFF
        return bytearray([high_byte, low_byte])

class NumByteSender:
    def __init__(self, array: NumSendByte):

