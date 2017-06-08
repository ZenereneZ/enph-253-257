# -*- coding: utf-8 -*-
"""
Created on Fri Jun  2 13:16:43 2017

@author: forsu
"""

sensors = [[] for i in arange(6)]

import csv
with open('07-Jun-2017_14-11-54.csv', 'r') as file:
    dataReader = csv.reader(file, delimiter = ',')
    #stores data in a list of 6 lists(one for each sensor)
    for row in dataReader:
        j = 0
        for i in arange(6):
            sensors[j].append(float(row[i]))
            j += 1
file.close()

t = arange(1, len(sensors[0]) + 1)

subplot(2,1,1)
for i in arange(6):
    plot(t, sensors[i])
plt.xlim([0, 3000])
title('Raw Voltage vs. Time')

subplot(2,1,2)
for i in arange(6):
    plot(t, [j*100/7.67 for j in sensors[i]])
plt.xlim([0, 3000])
title('Temperature vs. Time')

