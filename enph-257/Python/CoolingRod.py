# -*- coding: utf-8 -*-
"""
Created on Thu May 25 10:53:13 2017

@author: forsu
"""

simSensors = [[] for i in arange(6)]
k = 205
L = 0.3
a = 0.01
c = 900
roh = 2700
dx = 0.01 
dt = 0.1
P = 10
kc = 5 
Tamb = 20
sigma = 5.67*10**(-8)
epsilon = 1

x_left = linspace(0, 2.9, 30)
x_n = linspace(0.005, 0.295, 30)
T_n = linspace(0, 2.9, 30)

for i in arange(30):
    T_n[i] = 20

for i in arange(66380):
    if (i > 40000):
        P = 0
    dT_nfirst = -k*(T_n[0] - T_n[1])*dt/(c*roh*dx**2)
    dT_nlast = k*(T_n[-2] - T_n[-1])*dt/(c*roh*dx**2)
    dT_convectivefirst = -2*kc*(T_n[0] - Tamb)*dt/(c*roh*a)
    dT_convectivelast = -2*kc*(T_n[-1] - Tamb)*dt/(c*roh*a)
    T_n[0] += (dT_nfirst + dT_convectivefirst + P*dt/(c*roh*pi*(a**2)*dx))
    T_n[-1] += (dT_nlast + dT_convectivelast)
    
    for j in linspace(1, 28, 28):
        dT_n = (k/(c*roh))*dt*(T_n[j-1]-2*T_n[j]+T_n[j+1])/(dx**2)
        dT_radiative = - (2/(c*roh*a))*dt*(kc*(T_n[j]-Tamb)+epsilon*sigma*((T_n[j]+273.15)**4-(Tamb+273.15)**4))
        T_n[j] += (dT_n + dT_radiative)
    if (i % 10 == 0):
        simSensors[0].append(T_n[2])
        simSensors[1].append(T_n[7])
        simSensors[2].append(T_n[12])
        simSensors[3].append(T_n[17])
        simSensors[4].append(T_n[22])
        simSensors[5].append(T_n[27])
    
    """
    plot(x_n, T_n)
    plt.ylim([20, 60])
    pause(0.001)
    clf()
    """
    """
    #print("Mean Temp: ", sum(T_n)/len(T_n))
    if (i == 69999):
        plot(x_n, T_n)
        plt.ylim([20, 60])
        xlabel("Position (m)")
        ylabel("Temperature (C)")
        title("Steady State Temperature vs. Position Graph")
     """   
figure()
for i in arange(6):
plot(t, simSensors[i])