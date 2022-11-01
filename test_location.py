import math
import matplotlib.pyplot as plt
import numpy as np
scan = np.loadtxt('./laserscan.dat')
angle = np.linspace(-math.pi/2, math.pi/2, np.shape(scan)[0], endpoint=True)
x = [0]*361
y = [0]*361
i = 0
print(len(angle))
for dist in scan:
    x[i] = dist * math.cos(angle[i])
    y[i] = dist * math.sin(angle[i])
    i = i + 1
plt.plot(x, y, 'or', alpha=0.7, label="first", lw=5, mec='b', mew=2, ms=10)
plt.show()
