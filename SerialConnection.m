% Copyright 2014 The MathWorks, Inc.
clear all;
clear;
%% Create serial object for Arduino
s = serial('COM5'); % change the COM Port number as needed
set(s, 'BaudRate', 9600);
set(s, 'DataBits', 8);
set(s, 'StopBits', 1);
set(s, 'Parity', 'none');
%% Connect the serial port to Arduino
%s.InputBufferSize = 1; % read only one byte every time
while(true)
try
    fopen(s);
    break;
catch err
    fclose(instrfind);
end
end
a = 'a';
while(a~='d')
    a = 'a';
slave = input('Which slave do you want to control (2-3): ');
c1 = input('First coordinate: ');
c2 = input('Second coordinate: ');
c3 = input('Third coordinate: ');
fprintf(s, '%i', slave);
fwrite(s, 's');
fprintf(s, '%1.2f', c1*1.00);
fwrite(s, 's');
fprintf(s, '%1.2f', c2*1.00);
fwrite(s, 's');
fprintf(s, '%1.2f', c3*1.00);
while(a(1, 1)~='d' && a(1, 1)~='c')
    while(get(s, 'BytesAvailable')==0)
    end
   %a=fread(s, get(s, 'BytesAvailable'));
   a=fread(s, 1);
   disp(a);
end
end
disp('program completed');
%fscanf(s, '%u');
fclose(s);
clear all;