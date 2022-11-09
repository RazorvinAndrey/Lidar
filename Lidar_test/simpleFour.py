from math import floor
from rplidar import RPLidar
import time
import serial
import numpy as np


# Setup the RPLidar
PORT_NAME = 'COM9'
lidar = RPLidar(PORT_NAME, timeout=3)

# used to scale data to fit on the screen
max_distance = 0
arduino=serial.Serial('COM1', 9600)
time.sleep(2)

def process_data(data):
    print(data)

scan_data = [None]*360

try:
#    print(lidar.get_info())
    for scan in lidar.iter_scans():
        scan_data = [None]*360
        for (_, angle, distance) in scan:
            scan_data[floor(angle)] = distance
        process_data(scan_data)
        #    *обработка этих данных  и написание программы по которой с помощью этих данных мы определяем с какой скоростью
        # и как надо ехать нашей машинке итог которой составляет массив с типо данных byte*
        tri_iznerenya = np.array([scan_data[0], scan_data[90], scan_data[180]], dtype=np.int8)
        arduino.write(tri_iznerenya)

except KeyboardInterrupt:
    print('Stopping.')

lidar.stop()
lidar.stop_motor()
lidar.disconnect()
