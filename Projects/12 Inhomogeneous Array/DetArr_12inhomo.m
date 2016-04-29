% 12-module flat inhomogenuous array
% close all; clear all;

N = 12;  % number of modules

% Read data from file
fileID = fopen('capture3850nmturb.txt');
rawdata = fread(fileID);

%rawdata = hex2dec(textscan(fileID, '%2c'));    

%------------------------------------------
%% For loose data
% Trim data to integer scans from start
% for i = 1 : length(rawdata)
%     if (rawdata(i) == 32 && rawdata(i+1) == 10 && rawdata(i+2) == 13)
%         startRaw = i+3;
%         break;
%     end
% end
% rawdata = rawdata(startRaw:length(rawdata));
% lenRaw = length(rawdata)-rem(length(rawdata),3*N+2); % Trim the data for integer scans from end
% % Remove sync symbols
% rawrawdata = rawdata; rawdata = zeros((lenRaw/(3*N+2))*(2*N),1); k1 = 1; k2 = 1;
% for i = 1 : lenRaw/(3*N+2)
%     for j = 1 : N
%         rawdata(k1) = rawrawdata(k2);
%         rawdata(k1+1) = rawrawdata(k2+1);
%         k1 = k1 + 2;
%         k2 = k2 + 3;
%     end
%     k2 = k2 + 2;
% end
% lenRaw = length(rawdata);
%------------------------------------------
%% Arrange data into modules
lenRaw = length(rawdata)-rem(length(rawdata),2*N); % Trim the data for integer number of scans

data = zeros(lenRaw/2,1);
modules = zeros(N,lenRaw/(2*N));
k = 0;
for i = 1 : 2 : lenRaw
    k = k + 1;
    data(k) = 256 * rawdata(i) + rawdata(i+1);
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

% Module locations
d = 30; % optical resolution
s = d/sqrt(3);  % module side lenght
modloc = [ 
    3.5*d, 4.0*s ;  % 1
    3.0*d, 5.5*s ;  % 2
    2.0*d, 5.5*s ;  % 3
    1.0*d, 5.5*s ;  % 4 
    2.5*d, 4.0*s ;  % 5
    1.5*d, 4.0*s ;  % 6
    0.5*d, 4.0*s ;  % 7
    3.0*d, 2.5*s ;  % 8 
    2.0*d, 2.5*s ;  % 9
    1.0*d, 2.5*s ;  % 10
    2.5*d, 1.0*s ;  % 11
    1.5*d, 1.0*s ;  % 12
    ];


% Calculate max value (ADC units)
maxVal = 1.0 * max(max(modules));
%maxVal = 200;


% % Create video object
% writerObj = VideoWriter('repeated_980/capture950nm_turb4.mp4','MPEG-4');
% %writerObj = VideoWriter('37_flat_speed');
% writerObj.FrameRate = 48;
% %writerObj.FrameRate = 34;
% open(writerObj);


% plot data
M = struct('cdata',{},'colormap',{});
figure; hold on;

[xq,yq] = meshgrid(0:120, 0:120);   % mm
 
tic
%for t = 1 : length(modules)
for t = 50 : 120
    
    clf; hold on; colorbar;
    set(gca, 'Clim', [0 maxVal]);
    axis([0 120 0 120 0 maxVal]);
    axis equal; 
    
    % Surface Interpolation
    v = modules(:,t);
    F = scatteredInterpolant(modloc(:,1),modloc(:,2),v,'natural','none');
    vq = F(xq,yq);

    % draw interpolated surface
    mesh(xq,yq,vq);
    
    % plot measured data
    for i = 1 : N
        %plot(modloc(i,1),modloc(i,2),'*','Color',[4096 4096-modules(i,1) 4096-modules(i,1)]/4096)
        %plot3(modloc(i,1),modloc(i,2),maxVal,'*','Color',[maxVal maxVal-modules(i,t) maxVal-modules(i,t)]/maxVal);
        plot3(modloc(i,1),modloc(i,2),maxVal,'*','Color','black');
    end
    
    drawnow;
    
%     % Record video
%     M(t) = getframe;
%     % Save video to hard disk
%     writeVideo(writerObj,M(t));

    %pause(0.01);

end
toc


% close(writerObj);
fclose(fileID);
