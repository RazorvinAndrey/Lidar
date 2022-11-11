# SPDX-FileCopyrightText: 2019 Dave Astels for Adafruit Industries
#
# SPDX-License-Identifier: MIT

"""
Consume LIDAR measurement file and create an image for display.

Adafruit invests time and resources providing this open source code.
Please support Adafruit and open source hardware by purchasing
products from Adafruit!

Written by Dave Astels for Adafruit Industries
Copyright (c) 2019 Adafruit Industries
Licensed under the MIT license.

All text above must be included in any redistribution.
"""

from math import cos, sin, pi, floor
import pygame
from rplidar import RPLidar
import matplotlib.pyplot as plt

# Set up pygame and the display
pygame.init()
lcd = pygame.display.set_mode((1024,720))
pygame.mouse.set_visible(False)
lcd.fill((0,0,0))
pygame.display.update()

# Setup the RPLidar
PORT_NAME = 'COM9'
lidar = RPLidar(PORT_NAME, timeout=3)

# used to scale data to fit on the screen
max_distance = 0

#pylint: disable=redefined-outer-name,global-statement
def process_data(data):
    global max_distance
    lcd.fill((0,0,0))
    for angle in range(360):
        distance = data[angle]
        if distance > 0:                  # ignore initially ungathered data points
            max_distance = max([min([5000, distance]), max_distance])
            radians = angle * pi / 180.0
            x = distance * cos(radians)
            y = distance * sin(radians)
            point = (360 + int(x / max_distance * 300), 320 + int(y / max_distance * 300))
            lcd.set_at(point, pygame.Color(255, 255, 255))
    pygame.display.update()
def process_data_new(data):
    global max_distance
    plt.scatter(0, 0, label='robot')
    plt.grid()
    plt.legend()
    # plt.xlim([-5, 5])
    # plt.ylim([-7, 2])
    plt.ion()

    for angle in range(360):
        distance = data[angle]
        if distance > 0:                  # ignore initially ungathered data points
            radians = angle * pi / 180.0
            x = distance * cos(radians)
            y = distance * sin(radians)
            plt.plot([x, y], linewidth=1, c='blue')
            plt.xlabel('X, m')
            plt.ylabel('Y, m')
            plt.draw()

    plt.ioff()
    plt.show()


scan_data = [0]*360

try:
    for scan in lidar.iter_scans():
        scan_data = [0] * 360
        for (_, angle, distance) in scan:
            scan_data[min([359, floor(angle)])] = distance
        process_data_new(scan_data)

except KeyboardInterrupt:
    print('Stoping.')
lidar.stop()
lidar.disconnect()
