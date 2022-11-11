import math
from math import floor
from rplidar import RPLidar
import serial
import time
import json

# Setup the RPLidar
PORT_NAME = 'COM9'
lidar = RPLidar(PORT_NAME, timeout=3)

scan_data = [None]*361
scale_Y = [None]*361
scale_X = [None]*361
try:
    serialPort = serial.Serial('COM10', 115200)
    for scan in lidar.iter_scans(max_buf_meas=500):
        scan_data = [None]*361
        scale_Y = [None]*361
        scale_X = [None]*361
        for (_, angle, distance) in scan:
            scan_data[floor(angle)] = distance
            scale_X[floor(angle)] = distance * math.fabs(math.cos(floor(angle)*math.pi/180))
            scale_Y[floor(angle)] = distance * math.fabs(math.sin(floor(angle)*math.pi/180))

        sumR = 0
        k = 0
        for i in range (165, 195):  # считает среднее растояние справа в диапазоне пяти градусов
            if scale_X[i] != None:
                sumR += scale_X[i]
                k += 1
        if k == 0:
            k = 1
        distR = sumR / k
        print("distR   ", distR)
        sumT = 0
        n = 0
        for j in range(80, 100):  # этот и следущий цикл считают расстояние впереди в диапазоне от  358 до 2
            if  scale_Y[j] != None:
                sumT += scale_Y[j]
                n += 1

        if n == 0:
            n=1
        distT = sumT / n
        print("distT   ", distT)
        if distT >= 1000: #проверяет может ли ехать прямо
            if distR > 1000 and distR < 1500: #если может ехать прямо, смотрит по расстянию справа и едет просто прямо
                speedA = 150
                speedB = 150
            elif distR > 1500: #едет направо
                speedA = 200
                speedB = -200
            else: #едет налево
                speedA = -200
                speedB = 200
        else:#остановка потому что спереди расстояние меньше метра
            speedA = 0
            speedB = 0

        msg = {
            "speedA": speedA,  # в пакете посылается скорость на левый и правый борт тележки
            "speedB": speedB #
        }

        serialPort.write(json.dumps(msg, ensure_ascii=False).encode("utf8"))  # отправляем пакет в виде json файла
        time.sleep(1 / 10)

except KeyboardInterrupt:
    print('Stopping.')

lidar.stop()
lidar.stop_motor()
lidar.disconnect()
