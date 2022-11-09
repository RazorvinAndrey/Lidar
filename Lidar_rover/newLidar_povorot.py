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
        for i in range (165, 195):  # считает среднее растояние справа в диапазоне 30 градусов
            if scale_X[i] != None:
                sumR += scale_X[i]
                k += 1
        if k == 0:
            k = 1
        distR = sumR / k

        sumT = 0
        n = 0
        for j in range(80, 100):  # этот и следущий цикл считают расстояние впереди в диапазоне от  80 до 100
            if  scale_Y[j] != None:
                sumT += scale_Y[j]
                n += 1

        if n == 0:
            n=1
        distT = sumT / n

        if distT >= 1000: #проверяет может ли ехать прямо
            if distR > 600 and distR < 800: #если может ехать прямо, смотрит по расстянию справа и едет просто прямо
                if scan_data[160] == None:
                    scan_data[160] = 15000
                if scan_data[157] == None:
                    scan_data[157] = 15000
                if scan_data[210] == None:
                    scan_data[210] = 15000
                if scan_data[213] == None:
                    scan_data[213] = 15000
                if (scan_data[160] < scan_data[157]) and (scan_data[210] < scan_data[213]):
                    sum_proverki_1 = 0
                    s = 0
                    for i in range(155, 160):  # считает среднее растояние справа в диапазоне пяти градусов
                        if scan_data[i] != None:
                            sum_proverki_1 += scan_data[i]
                            s += 1
                    if s == 0:
                        s = 1
                    dist_proverka_1 = sum_proverki_1 / s
                    sum_proverki_2 = 0
                    s = 0
                    for i in range(155, 160):  # считает среднее растояние справа в диапазоне пяти градусов
                        if scan_data[i] != None:
                            sum_proverki_2 += scan_data[i]
                            s += 1
                    if s == 0:
                        s = 1
                    dist_proverka_2 = sum_proverki_2 / s

                    if dist_proverka_2 > (dist_proverka_1 - 30):
                        # поворот налево
                        method = 6
                        speed = 100

                    elif dist_proverka_1 > (dist_proverka_2 + 30):
                        # поворот направо
                        method = 5
                        speed = 100
                    else:
                        # прямо едет
                        method = 1
                        speed = 200
                else:
                    method = 1
                    speed = 200
            elif distR > 800: #едет направо
                method = 4
                speed = 200
            else: #едет налево
                method = 3
                speed = 200
        else:#остановка потому что спереди расстояние меньше метра
            method = 6
            speed = 150

        msg = {
            "method": method,  # в пакете посылается скорость на левый и правый борт тележки
            "speed": speed #
        }

        serialPort.write(json.dumps(msg, ensure_ascii=False).encode("utf8"))  # отправляем пакет в виде json файла
        time.sleep(1 / 10)

except KeyboardInterrupt:
    print('Stopping.')

lidar.stop()
lidar.stop_motor()
lidar.disconnect()
