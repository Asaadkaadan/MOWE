% 37-module flat array
close all; clear all;

N = 37;  % number of modules

% Read data from file
%fileID = fopen('hand_50.txt');
fileID = fopen('speed.txt');
rawdata = hex2dec(textscan(fileID, '%2c'));
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

% Correct module 1 scale
modules(1,:) = modules(1,:)/3;

% Calculate max value (ADC units)
maxVal = 1.0 * max(max(modules));
%maxVal = 200;

% Module equalization
ave = zeros(1,N);
for j = 1 : N
   ave(j) = mean(modules(j,1:100));   
end
totalAve = mean(ave);
for j = 1 : N
   modules(j,:) = modules(j,:)*(totalAve/ave(j));   
end


% % Create video object
% writerObj = VideoWriter('37_flat_pas_eq.mp4','MPEG-4');
% %writerObj = VideoWriter('37_flat_speed');
% writerObj.FrameRate = 48;
% %writerObj.FrameRate = 34;
% open(writerObj);


% plot data
M = struct('cdata',{},'colormap',{});
figure; hold on;

[xq,yq] = meshgrid(0:200, 0:200);   % mm
n = 0; midx = 0; midy = 0; route = zeros(2,length(modules));
 
tic
%for t = 1 : length(modules)
for t = 50 : 120
    
    clf; hold on; colorbar;
    set(gca, 'Clim', [0 maxVal]);
    axis([0 200 0 200 0 maxVal]);
    axis equal; 
    %view([16 20]);
    
    % Surface Interpolation
    v = modules(:,t);
    F = scatteredInterpolant(modloc(:,1),modloc(:,2),v,'natural','none');
    vq = F(xq,yq);
%     vq = triscatteredinterp(x,y,v,xq,yq);

    % draw interpolated surface
    mesh(xq,yq,vq);
    
    % plot measured data
    for i = 1 : N
        %plot(modloc(i,1),modloc(i,2),'*','Color',[4096 4096-modules(i,1) 4096-modules(i,1)]/4096)
        plot3(modloc(i,1),modloc(i,2),maxVal,'*','Color',[maxVal maxVal-modules(i,t) maxVal-modules(i,t)]/maxVal);
    end
    
%     % Track objects and draw route
%     for i = 1 : N
%         if (modules(i,t) > 500)        
%             midx = ( midx + modloc(i,1) ) / 2;
%             midy = ( midy + modloc(i,2) ) / 2;                  
%         end    
%     end
%     if (midx ~= 0 || midy ~= 0)
%         n = n + 1;
%         route(:,n) = [midx; midy];
%         plot3(route(1,1:n),route(2,1:n),maxVal*ones(1,n),'-sk');
%     end
    
    drawnow;
    
%     % Record video
%     M(t) = getframe;
%     % Save video to hard disk
%     writeVideo(writerObj,M(t));

    %pause(0.01);

end
toc

% tic
% movie(M)
% toc

% close(writerObj);
fclose(fileID);
