import numpy as np
import matplotlib.pyplot as plt
import math

scan = np.loadtxt('laserscan.dat')
angle = np.linspace(-math.pi/2, math.pi/2, np.shape(scan)[0], endpoint=True)





def trans(x,y,theta):
    T = np.array([[np.cos(theta), -np.sin(theta), x],
                  [np.sin(theta), np.cos(theta),  y],
                  [0,                 0,          1]])
    return T



lidar_gsc = np.dot(trans(1,0.5,np.pi/4),np.array([0.2,0,1]))


x_ssc = scan*np.cos(angle)
y_ssc = scan*np.sin(angle)

obs_lsk = []
for i in range(x_ssc.shape[0]):
    obs_lsk_temp = np.dot(trans(0.2,0,np.pi),np.array([x_ssc[i],y_ssc[i],1]))
    obs_lsk.append(obs_lsk_temp)

obs_lsk = np.array(obs_lsk).T

obs = []
for i in range(obs_lsk.shape[1]):
    obs_temp = np.dot(trans(1, 0.5, np.pi/4), np.array([obs_lsk[0,i], obs_lsk[1,i], 1]))
    obs.append(obs_temp)

obs = np.array(obs).T

# plt.figure(1)
# plt.subplot(2,1,1)
# plt.plot(x_ssc,y_ssc,'r o',linewidth=2)
# plt.grid()
# plt.xlabel('x_lsc, m')
# plt.ylabel('y_lsc, m')
# plt.gca().set_aspect('equal', adjustable='box')
# plt.subplot(2,1,2)
# plt.plot(angle,scan,'b',linewidth=2)
# plt.grid()
# plt.xlabel('angles, rad')
# plt.ylabel('ranges, m')
# # plt.gca().set_aspect('equal', adjustable='box')
# plt.show()
#
#
#
#
# plt.figure(2)
# plt.scatter(1,0.5,label='robot')
# plt.scatter(lidar_gsc[0],lidar_gsc[1],label='lidar')
# plt.scatter(obs[0],obs[1],label='observations')
# plt.grid()
# plt.xlabel('x_gsc, m')
# plt.ylabel('y_gsc, m')
# plt.legend()
# plt.gca().set_aspect('equal', adjustable='box')
# plt.show()



plt.figure(2)
plt.scatter(1,0.5,label='robot')
plt.scatter(lidar_gsc[0],lidar_gsc[1],label='lidar')
plt.grid()
plt.legend()
# plt.gca().set_aspect('equal', adjustable='box')
plt.xlim([-5,5])
plt.ylim([-7,2])
plt.ion()
for i in range(obs.shape[1]):
    plt.scatter(obs[0,i],obs[1,i],s=10,c='green')
    plt.plot([lidar_gsc[0],obs[0,i]],[lidar_gsc[1],obs[1,i]],linewidth=1,c='yellow')
    plt.xlabel('x_gsc, m')
    plt.ylabel('y_gsc, m')
    plt.draw()
    # plt.pause(0.0005)

plt.ioff()
plt.show()

