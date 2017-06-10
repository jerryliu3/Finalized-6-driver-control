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
        %integrated code
        %inputs (all in mm)
        
        %0 to 10 range
        double z;
        %-pi/4 to pi/4 range
        double theta;
        %-pi/2 to pi/2 range
        double gamma;
        z = input('What is the desired height? ');
        %enter coordinates
        x1 = input('First x coordinate: ');
        x2 = input('Second x coordinate: ');
        x3 = input('Third x coordinate: ');
        y1 = input('First y coordinate: ');
        y2 = input('Second y coordinate: ');
        y3 = input('Third y coordinate: ');
        theta = input('What is the desired theta? ');
        gamma = input('What is the desired gamma? ');
        
        %initial coordinates
        c1i = [x1 y1 0];
        c2i = [x2 y2 0];
        c3i = [x3 y3 0];
        
        %matrix of coordinates
        global A;
        A = [c1i;c2i;c3i];
        
        disp('The first pin is at: ');
        disp(A(1,:));
        disp('The second pin is at: ');
        disp(A(2,:));
        disp('The third pin is at: ');
        disp(A(3,:));
        
        %intial metasurface hexagon and plane of final metasurface hexagon
        figure;
        [x, y] = meshgrid(-5:1:5);
        Zv = @(x,y) -sin(theta)*cos(gamma)*x/cos(theta) - sin(theta)*sin(gamma)*y/cos(theta);
        mesh(x,y,Zv(x,y))
        
        axis([-10 10 -10 10 -10 10])
        xlabel('x values') % x-axis label
        ylabel('y values') % y-axis label
        zlabel('z values') % z-axis label
        
        %new matrix of coordinates
        A = calculateHeightGeneral(c1i, c2i, c3i, theta, gamma, z);
        
        disp('The first pin has been moved to: ');
        disp(A(1,:));
        disp('The second pin has been moved to: ');
        disp(A(2,:));
        disp('The third pin has been moved to: ');
        disp(A(3,:));
        
        %generalTrace(c1i, c2i, c3i);
        %end of integrated code
        c1 = A(1, :);
        c2 = A(2, :);
        c3 = A(3, :);
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

%move centre to desired location
function newA = adjustPlatform(raise, c1f, c2f, c3f)
    c1f(1, 3) = c1f(1, 3) + raise;
    c2f(1, 3) = c2f(1, 3) + raise;
    c3f(1, 3) = c3f(1, 3) + raise;
    
    newA = [c1f;c2f;c3f];
    
end

%obtain new heights of 3 pins
function newA = calculateHeightGeneral(c1, c2, c3, theta, gamma, raise)

    x1 = c1(1, 1);
    y1 = c1(1, 2);

    x2 = c2(1, 1);
    y2 = c2(1, 2);

    x3 = c3(1, 1);
    y3 = c3(1, 2);

    x12 = x2-x1;
    x13 = x3-x1;
    y12 = y2-y1;
    y13 = y3-y1;
    x32 = x2-x3;

    numerator = x13 - (x12 * y13 / y12);
    denominator = (x1*y3-y1*x3)/(x1*y2-y1*x2) - (y13/y12);

    n = (x12*y13-x13*y12)/cos(theta);

    z1 = (n*sin(theta)*sin(gamma) + (x12*n*sin(theta)*cos(gamma)/y12) - (n*sin(theta)*cos(gamma)/y12/denominator))/((1-y13/y12-(y2*x3-x2*y3+y1*x32)/(x1*y2-y1*x2))*numerator/denominator - (x13 - x12*y13/y12));
    z2 = (n*sin(theta)*sin(gamma) + x12*n*sin(theta)*cos(gamma)/y12+z1*(x13-x12*y13/y12))/(x13-x12*y13/y12);
    z3 = (n*sin(theta)*cos(gamma) + y13 * (z2-z1))/y12 + z1;
    
    %correct normal vector but height has been changed
    c1f = [x1 y1 z1];
    c2f = [x2 y2 z2];
    c3f = [x3 y3 z3];

    v1 = c2f-c1f;
    v2 = c3f-c1f;
    disp('The new normal vector is');
    n = cross(v2, v1);
    disp(n);
    
    %move the plane back to (0, 0, 0)
    d = c1f(1, 1)*n(1, 1) + c1f(1, 2)*n(1, 2) + c1f(1, 3)*n(1, 3);
    z = d/n(1, 3);
    c1f(1, 3) = c1f(1, 3)-z;
    c2f(1, 3) = c2f(1, 3)-z;
    c3f(1, 3) = c3f(1, 3)-z;

    hold on
    patch([3.75 7.5 3.75 -3.75 -7.5 -3.75], [6.5 0 -6.5 -6.5 0 6.5], [0 0 0 0 0 0], [-1 -1 -1 -1 -1 -1])
    vectarrow([0 0 0], n*10, 'b');
    vectarrow(c1, c1f, 'r');
    vectarrow(c2, c2f, 'g');
    vectarrow(c3, c3f, 'b');
    
    trace3a(c1, c2, c3, c1f, c2f, c3f, theta, gamma);
    newA = adjustPlatform(raise, c1f, c2f, c3f);
end

%calculate heights without displaying them
function newA = calculateHeightNoDisplay(c1, c2, c3, theta, gamma)

    x1 = c1(1, 1);
    y1 = c1(1, 2);

    x2 = c2(1, 1);
    y2 = c2(1, 2);

    x3 = c3(1, 1);
    y3 = c3(1, 2);

    x12 = x2-x1;
    x13 = x3-x1;
    y12 = y2-y1;
    y13 = y3-y1;
    x32 = x2-x3;

    numerator = x13 - (x12 * y13 / y12);
    denominator = (x1*y3-y1*x3)/(x1*y2-y1*x2) - (y13/y12);

    n = (x12*y13-x13*y12)/cos(theta);

    z1 = (n*sin(theta)*sin(gamma) + (x12*n*sin(theta)*cos(gamma)/y12) - (n*sin(theta)*cos(gamma)/y12/denominator))/((1-y13/y12-(y2*x3-x2*y3+y1*x32)/(x1*y2-y1*x2))*numerator/denominator - (x13 - x12*y13/y12));
    z2 = (n*sin(theta)*sin(gamma) + x12*n*sin(theta)*cos(gamma)/y12+z1*(x13-x12*y13/y12))/(x13-x12*y13/y12);
    z3 = (n*sin(theta)*cos(gamma) + y13 * (z2-z1))/y12 + z1;
    
    %correct normal vector but height has been changed
    c1f = [x1 y1 z1];
    c2f = [x2 y2 z2];
    c3f = [x3 y3 z3];

    v1 = c2f-c1f;
    v2 = c3f-c1f;
    n = cross(v2, v1);
    %move the plane back to (0, 0, 0)
    d = c1f(1, 1)*n(1, 1) + c1f(1, 2)*n(1, 2) + c1f(1, 3)*n(1, 3);
    z = d/n(1, 3);
    c1f(1, 3) = c1f(1, 3)-z;
    c2f(1, 3) = c2f(1, 3)-z;
    c3f(1, 3) = c3f(1, 3)-z;
    hold on
    trace3a(c1, c2, c3, c1f, c2f, c3f, theta, gamma);
end

%produce trace from initial to final coordinates
function trace3a = trace3a(c1, c2, c3, c1f, c2f, c3f, theta, gamma)

    v1 = c2f-c1f;
    v2 = c3f-c1f;
    n = cross(v2, v1);    
    
    u = [n(1, 1)/sqrt(n(1, 1)*n(1, 1) + n(1, 2)* n(1, 2)) n(1, 2)/sqrt(n(1, 1)*n(1, 1) + n(1, 2)* n(1, 2)) 0];
    d1 = sqrt(sum(c1f.*c1f))-sqrt(sum(c1.*c1));
    vector1 = u*d1;
    
    if(c1(1, 3) > (-n(1, 1)*c1f(1, 1) - n(1, 2)*c1f(1, 2))/n(1, 3))
        vector1 = -1*vector1;
    end
    newP = c1 + vector1;
    vectarrow(c1, newP, 'r');
    
    d2 = sqrt(sum(c2f.*c2f))-sqrt(sum(c2.*c2));
    vector2 = u*d2;
    
    if(c2(1, 3) > (-n(1, 1)*c2f(1, 1) - n(1, 2)*c2f(1, 2))/n(1, 3))
        vector2 = -1*vector2;
    end
    newP = c2 + vector2;
    vectarrow(c2, newP, 'g');
    
    d3 = sqrt(sum(c3f.*c3f))-sqrt(sum(c3.*c3));
    vector3 = u*d3;
    
    if(c3(1, 3) > (-n(1, 1)*c3f(1, 1) - n(1, 2)*c3f(1, 2))/n(1, 3))
        vector3 = -1*vector3;
    end
    newP = c3 + vector3;
    vectarrow(c3, newP, 'b');
end

%trace of all possible orientations and pin positions
function generalTrace = generalTrace(c1i, c2i, c3i)
    figure;
    axis([-10 10 -10 10 -10 10])
    xlabel('x values') % x-axis label
    ylabel('y values') % y-axis label
    zlabel('z values') % z-axis label
    patch([3.75 7.5 3.75 -3.75 -7.5 -3.75], [6.5 0 -6.5 -6.5 0 6.5], [0 0 0 0 0 0], [-1 -1 -1 -1 -1 -1])
    for a = -pi/4:0.1:pi/4
        for b = -pi:0.1:pi
           calculateHeightNoDisplay(c1i, c2i, c3i, a, b)
        end
    end
end

function newA = returnA()
    global A;
    newA = A;
end