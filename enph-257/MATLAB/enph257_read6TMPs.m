%connects to an arduino and reads all 6 analog inputs, displaying it and
%writing it to a file
clear a;
a = arduino('/dev/cu.usbmodem1411','uno');  %port number hard-coded to Yuni's laptop
fileName = [pwd '/output_files/' datestr(datetime('now')) '.csv']; %name of file to write sensor data to


vals = zeros(1,6);    %array of values to append to file
offset = [0.25347 0.32677 0.24857 0.24367 0.18997 0.24367];

while true
    tic;
    for i = 1:6                             %iterate through each analog pin
        pin = strcat('A', int2str(i - 1));    %name of the pin to read from
        vals(i) = readVoltage(a, pin) - offset(i);
    end
    disp(vals); %write value to command line output
    dlmwrite(fileName, vals, '-append'); %append to csv file
    
    %if less than 1 second has elapsed, wait 1 second
    %if more than 1 second has elapsed, just move on
    if (toc < 1)
        pause(1-toc);
    end
end