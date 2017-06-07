M = csvread('output_files/june5_1-41.csv');
time = 1:length(M);
title('Raw Voltage vs. Time')
xlabel('Time')
ylabel('Voltage')
plot(time, M);
figure;
title('Temperature vs. Time')
xlabel('Time')
ylabel('Temperature')
plot(time, M * 100/7.67);