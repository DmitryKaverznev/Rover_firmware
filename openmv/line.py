import sensor
import math

def line_length(line):
    x1, y1, x2, y2 = line
    return math.sqrt((x2 - x1)**2 + (y2 - y1)**2)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)

while(True):
    img = sensor.snapshot()
    img = img.rotation_corr(vflip=True)

    img = img.to_grayscale()
    img = img.binary([(0, 50)])
    img.erode(1)
    img.dilate(1)

    blobs = img.find_blobs([(255, 255)], pixels_threshold=40, area_threshold=40, merge=True)

    img.to_rgb565()
    if blobs:
        blobs.sort(key=lambda b: line_length(b.major_axis_line()), reverse=True)

        largest_blob = blobs[0]

        major_axis_line = largest_blob.major_axis_line()

        img.draw_rectangle(largest_blob.rect(), [0, 255, 0])
        img.draw_cross(largest_blob.cx(), largest_blob.cy())
        img.draw_cross(largest_blob.cx(), largest_blob.cy(), [128, 0, 128], 5, -1)


        if major_axis_line:
            img.draw_line(major_axis_line, [0, 0, 128])

        for i in range(1, len(blobs)):
            blob = blobs[i]
            img.draw_rectangle(blob.rect(), [128, 0, 0])
