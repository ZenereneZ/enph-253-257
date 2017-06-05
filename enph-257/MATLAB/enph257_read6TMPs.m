%connects to an arduino and reads all 6 analog inputs, displaying it and
%writing it to a file
clear a;
a = arduino();  %port number hard-coded to Yuni's laptop

dateTime = datestr(datetime('now'));
dateTime = strrep(dateTime,':','-');
dateTime = strrep(dateTime,' ','_');
fileName = [pwd '/output_files/' dateTime '.csv']; %use this for windows
%fileName = [pwd '\output_files\' dateTime '.csv']; %use this mac

vals = zeros(1,6);    %array of values to append to file

while true
    tic;
    for i = 1:6                             %iterate through each analog pin
        pin = strcat('A', int2str(i - 1));    %name of the pin to read from
        vals(i) = readVoltage(a, pin);
    end
    disp(vals); %write value to command line output
    dlmwrite(fileName, vals, '-append'); %append to csv file
    
    %if less than 1 second has elapsed, wait 1 second
    %if more than 1 second has elapsed, just move on
    if (toc < 1)
        pause(1-toc);
    end
end