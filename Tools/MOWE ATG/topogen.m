%% /**
%   ******************************************************************************
%   * File Name          : topogen.m
%   * Date               : 08/11/2015 
%   * Version            : 1.1 
% 	* Author 			 : Asaad Kaadan
%   * Description        : Modular Optical Wireless Elements (MOWE) topology 
%   *                      generation tool.
% 	* License 			 : The MIT Open-source License (MIT). 
%   *                      Please check the license file (License.txt)
%   *
%   ******************************************************************************
%   */
function topogen()	
close all;
%% Welcome message
fprintf('\n\r');
disp('====================================================');
disp('====================================================');
disp('||       Welcome to MOWE Topology Generator!      ||');
disp('||          OU 2015. All rights reserved.         ||');
disp('||                                                ||');
disp('||    Type help for a list of possible commands.  ||');
disp('====================================================');
disp('====================================================');
fprintf('\n\n\r'); 

hfig = figure; hold on; grid on; axis equal;
% Prepare texts
%text('FontUnits', 'points');

hexaMode = 'h';     % Default hexagon orientation is horizontal
side = 17.32;       % 30mm default size
height = 30;
moduleID = 0;
modules = zeros(21,1);


%% Main loop - Query user input
while(1)
    % Request user input
    user = input('Enter a command: ', 's');
    % Seperate the keywords
    commands = strsplit(user);  
    
    % Check first keyword
    switch char(commands(1))
        case 'add' 
            addRoutine();           
%         case 'del'
%                          
%         case 'edit'          
        case 'hexamode'
            hexaMode = char(commands(2));           
        case 'size'
            setModuleSize();    
        case 'make'
            if (strcmp(commands(2), 'mcu'))            % Generate output for MCU firmware
                makeFileMCU(); 
            elseif (strcmp(commands(2), 'sim'))         % Generate output for MOWE-Tandy simulator
                makeFileSIM();
            end
        case 'help'
            help(); 
        case 'exit'
            break;
        otherwise
            fprintf('Wrong input!\n\r');   
    end
    
end


%% Add a module
function addRoutine()
    
    % Check shape
    if (strcmp(commands(2), 'hexa'))            % Hexagon
        
        moduleID = moduleID + 1;
            
        % Check functionality
        if (strcmp(commands(3), 'tx'))
            modules(1,moduleID) = 1;    
        elseif (strcmp(commands(3), 'rx'))
            modules(1,moduleID) = 0;
        else
            fprintf('Wrong module type!\n\r'); 
        end
        
        %  Define ports and location
        if (moduleID == 1)
            x0 = 0; y0 = 0;
            modules(4,moduleID) = 1;    % P1
            modules(5,moduleID) = 2;    % P2
            modules(6,moduleID) = 3;    % P3
            modules(7,moduleID) = 4;    % P4
            modules(8,moduleID) = 5;    % P5
            modules(9,moduleID) = 6;    % P6
        else
            % Ports
            con = strsplit(char(commands(4)),{'>',':'},'CollapseDelimiters',true);  
            myPort = char(con(1));
            adjModule = char(con(2));
            adjPort = char(con(3));     
            myPort = str2double(myPort(2));
            adjModule = str2double(adjModule(2:length(adjModule)));
            adjPort = str2double(adjPort(2));
            adjModDir = find(modules(4:9,adjModule)==adjPort);
            myDir = mod(adjModDir+3,6); 
            if (myDir == 0)
                myDir = 6;
            end
            
            
            if (modules(adjModDir+9,moduleID) ~= 0 || modules(adjModDir+9,adjModule) ~= 0)
                fprintf('This port is already used!\n\r');
                moduleID = moduleID - 1;
                return;
            else
                modules(myDir+9,moduleID) = bitor(bitshift(adjModule,3), modules(adjPort+3,adjModule));
                modules(adjModDir+9,adjModule) = bitor(bitshift(moduleID,3), modules(myPort+3,moduleID));
                modules(4:9,moduleID) = circshift((1:6)',myDir-myPort);
            end
            
            
            
            % Location
            if (hexaMode == 'h')
                switch adjModDir
                    case 1
                        x0 = modules(2,adjModule)+side+sqrt(side^2-((height^2)/4));
                        y0 = modules(3,adjModule)+height/2;
                    case 2
                        x0 = modules(2,adjModule)+side+sqrt(side^2-((height^2)/4));
                        y0 = modules(3,adjModule)-height/2;                    
                    case 3
                        x0 = modules(2,adjModule);
                        y0 = modules(3,adjModule)-height;                       
                    case 4
                        x0 = modules(2,adjModule)-side-sqrt(side^2-((height^2)/4));
                        y0 = modules(3,adjModule)-height/2;                
                    case 5
                        x0 = modules(2,adjModule)-side-sqrt(side^2-((height^2)/4));
                        y0 = modules(3,adjModule)+height/2;                         
                    case 6
                        x0 = modules(2,adjModule);
                        y0 = modules(3,adjModule)+height;                         
                end
            elseif (hexaMode == 'v')
                switch adjModDir
                    case 1
                        x0 = modules(2,adjModule)+height/2;
                        y0 = modules(3,adjModule)+sqrt(3)*height/2;                    
                    case 2
                        x0 = modules(2,adjModule)+height; 
                        y0 = modules(3,adjModule);                       
                    case 3
                        x0 = modules(2,adjModule)+height/2;
                        y0 = modules(3,adjModule)-sqrt(3)*height/2;                
                    case 4
                        x0 = modules(2,adjModule)-height/2;
                        y0 = modules(3,adjModule)-sqrt(3)*height/2;                         
                    case 5
                        x0 = modules(2,adjModule)-height; 
                        y0 = modules(3,adjModule); 
                    case 6
                        x0 = modules(2,adjModule)-height/2;
                        y0 = modules(3,adjModule)+sqrt(3)*height/2;                     
                end                
            end
        end
        %  Create the module
        modules(2,moduleID) = x0;    % x
        modules(3,moduleID) = y0;    % y
        plotHexa(side,x0,y0);
        
    elseif (strcmp(commands(2), 'penta'))       % Pentagon
        moduleID = moduleID + 1;
        
        
    else
        fprintf('Wrong shape!\n\r'); 
    end
        

end

%% Plot a hexagon
function plotHexa(side, x0, y0)
   %side= side size;(x0,y0) exagon center coordinates;

   % Plot shape
   if (hexaMode == 'h')
       % Calculate shape
       x=side*[-1 -0.5 0.5 1 0.5 -0.5 -1]+x0;
       y=side*sqrt(3)*[0 -0.5 -0.5 0 0.5 0.5 0]+y0;
       plot(x,y,'b','Linewidth',4);
       % Plot ports
       temp = sprintf('P%d',modules(4,moduleID));
       htxtp1 = text(side*0.75+x0, side*sqrt(3)*0.25+y0, temp,'FontSize',side,'HorizontalAlignment','right','VerticalAlignment','top');
       temp = sprintf('P%d',modules(5,moduleID));
       htxtp2 = text(side*0.75+x0, -side*sqrt(3)*0.25+y0, temp,'FontSize',side,'HorizontalAlignment','right','VerticalAlignment','bottom');
       temp = sprintf('P%d',modules(6,moduleID));
       htxtp3 = text(x0, -side*sqrt(3)*0.495+y0, temp,'FontSize',side,'HorizontalAlignment','center','VerticalAlignment','bottom');
       temp = sprintf('P%d',modules(7,moduleID));
       htxtp4 = text(-side*0.75+x0, -side*sqrt(3)*0.25+y0, temp,'FontSize',side,'HorizontalAlignment','left','VerticalAlignment','bottom');
       temp = sprintf('P%d',modules(8,moduleID));
       htxtp5 = text(-side*0.75+x0, side*sqrt(3)*0.25+y0, temp,'FontSize',side,'HorizontalAlignment','left','VerticalAlignment','top');
       temp = sprintf('P%d',modules(9,moduleID));
       htxtp6 = text(x0, side*sqrt(3)*0.45+y0, temp,'FontSize',side,'HorizontalAlignment','center','VerticalAlignment','top');
   elseif (hexaMode == 'v')
       % Calculate shape
       x=side*sqrt(3)*[0 -0.5 -0.5 0 0.5 0.5 0]+x0;
       y=side*[-1 -0.5 0.5 1 0.5 -0.5 -1]+y0;
       plot(x,y,'b','Linewidth',4);
       % Plot ports
       temp = sprintf('P%d',modules(4,moduleID));
       htxtp1 = text(side*0.5+x0, 0.7*side+y0, temp,'FontSize',side,'HorizontalAlignment','right','VerticalAlignment','top');
       temp = sprintf('P%d',modules(5,moduleID));
       htxtp2 = text(side*sqrt(3)*0.45+x0, y0, temp,'FontSize',side,'HorizontalAlignment','right','VerticalAlignment','middle');
       temp = sprintf('P%d',modules(6,moduleID));
       htxtp3 = text(side*0.5+x0, -0.7*side+y0, temp,'FontSize',side,'HorizontalAlignment','right','VerticalAlignment','bottom');
       temp = sprintf('P%d',modules(7,moduleID));
       htxtp4 = text(-side*0.5+x0, -0.7*side+y0, temp,'FontSize',side,'HorizontalAlignment','left','VerticalAlignment','bottom');
       temp = sprintf('P%d',modules(8,moduleID));
       htxtp5 = text(-side*sqrt(3)*0.45+x0, y0, temp,'FontSize',side,'HorizontalAlignment','left','VerticalAlignment','middle');
       temp = sprintf('P%d',modules(9,moduleID));
       htxtp6 = text(-side*0.5+x0, 0.7*side+y0, temp,'FontSize',side,'HorizontalAlignment','left','VerticalAlignment','top');
   end
   % Plot label & part number
   temp = sprintf('%d',moduleID);
   htxtID = text(x0,y0,temp,'FontSize',2*side,'HorizontalAlignment','center');
   htxtPN = text(x0,-side*sqrt(3)*0.25+y0,pn(moduleID),'FontSize',side/2,'HorizontalAlignment','center');
   % Choose text color
   if (modules(1,moduleID) == 1)
       set(htxtp1, 'Color','red');
       set(htxtp2, 'Color','red');
       set(htxtp3, 'Color','red');
       set(htxtp4, 'Color','red');
       set(htxtp5, 'Color','red');
       set(htxtp6, 'Color','red');
       set(htxtID, 'Color','red');
       set(htxtPN, 'Color','red');
   else
       set(htxtp1, 'Color','green');
       set(htxtp2, 'Color','green');
       set(htxtp3, 'Color','green');
       set(htxtp4, 'Color','green');
       set(htxtp5, 'Color','green');
       set(htxtp6, 'Color','green');
       set(htxtID, 'Color','green');
       set(htxtPN, 'Color','green');
   end
end

%% Plot a pentagon
function plotPenta(x, y)

end


%% Set module size
function setModuleSize()
    if (strcmp(commands(2), '20mm'))           
        side = 11.547;       
        height = 20;
    elseif (strcmp(commands(2), '30mm'))       
        side = 17.32;      
        height = 30;
    else
        fprintf('Wrong input!\n\r'); 
    end
    
end

%% Create topology.h file for MCU firmware
function makeFileMCU()
    
    reviewCon();  
    
    if (length(commands) > 2)
        filename = strcat(char(commands(3)), '.h');
    else
        filename = 'topology.h';
    end
    
    fileID = fopen(filename,'w');
    
    % File
    fprintf(fileID,'/**');
    fprintf(fileID,'\n  ******************************************************************************');
    fprintf(fileID,'\n  * File Name          : %s', filename);
    fprintf(fileID,'\n  * Date               : %s', date);
    fprintf(fileID,'\n  *	Author 			   : Asaad Kaadan');
    fprintf(fileID,'\n  * Description        : This file is automatically generated by the ''topogen'' MATLAB tool.');
    fprintf(fileID,'\n  *                      This version is compiled for MCU firmware use.');
    fprintf(fileID,'\n  *');
    fprintf(fileID,'\n  ******************************************************************************');
    fprintf(fileID,'\n  */');
    fprintf(fileID,'\n\n/* Define to prevent recursive inclusion -------------------------------------*/');
    fprintf(fileID,'\n#ifndef __%s_H', filename(1:length(filename)-2));
    fprintf(fileID,'\n#define __%s_H', filename(1:length(filename)-2));
    fprintf(fileID,'\n#ifdef __cplusplus');
    fprintf(fileID,'\n extern "C" {');
    fprintf(fileID,'\n#endif');
    fprintf(fileID,'\n\n/* Includes ------------------------------------------------------------------*/');
    fprintf(fileID,'\n#include "stm32f0xx_hal.h"');
    fprintf(fileID,'\n\n// Module Part Numbers:');
    fprintf(fileID,'\n// HO01R1: Hexagon receiver with VEMT3700 - 30mm optical resolution.');
    fprintf(fileID,'\n// HO01R2: Hexagon receiver with VEMT3700 - 20mm optical resolution.');
    fprintf(fileID,'\n// HO02R0: Hexagon transmitter  with VSMY2850G - 30mm optical resolution.');
    fprintf(fileID,'\n// HO02R1: Hexagon transmitter  with VSMY2850G - 20mm optical resolution.');
    fprintf(fileID,'\n// PO01R0: Pentagon receiver with VEMT3700 compatible with hexagons 30mm optical resolution.');
    fprintf(fileID,'\n// PO02R0: Pentagon transmitter  with VSMY2850G compatible with hexagons 30mm optical resolution.');
    fprintf(fileID,'\n\n// Enumerations');
    fprintf(fileID,'\nenum	modulePartNumbers{_HO01R1, _HO01R2, _HO02R0, _HO02R1, _PO01R0, _PO02R0};');
    fprintf(fileID,'\nenum	portPol{_normal, _reversed};');
    fprintf(fileID,'\nenum	modulePorts{_P1=1, _P2, _P3, _P4, _P5, _P6};');
    fprintf(fileID,'\n\n#define _N	%d					// Number of array modules', moduleID);   
    fprintf(fileID,'\n\n// Array modules');
    for i = 1 : moduleID
        fprintf(fileID,'\n#define _mod%d	%d<<3', i, i);
    end   
    fprintf(fileID,'\n\n// Topology');
    fprintf(fileID,'\nstatic uint16_t array[_N][7] = {');
    for i = 1 : moduleID-1
        fprintf(fileID,'\n{ _%s, %s, %s, %s, %s, %s, %s},	// Module %d', pn(i), topo(1,i), topo(2,i), topo(3,i),...
            topo(4,i), topo(5,i), topo(6,i), i);
    end 
    fprintf(fileID,'\n{ _%s, %s, %s, %s, %s, %s, %s}    // Module %d', pn(moduleID), topo(1,moduleID), topo(2,moduleID),...
             topo(3,moduleID), topo(4,moduleID), topo(5,moduleID), topo(6,moduleID), moduleID);	
    fprintf(fileID,'\n};');    
    fprintf(fileID,'\n\n// Configurations for duplex serial ports');
    for i = 1 : moduleID
        fprintf(fileID,'\n#ifdef _module_%d',i);
        fprintf(fileID,'\n\t#define	_module	%d',i);
        fprintf(fileID,'\n\t#define	%s	1',pn(i));
        fprintf(fileID,'\n\t#define	_P1pol_%s	1',portPol(i,1));
        fprintf(fileID,'\n\t#define	_P2pol_%s	1',portPol(i,2));
        fprintf(fileID,'\n\t#define	_P3pol_%s	1',portPol(i,3));
        fprintf(fileID,'\n\t#define	_P4pol_%s	1',portPol(i,4));
        fprintf(fileID,'\n\t#define	_P5pol_%s	1',portPol(i,5));
        fprintf(fileID,'\n\t#define	_P6pol_%s	1',portPol(i,6));
        fprintf(fileID,'\n#endif');
    end
    fprintf(fileID,'\n\n\n#ifdef __cplusplus');
    fprintf(fileID,'\n}');
    fprintf(fileID,'\n#endif');
    fprintf(fileID,'\n#endif /*__%s_H */', filename(1:length(filename)-2));
    fprintf(fileID,'\n/**');
    fprintf(fileID,'\n  * @}');
    fprintf(fileID,'\n  */');
    fprintf(fileID,'\n\n/**');
    fprintf(fileID,'\n  * @}');
    fprintf(fileID,'\n  */');
    fprintf(fileID,'\n\n/************************ (C) COPYRIGHT OU *****END OF FILE****/\n');

    fclose(fileID);
end

%% Create topology.h file for MOWE-Tandy simulator
function makeFileSIM()
    
    reviewCon();  
    
    if (length(commands) > 2)
        filename = strcat(char(commands(3)), '.h');
    else
        filename = 'topology.h';
    end
    
    fileID = fopen(filename,'w');
    
    % File
    fprintf(fileID,'/**');
    fprintf(fileID,'\n  ******************************************************************************');
    fprintf(fileID,'\n  * File Name          : %s', filename);
    fprintf(fileID,'\n  * Date               : %s', date);
    fprintf(fileID,'\n  *	Author 			   : Asaad Kaadan');
    fprintf(fileID,'\n  * Description        : This file is automatically generated by the ''topogen'' MATLAB tool.');
    fprintf(fileID,'\n  *                      This version is compiled for MOWE-Tandy simulator use.');
    fprintf(fileID,'\n  *');
    fprintf(fileID,'\n  ******************************************************************************');
    fprintf(fileID,'\n  */');
    fprintf(fileID,'\n\n/* Includes ------------------------------------------------------------------*/');
    fprintf(fileID,'\n#include <stdint.h>');
    fprintf(fileID,'\n\n// Module Part Numbers:');
    fprintf(fileID,'\n// HO01R1: Hexagon receiver with VEMT3700 - 30mm optical resolution.');
    fprintf(fileID,'\n// HO01R2: Hexagon receiver with VEMT3700 - 20mm optical resolution.');
    fprintf(fileID,'\n// HO02R0: Hexagon transmitter  with VSMY2850G - 30mm optical resolution.');
    fprintf(fileID,'\n// HO02R1: Hexagon transmitter  with VSMY2850G - 20mm optical resolution.');
    fprintf(fileID,'\n// PO01R0: Pentagon receiver with VEMT3700 compatible with hexagons 30mm optical resolution.');
    fprintf(fileID,'\n// PO02R0: Pentagon transmitter  with VSMY2850G compatible with hexagons 30mm optical resolution.');
    fprintf(fileID,'\n\n// Enumerations');
    fprintf(fileID,'\nenum	modulePartNumbers{_HO01R1, _HO01R2, _HO02R0, _HO02R1, _PO01R0, _PO02R0};');
    fprintf(fileID,'\nenum	portPol{_normal, _reversed};');
    fprintf(fileID,'\nenum	modulePorts{_P1=1, _P2, _P3, _P4, _P5, _P6};');
    fprintf(fileID,'\n\n#define _N	%d					// Number of array modules', moduleID);   
    fprintf(fileID,'\n\n#define portCount	6');   
    fprintf(fileID,'\n\n// Array modules');
    for i = 1 : moduleID
        fprintf(fileID,'\n#define _mod%d	%d<<8', i, i);
    end   
    fprintf(fileID,'\n\n// Topology');
    fprintf(fileID,'\nstatic uint16_t array[_N][7] = {');
    for i = 1 : moduleID-1
        fprintf(fileID,'\n{ _%s, %s, %s, %s, %s, %s, %s},	// Module %d', pn(i), topo(1,i), topo(2,i), topo(3,i),...
            topo(4,i), topo(5,i), topo(6,i), i);
    end 
    fprintf(fileID,'\n{ _%s, %s, %s, %s, %s, %s, %s}    // Module %d', pn(moduleID), topo(1,moduleID), topo(2,moduleID),...
             topo(3,moduleID), topo(4,moduleID), topo(5,moduleID), topo(6,moduleID), moduleID);	
    fprintf(fileID,'\n};');    
    fprintf(fileID,'\n\n/************************ (C) COPYRIGHT OU *****END OF FILE****/\n');

    fclose(fileID);
end

%% Prepare toplogy matrix in the right format
function out = topo(port, module)
    
    temp = find(modules(4:9,module)==port);
    
    if (modules(temp+9,module) == 0)
        out = '0';
    else
        out = sprintf('_mod%d|_P%d', bitshift(modules(temp+9,module), -3), bitand(modules(temp+9,module), 7));
    end 
    
end

%% Get module part number
function out = pn(module)
    
    if (side == 17.32 && modules(1,module) == 1)       % 30mm TX
       out = 'HO02R0';
    elseif (side == 17.32 && modules(1,module) == 0)   % 30mm RX
       out = 'HO01R1';
    elseif (side == 17.32 && modules(1,module) == 0)   % 20mm TX
       out = 'HO02R1';
    elseif (side == 17.32 && modules(1,module) == 0)   % 20mm RX
       out = 'HO01R2';
    end 
end

%% Review array connections
function reviewCon()

    for i = 1 : moduleID
        % Check distance to other modules
        for j = 1 : moduleID
            dx = modules(2,i)-modules(2,j);
            dy = modules(3,i)-modules(3,j);
            dist = abs(sqrt(dx^2+dy^2)-height);
            
            if (hexaMode == 'h')
                if (dist <= 0.1 && j ~= i)       % Module j is adjacent to module i
                    % Figure out the neighbor orientation
                    if (dx < 0 && dy < 0)           % i:p1 > j:p4  
                        modules(1+9,i) = bitor(bitshift(j,3), modules(4+3,j));  
                        modules(4+9,j) = bitor(bitshift(i,3), modules(1+3,i));
                    elseif (dx < 0 && dy > 0)       % i:p2 > j:p5
                        modules(2+9,i) = bitor(bitshift(j,3), modules(5+3,j));
                        modules(5+9,j) = bitor(bitshift(i,3), modules(2+3,i));                  
                    elseif (dx == 0 && dy > 0)      % i:p3 > j:p6
                        modules(3+9,i) = bitor(bitshift(j,3), modules(6+3,j));   
                        modules(6+9,j) = bitor(bitshift(i,3), modules(3+3,i));                    
                    elseif (dx > 0 && dy > 0)       % i:p4 > j:p1
                        modules(4+9,i) = bitor(bitshift(j,3), modules(1+3,j)); 
                        modules(1+9,j) = bitor(bitshift(i,3), modules(4+3,i));                     
                    elseif (dx > 0 && dy < 0)       % i:p5 > j:p2
                        modules(5+9,i) = bitor(bitshift(j,3), modules(2+3,j));
                        modules(2+9,j) = bitor(bitshift(i,3), modules(5+3,i));                     
                    elseif (dx == 0 && dy < 0)      % i:p6 > j:p3
                        modules(6+9,i) = bitor(bitshift(j,3), modules(3+3,j)); 
                        modules(3+9,j) = bitor(bitshift(i,3), modules(6+3,i));                    
                    end            
                end 
            elseif (hexaMode == 'v')
                if (dist <= 0.1 && j ~= i)       % Module j is adjacent to module i
                    % Figure out the neighbor orientation
                    if (dx < 0 && dy < 0)           % i:p1 > j:p4  
                        modules(1+9,i) = bitor(bitshift(j,3), modules(4+3,j));  
                        modules(4+9,j) = bitor(bitshift(i,3), modules(1+3,i));
                    elseif (dx < 0 && dy == 0)      % i:p2 > j:p5
                        modules(2+9,i) = bitor(bitshift(j,3), modules(5+3,j));
                        modules(5+9,j) = bitor(bitshift(i,3), modules(2+3,i));                  
                    elseif (dx < 0 && dy > 0)       % i:p3 > j:p6
                        modules(3+9,i) = bitor(bitshift(j,3), modules(6+3,j));   
                        modules(6+9,j) = bitor(bitshift(i,3), modules(3+3,i));                    
                    elseif (dx > 0 && dy > 0)       % i:p4 > j:p1
                        modules(4+9,i) = bitor(bitshift(j,3), modules(1+3,j)); 
                        modules(1+9,j) = bitor(bitshift(i,3), modules(4+3,i));                     
                    elseif (dx > 0 && dy == 0)      % i:p5 > j:p2
                        modules(5+9,i) = bitor(bitshift(j,3), modules(2+3,j));
                        modules(2+9,j) = bitor(bitshift(i,3), modules(5+3,i));                     
                    elseif (dx > 0 && dy < 0)       % i:p6 > j:p3
                        modules(6+9,i) = bitor(bitshift(j,3), modules(3+3,j)); 
                        modules(3+9,j) = bitor(bitshift(i,3), modules(6+3,i));                    
                    end            
                end                                
            end
        end 
    end
    
end

%% Help command
function help()
    fprintf('\nList of available commands (''/'' refers to options and ''[]'' refer to default values):');
    fprintf('\n\nChoose module size [20mm]:');
    fprintf('\n\tsize 20mm/30mm');
    fprintf('\n\nChoose hexagon orientation horizontal or vertical [h]:');
    fprintf('\n\thexamode h/v');    
    fprintf('\n\nAdd a new module:');
    fprintf('\n\tadd hexa/penta tx/rx px>my:py');
    fprintf('\n\tPort ''px'' from this module connects to port ''py'' from module ''my''.');
    fprintf('\n\tEx.:');
    fprintf('\n\t\tadd hexa tx');
    fprintf('\n\t\tadd hexa rx p1>m1:p3');
    fprintf('\n\t\tadd hexa tx p3>m2:p6');
    fprintf('\n\nGenerate the header topology file [topology.h] for MCU firmware');
    fprintf('\n\tmake mcu filename');
    fprintf('\n\nGenerate the header topology file [topology.h] for MOWE-Tandy simulator');
    fprintf('\n\tmake sim filename');
    fprintf('\n\nQuit the software:');
    fprintf('\n\texit\n\n');  
end

%% Determine port polarity
function out = portPol(module, port)
    
    temp = find(modules(4:9,module)==port);
    
    if (module == 1)
        out = 'normal';
        modules(port+15,1) = 0;
    elseif (module > 1 && modules(temp+9,module) == 0)
        out = 'normal';
        modules(port+15,module) = 0;
    elseif (module > 1 && modules(temp+9,module) ~= 0)
        adjmodule = bitshift(modules(temp+9,module), -3);
        adjPort = bitand(modules(temp+9,module), 7);
        
        if (modules(port+15,module) == 0)
            modules(adjPort+15,adjmodule) = 1;
            out = 'reversed';
        elseif (modules(port+15,module) == 1)
            modules(adjPort+15,adjmodule) = 0;
            out = 'normal';
        end
        
    end
    
end
    

end