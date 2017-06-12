
# -*- coding: utf-8 -*-
"""
Created on Thu May 25 10:53:13 2017
@author: forsu
"""
import matplotlib.pyplot as plt
from numpy import *
dt = 0.1
simSensors = [[] for i in arange(6)]
k = 230
L = 0.3
a = 0.01
c = 900
roh = 2700
dx = 0.01
P = 9.5
kc = 11
Tamb = 20
sigma = 5.67*10**(-8)
epsilon = 1
colors = ['y','m','c','r','g','b']

x_left = linspace(0, 2.9, 30)
x_n = linspace(0.005, 0.295, 30)
T_n = linspace(0, 2.9, 30)

for i in arange(30):
    T_n[i] = 27.5

for i in arange(3000/dt):
    if (int(i/(300/dt)) % 2 == 1):
        P = 0
    else:
        P = 9.5

    dT_nfirst = -k*(T_n[0] - T_n[1])*dt/(c*roh*dx**2)
    dT_nlast = k*(T_n[-2] - T_n[-1])*dt/(c*roh*dx**2)
    dT_convectivefirst = -2*kc*(T_n[0] - Tamb)*dt/(c*roh*a)
    dT_convectivelast = -2*kc*(T_n[-1] - Tamb)*dt/(c*roh*a)

    T_n[0] += (dT_nfirst + dT_convectivefirst + P*dt/(c*roh*pi*(a**2)*dx))
    T_n[-1] += (dT_nlast + dT_convectivelast)

    for j in arange(1, 29):
        dT_n = (k/(c*roh))*dt*(T_n[j-1]-2*T_n[j]+T_n[j+1])/(dx**2)
        dT_radiative = - (2/(c*roh*a))*dt*(kc*(T_n[j]-Tamb)+epsilon*sigma*((T_n[j]+273.15)**4-(Tamb+273.15)**4))
        T_n[j] += (dT_n + dT_radiative)

    if (i % (1/dt) == 0):
        simSensors[0].append(T_n[1])
        simSensors[1].append((T_n[6]+T_n[7])/2)
        simSensors[2].append(T_n[12])
        simSensors[3].append((T_n[17]+T_n[18])/2)
        simSensors[4].append(T_n[23])
        simSensors[5].append((T_n[28]+T_n[29])/2)

plt.figure()
t = arange(1, 3001)
for i in arange(6):
    plt.scatter(t, simSensors[i], c = colors[i], marker = 'x', linewidths = 0.01)
    plt.xlim([0, 3000])

sensors = [[] for i in arange(6)]

import csv
with open('../MATLAB/output_files/june5_1-41.csv', 'r') as file:
    dataReader = csv.reader(file, delimiter = ',')
    #stores data in a list of 6 lists(one for each sensor)
    for row in dataReader:
        j = 0
        for i in arange(6):
            sensors[j].append(float(row[i]))
            j += 1
file.close()

t = arange(1, len(sensors[0]) + 1)

for i in arange(6):
    plt.plot(t, [j*100/7.67 for j in sensors[i]])
plt.xlim([0, 3000])
plt.title('Temperature vs. Time')
plt.show()
