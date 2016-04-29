%% Tube detector array
close all; clear all;

N = 24;  % number of modules

% Read data from file
fileID = fopen('Test8.txt');
rawdata = hex2dec(textscan(fileID, '%2c'));    

%------------------------------------------
%% For loose data
% Trim data to integer scans from start
for i = 1 : length(rawdata)
    if (rawdata(i) == 32 && rawdata(i+1) == 10 && rawdata(i+2) == 13)
        startRaw = i+3;
        break;
    end
end
rawdata = rawdata(startRaw:length(rawdata));
lenRaw = length(rawdata)-rem(length(rawdata),3*N+2); % Trim the data for integer scans from end
% Remove sync symbols
rawrawdata = rawdata; rawdata = zeros((lenRaw/(3*N+2))*(2*N),1); k1 = 1; k2 = 1;
for i = 1 : lenRaw/(3*N+2)
    for j = 1 : N
        rawdata(k1) = rawrawdata(k2);
        rawdata(k1+1) = rawrawdata(k2+1);
        k1 = k1 + 2;
        k2 = k2 + 3;
    end
    k2 = k2 + 2;
end
lenRaw = length(rawdata);
%------------------------------------------
%% Arrange data into modules
%lenRaw = length(rawdata)-rem(length(rawdata),2*N); % Trim the data for integer number of scans

data = zeros(lenRaw/2,1);
modules = zeros(N,lenRaw/(2*N));
k = 0;
for i = 1 : 2 : lenRaw
    k = k + 1;
    data(k) = 256 * rawdata(i) + rawdata(i+1);
    
    % Remove transmission errors
    if (data(k) == 65535)
        data(k) = data(k-1);
    end
end
j = 0; k = 1;
for i = 1 : length(data)
    j = j + 1; 
    if (j == N+1)
        j = 1;
        k = k + 1;
    end   
    modules(j,k) = data(i);  
end

%% Filter noise
windowSize = 5;
filtered = zeros(size(modules));

for i = 1 : N
    filtered(i,:) = filter(ones(1,windowSize)/windowSize,1,modules(i,:));
end

%% Tracking

% Background light
restVal = zeros(1,8);
for i = 1 : N
    restVal(i) = mean(filtered(i,25:75));
end

% Max x+
max_xp = min(filtered(11,75:length(modules)));
% Max x-
max_xm = min(filtered(15,75:length(modules)));
% Max y+
max_yp = min(filtered(13,75:length(modules)));
% Max y-
max_ym = min(filtered(9,75:length(modules)));

% Display
figure; 
theta = 0:pi/10:2*pi;
% Sensor angles
loc = [0,-1; 0.7071,-0.7071; 1,0; 0.7071,0.7071; 0,1; -0.7071,-0.7071; -1,0; -0.7071,0.7071];

% Offline calculation
for t = 1 : length(modules)
    % Calculate coordinates
    x(t) = (abs(restVal(11)-filtered(11,t))/abs(restVal(11)-max_xp))*35 - (abs(restVal(15)-filtered(15,t))/abs(restVal(15)-max_xm))*35;
    y(t) = (abs(restVal(13)-filtered(13,t))/abs(restVal(13)-max_yp))*35 - (abs(restVal(9)-filtered(9,t))/abs(restVal(9)-max_ym))*35;       
end
% Calculate coordinates
filteredx = filter(ones(1,windowSize)/windowSize,1,x);
filteredy = filter(ones(1,windowSize)/windowSize,1,y);

tic
for t = 1 : length(modules)
    
    clf; hold on; 
    axis equal;      
    
    % Tracking plot
%     subplot(1,2,1);
    xlabel('X(mm)'); ylabel('Y(mm)'); 
    
    % Frame
    plot(35*cos(theta),35*sin(theta),'k');
    plot(30*cos(theta),30*sin(theta),'k:');
    plot(25*cos(theta),25*sin(theta),'k:');
    plot(20*cos(theta),20*sin(theta),'k:');
    plot(15*cos(theta),15*sin(theta),'k:');
    plot(10*cos(theta),10*sin(theta),'k:');
    plot(5*cos(theta),5*sin(theta),'k:');

    % Plot cursor
    plot(filteredx(t), filteredy(t), '+r');
    axis([-40 40 -40 40]);
    
    % Sensors plot
%     subplot(1,2,2);
%     
%     for i = 1 : 8
%         xa(i) = -(filtered(i,t)-restVal(i))*loc(i,1);
%         ya(i) = -(filtered(i,t)-restVal(i))*loc(i,2);
%     end
%     compass(xa, ya);
%     axis equal;
    
    pause(0.001);
    
    %drawnow;
    
end
toc
%% Visualization

figure; hold on;
plot(filtered(9,:),'b')
plot(filtered(10,:),'r')
plot(filtered(11,:),'g')
plot(filtered(12,:),'k')
plot(filtered(13,:),'m')
plot(filtered(14,:),'c')
plot(filtered(15,:),'y')
plot(filtered(16,:),'k:')

legend('9','10','11','12','13','14','15','16')

figure; hold on;
plot(filtered(1,:),'b')
plot(filtered(2,:),'r')
plot(filtered(3,:),'g')
plot(filtered(4,:),'k')
plot(filtered(5,:),'m')
plot(filtered(6,:),'c')
plot(filtered(7,:),'y')
plot(filtered(8,:),'k:')

legend('1','2','3','4','5','6','7','8')

figure; hold on;
plot(filtered(17,:),'b')
plot(filtered(18,:),'r')
plot(filtered(19,:),'g')
plot(filtered(20,:),'k')
plot(filtered(21,:),'m')
plot(filtered(22,:),'c')
plot(filtered(23,:),'y')
plot(filtered(24,:),'k:')

legend('17','18','19','20','21','22','23','24')

%% Close
fclose(fileID);
