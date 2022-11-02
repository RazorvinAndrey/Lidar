import math
import matplotlib.pyplot as plt
import numpy as np
# получаем данные с датчика
scan = np.loadtxt('./laserscan.dat')
angle = np.linspace(-math.pi/2, math.pi/2, np.shape(scan)[0], endpoint=True)
# переводим данные с датчика из полярной в декартову систему координат
x = scan*np.cos(angle)
y = scan*np.sin(angle)
# положение робота относительно глобальной системы координат
xr = 1
yr = 0.5
tetR = np.pi/4 #угол робота
# положение лидара относительно локальной системы координат робота
xl = 0.2
yl = 0
tetL = np.pi #угол лидара

# функция для перемещения
def transport(x, y, thet):
    Tx = np.array([[np.cos(thet), -np.sin(thet), x],
                  [np.sin(thet), np.cos(thet),  y],
                  [0,                       0,  1]])
    return Tx
# записываем показания датчика в глобальных координатах
lidar_global = np.dot( transport(xr, yr, np.pi/4),np.array([xl, yl, 1]) )

# записываем показания лидара в глобальных координатах с учётом поворота
lidar_thet = []
for i in range(x.shape[0]):
    lidar_thet_buf = np.dot(transport(xl, yl, np.pi), np.array([x[i], y[i], 1]))
    lidar_thet.append(lidar_thet_buf)
# транспонируем матрицу
lidar_thet = np.array(lidar_thet).T

# записываем показания лидара в глобальных координатах с учётом поворота робота и лидара
robot_thet = []
for i in range(lidar_thet.shape[1]):
    robot_thet_buf = np.dot(transport(xr, yr, np.pi/4), np.array([lidar_thet[0, i], lidar_thet[1, i], 1]))
    robot_thet.append(robot_thet_buf)
# транспонируем матрицу
robot_thet = np.array(robot_thet).T
# выводим все точки
plt.scatter(xr, yr, label='robot')
plt.scatter(lidar_global[0], lidar_global[1], label='lidar')
plt.grid()
plt.legend()
plt.xlim([-5, 5])
plt.ylim([-7, 2])
plt.ion()
for i in range(robot_thet.shape[1]):
    plt.scatter(robot_thet[0, i], robot_thet[1, i], s=10, c='green')
    plt.plot([lidar_global[0], robot_thet[0, i]], [lidar_global[1], robot_thet[1, i]], linewidth=1, c='blue')
    plt.xlabel('X, m')
    plt.ylabel('Y, m')
    plt.draw()

plt.ioff()
plt.show()
