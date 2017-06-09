import numpy as np
import matplotlib.pyplot as plt

dt = 0.1
simSensors = [[] for i in np.arange(6)]
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
CToK = 273.15

T_n = np.full((30), 27.5)
def run_sim():
    for i in np.arange(3000/dt):
        if (int(i/(300/dt)) % 2 == 1):
            P = 0
        else:
            P = 9.5
        T_n[0] += delta_temp_first(T_n[0], T_n[1])
        T_n[-1] += delta_temp_last(T_n[-2], T_n[-1])
        for j in range(1, 29):
            T_n[j] += delta_temp_middle(T_n[j-1], T_n[j], T_n[j+1])

        if (i % (1/dt) == 0):
            simSensors[0].append(T_n[1])
            simSensors[1].append((T_n[6]+T_n[7])/2)
            simSensors[2].append(T_n[12])
            simSensors[3].append((T_n[17]+T_n[18])/2)
            simSensors[4].append(T_n[23])
            simSensors[5].append((T_n[28]+T_n[29])/2)

### Common radiation ###
def delta_temp_radiative(t):
    return -(2/(c*roh*a))*dt*(kc*(t-Tamb)+epsilon*sigma*((t+CToK)**4-(Tamb+CToK)**4))

### FIRST NODE ###
def delta_temp_first(t0, t1):
    convective = delta_temp_first_convective(t0)
    conductive = delta_temp_first_conductive(t0, t1)
    power_in = delta_temp_first_power()
    radiative = delta_temp_radiative(t0)
    return convective + conductive + power_in# + radiative
def delta_temp_first_convective(t0):
    return -2*kc*(t0 - Tamb)*dt/(c*roh*a)
def delta_temp_first_conductive(t0, t1):
    return -k*(t0 - t1)*dt/(c*roh*dx**2)
def delta_temp_first_power():
    return P*dt/(c*roh*np.pi*(a**2)*dx)

### MIDDLE NODES ###
def delta_temp_middle(t0, t1, t2):
    conductive = delta_temp_middle_conductive(t0, t1, t2)
    radiative = delta_temp_radiative(t1)
    return conductive + radiative
def delta_temp_middle_conductive(t0, t1, t2):
    return (k/(c*roh))*dt*(t0-2*t1+t2)/(dx**2)

### LAST NODE ###
def delta_temp_last(t99, t100):
    convective = delta_temp_last_convective(t100)
    conductive = delta_temp_last_conductive(t99, t100)
    radiative = delta_temp_radiative(t100)
    return convective + conductive + radiative
def delta_temp_last_convective(t100):
    return -2*kc*(t100 - Tamb)*dt/(c*roh*a)
def delta_temp_last_conductive(t99, t100):
    return k*(t99 - t100)*dt/(c*roh*dx**2)


'''
sensors = [[] for i in np.arange(6)]

import csv
with open('june5_1-41.csv', 'r') as file:
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
    plot(t, [j*100/7.67 for j in sensors[i]])
'''

def main():
    run_sim()
    plt.figure()
    t = range(1, 3001)
    for i in range(6):
        plt.scatter(t, simSensors[i], c = colors[i], marker = 'x', linewidths = 0.01)
    plt.xlim([0, 3000])
    plt.ylim([0, 100])
    plt.title('Temperature vs. Time')
    plt.show()


if __name__ == "__main__":
    main()
