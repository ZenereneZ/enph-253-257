import numpy as np
import matplotlib.pyplot as plt


### STUFF WE CAN CHANGE ###
epsilon = 1 # emissivity (0-1)
kc = 11 # conduction constant
efficiency = 1 # efficiency of power (0-1)


### DO NOT CHANGE ###
dt = 0.1 # delta time (s)
k = 205 # thermal conductivity (W/(m*k))
L = 0.3 # length of rod (m)
a = 0.01 # radius (m)
c = 900 # specific heat capacity
roh = 2700 # density (kg/m^3)
dx = 0.01 # delta x (m)
P = 9.5 # max power (W)
Tamb = 24 # ambient temperature (C)
sigma = 5.67*10**(-8) # stefan-boltzmann constant (W/(m^2 * k**4))
CToK = 273.15 # conversion from Celcius to Kelvin
seconds_of_simulation = 3000 # 50 minutes
colors = ['b','g','r','c','m','y']


### DEPENDANT VARIABLES
numSteps = seconds_of_simulation/dt
simSensors = np.zeros((seconds_of_simulation, 6))
T_n = np.full((30), Tamb, dtype=np.float32)


def run_sim():
    global P
    power = P * efficiency
    for i in np.arange(numSteps):
        if (int(i/(numSteps/10)) % 2 == 1):
            power = 0
        else:
            power = P * efficiency
        T_n[0] += delta_temp_first(T_n[0], T_n[1], power)
        T_n[-1] += delta_temp_last(T_n[-2], T_n[-1])
        for j in range(1, 29):
            T_n[j] += delta_temp_middle(T_n[j-1], T_n[j], T_n[j+1])

        if (i % (1/dt) == 0):
            idt = int(i * dt)
            simSensors[idt, 0] = (T_n[1])
            simSensors[idt, 1] = (T_n[6]+T_n[7])/2
            simSensors[idt, 2] = (T_n[12])
            simSensors[idt, 3] = (T_n[17]+T_n[18])/2
            simSensors[idt, 4] = (T_n[23])

### Common radiation ###
def delta_temp_radiative(t):
    return -(2/(c*roh*a))*dt*(kc*(t-Tamb)+epsilon*sigma*((t+CToK)**4-(Tamb+CToK)**4))

### FIRST NODE ###
def delta_temp_first(t0, t1, power):
    convective = delta_temp_first_convective(t0)
    conductive = delta_temp_first_conductive(t0, t1)
    power_in = delta_temp_first_power(power)
    radiative = delta_temp_radiative(t0)
    return convective + conductive + power_in# + radiative
def delta_temp_first_convective(t0):
    return -2*kc*(t0 - Tamb)*dt/(c*roh*a)
def delta_temp_first_conductive(t0, t1):
    return -k*(t0 - t1)*dt/(c*roh*dx**2)
def delta_temp_first_power(power):
    return power*dt/(c*roh*np.pi*(a**2)*dx)

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


def main():
    run_sim()
    plt.figure()
    t = range(1, seconds_of_simulation + 1)
    for i in range(6):
        plot(t, simSensors[:, i])
    plt.xlim([0, seconds_of_simulation])
    plt.ylim([0, 100])
    plt.title('Temperature vs. Time')
    plt.show()

    sensors = [[] for i in np.arange(6)]
    
    import csv
    with open('C:\\Users\\forsu\\Documents\\ENPH257(Thermo)\\june5_1-41.csv', 'r') as file:
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
        plt.scatter(t, [j*100/7.67 for j in sensors[i]], c = colors[i], marker = 'x', linewidths = 0.5)
    plt.ylim([20, 60])
if __name__ == "__main__":
    main()
