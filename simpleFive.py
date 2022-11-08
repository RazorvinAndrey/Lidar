from math import floor
from rplidar import RPLidar


# Setup the RPLidar
PORT_NAME = 'COM9'
lidar = RPLidar(PORT_NAME, timeout=3, baudrate=256000)

# used to scale data to fit on the screen
max_distance = 0

def process_data(data):
    print(data)

scan_data = [None]*360

try:
#    print(lidar.get_info())
    for scan in lidar.iter_scans(max_buf_meas=500):
        scan_data = [None]*360
        for (_, angle, distance) in scan:
            scan_data[floor(angle)] = distance
        process_data(scan_data)

except KeyboardInterrupt:
    print('Stopping.')

lidar.stop()
lidar.stop_motor()
lidar.disconnect()
