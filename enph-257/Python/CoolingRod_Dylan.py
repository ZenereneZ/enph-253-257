import numpy as np
import matplotlib.pyplot as plt
import csv

### STUFF WE CAN CHANGE ###
epsilon = 0.3 # emissivity (0-1)
kc = 16 # conduction constant
efficiency = 0.9 # efficiency of power (0-1)
c = 900 # specific heat capacity
Tamb = 24 # ambient temperature (C)
k = 220 # thermal conductivity (W/(m*k))

### DO NOT CHANGE ###
dt = 0.1 # delta time (s)
L = 0.305 # length of rod (m)
a = 0.0118 # radius (m)
rho = 2700 # density (kg/m^3)
dx = 0.01 # delta x (m)
P = 9.5 # max power (W)
sigma = 5.67*10**(-8) # stefan-boltzmann constant (W/(m^2 * k**4))
CToK = 273.15 # conversion from Celcius to Kelvin
seconds_of_simulation = 3000 # 50 minutes
colors = ['b','g','r','c','m','y']

### DEPENDANT VARIABLES ###
numSteps = seconds_of_simulation/dt
#simSensors = np.empty((seconds_of_simulation, 6), dtype = np.float16)
simSensors = [[] for i in np.arange(6)]
T_n = np.full((30), Tamb, dtype=np.float32)

def run_sim():
    global P
    power = P * efficiency
    for i in np.arange(numSteps):
        if (int((i)/(numSteps *dt)) % 2 == 1):
            power = 0
        else:
            power = P * efficiency
        T_n[0] += delta_temp_first(T_n[0], T_n[1], power)
        T_n[-1] += delta_temp_last(T_n[-2], T_n[-1])
        for j in range(1, 29):
            T_n[j] += delta_temp_middle(T_n[j-1], T_n[j], T_n[j+1])

        if (i % (1/dt) == 0):
            '''
            idt = int(i * dt)
            simSensors[idt, 0] = (T_n[1] + T_n[2])/2
            simSensors[idt, 1] = (T_n[7])
            simSensors[idt, 2] = (T_n[12] + T_n[13])/2
            simSensors[idt, 3] = (T_n[18])
            simSensors[idt, 4] = (T_n[23] + T_n[24])/2
            simSensors[idt, 5] = (T_n[29])
            '''
            simSensors[0].append((T_n[1] + T_n[2])/2)
            simSensors[1].append(T_n[7])
            simSensors[2].append((T_n[12] + T_n[13])/2)
            simSensors[3].append(T_n[18])
            simSensors[4].append((T_n[23] + T_n[24])/2)
            simSensors[5].append(T_n[29])


### Common radiation for all nodes ###
def delta_temp_radiative(t):
    return -(2/(c*rho*a))*dt*(kc*(t-Tamb)+epsilon*sigma*((t+CToK)**4-(Tamb+CToK)**4))

### FIRST NODE ###
def delta_temp_first(t0, t1, power):
    convective = delta_temp_first_convective(t0)
    conductive = delta_temp_first_conductive(t0, t1)
    power_in = delta_temp_first_power(power)
    radiative = delta_temp_radiative(t0)
    return convective + conductive + power_in + radiative
def delta_temp_first_convective(t0):
    return -2*kc*(t0 - Tamb)*dt/(c*rho*a)
def delta_temp_first_conductive(t0, t1):
    return -k*(t0 - t1)*dt/(c*rho*dx**2)
def delta_temp_first_power(power):
    return power*dt/(c*rho*np.pi*(a**2)*dx)

### MIDDLE NODES ###
def delta_temp_middle(t0, t1, t2):
    conductive = delta_temp_middle_conductive(t0, t1, t2)
    radiative = delta_temp_radiative(t1)
    return conductive + radiative
def delta_temp_middle_conductive(t0, t1, t2):
    return (k/(c*rho))*dt*(t0-2*t1+t2)/(dx**2)

### LAST NODE ###
def delta_temp_last(t99, t100):
    convective = delta_temp_last_convective(t100)
    conductive = delta_temp_last_conductive(t99, t100)
    radiative = delta_temp_radiative(t100)
    return convective + conductive + radiative
def delta_temp_last_convective(t100):
    return -2*kc*(t100 - Tamb)*dt/(c*rho*a)
def delta_temp_last_conductive(t99, t100):
    return k*(t99 - t100)*dt/(c*rho*dx**2)

def main():
    run_sim()
    plt.figure()
    t = range(1, seconds_of_simulation + 1)
    for i in range(6):
        #plt.plot(t, simSensors[:, i])
        plt.plot(t, simSensors[i])
    plt.xlim([0, seconds_of_simulation])
    plt.ylim([20, 50])
    plt.xlabel("Time (s)")
    plt.ylabel("Temperature (C)")

    actual_sensors = [[] for i in range(6)]
    with open('../MATLAB/output_files/june5_1-41.csv', 'r') as file:
        dataReader = csv.reader(file, delimiter = ',')
        #stores data in a list of 6 lists(one for each sensor)
        for row in dataReader:
            for i in range(6):
                actual_sensors[i].append(float(row[i]))



    actual_sensors[0] = np.add(np.multiply(actual_sensors[0], 12.99), 0.1225)
    actual_sensors[1] = np.add(np.multiply(actual_sensors[1], 12.434), 1.974)
    actual_sensors[2] = np.add(np.multiply(actual_sensors[2], 13), 0.3263)
    actual_sensors[3] = np.add(np.multiply(actual_sensors[3], 12.02), 1.981)
    actual_sensors[4] = np.add(np.multiply(actual_sensors[4], 12.62), 1.1282)
    actual_sensors[5] = np.add(np.multiply(actual_sensors[5], 11.95), 2.5395)

    for i in range(6):
        factor = 50
        cumsum_vec = np.cumsum(np.insert(actual_sensors[i], 0, 0))
        ma_vec = (cumsum_vec[factor:] - cumsum_vec[:-factor]) / factor
        moving_average = ma_vec[::factor]
        t = np.arange(1, len(moving_average) + 1)
        t = np.multiply(t, factor)
        plt.scatter(t, moving_average,c=colors[i], marker = '.', linewidths = 0.1)

    plt.show()


if __name__ == "__main__":
    main()
