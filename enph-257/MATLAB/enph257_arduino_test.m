clear a;
a = arduino('/dev/cu.usbmodem1411','uno'); %port location hard coded to Yuni's laptop

for i = 1:10
    writeDigitalPin(a, 'D13', 0);   %D13 is connected to the on-board LED
    pause(2);
    writeDigitalPin(a, 'D13', 1);
    pause(2);
end