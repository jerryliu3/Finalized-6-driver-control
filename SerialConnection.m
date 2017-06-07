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
    if(command ~= 1)
        c1 = input('First coordinate: ');
        c2 = input('Second coordinate: ');
        c3 = input('Third coordinate: ');
    end
    fprintf(s, '%i', slave);
    fwrite(s, 's');
    fprintf(s, '%i', command);
    fwrite(s, 's');
    if(command ~=1)
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
            b=fread(s, 2);
            disp(a);
            disp(b);
        end
    else
        %disp("loading");
        while(get(s, 'BytesAvailable')==0)
        end
        %a=fread(s, get(s, 'BytesAvailable'));
        for x=1:3
            c = '';
            a=fread(s, 4);
            %disp(a);
            for y=1:4
                %disp("the character is");
                %disp(char(a(y, 1)));
                c = char(a(:, 1).');
            end
            %c = char(a(1, 1)) + char(a(2, 1)) + char(a(3, 1)) + char(a(4, 1));
            %disp("c is");
            %disp(c);
            %tempNumber = 0.00;
            tempNumber = str2double(string(c));
            %disp(string(c));
            disp(tempNumber);
            fread(s, 2);
        end
    end
end
disp('program completed');
%fscanf(s, '%u');
fclose(s);
clear all;