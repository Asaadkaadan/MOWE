/**
  ******************************************************************************
  * File Name          : aos_iac_routing.c
  * Created            : 07/23/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : AOS Inter-array Communication - Routing routines.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "aos_iac_routing.h"


/* Variables -----------------------------------------------------------------*/
uint8_t outPort = 0; 
uint8_t bcastIn = 0;
uint8_t bcastOut[6] = {1,1,1,1,1,1};
uint8_t message[8] = {0};
uint16_t routeDist[_N] = {0}; 
uint16_t routePrev[_N] = {0}; 
uint16_t route[_N] = {0};
ITStatus P1Ready = RESET;
ITStatus P2Ready = RESET;
ITStatus P3Ready = RESET;
ITStatus P4Ready = RESET;
ITStatus P5Ready = RESET;
ITStatus P6Ready = RESET;
ITStatus PCReady = RESET;
uint8_t portStatus[7] = {0};		// All ports are initially free
uint8_t mesStack[6][6] = {0};				// Message dispatch stack = 6 messages x 6 paramters (1 message type + 5 paramters)


/* Private function prototypes -----------------------------------------------*/
uint16_t minArr(uint16_t* arr, uint16_t* Q);
uint8_t QnotEmpty(uint16_t* Q);


// --- Sending text via an array port (a single packet of 10 bytes) ---
void sendPacket(uint8_t port, const char* text)
{	
		switch (port)
		{
				case P1 : 
						HAL_UART_Transmit(P1uart,(uint8_t *)text,10,HAL_MAX_DELAY);
						break;
				case P2 :
						HAL_UART_Transmit(P2uart,(uint8_t *)text,10,HAL_MAX_DELAY);
						break;
				case P3 :
						HAL_UART_Transmit(P3uart,(uint8_t *)text,10,HAL_MAX_DELAY);
						break;
				case P4 :
						HAL_UART_Transmit(P4uart,(uint8_t *)text,10,HAL_MAX_DELAY);
						break;
				case P5 :
						HAL_UART_Transmit(P5uart,(uint8_t *)text,10,HAL_MAX_DELAY);
						break;
				#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || HO01R3 || HO02R2)
				case P6 :
						HAL_UART_Transmit(P6uart,(uint8_t *)text,10,HAL_MAX_DELAY);
						break;
				#endif
				default:
						break;
		}	
}

// --- Sending text via an array port ---
void send(uint8_t port, const char* text)
{	
		switch (port)
		{
				case P1 : 
						HAL_UART_Transmit(P1uart,(uint8_t *)text,strlen(text),HAL_MAX_DELAY);
						break;
				case P2 :
						HAL_UART_Transmit(P2uart,(uint8_t *)text,strlen(text),HAL_MAX_DELAY);
						break;
				case P3 :
						HAL_UART_Transmit(P3uart,(uint8_t *)text,strlen(text),HAL_MAX_DELAY);
						break;
				case P4 :
						HAL_UART_Transmit(P4uart,(uint8_t *)text,strlen(text),HAL_MAX_DELAY);
						break;
				case P5 :
						HAL_UART_Transmit(P5uart,(uint8_t *)text,strlen(text),HAL_MAX_DELAY);
						break;
				#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || HO01R3 || HO02R2)
				case P6 :
						HAL_UART_Transmit(P6uart,(uint8_t *)text,strlen(text),HAL_MAX_DELAY);
						break;
				#endif
				default:
						break;
		}	
}

// --- Read one of the array ports ---
void readPort(uint8_t port, uint16_t size)
{	
		switch (port)
		{
				case P1 : 
						readP1(size);
						break;
				case P2 :
						readP2(size);
						break;
				case P3 :
						readP3(size);
						break;
				case P4 :
						readP4(size);
						break;
				case P5 :
						readP5(size);
						break;
				#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || HO01R3 || HO02R2)
				case P6 :
						readP6(size);
						break;
				#endif
				default:
					break;
		}	
}

// --- Get the UART for a given port ---
UART_HandleTypeDef* getUart(uint8_t port)
{
		switch (port)
		{
				case P1 : 
						return P1uart;		
				case P2 :
						return P2uart;
				case P3 :
						return P3uart;
				case P4 :
						return P4uart;
				case P5 :
						return P5uart;
				#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || HO01R3 || HO02R2)
				case P6 :
						return P6uart;
				#endif
				default:
						return 0;
		}		
}

// --- Get the port for a given UART ---
int getPort(UART_HandleTypeDef *huart)
{
#if (HO01R1 || HO02R0)
		if (huart->Instance == USART2)
				return P1;
		else if (huart->Instance == USART3)
				return P2;
		else if (huart->Instance == USART7)
				return P3;
		else if (huart->Instance == USART8)
				return P4;
		else if (huart->Instance == USART4)
				return P5;
		else if (huart->Instance == USART5)
				return P6;
#endif
#if (HO01R2 || HO02R1)
		if (huart->Instance == USART2)
				return P1;
		else if (huart->Instance == USART6)
				return P2;
		else if (huart->Instance == USART3)
				return P3;
		else if (huart->Instance == USART5)
				return P4;
		else if (huart->Instance == USART1)
				return P5;
		else if (huart->Instance == USART4)
				return P6;
#endif
#if (HO01R3 || HO02R2)
	if (huart->Instance == USART2)
		return P1;
	else if (huart->Instance == USART3)
		return P2;
	else if (huart->Instance == USART7)
		return P3;
	else if (huart->Instance == USART8)
		return P4;
	else if (huart->Instance == USART4)
		return P5;
	else if (huart->Instance == USART1)
		return P6;
#endif
#if (PO01R0 || PO02R0)
		if (huart->Instance == USART5)
				return P1;
		else if (huart->Instance == USART2)
				return P2;
		else if (huart->Instance == USART3)
				return P3;
		else if (huart->Instance == USART8)
				return P4;
		else if (huart->Instance == USART4)
				return P5;
#endif
		
		return 0;
}

// --- Sending a message to another module ---
void mesSend(uint16_t src, uint16_t dst)
{	
		outPort = findRoute(src, dst);
		sertemp[0] = src;
		sertemp[1] = dst;
		memcpy(sertemp+2,message,8);
	
		sendPacket(outPort, sertemp);
	
		// Clear the buffer
		memset(message,0,8);	
}

// --- Dispatch pending messages ---
void mesDispatch(void)
{
		// Iterate through the dispatch message stack
		for(char i=0 ; i<=5 ; i++)
		{
				if (mesStack[i][0])
				{
						switch (mesStack[i][0])
						{
								case _MesID_reroute 				:								// Pull a reroute message							
										mesSend(mesStack[i][1], mesStack[i][2]);																		
										// if streamSISO inquiry received
										if (mesStack[i][3] == _MesID_streamSISO) {
												// Start local port-to-port stream
												stream_Port_Port(mesStack[i][4], outPort);
										}
										break;
								
								case _MesID_pingOK 					:								// Pull a pingOK message
										pingOK(mesStack[i][1]);
										break;
								case _MesID_singlePingDisp 	:								// Pull a singlePingDisp message
										sprintf(sertemp,"\n\rHi from Module #%03d\n\r",mesStack[i][1]); 
										send(pcPort, sertemp);			
										break;		
								case _MesID_pingAllDisp 		:								// Pull a pingAllDisp message
										pingAllDisp();
										break;
								
								case _MesID_samOK 					:								// Pull a samOK message
										samOK(mesStack[i][1]);
										break;
								case _MesID_singleSamDisp 	:								// Pull a singleSamDisp message							
										sprintf(sertemp,"\n\rModule #%03d: %d\n\r",mesStack[i][1],mesStack[i][2]+256*mesStack[i][3]); 
										send(pcPort, sertemp);
										break;
								case _MesID_sampleAllDisp 	:								// Pull a sampleAllDisp message
										sampleAllDisp();
										break;
								
								case _MesID_readOK 					:								// Pull a readOK message
										break;								
								case _MesID_singleReadDisp 	:								// Pull a singleReadDisp message
										sprintf(sertemp,"%d\n\r",mesStack[i][1]+256*mesStack[i][2]); 
										send(pcPort, sertemp);	
										break;
								case _MesID_readAllDisp 		:								// Pull a readAllDisp message
										readAllDisp();
										break;
								
								case _MesID_stream					:								// Pull a stream message	
										myTrig = 1;
										stream();
										break;
								
								case _MesID_streamSISO					:						// Pull a streamSISO message	
										message[0] = _MesID_streamSISO;		
										mesSend(myID, mesStack[i][1]);	
										break;
								
								case _MesID_broadcast				:								// Pull a broadcast message	
										count_us(rand()%1000);
										broadcast(mesStack[i][1]);
										break;
								
								default:
										break;
						}
					

						// Clear the stack
						memset(mesStack[i],0,6);			
				}
		}	
		
		// Stream port-to-front-end section
#ifdef TX
		if(stream2FE)	streamToFE(stream2FEport);
#endif
		
}

// --- Parse messages from other modules ---
void parseMessage(uint8_t port)
{
		uint16_t srcID = 0; uint16_t dstID = 0; 
		uint8_t bcastFlag = 0;
		uint32_t rateFactor = 0;
	
		// Extract source and destination IDs
		srcID = DMAbuffer[port-1][0];
		dstID = DMAbuffer[port-1][1]; 

		// Got a broadcast command
		if (!dstID)
				dstID = myID;
		
    // If this module is not the target
		if (dstID != myID) 			
		{			
				// Push a reroute message	
				memcpy(message,DMAbuffer[port-1]+2,8);				
				pushMessage(5, _MesID_reroute, srcID, dstID, DMAbuffer[port-1][2], port);												
		}
		// If this module is the target
		else
		{
				// Is it a broadcast?
				if (DMAbuffer[port-1][2] == _MesID_broadcast)	{	
						memcpy(message,DMAbuffer[port-1]+2,8);
						pushMessage(2, _MesID_broadcast, srcID);							// Push a broadcast message	
						bcastIn = port;
						bcastFlag = 1;
				}		
				
				// Pinging
				if (DMAbuffer[port-1][2] == _MesID_ping || (bcastFlag && DMAbuffer[port-1][3] == _MesID_ping)) {					// ping inquiry received
						pushMessage(2, _MesID_pingOK, srcID);						// Push a pingOK message				
				} else if (DMAbuffer[port-1][2] == _MesID_pingOK) {																								// pingOK response received
						if (modStack == 1) {			// Expect only one response
								pushMessage(2, _MesID_singlePingDisp, srcID);				// Push a single ping response message
						} else {									// Expect responses from all
								modBuf[srcID-1] = 1;
								--modStack;
								pushMessage(1, _MesID_pingAllDisp);								// Push a pingAllDisp message	
						}						
				// Sampling
				} else if (DMAbuffer[port-1][2] == _MesID_sample || (bcastFlag && DMAbuffer[port-1][3] == _MesID_sample)) {		// sample inquiry received
						pushMessage(2, _MesID_samOK, srcID);						// Push a samOK message	
				} else if (DMAbuffer[port-1][2] == _MesID_samOK) {																								// samOK response received
						if (modStack == 1) {			// Expect only one response
								pushMessage(4, _MesID_singleSamDisp, srcID, DMAbuffer[port-1][3], DMAbuffer[port-1][4]);			// Push a single sample response message
						} else {									// Expect responses from all
								modBuf[srcID-1] = DMAbuffer[port-1][3]+256*DMAbuffer[port-1][4];
								--modStack;
								pushMessage(1, _MesID_sampleAllDisp);								// Push a sampleAllDisp message	
						}					
				// Read
				} else if (DMAbuffer[port-1][2] == _MesID_read || (bcastFlag && DMAbuffer[port-1][3] == _MesID_read)) {			// read inquiry received
						streamMode = 3;
						readComSrc = srcID;						
						if (bcastFlag) {		// Broadcast, periodic beat is set by master
//								if (DMAbuffer[port-1][2] == 40)) {			// streaming a sphere
//										rwComRate = atof(message[port-1][5]);
//										rwComLED = atoi(message[port-1][6]);
//								} else {
								rwComLED = DMAbuffer[port-1][8];				
								pushMessage(1, _MesID_stream);											// Push a stream message	
//								}
						} else {						// Singlecast, periodic beat is set locally
								rateFactor = DMAbuffer[port-1][3]+(DMAbuffer[port-1][4]<<8)+(DMAbuffer[port-1][5]<<16)+(DMAbuffer[port-1][6]<<24);
								rwComLED = DMAbuffer[port-1][7];
								startPerEvent(rateFactor);
						}			
				} else if (DMAbuffer[port-1][2] == _MesID_readOK) {																										// readOK response received
						if (modStack == 1) {			// Expect only one response
								pushMessage(3, _MesID_singleReadDisp, DMAbuffer[port-1][3], DMAbuffer[port-1][4]);			// Push a single read response message
						} else {									// Expect responses from all
								modBuf[srcID-1] = DMAbuffer[port-1][3]+256*DMAbuffer[port-1][4];
								--modStack;
						}		
				// Write
				} else if (DMAbuffer[port-1][2] == _MesID_write || (bcastFlag && DMAbuffer[port-1][3] == _MesID_write)) {			// write inquiry received
						streamMode = 3;
						rwComPort = port;
						readComSrc = srcID;						
						if (bcastFlag) {		// Broadcast, periodic beat is set by master
								rwComLED = DMAbuffer[port-1][8];				
								pushMessage(1, _MesID_stream);											// Push a stream message	
						} else {						// Singlecast, periodic beat is set locally
								rateFactor = DMAbuffer[port-1][3]+(DMAbuffer[port-1][4]<<8)+(DMAbuffer[port-1][5]<<16)+(DMAbuffer[port-1][6]<<24);
								rwComLED = DMAbuffer[port-1][7];
								startPerEvent(rateFactor);
						}			
				// Stop
				} else if (DMAbuffer[port-1][2] == _MesID_stop || (bcastFlag && DMAbuffer[port-1][3] == _MesID_stop)) {			// stop inquiry received
						if (DMAbuffer[port-1][3] == _MesID_stopPWM || (bcastFlag && DMAbuffer[port-1][4] == _MesID_stopPWM)) {			// stopPWM inquiry received
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
								readComSrc = 0;
								rwComLED = 0; 
								rateFactor = 0;
								modStack = 1;
								LED_off();				
								if (!bcastFlag) {		// Singlecast, periodic beat is set locally
										myTrig = 0;
										HAL_TIM_Base_Stop_IT(&htim2);
								}			
						}
				// Pause
				} else if (DMAbuffer[port-1][2] == _MesID_pause || (bcastFlag && DMAbuffer[port-1][3] == _MesID_pause)) {			// pause inquiry received
						if (!bcastFlag) {		// Singlecast, periodic beat is set locally
								myTrig = 0;
								HAL_TIM_Base_Stop_IT(&htim2);		
						}					
				// Resume
				} else if (DMAbuffer[port-1][2] == _MesID_resume || (bcastFlag && DMAbuffer[port-1][3] == _MesID_resume)) {			// resume inquiry received
						if (!bcastFlag) {		// Singlecast, periodic beat is set locally
								HAL_TIM_Base_Start_IT(&htim2);		
						}							
				// Array discovery


				// Turn front-end LED on
				} else if (DMAbuffer[port-1][2] == _MesID_LEDonFE || (bcastFlag && DMAbuffer[port-1][3] == _MesID_LEDonFE)) { 			// on inquiry received
						FE_LED_on();
				// Turn front-end LED off
				} else if (DMAbuffer[port-1][2] == _MesID_LEDoffFE || (bcastFlag && DMAbuffer[port-1][3] == _MesID_LEDoffFE)) { 			// off inquiry received
						FE_LED_off();
				// Toggle front-end LED
				} else if (DMAbuffer[port-1][2] == _MesID_LEDtoggleFE || (bcastFlag && DMAbuffer[port-1][3] == _MesID_LEDtoggleFE)) { 			// toggle inquiry received
						FE_LED_toggle();
				// Pulse front-end LED
				} else if (DMAbuffer[port-1][2] == _MesID_LEDpulseFE || (bcastFlag && DMAbuffer[port-1][3] == _MesID_LEDpulseFE)) { 			// pulse inquiry received
						if (bcastFlag) {
								FE_nblkBlink(DMAbuffer[port-1][4]);
						} else {
								FE_nblkBlink(DMAbuffer[port-1][3]);
						}
				// Output a PWM signal on front-end LED
				} else if (DMAbuffer[port-1][2] == _MesID_LEDpwmFE || (bcastFlag && DMAbuffer[port-1][3] == _MesID_LEDpwmFE)) { 			// pwm inquiry received
						if (bcastFlag) {
								startPWM_FE(DMAbuffer[port-1][4]+(DMAbuffer[port-1][5]<<8), DMAbuffer[port-1][6]+(DMAbuffer[port-1][7]<<8));
						} else {
								startPWM_FE(DMAbuffer[port-1][3]+(DMAbuffer[port-1][4]<<8), DMAbuffer[port-1][5]+(DMAbuffer[port-1][6]<<8));
						}
				// Set paramters
				} else if (DMAbuffer[port-1][2] == _MesID_set || (bcastFlag && DMAbuffer[port-1][3] == _MesID_set)) {				// set inquiry received
						// pdMode
						if (DMAbuffer[port-1][3] == _MesID_SetPdMode || (bcastFlag && DMAbuffer[port-1][4] == _MesID_SetPdMode)) {					// pdMode inquiry received
								#ifndef TX
								if (bcastFlag)		
										pdMode = DMAbuffer[port-1][5];
								else 
										pdMode = DMAbuffer[port-1][4];				
								#endif	
						}
						// zerolevel
						#ifndef TX
						if (DMAbuffer[port-1][3] == _MesID_setzerolevel) 						// zerolevel inquiry received
								zerolevel = DMAbuffer[port-1][4]+256*DMAbuffer[port-1][5];											
						else if (bcastFlag && DMAbuffer[port-1][4] == _MesID_setzerolevel) 
								zerolevel = DMAbuffer[port-1][5]+256*DMAbuffer[port-1][6];						
						#endif	
						// onelevel
						#ifndef TX
						if (DMAbuffer[port-1][3] == _MesID_setonelevel) 						// onelevel inquiry received
								onelevel = DMAbuffer[port-1][4]+256*DMAbuffer[port-1][5];																					
						else if (bcastFlag && DMAbuffer[port-1][4] == _MesID_setonelevel) 
								onelevel = DMAbuffer[port-1][5]+256*DMAbuffer[port-1][6];					
						#endif	
						// ledMode
						if (DMAbuffer[port-1][3] == _MesID_SetLedMode || (bcastFlag && DMAbuffer[port-1][4] == _MesID_SetLedMode)) {					// ledMode inquiry received
								#ifndef TX
								if (bcastFlag)		
										ledMode = DMAbuffer[port-1][5];
								else 
										ledMode = DMAbuffer[port-1][4];				
								#endif	
						}
				// Streaming
				} else if (DMAbuffer[port-1][2] == _MesID_streamDMA) { 				// streamDMA inquiry received
						if (!DMAbuffer[port-1][5])					// Inter-module (single module) streaming
						{			
								if (!DMAbuffer[port-1][4]) { 										// Stream port-to-fe/fe-to-port		
										stream_FE_Port(DMAbuffer[port-1][3]);
								} else {												// Stream port-to-port	
										stream_Port_Port(DMAbuffer[port-1][3], DMAbuffer[port-1][4]);
								}		
						}	
						else									// Intra-module (module to module) streaming
						{
								pushMessage(2, _MesID_streamSISO, DMAbuffer[port-1][5]);					// Push a streamSISO message	
						}								 
				// Reset
				} else if (DMAbuffer[port-1][2] == _MesID_reset || (bcastFlag && DMAbuffer[port-1][3] == _MesID_reset)) { 				// reset inquiry received
						NVIC_SystemReset();	 	
				// Commands format long
				} else if (bcastFlag && DMAbuffer[port-1][3] == _MesID_FormLong) { 				// format long inquiry received
						comFormat = 0;	 			
				// Commands format short
				} else if (bcastFlag && DMAbuffer[port-1][3] == _MesID_FormShort) { 			// format short inquiry received
						comFormat = 1;	 	
				}	
				
				// Reset the broadcast flag if needed
				bcastFlag = 0;
			
		}
			
		
		// Clear the buffer
		memset(DMAbuffer[port-1],0,10);	
    
}  

// --- Push a message inside the dispatch message stack ---
void pushMessage(uint8_t n_args, ...)
{
		uint8_t row = 0;
		va_list arguments;
	
		va_start(arguments, n_args);
	
		// Find the next empty row in the stack
		for(char i=0 ; i<=5 ; i++)
		{
				if (!mesStack[i][0])
				{
						row = i;
						break;
				}
		}

		// Message type
		mesStack[row][0] = va_arg(arguments, int);
		
		// Read other argument
		for(char j=1 ; j<=n_args ; j++)
		{
				mesStack[row][j] = va_arg(arguments, int);
		}
}	

// --- Find the shortest route to a module using Dijkstra's algorithm ---
/* -------------------------------------------
Algorithm (from Wikipedia):

1- Assign to every node a tentative distance value: set it to zero for our initial node 
and to infinity for all other nodes.

2- Set the initial node as current. Mark all other nodes unvisited. Create a set of all 
the unvisited nodes called the unvisited set.

3- For the current node, consider all of its unvisited neighbors and calculate their tentative
distances. Compare the newly calculated tentative distance to the current assigned value and 
assign the smaller one. For example, if the current node A is marked with a distance of 6, 
and the edge connecting it with a neighbor B has length 2, then the distance to B (through A) 
will be 6 + 2 = 8. If B was previously marked with a distance greater than 8 then change it to 8. 
Otherwise, keep the current value.

4- When we are done considering all of the neighbors of the current node, mark the current 
node as visited and remove it from the unvisited set. A visited node will never be checked again.

5- If the destination node has been marked visited (when planning a route between two specific 
nodes) or if the smallest tentative distance among the nodes in the unvisited set is infinity 
(when planning a complete traversal; occurs when there is no connection between the initial 
node and remaining unvisited nodes), then stop. The algorithm has finished.

6- Otherwise, select the unvisited node that is marked with the smallest tentative distance, 
set it as the new "current node", and go back to step 3.

---------------------------------------- */
uint8_t findRoute(uint16_t sourceID, uint16_t desID)
{
		uint16_t Q[_N] = {0};		// All nodes initially in Q (unvisited nodes)
		uint16_t alt = 0; uint16_t u = 0; uint16_t v = 0; uint16_t j = 0;
		
		memset(route,0,sizeof(route));
		routeDist[sourceID-1] = 0;                  // Distance from source to source
		routePrev[sourceID-1] = 0;               		// Previous node in optimal path initialization undefined
			
		// Check adjacent neighbors first!
		for(int col=1 ; col<=6 ; col++)
		{
				if (array[sourceID-1][col] && ((array[sourceID-1][col]>>3) == desID)) {
						routeDist[desID-1] = 1;
						route[0] = desID;
						return col;	
				}
		}						
		
		// Initialization
		for (int i=1 ; i<=_N ; i++)   					
		{
				if (i != sourceID)            					// Where i has not yet been removed from Q (unvisited nodes)
				{
						routeDist[i-1] = 0xFFFF;        		// Unknown distance function from source to i
						routePrev[i-1] = 0;            			// Previous node in optimal path from source
				}                    			
		}
		
		// Algorithm
		while (!QnotEmpty(Q))
		{				
				u = minArr(routeDist, Q)+1;							// Source node in first case
				if (u == desID) 
				{
						goto finishedRoute;
				}
				else
						Q[u-1] = 1;													// Remove u from Q 
																									
				// For each neighbor v where v is still in Q.
				for (uint8_t n=1 ; n<=6 ; n++)      				// Check all module ports
				{
						if (array[u-1][n])											// There's a neighbor v at this port n
						{	
								v = (array[u-1][n]>>3);
								if (!Q[v-1])												// v is still in Q
								{
										alt = routeDist[u-1] + 1;				// Add one hop
										if (alt < routeDist[v-1])      	// A shorter path to v has been found
										{
												routeDist[v-1] = alt; 
												routePrev[v-1] = u; 
										}
								}
						}
				}
		}	
		
finishedRoute:
		
		// Build the virtual route	
		while (routePrev[u-1])        		// Construct the shortest path with a stack route
		{
				route[j++] = u;          			// Push the vertex onto the stack
				u = routePrev[u-1];           // Traverse from target to source
		}
		
		// Check which port leads to the correct module
		for(int col=1 ; col<=6 ; col++)	
		{					
				if ( array[sourceID-1][col] && ((array[sourceID-1][col]>>3) == route[routeDist[desID-1]-1]) ) {
						return col;	
				}
		}	

		return 0;			
}

// --- Find the index of the minimum module in dist that is still unvisited ---
uint16_t minArr(uint16_t* arr, uint16_t* Q)
{
		uint16_t smallest = 0xFFFF; uint16_t index = 0;
	
		//Consider first element as smallest
		if (!Q[0])						// Not visited yet
			smallest = arr[0];

		for (int i=0 ; i<_N ; i++) {
				if ((arr[i] < smallest) && !Q[i]) {
						smallest = arr[i];
						index = i;
				}
		}
		
		return index;
}

// --- Check if Q is empty (all modules have been visited) ---
uint8_t QnotEmpty(uint16_t* Q)
{		
		char temp = 1;
	
		for (int i=0 ; i<_N ; i++) {
				temp &= Q[i];
		}	
		
		return temp;
}

// --- Handeling broadcast requests ---
void broadcast(uint16_t originID)
{

//		// Broadcast route discovery
//		if (!strcmp(message[port-1][3],"INIT"))
//		{
//				bcastIn = port;
//				baseModule = originID;
//				findBroadcastRoute();
//				// Cancel redundant ports
//				for (int i=1 ; i<=6 ; i++)
//				{
//						if (i != bcastIn && i != pcPort)
//						{
//								sprintf(sertemp,"%% #%03d #000 BCAST RED\r",myID); 	
//								portSend(i, sertemp);	
//						}											
//				}
//				bcastIn = 0;
//		}
//		// Broadcast route refinement
//		else if (!strcmp(message[port-1][3],"RED"))
//		{
//				bcastOut[port-1] = 0;
//		}
		// Regular broadcast
//		else
//		{		
					sertemp[0] = originID;
					sertemp[1] = 0;
					memcpy(sertemp+2,message,8);
					
					for (uint8_t j=1 ; j<=6 ; j++)
					{
							if (bcastOut[j-1] && j != pcPort && j != bcastIn)
							{
									sendPacket(j, sertemp);								
							}
					}

					// Clear the buffer
					memset(message,0,8);
					memset(sertemp,0,sizeof(sertemp));
//		}
}


/************************ (C) COPYRIGHT OU *****END OF FILE****/
