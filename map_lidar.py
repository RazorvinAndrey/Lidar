from math import cos, sin, pi, floor
from rplidar import RPLidar
import pygame

pygame.init()
pygame.init()
lcd = pygame.display.set_mode((320,240))
pygame.mouse.set_visible(False)
lcd.fill((0,0,0))
pygame.display.update()


# Setup the RPLidar
PORT_NAME = 'COM9'
lidar = RPLidar(PORT_NAME, timeout=3)

# used to scale data to fit on the screen
max_distance = 0

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
            point = (160 + int(x / max_distance * 119), 120 + int(y / max_distance * 119))
            lcd.set_at(point, pygame.Color(255, 255, 255))
    pygame.display.update()

scan_data = [None]*360

try:
#    print(lidar.get_info())
    for scan in lidar.iter_scans():
        scan_data = [None]*360
        for (_, angle, distance) in scan:
            scan_data[floor(angle)] = distance
        process_data(scan_data)

except KeyboardInterrupt:
    print('Stopping.')

lidar.stop()
lidar.stop_motor()
lidar.disconnect()
