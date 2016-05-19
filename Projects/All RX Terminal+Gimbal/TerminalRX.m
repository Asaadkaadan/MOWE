%% TerminalRX 37-module all-rx flat array
close all; clear all; 

N = 37;  % number of modules

%% Module locations
d = 30; % optical resolution
s = d/sqrt(3);  % module side lenght
modloc = [ 
    0.5*d, 5.5*s ;  % 1
    1.0*d, 7.0*s ;  % 2
    1.5*d, 8.5*s ;  % 3
    2.0*d, 10.0*s ;  % 4 
    3.0*d, 10.0*s ;  % 5
    4.0*d, 10.0*s ;  % 6
    5.0*d, 10.0*s ;  % 7
    2.5*d, 8.5*s ;  % 8 
    3.5*d, 8.5*s ;  % 9
    4.5*d, 8.5*s ;  % 10
    5.5*d, 8.5*s ;  % 11
    1.5*d, 5.5*s ;  % 12
    2.0*d, 7.0*s ;  % 13 
    3.0*d, 7.0*s ;  % 14
    4.0*d, 7.0*s ;  % 15
    5.0*d, 7.0*s ;  % 16    
    6.0*d, 7.0*s ;  % 17
    2.5*d, 5.5*s ;  % 18 
    3.5*d, 5.5*s ;  % 19
    4.5*d, 5.5*s ;  % 20
    5.5*d, 5.5*s ;  % 21    
    6.5*d, 5.5*s ;  % 22
    1.0*d, 4.0*s ;  % 23
    2.0*d, 4.0*s ;  % 24 
    3.0*d, 4.0*s ;  % 25
    4.0*d, 4.0*s ;  % 26
    5.0*d, 4.0*s ;  % 27    
    6.0*d, 4.0*s ;  % 28
    2.5*d, 2.5*s ;  % 29
    3.5*d, 2.5*s ;  % 30
    4.5*d, 2.5*s ;  % 31
    5.5*d, 2.5*s ;  % 32
    1.5*d, 2.5*s ;  % 33
    2.0*d, 1.0*s ;  % 34
    3.0*d, 1.0*s ;  % 35
    4.0*d, 1.0*s ;  % 36
    5.0*d, 1.0*s ;  % 37
    ];

%% Visualization data
M = struct('cdata',{},'colormap',{});
figure; hold on;
[xq,yq] = meshgrid(0:5:200, 0:5:200);   % mm
ave = zeros(1,N); t = 0; F = 0; 
%vq = zeros(length(xq),length(xq));
%rawrawdata = zeros(3*N,1); 
oldrawrawdata = zeros(3*N,1);

%% Stream data
port = serial('COM3','BaudRate',921600,'DataBits',8);
fopen(port); sync1 = 0; sync2 = 0;

while(1)
    % Frame synchronization
    while(sync1 ~= 10 && sync2 ~= 13)        
        sync1 = fread(port,1,'uint8'); 
        sync2 = fread(port,1,'uint8');
    end
    sync1 = 0; sync2 = 0;
    
    % Read data
    rawrawdata = fread(port,3*N,'uint8');
    
    % Check data is correct
    for j = 3 : 3 : length(rawrawdata)
        if (rawrawdata(j) ~= 32)
            rawrawdata = oldrawrawdata;     % Use old data
            break;
        end
    end

    % Remove sync symbols and arrange data into modules
    rawdata = zeros(2*N,1); modules = zeros(N,1); k = 1; n = 1;
    for j = 3 : 3 : length(rawrawdata)
        rawdata(k) = rawrawdata(j-2);
        rawdata(k+1) = rawrawdata(j-1);
        modules(n) = 256 * rawdata(k) + rawdata(k+1);
        k = k + 2; n = n + 1;
    end

%     % Correct module 1 scale
%     modules(1,:) = modules(1,:)/3;

    % Calculate max value (ADC units)
    maxVal = 1.0 * max(max(modules));
    %maxVal = 2000;
    
    % Module equalization
    if (t ~= 1000)
        % Measure the base
        if (t < 50)
            for j = 1 : N
                ave(j) = ave(j) + modules(j);   
            end
            t = t + 1;
        else
            t = 1000;
            totalAve = mean(ave);
        end
    else
        % Equalize
        for j = 1 : N
           modules(j) = modules(j)*(totalAve/ave(j));   
        end        
    end
    
    % Visualization
    clf; hold on; 
    %colorbar;
    set(gca, 'Clim', [0 maxVal]);
    %axis([0 200 0 200 0 maxVal]);
    axis([0 200 0 200]);
    %axis equal; 
    
    % Surface Interpolation
    F = scatteredInterpolant(modloc(:,1),modloc(:,2),modules,'natural','none');
    vq = F(xq,yq);

    % draw interpolated surface
    mesh(xq,yq,vq);   
    
    % plot measured data
    for i = 1 : N
        plot3(modloc(i,1),modloc(i,2),maxVal,'*','Color','black');
    end
    
    drawnow;

    % store rawrawdata for later
    oldrawrawdata = rawrawdata;
end

fclose(port);
