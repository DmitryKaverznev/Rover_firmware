import sensor
import math
import json
from pyb import UART
import time
from machine import LED

uart = UART(3, 115200)
led = LED("LED_GREEN")

last_blink_time = time.ticks_ms()
led_state = False

def line_length(line):
    x1, y1, x2, y2 = line
    return math.sqrt((x2 - x1)**2 + (y2 - y1)**2)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)

def getLineDots():
    blobs = img.find_blobs([(255, 255)], pixels_threshold=40, area_threshold=40, merge=True)

    img.to_rgb565()
    if blobs:
        blobs.sort(key=lambda b: line_length(b.major_axis_line()), reverse=True)

        largest_blob = blobs[0]

        major_axis_line = largest_blob.major_axis_line()

        img.draw_rectangle(largest_blob.rect(),[0, 255, 0])
        img.draw_cross(largest_blob.cx(), largest_blob.cy(), [128, 0, 128], 5, -1)


        if major_axis_line:
            img.draw_line(major_axis_line, [0, 0, 128])
            img.draw_cross(major_axis_line[0], major_axis_line[1], [120, 50, 128], 3, -1)
            img.draw_cross(major_axis_line[2], major_axis_line[3], [120, 50, 128], 3, -1)


        for i in range(1, len(blobs)):
            blob = blobs[i]
            img.draw_rectangle(blob.rect(), [128, 0, 0])

        return major_axis_line

def toJson(data):
    if data is None:
        return -1
    jsonData = {
        "data": data
    }
    return json.dumps(jsonData)


while(True):
    img = sensor.snapshot()
    img = img.rotation_corr(vflip=True)



    for tag in img.find_apriltags():
        img.draw_rectangle(tag.rect, color=(255, 0, 0))
        img.draw_cross(tag.cx, tag.cy, color=(0, 255, 0))
        jsonData = toJson(tag.id)
        uart.write(jsonData + '\n')

    if time.ticks_diff(time.ticks_ms(), last_blink_time) > 250:
        last_blink_time = time.ticks_ms()
        led_state = not led_state
        if led_state:
            led.on()
        else:
            led.off()
