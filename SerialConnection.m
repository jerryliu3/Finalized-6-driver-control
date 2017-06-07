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
    command = input('Which command do you want: ');
    if(command == 2)
        c1 = input('First coordinate: ');
        c2 = input('Second coordinate: ');
        c3 = input('Third coordinate: ');
    elseif(command == 3)
        c1 = input('Change first coordinate by: ');
        c2 = input('Change second coordinate by: ');
        c3 = input('Change third coordinate by: ');
    end
    fprintf(s, '%i', slave);
    fwrite(s, 's');
    fprintf(s, '%i', command);
    fwrite(s, 's');
    if(command == 2 || command == 3)
        fprintf(s, '%1.2f', c1*1.00);
        fwrite(s, 's');
        fprintf(s, '%1.2f', c2*1.00);
        fwrite(s, 's');
        fprintf(s, '%1.2f', c3*1.00);
            while(get(s, 'BytesAvailable')==0)
            end
            %a=fread(s, get(s, 'BytesAvailable'));
            a=fread(s, 1);
            b=fread(s, 2);
            if(a == 'c')
                disp("Continuing Program...");
            else
                disp("Stopping Program...");
            end
            %disp(b);
    else
        while(get(s, 'BytesAvailable')==0)
        end
        for x=1:3
            c = '';
            a=fread(s, 4);
            for y=1:4
                c = char(a(:, 1).');
            end
            tempNumber = str2double(string(c));
            disp("The current coordinates are: ");
            disp(tempNumber);
            fread(s, 2);
        end
    end
    if(command == 4)
        while(get(s, 'BytesAvailable')==0)
        end
        %a=fread(s, get(s, 'BytesAvailable'));
        a=fread(s, 1);
        b=fread(s, 2);
        disp("The program will now stop...");
        %disp(b);
    end
end
disp('Execution Ended');
%fscanf(s, '%u');
fclose(s);
clear all;