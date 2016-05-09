/**
  ******************************************************************************
  * File Name          : aos_clp.c
  * Created            : 07/22/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : AOS Command Line Parser (CLP).
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
// <<< Use Configuration Wizard in Context Menu >>>
// <h> CLP commands
//   <q> Info
//   		<i> Array information
#define _comInfo 1
//   <q> Status
//   		<i> Module Status
#define _comStatus 1
//   <q> Help
//   		<i> List of available commands
#define _comHelp 1
//   <q> Name
//   		<i> Name the modules (alias)
#define _comName 1
//   <q> Ping
//   		<i> Ping a module
#define _comPing 1
//   <q> Sample
//   		<i> Read one sample
#define _comSample 1
//   <q> Stream
//   		<i> Stream in/out using DMA
#define _comStream 1
//   <q> Read
//   		<i> Read data from receiver
#define _comRead 1
//   <q> Write
//   		<i> Write data to transmitter
#define _comWrite 1
//   <q> OnOff
//   		<i> Turn on/off a transmitter
#define _comOnOff 1
//   <q> Pulse
//   		<i> Pulse a transmitter
#define _comPulse 1
//   <q> PWM
//   		<i> Output a PWM signal on the transmitter
#define _comPWM 1
//   <q> Stop
//   		<i> Stop a stream/read/write process
#define _comStop 1
//   <q> Pause
//   		<i> Pause/Resume a stream/read/write process
#define _comPause 1
//   <q> Filter
//   		<i> Turn on/off the background noise filter
#define _comFilter 0
//   <q> SetZeroLevel
//   		<i> Set the zeros voltage level
#define _comSetZeroLevel 1
//   <q> SetOneLevel
//   		<i> Set the ones voltage level
#define _comSetOneLevel 1
//   <q> Link
//   		<i> Link multiple modules together
#define _comLink 1
//   <q> Unlink
//   		<i> Unlink multiple modules from each other
#define _comUnlink 1
//   <q> Group
//   		<i> Group multiple modules together
#define _comGroup 0
//   <q> Reset
//   		<i> Reset a module
#define _comReset 1
//   <q> RunBootloader
//   		<i> Force the module into bootloader mode
#define _comRunBootloader 1
//   <q> Route
//   		<i> Calculate the shortest route between two modules using Dijkstra's algorithm
#define _comRoute 1
//   <q> SetPdMode
//   		<i> Set receiver PD mode for digital/analog
#define _comSetPdMode 1
//   <q> SetLedMode
//   		<i> Set transmitter LED mode for digital/analog and binary/Ascii
#define _comSetLedMode 1
//   <q> SetComFormat
//   		<i> Set commands format (short/long)
#define _comComFormat 1
// </h>
// <<< end of configuration section >>>



/* Includes ------------------------------------------------------------------*/
#include "aos_clp.h"


/* Variables -----------------------------------------------------------------*/
char command[6][8][10] = {0};   	// Serial command buffer : 6 commands (one current and five old) * 8 keywords * 9 char per keyword
uint8_t pcPort = 0;
char cptr = 0;				// Current command
char sertemp[100] = {0};
uint16_t modBuf[_N] = {0};
uint16_t modStack = 1;
uint8_t comFormat = 0;
UART_HandleTypeDef *PCuart;		// Pointer to PC uart
uint8_t RXcom[10] = {0};			// User buffer
char chrComIdx = 0; 
char wordComIdx = 0;


/* Private function prototypes -----------------------------------------------*/
void checkModuleName(char* name);
char streamWarning(void);
char sphereWarning(void);
void welcomeMessage(void);
void pushCommand(uint8_t port);
void togglePrevCom(void);
void moduleStatus(void);


// commands
#if _comInfo
	int comInfo(void);
#endif
#if _comHelp
	int comHelp(void);
#endif
#if _comStatus
	int comStatus(char* moduleName);
#endif
#if _comName
	int comName(char* oldName, char* newName);
#endif
#if _comPing
	int comPing(char* moduleName);
#endif
#if _comSample
	int comSample(char* moduleName);
#endif
#if _comStream
	int comStream(char* moduleName, char* srcPort, char* dstPort);
#endif
#if _comRead
	int comRead(char* par1, char* par2, char* par3, char* par4);
#endif
#if _comWrite
	int comWrite(char* moduleName, char* par2, char* par3, char* par4);
#endif
#if _comOnOff
	int comOnOff(char* moduleName, uint8_t state);
#endif
#if _comPulse
	int comPulse(char* par1, char* par2);
#endif
#if _comPWM
	int comPWM(char* moduleName, char* period , char* width);
#endif
#if _comStop
	int comStop(char* moduleName, char* par2);
#endif
#if _comPause
	int comPause(char* par1);
	int comResume(char* par1);
#endif
#if _comFilter
	int comFilter(char* par1, char* par2);
#endif
#if _comSetZeroLevel
	int comSetZeroLevel(char* moduleName, char* value);
#endif
#if _comSetOneLevel
	int comSetOneLevel(char* moduleName, char* value);
#endif
#if _comLink
	int comLink(char par[8][10]);
#endif
#if _comUnlink
	int comUnlink(char* moduleRX, char* moduleTX);
#endif
#if _comGroup
	int comGroup(char par[][10]);
#endif
#if _comReset
	int comReset(char* moduleName);
#endif
#if _comRunBootloader
	int comRunBootloader(char* moduleName);
#endif
#if _comRoute
	int comRoute(char* source, char* destination);
#endif
#if _comSetPdMode
	int comSetPdMode(char* moduleName, char* value);
#endif
#if _comSetLedMode
	int comSetLedMode(char* moduleName, char* par2, char* par3, char* par4);
#endif
#if _comComFormat
	int comComFormat(char* value);
#endif


// --- Check if connected to user ---
void checkUser(void)
{
		// If user is not connected
		if (!pcPort)
		{
				for(char i=0 ; i<=5 ; i++)
				{
						if (DMAbuffer[i][0] == 13)
						{	
								pcPort = i+1;
								portStatus[pcPort] = ReceivingCommand;
								PCuart = getUart(pcPort);
								welcomeMessage();
								// Disable DMA
								disableDMAport(pcPort);	
								// Clear the buffer
								memset(DMAbuffer[i],0,10);	
								PCuart->State = HAL_UART_STATE_READY;
								break;
						}
				}
		}
		// If user is connected
		else
		{
				checkPx(PCuart); 
				if (PCReady == SET)
				{
						PCReady = RESET; 
						receiveCommand();
				}
		}			
}

// --------------------------------- User Commands --------------------------------------
#if _comInfo
// --- command : Array information ---
int comInfo(void)
{
		moduleStatus();
		
		send(pcPort, "\n\r*** Module Firmware ***\n");
    sprintf(sertemp, "\n\rFirmware version: %s",_firmVersion);
		send(pcPort, sertemp);
    sprintf(sertemp, "\n\rFirmware date:    %s",_firmDate);
		send(pcPort, sertemp);
    sprintf(sertemp, "\n\rFirmware time:    %s\n\r",_firmTime);
		send(pcPort, sertemp);
		
		send(pcPort, "\n\r*** Array Design ***\n");
		send(pcPort, "\n\r(Module:Port)\t\tP1\tP2\tP3\tP4\tP5\tP6\n\n\r");
		for(int row=0 ; row<=_N-1 ; row++)
		{
				sprintf(sertemp, "Module %d:\t",row+1);
				send(pcPort, sertemp);
				switch (array[row][0])
				{
						case _HO01R1 : 
								send(pcPort, "HO01R1\t"); break;
						case _HO01R2 :
								send(pcPort, "HO01R2\t"); break;
						case _HO01R3 :
								send(pcPort, "HO01R3\t"); break;
						case _HO02R0 :
								send(pcPort, "HO02R0\t"); break;
						case _HO02R1 :
								send(pcPort, "HO02R1\t"); break;
						case _HO02R2 :
								send(pcPort, "HO02R2\t"); break;
						case _PO01R0 :
								send(pcPort, "PO01R0\t"); break;
						case _PO02R0 :
								send(pcPort, "PO02R0\t"); break;
						default:
								break;
				}			
				for(int col=1 ; col<=6 ; col++)
				{
						if (!array[row][col])
								sprintf(sertemp,"%d\t",0);
						else
								sprintf(sertemp,"%d:%d\t",array[row][col]>>3,array[row][col]&0x07);
						send(pcPort, sertemp);		
				}
				send(pcPort, "\n\r");
		}
		
		send(pcPort, "\n");
		
		// Check the array status: Rediscover broadcast routes and neighboring modules
		//findBroadcastRoute();
		
		errFlag = NoErrors;
    return errFlag;
}
#endif
#if _comStatus
// --- command : Module status ---
int comStatus(char* moduleName)
{
		// Check syntax
		checkModuleName(moduleName); 
	
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto statusMeRoutine;
						else {
								message[0] = _MesID_status;		// status command
								mesSend(myID, atoi(moduleName+1));			
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto statusMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules

				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto statusMeRoutine;
						else {
								message[0] = _MesID_status;		// status command
								mesSend(myID, getID(moduleName));		
								// Exit
								return errFlag;
						}
				}
				
statusMeRoutine:				
				moduleStatus();							
		}
		 
    return errFlag;
}
#endif
#if _comHelp
// --- command : Array help ---
int comHelp(void)
{
		send(pcPort, "\n\n\rAvailable commmands (all lower case):\n\n\r");
#if _comInfo		
		send(pcPort, "info\n\r");
		send(pcPort, "..... List information about this array.\n\n\r");
#endif
#if _comStatus
		send(pcPort, "status #XXX\n\r");
		send(pcPort, "..... Get module #XXX (ID/alias/me) status.\n\n\r");
#endif
		send(pcPort, "help\n\r");
		send(pcPort, "..... List available commands.\n\n\r");
#if _comName
		send(pcPort, "name #XXX CoolName\n\r");
		send(pcPort, "..... Assign the alias name 'CoolName' to module #XXX.\n\n\r");
#endif
#if _comPing
		send(pcPort, "ping #XXX\n\r");
		send(pcPort, "..... Ping module #XXX (ID/alias/me/all).\n\n\r");
#endif
#if _comSample
		send(pcPort, "sample #XXX\n\r");
		send(pcPort, "..... Read one sample from receiver #XXX (ID/alias/me/all).\n\n\r");
#endif
#if _comStream
		send(pcPort, "stream #XXX PortX PortY\n\r");
		send(pcPort, "..... Inter-module streaming in module #XXX (ID/alias/me) from PortX to PortY.\n\r");
		send(pcPort, "      If PortY is not mentioned, then PortX is streamed in/out the front-end.\n\n\r");
		send(pcPort, "stream #XXX #YYY\n\r");
		send(pcPort, "..... Intra-module streaming from module #XXX to module #YYY.\n\n\r");
#endif
#if _comRead
		send(pcPort, "read #XXX rate port --noled\n\r");
		send(pcPort, "..... Read data from receiver #XXX (ID/alias/me/all) into 'port' using the 'rate' sample rate.\n\r");
		send(pcPort, "      The indicator LED is toggled at each sample. Use --noled to turn it off.\n\n\r");
#endif
#if _comWrite
		send(pcPort, "write #XXX rate port --noled\n\r");
		send(pcPort, "..... Write data to transmitter #XXX (ID/alias/me/all) from 'port' using the 'rate' sample rate.\n\r");
		send(pcPort, "      The indicator LED is toggled at each sample. Use --noled to turn it off.\n\n\r");
#endif
#if _comOnOff
		send(pcPort, "on #XXX\n\r");
		send(pcPort, "..... Turn transmitter #XXX (ID/alias/me/all) on.\n\n\r");
		send(pcPort, "off #XXX\n\r");
		send(pcPort, "..... Turn transmitter #XXX (ID/alias/me/all) off.\n\n\r");
		send(pcPort, "toggle #XXX\n\r");
		send(pcPort, "..... Toggle transmitter #XXX (ID/alias/me/all).\n\n\r");
#endif
#if _comPulse
		send(pcPort, "pulse #XXX width\n\r");
		send(pcPort, "..... Output a single pulse on transmitter #XXX (ID/alias/me/all) with 'width' in ms.\n\n\r");
#endif
#if _comPWM
		send(pcPort, "pwm #XXX period width\n\r");
		send(pcPort, "..... Output a PWM single pulse on transmitter #XXX (ID/alias/me/all) with 'period' and 'width' in micro seconds.\n\n\r");
#endif
#if _comStop
		send(pcPort, "stop #XXX (pwm)\n\r");
		send(pcPort, "..... Stop a stream/read/write process to/from module #XXX (ID/alias/me/all).\n\r");
		send(pcPort, "      Use 'pwm' to stop a transmitter PWM output.\n\n\r");
#endif
#if _comPause
		send(pcPort, "pause #XXX\n\r");
		send(pcPort, "..... Pause a stream/read/write process to/from module #XXX (ID/alias/me/all).\n\n\r");
		send(pcPort, "resume #XXX\n\r");
		send(pcPort, "..... Resume a stream/read/write process to/from module #XXX (ID/alias/me/all).\n\n\r");
#endif
#if _comFilter

#endif
#if _comSetZeroLevel
		send(pcPort, "set zerolevel #XXX level\n\r");
		send(pcPort, "..... Set the detector threshold for 'zeros' in receiver #XXX (ID/alias/me/all) to the 'level' in ADC units.\n\n\r");
#endif
#if _comSetOneLevel
		send(pcPort, "set onelevel #XXX level\n\r");
		send(pcPort, "..... Set the detector threshold for 'ones' in receiver #XXX (ID/alias/me/all) to the 'level' in ADC units.\n\n\r");
#endif
#if _comLink

#endif
#if _comUnlink

#endif
#if _comGroup

#endif
#if _comReset
		send(pcPort, "reset #XXX\n\r");
		send(pcPort, "..... Reset module #XXX (ID/alias/me/all).\n\n\r");
#endif
#if _comRunBootloader
		send(pcPort, "update #XXX\n\r");
		send(pcPort, "..... Force module #XXX (ID/alias/me) into bootloader mode for firmware upgrade.\n\n\r");
#endif
#if _comRoute
		send(pcPort, "route #XXX #YYY\n\r");
		send(pcPort, "..... Calculate the shortest route between two modules using Dijkstra's algorithm (source, destination).\n\n\r");
#endif
#if _comSetPdMode
		send(pcPort, "set pdmode #XXX analog/digital\n\r");
		send(pcPort, "..... Set the photodiode mode in receiver #XXX (ID/alias/me/all) to analog or digital.\n\r");
		send(pcPort, "      This applies to sampling and reading commands. Digital values will use the onelevel and zerolevel parameters.\n\n\r");
#endif
#if _comSetLedMode
		send(pcPort, "set ledmode #XXX analog/digital binary/ascii 8/16\n\r");
		send(pcPort, "..... Set the front-end LED mode in transmitter #XXX (ID/alias/me/all) to output zeros and ones from the incoming stream directly");
		send(pcPort, "(digital) or digitize the incoming stream using the onelevel and zerolevel parameters (analog). The stream is read as 'binary' or ");
		send(pcPort, "'ascii' data and each sample is '8' or '16' bits.\n\n\r");
#endif
#if _comComFormat
		send(pcPort, "com long/short\n\r");
		send(pcPort, "..... Setup commands format 'long' or 'short'.\n\n\r");
#endif

		errFlag = NoErrors;
		return errFlag;
}
#endif
#if _comName
// --- command : Name the modules (alias) ---
int comName(char* oldName, char* newName)
{
    // Check syntax
    checkModuleName(oldName);

    if (errFlag == NoErrors) {
				// TID - ID
				if (oldName[0] == '#') {
						strcpy(moduleAlias[atol(oldName+1)], newName);
						sprintf(sertemp,"\n\rModule #%03d is named %s\n\r",(int)atol(oldName+1),newName);			
				// TID - Me
				}	else if (!strcmp(oldName,"me")) {
						strcpy(moduleAlias[myID], newName);
						sprintf(sertemp,"\n\rI'm Module #%03d. My new name is %s\n\r",myID,newName);							
				// TID - Alias	
				} else {		
						strcpy(moduleAlias[getID(oldName)], newName);
						sprintf(sertemp,"\n\rModule #%03d is named %s\n\r",getID(oldName),newName);
				}       
				send(pcPort, sertemp);			
    }
		else if (errFlag == WrongID) {
				send(pcPort, "\n\rYou entered a wrong module ID! Please try again.\n\r");
		}
		else if (errFlag == ModuleDoesNotExist) {
				send(pcPort, "\n\rThe module ID you entered does not exist! Please try again.\n\r");
		}
		else if (errFlag == WrongAlias) {
				send(pcPort, "\n\rThe alias name you entered does not match any existing module! Please try again.\n\r");
		}		
		
    return errFlag;
}
#endif
#if _comPing
// --- command : Ping a module ---
int comPing(char* moduleName)
{
		// Check syntax
		checkModuleName(moduleName); 
	
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto pingMeRoutine;
						else {
								message[0] = _MesID_ping;		// ping command
								mesSend(myID, atoi(moduleName+1));			
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto pingMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;		// broadcast command
						message[1] = _MesID_ping;		// ping command
						broadcast(myID);			
						// Activate the modules stack. Expect response from all receiver modules
						modStack = _N;		// Change to _N
						modBuf[myID-1] = 1;					
						// Start timeout
						nblkTimout = 1000;
						nblkEvent = pingAllEvent;	
						// Exit
						return errFlag;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto pingMeRoutine;
						else {
								message[0] = _MesID_ping;		// ping command
								mesSend(myID, getID(moduleName));		
								// Exit
								return errFlag;
						}
				}
				
pingMeRoutine:				
				sprintf(sertemp,"\n\rHi from Module #%03d\n\r",myID); 
				send(pcPort, sertemp);
				nblkBlink(200);							
		}
		 
    return errFlag;
}
#endif
#if _comSample
// --- command : Read one sample ---
int comSample(char* moduleName)
{
		uint16_t temp = 0;
	
		// Check syntax
		checkModuleName(moduleName); 
	
		if (errFlag == NoErrors) 
		{
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID) {
								goto sampleMeRoutine;
						} else {
								temp = atoi(moduleName+1);
								// Check if receivers
								if (array[temp-1][0] == _HO01R1 || array[temp-1][0] == _HO01R2 || array[temp-1][0] == _HO01R3 || array[temp-1][0] == _PO01R0)
								{		
										message[0] = _MesID_sample;		// sample command
										mesSend(myID, temp);			
								}
								else
								{							
										sprintf(sertemp,"\n\rModule %d is a transmitter module!\n\r", temp);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto sampleMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						send(pcPort, "\n\r");	
						for (uint16_t i=1 ; i<=_N ; i++)
						{
								// Check if receivers
								if (array[i-1][0] == _HO01R1 || array[i-1][0] == _HO01R2 || array[i-1][0] == _HO01R3 || array[i-1][0] == _PO01R0)
								{
										sprintf(sertemp,"Module %d\t",i); 
										send(pcPort, sertemp);
								}									
						}
						send(pcPort, "\n\r");	
						message[0] = _MesID_broadcast;		// broadcast command
						message[1] = _MesID_sample;		// sample command
						broadcast(myID);
						// Activate the modules stack. Expect response from all receiver modules
						modStack = numOfRX+1;		
						#ifndef TX
								modBuf[myID-1] = samplePD();
						#endif
						// Start timeout
						nblkTimout = 1000;
						nblkEvent = samAllEvent;	
						// Exit
						return errFlag;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID) {
								goto sampleMeRoutine;
						} else {
								temp = getID(moduleName);
								// Check if receivers
								if (array[temp-1][0] == _HO01R1 || array[temp-1][0] == _HO01R2 || array[temp-1][0] == _HO01R3 || array[temp-1][0] == _PO01R0)
								{							
										message[0] = _MesID_sample;		// sample command
										mesSend(myID, temp);		
								}
								else
								{							
										sprintf(sertemp,"\n\rModule %d is a transmitter module!\n\r", temp);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				}
				
sampleMeRoutine:
#ifdef TX	
				send(pcPort, "\n\rThis is a transmitter module!\n\r");
#else				
				sprintf(sertemp,"\n\rModule #%03d: %d\n\r",myID,samplePD()); 
				send(pcPort, sertemp);
#endif	
				
		}
			
    return errFlag;
}
#endif
#if _comStream
// --- command : Stream between two modules or between two ports in the same module  ---
int comStream(char* moduleName, char* srcPort, char* dstPort)
{
		uint8_t src = 0; uint8_t dst = 0;
		uint16_t srcMod = 0; uint16_t dstMod = 0;
	
		// Check syntax
		// Check source port
		if (!strcmp(srcPort,"p1")) {
				src = P1;
		} else if (!strcmp(srcPort,"p2")) {
				src = P2;
		} else if (!strcmp(srcPort,"p3")) {
				src = P3;
		} else if (!strcmp(srcPort,"p4")) {
				src = P4;
		} else if (!strcmp(srcPort,"p5")) {
				src = P5;
		} else if (!strcmp(srcPort,"p6")) {
				src = P6;
		} else if (!strcmp(srcPort,"pc")) {
				src = pcPort;
		} else {
				checkModuleName(srcPort); 
				if (errFlag == NoErrors) 
				{
						if (moduleName[0] == '#')
								dstMod = atoi(srcPort+1);
						else
								dstMod = getID(srcPort);
						goto skipDstPort;
				}
				else
				{
						errFlag = WrongPort;
						return errFlag;
				}
		}
		// Check destination port
		if (!strcmp(dstPort,"p1")) {
				dst = P1;
		} else if (!strcmp(dstPort,"p2")) {
				dst = P2;
		} else if (!strcmp(dstPort,"p3")) {
				dst = P3;
		} else if (!strcmp(dstPort,"p4")) {
				dst = P4;
		} else if (!strcmp(dstPort,"p5")) {
				dst = P5;
		} else if (!strcmp(dstPort,"p6")) {
				dst = P6;
		} else if (!strcmp(dstPort,"pc")) {
				dst = pcPort;
		} else if (!strcmp(dstPort,"")) {
				dst = 0;
		} else {
				errFlag = WrongPort;
				return errFlag;
		}
		
skipDstPort:
		// Check module name
		checkModuleName(moduleName); 
	
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						srcMod = atoi(moduleName+1);
						// Message the target module and wait for the message
						if (srcMod == myID)
								goto streamMeRoutine;
						else {
								if (streamWarning())			// Display warning 
								{	
										message[0] = _MesID_streamDMA;		// streamDMA command
										message[1] = src;							// source port
										message[2] = dst;							// destination port
										message[3] = dstMod;					// destination module (ID is considered 8-bit)
										mesSend(myID, srcMod);			
								}									
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto streamMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules

				// TID - Alias	
				} else {		
						srcMod = getID(moduleName);
						// Message the target module	
						if (srcMod == myID)
								goto streamMeRoutine;
						else {
								if (streamWarning())			// Display warning 
								{	
										message[0] = _MesID_streamDMA;		// streamDMA command
										message[1] = src;							// source port
										message[2] = dst;							// destination port
										message[3] = dstMod;					// destination module (ID is considered 8-bit)
										mesSend(myID, srcMod);			
								}
								// Exit
								return errFlag;
						}
				}
				
streamMeRoutine:				
							
				if (streamWarning())			// Display warning 
				{	
						if (!dstMod)					// Inter-module (single module) streaming
						{			
								if (!dst) { 										// Stream port-to-fe/fe-to-port		
										if (!stream_FE_Port(src)) {		// Failed
											
										} else {
												send(pcPort, "\n\rStream added successfully.\n\r");
										}									
								} else {												// Stream port-to-port	
										if (!stream_Port_Port(src, dst)) {	// Failed
												send(pcPort, "\n\rCannot add the requested stream! All five port-to-port DMA streams are used.\n\r");
												// Restart port DMAs
												startDMAport(src, 10);
												startDMAport(dst, 10);
										} else {
												send(pcPort, "\n\rStream added successfully.\n\r");
										}
								}		
						}	
						else									// Intra-module (module to module) streaming
						{
								// SISO
								message[0] = _MesID_streamSISO;		// streamSISO command
								mesSend(myID, dstMod);	
						}	
				}
		}
		 
    return errFlag;
	
}
#endif
#if _comRead
// --- command : Read data from receiver ---
int comRead(char* moduleName, char* par2, char* par3, char* par4)
{		
		uint32_t rateFactor = 0;
	
		// -- Check syntax
		// Check rate
		if (!isNumeric(par2)) {
				errFlag = WrongRate;
				return errFlag;
		} else {
				rwComRate = atof(par2);
				rateFactor = floor((48*1000000)/rwComRate);	
				errFlag = NoErrors;
		}
		// Check LED status
		if (!strcmp(par4,"--noled"))
				rwComLED = 0;
		else 
				rwComLED = 1;
		
		// Check if streaming a sphere
//		if (!strcmp(moduleName,"sphere")) {
//				if (sphereWarning())
//				{
//						sprintf(sertemp,"read sphere %f %d", rwComRate, rwComLED); 
//						bcast(myID, sertemp);
//						//HAL_Delay(1000);
//						sphereSetup();		
//				}
//				return errFlag;
//		}
		
		// Check port/file
		if (!strcmp(par3,"p1")) {
				rwComPort = P1;
		} else if (!strcmp(par3,"p2")) {
				rwComPort = P2;
		} else if (!strcmp(par3,"p3")) {
				rwComPort = P3;
		} else if (!strcmp(par3,"p4")) {
				rwComPort = P4;
		} else if (!strcmp(par3,"p5")) {
				rwComPort = P5;
		} else if (!strcmp(par3,"p6")) {
				rwComPort = P6;
		} else if (!strcmp(par3,"pc")) {
				rwComPort = pcPort;
		} else {
				errFlag = WrongPort;
				return errFlag;
		}

		// Check module name	
		checkModuleName(moduleName); 
			
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto readMeRoutine;
						else {
								rwComDst = atoi(moduleName+1);
								// Check if receivers
								if (array[rwComDst-1][0] == _HO01R1 || array[rwComDst-1][0] == _HO01R2 || array[rwComDst-1][0] == _HO01R3 || array[rwComDst-1][0] == _PO01R0)
								{		
										streamMode = 2; 
										message[0] = _MesID_read;										// read command
										message[1] = (uint8_t)rateFactor;		// rwComRate: Low Byte
										message[2] = (uint8_t)(rateFactor>>8);
										message[3] = (uint8_t)(rateFactor>>16);
										message[4] = (uint8_t)(rateFactor>>24);
										message[5] = rwComLED;						// rwComLED
										mesSend(myID, rwComDst);	
										sprintf(sertemp,"\n\rReading from #%03d to port P%d at a rate of %f Hz\n\r",rwComDst, rwComPort, rwComRate); 
										send(rwComPort, sertemp);
								}
								else
								{							
										sprintf(sertemp,"\n\rModule %d is a transmitter module!\n\r", rwComDst);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto readMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						send(pcPort, "\n\r");	
						for (uint16_t i=1 ; i<=_N ; i++)
						{
								// Check if receivers
								if (array[i-1][0] == _HO01R1 || array[i-1][0] == _HO01R2 || array[i-1][0] == _HO01R3 || array[i-1][0] == _PO01R0)
								{
										sprintf(sertemp,"Module %d\t",i); 
										send(pcPort, sertemp);
								}									
						}
						send(pcPort, "\n\r");			
						streamMode = 4;
						startPerEvent(rateFactor);
						// Exit
						return errFlag;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto readMeRoutine;
						else {
								rwComDst = getID(moduleName);
								// Check if receivers
								if (array[rwComDst-1][0] == _HO01R1 || array[rwComDst-1][0] == _HO01R2 || array[rwComDst-1][0] == _PO01R0)
								{		
										streamMode = 2; 				
										message[0] = _MesID_read;										// read command
										message[1] = (uint8_t)rateFactor;		// rwComRate: Low Byte
										message[2] = (uint8_t)(rateFactor>>8);
										message[3] = (uint8_t)(rateFactor>>16);
										message[4] = (uint8_t)(rateFactor>>24);
										message[5] = rwComLED;						// rwComLED
										mesSend(myID, rwComDst);	
										sprintf(sertemp,"\n\rReading from #%03d to port P%d at a rate of %f Hz\n\r",rwComDst, rwComPort, rwComRate); 
										send(rwComPort, sertemp);
								}
								else
								{							
										sprintf(sertemp,"\n\rModule %d is a transmitter module!\n\r", rwComDst);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				}
				
readMeRoutine:				
#ifdef TX	
				send(pcPort, "\n\rThis is a transmitter module!\n\r");
#else				
				startPerEvent(rateFactor);
				sprintf(sertemp,"\n\rReading from #%03d to port P%d at a rate of %f Hz\n\r",myID, rwComPort, rwComRate);
				send(pcPort, sertemp);
#endif												
		}
		 
    return errFlag;	
}
#endif
#if _comWrite
// --- command : Write data to transmitter ---
int comWrite(char* moduleName, char* par2, char* par3, char* par4)
{
		uint32_t rateFactor = 0;
	
		// -- Check syntax
		// Check rate
		if (!isNumeric(par2)) {
				errFlag = WrongRate;
				return errFlag;
		} else {
				rwComRate = atof(par2);
				rateFactor = floor((48*1000000)/rwComRate);	
				errFlag = NoErrors;
		}
		// Check LED status
		if (!strcmp(par4,"--noled"))
				rwComLED = 0;
		else 
				rwComLED = 1;
		
		// Check port/file
		if (!strcmp(par3,"p1")) {
				rwComPort = P1;
		} else if (!strcmp(par3,"p2")) {
				rwComPort = P2;
		} else if (!strcmp(par3,"p3")) {
				rwComPort = P3;
		} else if (!strcmp(par3,"p4")) {
				rwComPort = P4;
		} else if (!strcmp(par3,"p5")) {
				rwComPort = P5;
		} else if (!strcmp(par3,"p6")) {
				rwComPort = P6;
		} else if (!strcmp(par3,"pc")) {
				rwComPort = pcPort;
		} else {
				errFlag = WrongPort;
				return errFlag;
		}

		// Check module name	
		checkModuleName(moduleName); 
			
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto writeMeRoutine;
						else {
								rwComDst = atoi(moduleName+1);
								// Check if transmitters
								if (array[rwComDst-1][0] == _HO02R0 || array[rwComDst-1][0] == _HO02R1 || array[rwComDst-1][0] == _PO02R0)
								{		
										streamMode = 2; 
										message[0] = _MesID_write;										// write command
										message[1] = (uint8_t)rateFactor;		// rwComRate: Low Byte
										message[2] = (uint8_t)(rateFactor>>8);
										message[3] = (uint8_t)(rateFactor>>16);
										message[4] = (uint8_t)(rateFactor>>24);
										message[5] = rwComLED;						// rwComLED
										mesSend(myID, rwComDst);	
										sprintf(sertemp,"\n\rWriting to #%03d from port P%d at a rate of %f Hz\n\r",rwComDst, rwComPort, rwComRate); 
										send(rwComPort, sertemp);
								}
								else
								{							
										sprintf(sertemp,"\n\rModule %d is a receiver module!\n\r", rwComDst);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto writeMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						sprintf(sertemp,"\n\rWriting to all transmitters from port P%d at a rate of %f Hz\n\r", rwComPort, rwComRate); 
						send(rwComPort, sertemp);	
						streamMode = 5;
						startPerEvent(rateFactor);
						// Exit
						return errFlag;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto writeMeRoutine;
						else {
								rwComDst = getID(moduleName);
								// Check if transmitters
								if (array[rwComDst-1][0] == _HO02R0 || array[rwComDst-1][0] == _HO02R1 || array[rwComDst-1][0] == _PO02R0)
								{		
										streamMode = 2; 				
										message[0] = _MesID_write;										// write command
										message[1] = (uint8_t)rateFactor;		// rwComRate: Low Byte
										message[2] = (uint8_t)(rateFactor>>8);
										message[3] = (uint8_t)(rateFactor>>16);
										message[4] = (uint8_t)(rateFactor>>24);
										message[5] = rwComLED;						// rwComLED
										mesSend(myID, rwComDst);	
										sprintf(sertemp,"\n\rWriting to #%03d from port P%d at a rate of %f Hz\n\r",rwComDst, rwComPort, rwComRate); 
										send(rwComPort, sertemp);
								}
								else
								{							
										sprintf(sertemp,"\n\rModule %d is a receiver module!\n\r", rwComDst);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				}
				
writeMeRoutine:				
#ifndef TX	
				send(pcPort, "\n\rThis is a receiver module!\n\r");
#else							
				if (rwComPort != pcPort)
				{
						// Disable old DMA
						disableDMAport(rwComPort);	
						// Clear the buffer
						memset(DMAbuffer[rwComPort-1],0,10);
				}
				// Start new DMA
				startDMAport(rwComPort, 1);
				// Disable UART DMA ISR
				disableDMAISR(rwComPort);
				
				startPerEvent(rateFactor);
				sprintf(sertemp,"\n\rWriting to #%03d from port P%d at a rate of %f Hz\n\r",myID, rwComPort, rwComRate);
				send(pcPort, sertemp);
				send(pcPort, "This port will be locked. Please use another port to unlock it.\n\r");
				pcPort = 0;
#endif												
		}
		 
    return errFlag;	
}
#endif
#if _comOnOff
// --- command : Turn on/off a transmitter ---
int comOnOff(char* moduleName, uint8_t state)
{
		// Check syntax	
		checkModuleName(moduleName); 		
		
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto setTXRoutine;
						else {
								if (state == 2)
								{
										message[0] = _MesID_LEDtoggleFE;		// toggle command
										sprintf(sertemp,"\n\rModule %d is TOGGLED\n\r",atoi(moduleName+1));
								}
								else if (state == 1)
								{
										message[0] = _MesID_LEDonFE;		// on command
										sprintf(sertemp,"\n\rModule %d is ON\n\r",atoi(moduleName+1));
								}
								else if (state == 0)
								{
										message[0] = _MesID_LEDoffFE;		// off command
										sprintf(sertemp,"\n\rModule %d is OFF\n\r",atoi(moduleName+1));
								}
								mesSend(myID, atoi(moduleName+1));	
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto setTXRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;		// broadcast command
						if (state == 2)	
						{							
								message[1] = _MesID_LEDtoggleFE;		// toggle command
								#ifdef TX
										FE_LED_toggle();
								#endif
								send(pcPort, "\n\rAll transmitter modules are TOGGLED\n\r");
						}
						else if (state == 1)
						{
								message[1] = _MesID_LEDonFE;		// on command
								#ifdef TX
										FE_LED_on();
								#endif
								send(pcPort, "\n\rAll transmitter modules are ON\n\r");
						}
						else if (state == 0)
						{
								message[1] = _MesID_LEDoffFE;		// off command
								#ifdef TX
										FE_LED_off();
								#endif
								send(pcPort, "\n\rAll transmitter modules are OFF\n\r");
						}
						broadcast(myID);
						// Exit
						return errFlag;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto setTXRoutine;
						else {
								if (state == 2)
								{
										message[0] = _MesID_LEDtoggleFE;		// toggle command
										sprintf(sertemp,"\n\rModule %d is TOGGLED\n\r",getID(moduleName));
								}
								else if (state == 1)
								{
										message[0] = _MesID_LEDonFE;		// on command
										sprintf(sertemp,"\n\rModule %d is ON\n\r",getID(moduleName));
								}
								else if (state == 0)
								{
										message[0] = _MesID_LEDoffFE;		// off command
										sprintf(sertemp,"\n\rModule %d is OFF\n\r",getID(moduleName));
								}
								mesSend(myID, getID(moduleName));
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				}
				
setTXRoutine:				
				#ifdef TX
				if (state == 2) {
						FE_LED_toggle();
						if (FE_LED_state())
								sprintf(sertemp,"\n\rModule %d is ON\n\r",myID);
						else
								sprintf(sertemp,"\n\rModule %d is OFF\n\r",myID);
						send(pcPort, sertemp);
				} else if (state == 1) {
						FE_LED_on();
						sprintf(sertemp,"\n\rModule %d is ON\n\r",myID);
						send(pcPort, sertemp);
				} else if (state == 0) {
						FE_LED_off();
						sprintf(sertemp,"\n\rModule %d is OFF\n\r",myID);
						send(pcPort, sertemp);
				}
				#else
				;				
				#endif							
		}
		 
    return errFlag;	
}
#endif
#if _comPulse
// --- command : Pulse a transmitter ---
int comPulse(char* moduleName, char* par2)
{
		uint16_t width = 0; uint16_t temp = 0;
	
		// -- Check syntax
		// Check width
		if (!isNumeric(par2)) {
				errFlag = WrongWidth;
				return errFlag;
		} else {
				width = atoi(par2);
				errFlag = NoErrors;
		}

		// Check module name	
		checkModuleName(moduleName); 
			
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto pulseMeRoutine;
						else {
								temp = atoi(moduleName+1);
								// Check if transmitters
								if (array[temp-1][0] == _HO02R0 || array[temp-1][0] == _HO02R1 || array[temp-1][0] == _PO02R0)
								{		
										message[0] = _MesID_LEDpulseFE;		// pulse command
										message[1] = width;		
										mesSend(myID, temp);
								}
								else
								{							
										sprintf(sertemp,"\n\rModule #%03d is a receiver module!\n\r", temp);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto pulseMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;		// broadcast command
						message[1] = _MesID_LEDpulseFE;		// pulse command
						message[2] = width;		
						broadcast(myID);
						#ifdef TX
								FE_nblkBlink(width);
						#endif
						// Exit
						return errFlag;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto pulseMeRoutine;
						else {
								temp = getID(moduleName);
								// Check if transmitters
								if (array[temp-1][0] == _HO02R0 || array[temp-1][0] == _HO02R1 || array[temp-1][0] == _PO02R0)
								{		
										message[0] = _MesID_LEDpulseFE;		// pulse command
										message[1] = width;		
										mesSend(myID, temp);
								}
								else
								{							
										sprintf(sertemp,"\n\rModule %d is a receiver module!\n\r", temp);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				}
				
pulseMeRoutine:				
#ifdef TX	
				FE_nblkBlink(width);				
#else				
				send(pcPort, "\n\rThis is a receiver module!\n\r");
#endif												
		}
		 
    return errFlag;	
}
#endif
#if _comPWM
// --- command : utput a PWM signal on a transmitter ---
int comPWM(char* moduleName, char* par2, char* par3)
{
		uint16_t period = 0; uint16_t width = 0; uint16_t temp = 0;
		float rate = 0; float duty = 0;
	
		// -- Check syntax
		// Check period
		if (!isNumeric(par2)) {
				errFlag = WrongWidth;
				return errFlag;
		} else {
				period = atoi(par2);
				rate = (float)1000000/(float)period;
				errFlag = NoErrors;
		}
		// Check width
		if (!isNumeric(par3)) {
				errFlag = WrongWidth;
				return errFlag;
		} else {
				width = atoi(par3);
				duty = ((float)width/(float)period)*100;
				errFlag = NoErrors;
		}
		
		// Check module name	
		checkModuleName(moduleName); 
			
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto pwmMeRoutine;
						else {
								temp = atoi(moduleName+1);
								// Check if transmitters
								if (array[temp-1][0] == _HO02R0 || array[temp-1][0] == _HO02R1 || array[temp-1][0] == _PO02R0)
								{		
										message[0] = _MesID_LEDpwmFE;		// pwm command
										message[1] = (uint8_t)period;				// Low Byte
										message[2] = (uint8_t)(period>>8);	// High Byte
										message[3] = (uint8_t)width;				// Low Byte
										message[4] = (uint8_t)(width>>8);		// High Byte
										mesSend(myID, temp);
										sprintf(sertemp,"\n\rModule #%03d is transmitting a PWM signal at %f Hz and %f%% duty cycle.\n\r", temp, rate, duty);
										send(pcPort, sertemp);
								}
								else
								{							
										sprintf(sertemp,"\n\rModule #%03d is a receiver module!\n\r", temp);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto pwmMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;		// broadcast command
						message[1] = _MesID_LEDpwmFE;		// pwm command
						message[2] = (uint8_t)period;				// Low Byte
						message[3] = (uint8_t)(period>>8);	// High Byte
						message[4] = (uint8_t)width;				// Low Byte
						message[5] = (uint8_t)(width>>8);		// High Byte
						broadcast(myID);
						#ifdef TX
								startPWM_FE(period, width);
						#endif
						sprintf(sertemp,"\n\rAll transmitter modules are transmitting a PWM signal at %f Hz and %f%% duty cycle.\n\r", rate, duty);
						send(pcPort, sertemp);
						// Exit
						return errFlag;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto pwmMeRoutine;
						else {
								temp = getID(moduleName);
								// Check if transmitters
								if (array[temp-1][0] == _HO02R0 || array[temp-1][0] == _HO02R1 || array[temp-1][0] == _PO02R0)
								{		
										message[0] = _MesID_LEDpwmFE;		// pwm command
										message[1] = (uint8_t)period;				// Low Byte
										message[2] = (uint8_t)(period>>8);	// High Byte
										message[3] = (uint8_t)width;				// Low Byte
										message[4] = (uint8_t)(width>>8);		// High Byte
										mesSend(myID, temp);
										sprintf(sertemp,"\n\rModule #%03d is transmitting a PWM signal at %f Hz and %f%% duty cycle.\n\r", temp, rate, duty);
										send(pcPort, sertemp);
								}
								else
								{							
										sprintf(sertemp,"\n\rModule %d is a receiver module!\n\r", temp);
										send(pcPort, sertemp);
								}
								// Exit
								return errFlag;
						}
				}
				
pwmMeRoutine:				
#ifdef TX	
				startPWM_FE(period, width);		
				sprintf(sertemp,"\n\rModule #%03d is transmitting a PWM signal at %.02f Hz and %.02f%% duty cycle.\n\r", myID, rate, duty);
				send(pcPort, sertemp);
#else				
				send(pcPort, "\n\rThis is a receiver module!\n\r");
#endif												
		}
		 
    return errFlag;	
}
#endif
#if _comStop
// --- command : Stop reading/writing from/to a module ---
int comStop(char* moduleName, char* par2)
{		
		uint8_t stopPWM = 0;
	
		// -- Check syntax
		checkModuleName(moduleName); 
		
		// Stop PWM
		if (!strcmp(par2,"pwm")) {
				stopPWM = 1;
		}
		
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto stopMeRoutine;
						else {
								message[0] = _MesID_stop;		// stop command
								if (stopPWM)
										message[1] = _MesID_stopPWM;		// stopPWM command
								mesSend(myID, atoi(moduleName+1));
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto stopMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;		// broadcast command
						message[1] = _MesID_stop;		// stop command
						if (stopPWM)
								message[2] = _MesID_stopPWM;		// stopPWM command
						broadcast(myID);
						goto stopMeRoutine;				
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto stopMeRoutine;
						else {
								message[0] = _MesID_stop;		// stop command
								if (stopPWM)
										message[1] = _MesID_stopPWM;		// stopPWM command
								mesSend(myID, getID(moduleName));
								// Exit
								return errFlag;
						}
				}
				
stopMeRoutine:	

				if (stopPWM)
				{
						// Check if PWM is on
						if ((&htim3.Instance->CCER || (TIM_CCER_CC1E << TIM_CHANNEL_3)) == TIM_CCx_ENABLE)
						{
								HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
								HAL_TIM_Base_DeInit(&htim3);
								// Reset FE LED setup
							#ifdef TX
								IR_LED_Init();
							#endif
						}
				}
				else
				{
						streamMode = 0;
						rwComRate = 0;
						rwComPort = 0;
						rwComLED = 0; 
						myTrig = 0;
						modStack = 1;
						LED_off();
						HAL_TIM_Base_Stop_IT(&htim2);
						
						#ifdef TX				
						// Disable old DMA
						disableDMAport(rwComPort);	
						// Clear the buffer
						memset(DMAbuffer[rwComPort-1],0,10);
						// Restart port DMA
						startDMAport(rwComPort, 10);		
						#else				
						;
						#endif				
				}					
		}
		 
    return errFlag;				
}
#endif
#if _comPause
// --- command : Pause reading/writing from/to a module ---
int comPause(char* moduleName)
{
		// -- Check syntax
		checkModuleName(moduleName); 
		
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto pauseMeRoutine;
						else {
								message[0] = _MesID_pause;		// pause command
								mesSend(myID, atoi(moduleName+1));
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto pauseMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;		// broadcast command
						message[1] = _MesID_pause;		// pause command
						broadcast(myID);
						goto pauseMeRoutine;			
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto pauseMeRoutine;
						else {
								message[0] = _MesID_pause;		// pause command
								mesSend(myID, getID(moduleName));
								// Exit
								return errFlag;
						}
				}
				
pauseMeRoutine:	

				myTrig = 0;
				modStack = 1;
				HAL_TIM_Base_Stop_IT(&htim2);												
		}
		 
    return errFlag;		
}
// --- command : Resume reading/writing from/to a module ---
int comResume(char* moduleName)
{
		// -- Check syntax
		checkModuleName(moduleName); 
		
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto resumeMeRoutine;
						else {
								message[0] = _MesID_resume;		// resume command
								mesSend(myID, atoi(moduleName+1));
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto resumeMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;		// broadcast command
						message[1] = _MesID_resume;		// resume command
						broadcast(myID);
						goto resumeMeRoutine;			
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto resumeMeRoutine;
						else {
								message[0] = _MesID_resume;		// resume command
								mesSend(myID, getID(moduleName));
								// Exit
								return errFlag;
						}
				}
				
resumeMeRoutine:	

				HAL_TIM_Base_Start_IT(&htim2);												
		}
		 
    return errFlag;		
}
#endif
#if _comFilter
// --- command : Turn on/off the background noise filter ---
int comFilter(char* par1, char* par2)
{
    if (par1[0] == '#') {
        char* moduleName = par1;   
				checkModuleName(moduleName);			// Check syntax
        if (!strcmp(par2,"on") && errFlag == NoErrors) {   
            filterStatus = 1;
            errFlag = NoErrors;
            sprintf(sertemp,"\n\rfilter module %s %s \n\r",moduleName,par2); 
						send(pcPort, sertemp);
        } else if (!strcmp(par2,"off") && errFlag == NoErrors) {           
            filterStatus = 0;
            errFlag = NoErrors;
            sprintf(sertemp,"\n\rfilter module %s %s \n\r",moduleName,par2); 
						send(pcPort, sertemp);
        }
    } else {
        if (!strcmp(par1,"on")) {
            filterStatus = 1;
            errFlag = NoErrors;
            sprintf(sertemp,"\n\rfilter array %s \n\r",par1); 
						send(pcPort, sertemp);
        } else if (!strcmp(par1,"off")) {
            filterStatus = 0;
            errFlag = NoErrors;
            sprintf(sertemp,"filter array %s \n\r",par1); 
						send(pcPort, sertemp);
        }
    }
    
    return errFlag;
}
#endif
#if _comSetZeroLevel
// --- command : Set the zeros voltage level ---
int comSetZeroLevel(char* moduleName, char* value)
{
		uint16_t level = 0;
	
		// Check syntax	
		// Check level
		if (!isNumeric(value)) {
				errFlag = WrongRate;
				return errFlag;
		} else {
				level = atof(value);
				checkModuleName(moduleName); 
		}
		
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto setZeroMeRoutine;
						else {
								message[0] = _MesID_set;					// set command
								message[1] = _MesID_setzerolevel;					// zerolevel
								message[2] = (uint8_t)level; 				// Low Byte
								message[3] = (uint8_t)(level>>8);  	// High Byte								
								mesSend(myID, atoi(moduleName+1));								
								sprintf(sertemp,"\r\nModule %s 'zeros' level has been set to %d\r\n",moduleName,level); 
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto setZeroMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;					// broadcast command
						message[1] = _MesID_set;					// set command
						message[2] = _MesID_setzerolevel;					// zerolevel
						message[3] = (uint8_t)level; 				// Low Byte
						message[4] = (uint8_t)(level>>8);  	// High Byte			
						broadcast(myID);
						goto setZeroMeRoutine;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto setZeroMeRoutine;
						else {
								message[0] = _MesID_set;					// set command
								message[1] = _MesID_setzerolevel;					// zerolevel
								message[2] = (uint8_t)level; 				// Low Byte
								message[3] = (uint8_t)(level>>8);  	// High Byte			
								mesSend(myID, getID(moduleName));		
								sprintf(sertemp,"\r\nModule %s 'zeros' level has been set to %d\r\n",moduleName,level); 
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				}
				
setZeroMeRoutine:				
				#ifndef TX
				sprintf(sertemp,"\r\nModule %d 'zeros' level has been set to %d\r\n",myID,level); 
				send(pcPort, sertemp);
				zerolevel = level;
				#else
				;				
				#endif							
		}
		 
    return errFlag;	
}
#endif
#if _comSetOneLevel
// --- command : Set the ones voltage level ---
int comSetOneLevel(char* moduleName, char* value)
{
		uint16_t level = 0;
	
		// Check syntax	
		// Check level
		if (!isNumeric(value)) {
				errFlag = WrongRate;
				return errFlag;
		} else {
				level = atof(value);
				checkModuleName(moduleName); 
		}
		
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto setOneMeRoutine;
						else {
								message[0] = _MesID_set;					// set command
								message[1] = _MesID_setonelevel;					// onelevel
								message[2] = (uint8_t)level; 				// Low Byte
								message[3] = (uint8_t)(level>>8);  	// High Byte			
								mesSend(myID, atoi(moduleName+1));	
								sprintf(sertemp,"\r\nModule %s 'ones' level has been set to %d\r\n",moduleName,level); 
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto setOneMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;					// broadcast command
						message[1] = _MesID_set;					// set command
						message[2] = _MesID_setonelevel;					// onelevel
						message[3] = (uint8_t)level; 				// Low Byte
						message[4] = (uint8_t)(level>>8);  	// High Byte			
						broadcast(myID);
						goto setOneMeRoutine;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto setOneMeRoutine;
						else {
								message[0] = _MesID_set;					// set command
								message[1] = _MesID_setonelevel;					// onelevel
								message[2] = (uint8_t)level; 				// Low Byte
								message[3] = (uint8_t)(level>>8);  	// High Byte			
								mesSend(myID, getID(moduleName));	
								sprintf(sertemp,"\r\nModule %s 'ones' level has been set to %d\r\n",moduleName,level); 
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				}
				
setOneMeRoutine:				
				#ifndef TX
				sprintf(sertemp,"\r\nModule %d 'ones' level has been set to %d\r\n",myID,level); 
				send(pcPort, sertemp);
				onelevel = level;
				#else
				;
				#endif							
		}
		 
    return errFlag;	
}
#endif
#if _comLink
// --- command : Link multiple modules together ---
int comLink(char par[8][10])
{
    char moduleRX[5];
    char moduleTX[8][5];

    strcpy(moduleRX,par[1]);
    checkModuleName(moduleRX);       // Check syntax
	
    sprintf(sertemp,"\n\rLink RX %s TX",par[1]); 
		send(pcPort, sertemp);
	
    for ( int i=2 ; i<=7 ; i++ ) {
        if (par[i] != 0) {
            strcpy(moduleTX[i-2],par[i]);
            checkModuleName(par[i]);       // Check syntax
            if (errFlag == NoErrors) {
                sprintf(sertemp," %s",par[i]); 
								send(pcPort, sertemp);
            }
        }
    }
    send(pcPort, "\n\r");
    
    return errFlag;
}
#endif
#if _comUnlink
// --- command : Unlink multiple modules from each other ---
int comUnlink(char* moduleRX, char* moduleTX)
{		
		// Check syntax
    checkModuleName(moduleRX); errFlag = Error;       
		checkModuleName(moduleTX);
	
    if (errFlag == NoErrors) {
        sprintf(sertemp,"\n\rUnlink RX %s TX %s \n\r",moduleRX,moduleTX); 
				send(pcPort, sertemp);
    }
    
    return errFlag;
}
#endif
#if _comGroup
// --- command : Group multiple modules together ---
int comGroup(char par[][10])
{
    char* groupName = par[1];
    char moduleName[8][5];

    sprintf(sertemp,"\n\rGroup %s ",groupName); 
		send(pcPort, sertemp);
    for ( int i=2 ; i<10 ; i++ ) {
        if (par[i] != 0) {
            strcpy(moduleName[i-2],par[i]);
            checkModuleName(par[i]);       // Check syntax
            if (errFlag == NoErrors) {
                sprintf(sertemp," %s",par[i]); 
								send(pcPort, sertemp);
            }
        }
    }
    send(pcPort, "\n\r");
    
    return errFlag;
}
#endif
#if _comRoute
// --- command : Calculate the shortest route between two modules using Dijkstra's algorithm ---
int comRoute(char* sourceID, char* desID)
{
		uint16_t source = 0; uint16_t destination = 0;
	
		// Check syntax
    checkModuleName(sourceID);  
		if (errFlag == NoErrors) 
		{
				errFlag = Error;
				checkModuleName(desID);
				if (errFlag == NoErrors) 
				{
						// Retrieve source and destination IDs
						if (sourceID[0] == '#') {				// From ID
								source = atol(sourceID+1);
						} else {												// From alias
								source = getID(sourceID);
						}
						if (desID[0] == '#') {
								destination = atol(desID+1);
						} else {
								destination = getID(desID);
						}						
						// Calculate the route
						outPort = findRoute(source, destination);
						sprintf(sertemp,"\n\rYou are in Module %d, take port %d to reach module %d\n\r", source, outPort, destination); 
						send(pcPort, sertemp);
						// Display the route
						sprintf(sertemp, "Number of hops = %d\n\r", routeDist[destination-1]); 
						send(pcPort, sertemp);
						send(pcPort, "Shortest route:\n\r");
						sprintf(sertemp, "Module %d",source);	
						send(pcPort, sertemp);
						for (int i=_N-1 ; i>=0 ; i--)  
						{
								if (route[i]) {
										sprintf(sertemp," >> Module %d",route[i]);	
										send(pcPort, sertemp);
								}
						}
				}	
		}
    send(pcPort, "\n\r");
		
    return errFlag;	
}
#endif
#if _comReset
// --- command : Reset a module ---
int comReset(char* moduleName)
{
		// Check syntax
		checkModuleName(moduleName); 
	
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto resetMeRoutine;
						else {
								message[0] = _MesID_reset;					// reset command
								mesSend(myID, atoi(moduleName+1));				
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto resetMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;		// broadcast command
						message[1] = _MesID_reset;			// reset command
						broadcast(myID);
						goto resetMeRoutine;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto resetMeRoutine;
						else {
								message[0] = _MesID_reset;					// reset command
								mesSend(myID, getID(moduleName));				
								// Exit
								return errFlag;
						}
				}
				
resetMeRoutine:				
				NVIC_SystemReset();							
		}
		 
    return errFlag;	
}
#endif
#if _comRunBootloader
// --- command : Force the module into bootloader mode ---
int comRunBootloader(char* moduleName)
{	
		// Check syntax
		checkModuleName(moduleName); 
	
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto updateMeRoutine;
						else {
//								if (atoi(moduleName+1) == 3)
//								{
//										outPort = findRoute(myID, atoi(moduleName+1));
//																	
//										MX_DMA_PORT_PORT_LINK1_Setup(P1uart, P4uart);
//										MX_DMA_PORT_PORT_LINK2_Setup(P4uart, P1uart);
//									
//										*P1uart.Init.BaudRate = 57600;
//										HAL_UART_Init(P1uart);
//									
//										*P4uart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
//										*P4uart.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
//										*P4uart.Init.BaudRate = 57600;
//										HAL_UART_Init(P4uart);
//									
//								}
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto updateMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
					
						// Exit
						return errFlag;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto updateMeRoutine;
						else {

								// Exit
								return errFlag;
						}
				}
				
updateMeRoutine:				
				sprintf(sertemp,"\n\rUpdate firmware for module #%03d\n\r",myID); 
				send(pcPort, sertemp);
				send(pcPort, "\n\rThe module will be forced into bootloader mode.\n\r");
				send(pcPort, "Please use the \"STM Flash Loader Demonstrator\" utility to update the firmware.\n\r");
				send(pcPort, "\n     *** Important ***\n\r");
				send(pcPort, "If this module is connected directly to PC please close this port first.\n\r");	
				
				*((unsigned long *)0x20007FF0) = 0xDEADBEEF; //  Address for RAM signature (STM32F09x) - Last 4 words of SRAM
				
				NVIC_SystemReset();					
		}
		 
    return errFlag;	 
}
#endif
#if _comSetPdMode
// --- command : Set receiver photodiode mode for digital/analog ---
int comSetPdMode(char* moduleName, char* value)
{
		// Check syntax
		checkModuleName(moduleName); 
	
		// Read paramters
		if (!strcmp(value,"analog"))
				pdMode = 0; 					// Analog		
		else if (!strcmp(value,"digital"))				
				pdMode = 1; 					// Digital	
								
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto setpdMeRoutine;
						else {
								message[0] = _MesID_set;					// set command
								message[1] = _MesID_SetPdMode;					// pdMode
								message[2] = pdMode;
								mesSend(myID, atoi(moduleName+1));	
								sprintf(sertemp,"\r\nModule %s PD mode has been set to %s\r\n",moduleName,value); 
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto setpdMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;					// broadcast command
						message[1] = _MesID_set;					// set command
						message[2] = _MesID_SetPdMode;					// pdMode
						message[3] = pdMode;
						broadcast(myID);
						goto setpdMeRoutine;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto setpdMeRoutine;
						else {
								message[0] = _MesID_set;					// set command
								message[1] = _MesID_SetPdMode;					// pdMode
								message[2] = pdMode;
								mesSend(myID, getID(moduleName));	
								sendPacket(outPort, sertemp);
								sprintf(sertemp,"\r\nModule %s PD mode has been set to %s\r\n",moduleName,value); 
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				}
				
setpdMeRoutine:				
				#ifndef TX
				sprintf(sertemp,"\r\nModule %d PD mode has been set to %s\r\n",myID,value); 
				send(pcPort, sertemp);
				#else
				;
				#endif							
		}
		 
    return errFlag;	
}
#endif
#if _comSetLedMode
// --- command : Set transmitter LED mode for digital/analog and binary/Ascii ---
int comSetLedMode(char* moduleName, char* par2, char* par3, char* par4)
{
		// Check syntax
		checkModuleName(moduleName); 

		// Read paramters
		if (!strcmp(par2,"digital") && !strcmp(par3,"ascii") && !strcmp(par4,"8"))			
				ledMode = 0; 				
		else if (!strcmp(par2,"digital") && !strcmp(par3,"binary") && !strcmp(par4,"8"))				
				ledMode = 1; 	
		else if (!strcmp(par2,"analog") && !strcmp(par3,"binary") && !strcmp(par4,"8"))
				ledMode = 2; 				
		
		if (errFlag == NoErrors) {
				// TID - ID
				if (moduleName[0] == '#') {
						// Message the target module and wait for the message
						if (atoi(moduleName+1) == myID)
								goto setLedMeRoutine;
						else {
								message[0] = _MesID_set;							// set command
								message[1] = _MesID_SetLedMode;				// ledMode
								message[2] = ledMode; 						
								mesSend(myID, atoi(moduleName+1));	
								sprintf(sertemp,"\r\nModule %d LED mode has been set to %s %s %s bits/sample.\r\n",atoi(moduleName+1),par2,par3,par4); 
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				// TID - Me
				}	else if (!strcmp(moduleName,"me")) {
						goto setLedMeRoutine;
				// TID - All
				}	else if (!strcmp(moduleName,"all")) {
						// Message all other modules
						message[0] = _MesID_broadcast;					// broadcast command
						message[1] = _MesID_set;								// set command
						message[2] = _MesID_SetLedMode;					// ledMode
						message[3] = ledMode; 	
						broadcast(myID);
						goto setLedMeRoutine;
				// TID - Alias	
				} else {		
						// Message the target module	
						if (getID(moduleName) == myID)
								goto setLedMeRoutine;
						else {
								message[0] = _MesID_set;							// set command
								message[1] = _MesID_SetLedMode;				// ledMode
								message[2] = ledMode; 						
								mesSend(myID, atoi(moduleName+1));	
								sprintf(sertemp,"\r\nModule %d LED mode has been set to %s %s %s bits/sample.\r\n",getID(moduleName),par2,par3,par4); 
								send(pcPort, sertemp);
								// Exit
								return errFlag;
						}
				}
				
setLedMeRoutine:				
				#ifdef TX
				sprintf(sertemp,"\r\nModule %d LED mode has been set to %s %s %s bits/sample.\r\n",myID,par2,par3,par4); 
				send(pcPort, sertemp);	
				#else
				;
				#endif							
		}
		 
    return errFlag;	
}
#endif
#if _comComFormat
// --- command : Set commands format (short/long) ---
int comComFormat(char* value)
{
		if (!strcmp(value,"long"))
		{
				comFormat = 0; 					// Long commands
				message[1] = _MesID_FormLong;						// Format long command
				send(pcPort, "\r\nLong commands are activated.\r\n");
		}
		else if (!strcmp(value,"short"))		
		{					
				comFormat = 1; 					// Short commands	
				message[1] = _MesID_FormShort;						// Format short command
				send(pcPort, "\r\nShort commands are activated.\r\n");
		}

		// Message all other modules
		message[0] = _MesID_broadcast;			// broadcast command			
		broadcast(myID);
		 
    return errFlag;	
}
#endif

// --------------------------------------------------------------------------------------

// --- Receive user commands ---
void receiveCommand(void)
{
		if (RXcom[0] == '\r' && !cptr)         	 	// Received (enter) end of command
		{
				chrComIdx = 0; wordComIdx = 0;
				// Do something with command	
				//send(pcPort, "\n\rGot Command\n\r"); 										
				parseCommand();			
		}		
		else if (RXcom[0] == '\r' && cptr)        // Received (enter) end of command while repeating a previous command
		{
				parseCommand();
				cptr = 0;
		}			
		else if (RXcom[0] == 27 && cptr)          // Received (escape) while repeating a previous command
		{
				cptr = 0;
				send(pcPort, "\n\r");
		}					
		else if (RXcom[0] == 'x' && rwComRate)	// Stop streaming to PC						
		{					
				rwComRate = 0;
				modStack = 1;
				LED_off();
		}
		else if (RXcom[0] == 26)									// Repeat command (Ctrl+z)						
		{					
				togglePrevCom();
		}
		else
		{
				pushCommand(pcPort);	
		}	
	
}

// --- Push the received command in the buffer ---
void pushCommand(uint8_t port)
{
    if (RXcom[0] == 32) {   // Space
        ++wordComIdx;
        chrComIdx = 0;
		} else if (RXcom[0] == 0) {
				;
    } else {
        command[0][wordComIdx][chrComIdx++] = RXcom[0];
    }     
}

// --- Parse user commands ---
// return   0 : Unknown command
//         -1 : Wrong syntax
//          1 : Successful
int parseCommand(void)
{
    int result = 0; int status = 0;
		

    if (!strcmp(command[cptr][0],"info")) {
#if _comInfo 
        result = comInfo();
#endif	
    } else if (!strcmp(command[cptr][0],"help")) {
#if _comHelp		
        result = comHelp();
#endif		
    } else if (!strcmp(command[cptr][0],"status")) {
#if _comStatus		
        result = comStatus(command[cptr][1]);
#endif	
    } else if (!strcmp(command[cptr][0],"name")) {
#if _comName
        result = comName(command[cptr][1],command[cptr][2]);
#endif
    } else if ((!strcmp(command[cptr][0],"ping")) || (comFormat&&(!strcmp(command[cptr][0],"p")))) {
#if _comPing	
        result = comPing(command[cptr][1]);
#endif	
    } else if ((!strcmp(command[cptr][0],"sample")) || (comFormat&&(!strcmp(command[cptr][0],"s")))) {
#if _comSample	
        result = comSample(command[cptr][1]);
#endif	
    } else if (!strcmp(command[cptr][0],"stream")) {
#if _comStream	
        result = comStream(command[cptr][1],command[cptr][2],command[cptr][3]);
#endif
    } else if ((!strcmp(command[cptr][0],"read")) || (comFormat&&(!strcmp(command[cptr][0],"r")))) {
#if _comRead		
        result = comRead(command[cptr][1],command[cptr][2],command[cptr][3],command[cptr][4]);
#endif
    } else if ((!strcmp(command[cptr][0],"write")) || (comFormat&&(!strcmp(command[cptr][0],"w")))) {
#if _comWrite		
        result = comWrite(command[cptr][1],command[cptr][2],command[cptr][3],command[cptr][4]);
#endif
    } else if (!strcmp(command[cptr][0],"on")) {
#if _comOnOff		
        result = comOnOff(command[cptr][1],1);		
#endif
    } else if (!strcmp(command[cptr][0],"off")) {
#if _comOnOff		
        result = comOnOff(command[cptr][1],0);	
#endif			
    } else if ((!strcmp(command[cptr][0],"toggle")) || (comFormat&&(!strcmp(command[cptr][0],"t")))) {
#if _comOnOff		
        result = comOnOff(command[cptr][1],2);
#endif
    } else if (!strcmp(command[cptr][0],"pulse")) {
#if _comPulse		
        result = comPulse(command[cptr][1],command[cptr][2]);
#endif
    } else if (!strcmp(command[cptr][0],"pwm")) {
#if _comPWM		
        result = comPWM(command[cptr][1],command[cptr][2],command[cptr][3]);
#endif		
    } else if (!strcmp(command[cptr][0],"stop")) {
#if _comStop	
        result = comStop(command[cptr][1],command[cptr][2]);
#endif
    } else if (!strcmp(command[cptr][0],"pause")) {
#if _comPause		
        result = comPause(command[cptr][1]);
#endif		
    } else if (!strcmp(command[cptr][0],"resume")) {
#if _comPause		
        result = comResume(command[cptr][1]);
#endif	
    } else if (!strcmp(command[cptr][0],"filter")) {
#if _comFilter	
        result = comFilter(command[cptr][1],command[cptr][2]);
#endif	
    } else if (!strcmp(command[cptr][0],"link")) {
#if _comLink	
        result = comLink(command[cptr]);
#endif
    } else if (!strcmp(command[cptr][0],"unlink")) {
#if _comUnlink			
        result = comUnlink(command[cptr][1],command[cptr][2]);
#endif
    } else if (!strcmp(command[cptr][0],"group")) {
#if _comGroup	
        result = comGroup(command[cptr]);
#endif
    } else if (((!strcmp(command[cptr][0],"update"))) || (comFormat&&(!strcmp(command[cptr][0],"u")))) {
#if _comRunBootloader	
        result = comRunBootloader(command[cptr][1]);
#endif		
    } else if (!strcmp(command[cptr][0],"reset")) {
#if _comReset	
        result = comReset(command[cptr][1]);
#endif
    } else if (!strcmp(command[cptr][0],"route")) {
#if _comRoute		
        result = comRoute(command[cptr][1],command[cptr][2]);
#endif	
    } else if (!strcmp(command[cptr][0],"set") && !strcmp(command[cptr][1],"zerolevel")) {
#if _comSetZeroLevel	
        result = comSetZeroLevel(command[cptr][2],command[cptr][3]);
#endif
    } else if (!strcmp(command[cptr][0],"set") && !strcmp(command[cptr][1],"onelevel")) {
#if _comSetOneLevel	
        result = comSetOneLevel(command[cptr][2],command[cptr][3]);
#endif
    } else if (!strcmp(command[cptr][0],"set") && !strcmp(command[cptr][1],"pdmode")) {
#if _comSetPdMode	
        result = comSetPdMode(command[cptr][2],command[cptr][3]);
#endif	
    } else if (!strcmp(command[cptr][0],"set") && !strcmp(command[cptr][1],"ledmode")) {
#if _comSetLedMode	
        result = comSetLedMode(command[cptr][2],command[cptr][3],command[cptr][4],command[cptr][5]);
#endif	
		} else if (!strcmp(command[cptr][0],"com")) {
#if _comComFormat		
        result = comComFormat(command[cptr][1]);
#endif				
    } else {
        send(pcPort, "\n\rUnknown command!\n\r");
        status = 0;
        goto exit_parsecommand;        
    } 
    
    if (!result) {
        send(pcPort, "\n\rWrong syntax! Please check the user manual.\n\r"); 
        status = -1;
    }
    else 
        status = 1;   
       
exit_parsecommand:
		// Push the current command in the stack
		for(char i=5 ; i>=1 ; i--)
		{
				if (!strcmp(command[i][0],""))
				{
						if (cptr) 	
								memcpy(command[i],command[cptr],sizeof(command[cptr]));
						else
								memcpy(command[i],command[0],sizeof(command[0]));
						goto stackNotFull;
				}	
		}
		// Stack is full
		// If running a previous command, copy it to the current one
		if (cptr) memcpy(command[0],command[cptr],sizeof(command[cptr]));		
		// Push other commands in the stack
		memcpy(command[5],command[4],sizeof(command[4]));
		memcpy(command[4],command[3],sizeof(command[3]));
		memcpy(command[3],command[2],sizeof(command[2]));
		memcpy(command[2],command[1],sizeof(command[1]));
		// Copy the current command to the top of the stack
		memcpy(command[1],command[0],sizeof(command[0]));			
				
		stackNotFull:
		// Clear the current command buffer
		memset(command[0],0,sizeof(command[0]));
		    
    return status;
}

// --- Toggle between last five commands ---
void togglePrevCom(void)
{
		// First use for the stack
		if (!cptr)		
		{
				for(char j=1 ; j<=5 ; j++)
				{
						if (strcmp(command[j][0],""))
						{
								cptr = j;
								break;
						}		
				}
		}
		else
		{
				++cptr;
		}
		
		// Finished scaning the stack >> round back
		if (cptr == 6) 
		{
				cptr = 0;
				togglePrevCom();
				return;
		}
		
		// Display the command
		send(pcPort, "\n\r");
		for (int i=0 ; i<=7 ; i++)
		{
			if (strcmp(command[cptr][i],""))
			{
					send(pcPort, command[cptr][i]);
					send(pcPort, " ");
			}
		}
		
		return;
}

// --- Check module name or alias ---
void checkModuleName(char* name)
{
    errFlag = WrongID;
		
		// Check if module ID is correct and in the list of connected modules
    if (name[0] == '#' && strlen(name) == 4) {		
        if (isdigit(name[1])) {
            if (isdigit(name[2])) {
                if (isdigit(name[3])) {
                    if ((atol(name+1) > 0) && (atol(name+1) <= _N))
												errFlag = NoErrors;
										else
												errFlag = ModuleDoesNotExist;
                }
            }
        }
		// Or check if any of the target identifiers (TIDs)
    } else {	
				errFlag = WrongAlias;
				for(int i=1 ; i<=_N ; i++)
				{		
						// Is it one of the aliases?
						if (!strcmp(moduleAlias[i],name) && (*name != 0)) {
								errFlag = NoErrors;	
								break;

						// Or check if TIDs "me" or "all"
						} else if (!strcmp(name,"me") || !strcmp(name,"all")) {	
								errFlag = NoErrors;	
								break;							
						}
				}				
		}

}

// --- Check if text value is numeric (integer or float)  ---
int isNumeric(char *str)
{
    while(*str) {
        if( !(isdigit(*str)||(*str==',')||(*str=='.')) )
            return 0;
        str++;
    }
    return 1;
}


// --- Display a welcome message for the user ---
void welcomeMessage(void) 
{ 
    send(pcPort, "\n\r");
    send(pcPort, "\n\r====================================================");
    send(pcPort, "\n\r====================================================");
    send(pcPort, "\n\r||              Welcome to MOWE CLP!              ||");
		send(pcPort, "\n\r||          University of Oklahoma 2015.          ||");
    send(pcPort, "\n\r||                                                ||");
		send(pcPort, "\n\r||      Please check the project website at       ||");
		send(pcPort, "\n\r||         http://ouwecad.github.io/MOWE/         ||");
    send(pcPort, "\n\r||                                                ||");
    send(pcPort, "\n\r||   Type help for a list of available commands.  ||");
    send(pcPort, "\n\r====================================================");
    send(pcPort, "\n\r====================================================");
    send(pcPort, "\n\n\r");    
}

// --- Display a stream warning message for the user and record user input ---
char streamWarning(void)
{
		send(pcPort, "\n\rWARNING: Once the stream is initiated, this port ");
		send(pcPort, "will be locked and cannot be used unless unlocked by another port!\n\r");
		send(pcPort, "\n\rContinue? [y/n]\n\r ");
	
		getUart(pcPort)->State = HAL_UART_STATE_READY;
		readPort(pcPort,1);
	
		if (RXcom[0] == 'y' || RXcom[0] == 'Y') {	
				send(pcPort, "\n\r");
				return 1;
		} else	{
				send(pcPort, "\n\r");
				return 0;	
		}
}

// --- Display a warning message for the user and record user input when streaming a sphere ---
char sphereWarning(void)
{
		send(pcPort, "\n\rWARNING: Once the stream is initiated, the used ports ");
		send(pcPort, "will be locked and cannot be utilized until a power reset!\n\r");
		send(pcPort, "\n\rNote: read the stream from Port 1, Module 18 using a 921600 baudrate.\n\r");
		send(pcPort, "\n\rContinue? [y/n]\n\r ");
	
		getUart(pcPort)->State = HAL_UART_STATE_READY;
		readPort(pcPort,1);
	
		if (RXcom[0] == 'y' || RXcom[0] == 'Y') {	
				send(pcPort, "\n\r");
				return 1;
		} else	{
				send(pcPort, "\n\r");
				return 0;	
		}
}

// --- Display a description of current module status (Ports, DMAs) ---
void moduleStatus(void) 
{
	  send(pcPort, "\n\r*** Module Status ***\n");      
    sprintf(sertemp, "\n\rConnected via port: P%d\n\r",pcPort);
		send(pcPort, sertemp);
	
		send(pcPort, "\n\rPorts Status:\n\n\r");
	
		for(int i=1 ; i<=6 ; i++)
		{
				sprintf(sertemp, "P%d: ",i);
				send(pcPort, sertemp);
				switch (portStatus[i])
				{
						case FREE : 
								send(pcPort, "Free"); break;
						case STREAMING :
								send(pcPort, "Streaming"); break;
						case ReceivingMessage :
								send(pcPort, "Receiving messages"); break;
						case ReceivingCommand :
								send(pcPort, "Receiving commands"); break;
						default:
								break;
				}		
				send(pcPort, "\n\r");
		}
		
		send(pcPort, "\n\rPort-to-port DMAs Status:\n\r");

		if (dma_port_port_link1.Instance == 0) {
				send(pcPort, "\n\rPort-to-port DMA 1 is free.");
		} else {
				sprintf(sertemp, "\n\rPort-to-port DMA 1 is streaming from P%d to P%d.",getPort(dma_port_port_link1.Parent),getPort(DMAdst[0]));
				send(pcPort, sertemp);
		}
		if (dma_port_port_link2.Instance == 0) {
				send(pcPort, "\n\rPort-to-port DMA 2 is free.");
		} else {
				sprintf(sertemp, "\n\rPort-to-port DMA 2 is streaming from P%d to P%d.",getPort(dma_port_port_link2.Parent),getPort(DMAdst[1]));
				send(pcPort, sertemp);
		}
		if (dma_port_port_link3.Instance == 0) {
				send(pcPort, "\n\rPort-to-port DMA 3 is free.");
		} else {
				sprintf(sertemp, "\n\rPort-to-port DMA 3 is streaming from P%d to P%d.",getPort(dma_port_port_link3.Parent),getPort(DMAdst[2]));
				send(pcPort, sertemp);
		}
		if (dma_port_port_link4.Instance == 0) {
				send(pcPort, "\n\rPort-to-port DMA 4 is free.");
		} else {
				sprintf(sertemp, "\n\rPort-to-port DMA 4 is streaming from P%d to P%d.",getPort(dma_port_port_link4.Parent),getPort(DMAdst[3]));
				send(pcPort, sertemp);
		}		
		if (dma_port_port_link5.Instance == 0) {
				send(pcPort, "\n\rPort-to-port DMA 5 is free.");
		} else {
				sprintf(sertemp, "\n\rPort-to-port DMA 5 is streaming from P%d to P%d.",getPort(dma_port_port_link5.Parent),getPort(DMAdst[4]));
				send(pcPort, sertemp);
		}		
		if (dma_port_port_link6.Instance == 0) {
				send(pcPort, "\n\rPort-to-port DMA 6 is free.");
		} else {
				sprintf(sertemp, "\n\rPort-to-port DMA 6 is streaming from P%d to P%d.",getPort(dma_port_port_link6.Parent),getPort(DMAdst[5]));
				send(pcPort, sertemp);
		}				
		send(pcPort, "\n\r");
}

	
/************************ (C) COPYRIGHT OU *****END OF FILE****/
