/**
  ******************************************************************************
  * File Name          : aos_md_fe.c
  * Created            : 07/23/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : AOS Module Drivers - Front-end Drivers (FED).
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "aos_md_fe.h"


/* Variables -----------------------------------------------------------------*/
uint8_t streamOutFE = 0;
uint8_t pdMode = 0;
uint8_t ledMode = 0;
uint16_t onelevel = 0;
uint16_t zerolevel = 0;
uint8_t streamMode = 1;
uint8_t stream2FE = 0;
uint8_t stream2FEport = 0;

// Read paramters
float rwComRate = 0; 
uint8_t rwComPort = 0;
uint16_t readComSrc = 0;
uint16_t rwComDst = 0;
uint8_t rwComLED = 0;


/* Private function prototypes -----------------------------------------------*/




// --- Read one sample from the photo diode ---
uint16_t samplePD(void)
{
		uint16_t value = 0;
		
		HAL_ADC_Start(&hadc);
		
		while(HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY) != HAL_OK) {}
		
		if (pdMode == 0){					// Analog
				value = HAL_ADC_GetValue(&hadc);
				
		}else if (pdMode == 1)			// Digital
		{
				value = HAL_ADC_GetValue(&hadc);
				if (value >= onelevel)
						value = 1;
				else if (value <= zerolevel)
						value = 0;
				else
						value = round(rand()%1);
		}
			
		HAL_ADC_Stop(&hadc);
			
		return value;
}
/*
// --- This module was pinged ---
void pingOK(uint16_t originID)
{
	
		//HAL_Delay(routeDist[originID-1]*10 + rand()%10);
	
		message[0] = _MesID_pingOK;		// pingOK command
		mesSend(myID, originID);			

		nblkBlink(200);
}

// --- Display ping results from all receiver modules ---
void pingAllDisp(void)
{
		if (modStack == 1)
		{		
				nblkEvent = NoEvent;	
				nblkTimout = 0;
				nblkBlink(200);
				for (uint16_t i=1 ; i<=_N ; i++)
				{				
						if (modBuf[i-1]) 
						{
								sprintf(sertemp,"\n\rHi from Module #%03d",i); 
								send(pcPort, sertemp);	
						}
				}
				send(pcPort, "\n\r");	
				memset(modBuf,0,sizeof(modBuf));
				memset(sertemp,0,sizeof(sertemp));
		}					
}

// --- This module was sampled ---
void samOK(uint16_t originID)
{	
		uint16_t temp = 0;
	
		//HAL_Delay(routeDist[originID-1]*10 + rand()%10);
		temp = samplePD();
	
		message[0] = _MesID_samOK;		// samOK command
		message[1] = (uint8_t)temp; 				// Low Byte
		message[2] = (uint8_t)(temp>>8);  	// High Byte
		mesSend(myID, originID);	
}

// --- Display collected samples from all receiver modules ---
void sampleAllDisp(void)
{
		if (modStack == 1)
		{	
				nblkEvent = NoEvent;
				nblkTimout = 0;
				for (uint16_t i=1 ; i<=_N ; i++)
				{				
						// Check if receivers
						if (array[i-1][0] == _HO01R1 || array[i-1][0] == _HO01R2 || array[i-1][0] == _PO01R0)
						{
								sprintf(sertemp,"%d\t",modBuf[i-1]); 
								send(pcPort, sertemp);	
						}
				}
				send(pcPort, "\n\r");	
				memset(modBuf,0,sizeof(modBuf));
				memset(sertemp,0,sizeof(sertemp));
		}			
}

// --- This module was read ---
void readOK(uint16_t originID, uint8_t led)
{	
		uint16_t temp = 0;
	
		//HAL_Delay(routeDist[originID-1]*10 + rand()%10);
		temp = samplePD();
	
		message[0] = _MesID_readOK;		// readOK command
		message[1] = (uint8_t)temp; 				// Low Byte
		message[2] = (uint8_t)(temp>>8);  	// High Byte
		mesSend(myID, originID);	

		if (led)
				LED_toggle();
}

// --- Display read values from all receiver modules ---
void readAllDisp(void)
{
		for (uint16_t i=1 ; i<=_N ; i++)
		{				
				// Check if receivers
				if (array[i-1][0] == _HO01R1 || array[i-1][0] == _HO01R2 || array[i-1][0] == _PO01R0)
				{
						if (modBuf[i-1] == 0xFFFF)
								sprintf(sertemp," \t"); 
						else					
								sprintf(sertemp,"%d\t",modBuf[i-1]); 								 
						send(pcPort, sertemp);	
				}
		}
		send(pcPort, "\n\r");	
		
		memset(sertemp,0,sizeof(sertemp));
		// Reset to 0xFFFF
		memset(modBuf,0xFFFF,sizeof(modBuf));
}

// --- Load and start FE PWM ---
void startPWM_FE(uint16_t period, uint16_t width)
{
		// Setup PWM timer
		MX_TIM3_Init();
	
		htim3.Instance->CCR3 = width;
	
		htim3.Init.Period = period;
		HAL_TIM_Base_Init(&htim3);
	
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);		
}

// --- Stream front-end data in/out the module for read/write commands
void stream(void)
{
		uint32_t rateFactor = 0;
	
		if (myTrig) 
		{					
				myTrig = 0;
			
				if (streamMode == 1)											// This module is reading/writing itself
				{						
						#ifndef TX	
								sprintf(sertemp,"%d\n\r",samplePD()); 	
								send(rwComPort, sertemp);
								if (rwComLED)	LED_toggle();
						#else
								writeToFE(rwComPort);										
								if (rwComLED)	LED_toggle();
						#endif
				}
				else if (streamMode == 3)								// This module is being read/written by another module
				{
						#ifndef TX	
								readOK(readComSrc, rwComLED);
						#else
								writeToFE(rwComPort);								
								if (rwComLED)	LED_toggle();						
						#endif
				}		
				else if (streamMode == 4)								// This module is reading all other receiver
				{	
						// Display previous data
						readAllDisp();
					
						// Inquire new data
						rateFactor = floor((48*1000000)/rwComRate);	
					
						message[0] = _MesID_broadcast;										// broadcast command						
						message[1] = _MesID_read;										// read command
						message[2] = (uint8_t)rateFactor;		// rwComRate: Low Byte
						message[3] = (uint8_t)(rateFactor>>8);
						message[4] = (uint8_t)(rateFactor>>16);
						message[5] = (uint8_t)(rateFactor>>24);
						message[6] = rwComLED;						// rwComLED
						broadcast(myID);	
					
						// Activate the modules stack. Expect response from all receiver modules
						modStack = numOfRX+1;							
						#ifndef TX
								modBuf[myID-1] = samplePD();				
						#endif
				}		
				else if (streamMode == 5)								// This module is writing all other transmitters
				{	
						rateFactor = floor((48*1000000)/rwComRate);	
					
						message[0] = _MesID_broadcast;										// broadcast command						
						message[1] = _MesID_write;										// write command
						message[2] = (uint8_t)rateFactor;		// rwComRate: Low Byte
						message[3] = (uint8_t)(rateFactor>>8);
						message[4] = (uint8_t)(rateFactor>>16);
						message[5] = (uint8_t)(rateFactor>>24);
						message[6] = rwComLED;						// rwComLED
						broadcast(myID);	
									
						#ifdef TX
								writeToFE(rwComPort);		
						#endif
				}							
		}
}
*/
// --- Local stream from port to front-end
void streamToFE(uint8_t port)
{
		if (ledMode == 0)						// Digital Ascii 8-bit
		{
				if (LED_TX_Buf == '1')
						FE_LED_on();			
				else if (LED_TX_Buf == '0')
						FE_LED_off();	
		}
		else if (ledMode == 1)			// Digital binary 8-bit
		{
				if (LED_TX_Buf == 1)
						FE_LED_on();			
				else if (LED_TX_Buf == 0)
						FE_LED_off();	
		}
		else if (ledMode == 2)			// Analog binary 8-bit
		{
				if (LED_TX_Buf >= onelevel)
						FE_LED_on();
				else if (LED_TX_Buf <= zerolevel)
						FE_LED_off();	
		}	
}

// --- Local write from port to front-end
void writeToFE(uint8_t port)
{
		if (ledMode == 0)						// Digital Ascii 8-bit
		{
				if (DMAbuffer[port-1][0] == '1')
						FE_LED_on();			
				else if (DMAbuffer[port-1][0] == '0')
						FE_LED_off();	
		}
		else if (ledMode == 1)			// Digital binary 8-bit
		{
				if (DMAbuffer[port-1][0] == 1)
						FE_LED_on();			
				else if (DMAbuffer[port-1][0] == 0)
						FE_LED_off();	
		}
		else if (ledMode == 2)			// Analog binary 8-bit
		{
				if (DMAbuffer[port-1][0] >= onelevel)
						FE_LED_on();
				else if (DMAbuffer[port-1][0] <= zerolevel)
						FE_LED_off();	
		}	
}

// --- FE-to-Port/Port-to-FE streaming
uint8_t stream_FE_Port(uint8_t src)
{
		//if (src != pcPort) {										
				disableDMAport(src);						// Disable old DMA
				memset(DMAbuffer[src-1],0,10);	// Clear the buffer
		//}
		
		// Start DMA stream FE-to-Port/Port-to-FE	(DMA channel is shared with P5)
		#ifdef TX	
				if (src != P5) {
						// Disable P5 DMA
						disableDMAport(P5);
						// Assign DMA used in port src to P5
						switch (src)
						{
								case P1 : 
										MX_DMA_PORT_MEM1_Setup(GetUart(P5), 10, 0);
										break;
								case P2 :
										MX_DMA_PORT_MEM2_Setup(GetUart(P5), 10, 10);	
										break;
								case P3 :
										MX_DMA_PORT_MEM3_Setup(GetUart(P5), 10, 20);	
										break;
								case P4 :
										MX_DMA_PORT_MEM4_Setup(GetUart(P5), 10, 30);	
										break;
								#if (HO01R1 || HO02R0 || HO01R2 || HO02R1||HO02R2||HO01R3)
								case P6 :
										MX_DMA_PORT_MEM6_Setup(GetUart(P5), 10, 50);	
								#endif
								default:
										break;
						}	
				}	
				// Enable port-to-fe DMA: Assign DMA used in P5 to FE
				MX_DMA_PORT_FE_Setup(GetUart(src));		
				// Stream to FE
				stream2FE = 1;	
				stream2FEport = src;			
		#else										
				MX_DMA_PORT_FE_Setup(GetUart(src));			
		#endif
		
		return 1;
}

// --- Port-to-Port streaming
uint8_t stream_Port_Port(uint8_t src, uint8_t dst)
{
		// Disable old DMA & clean the buffer
		//if (src != pcPort) {										
				disableDMAport(src);						
				memset(DMAbuffer[src-1],0,10);	
		//}
		//if (dst != pcPort) {										
				disableDMAport(dst);						
				memset(DMAbuffer[dst-1],0,10);	
		//}
		
		// Start DMA stream Port-to-Port
		// Check which DMA stream is not used
		if (src == P1 && dma_port_port_link1.Instance == 0) {
				MX_DMA_PORT_PORT_LINK1_Init();
				MX_DMA_PORT_PORT_LINK1_Setup(GetUart(src), GetUart(dst));		
		} else if (src == P2 && dma_port_port_link2.Instance == 0) {
				MX_DMA_PORT_PORT_LINK2_Init();
				MX_DMA_PORT_PORT_LINK2_Setup(GetUart(src), GetUart(dst));								
		} else if (src == P3 && dma_port_port_link3.Instance == 0) {
				MX_DMA_PORT_PORT_LINK3_Init();
				MX_DMA_PORT_PORT_LINK3_Setup(GetUart(src), GetUart(dst));					
		} else if (src == P4 && dma_port_port_link4.Instance == 0) {
				MX_DMA_PORT_PORT_LINK4_Init();
				MX_DMA_PORT_PORT_LINK4_Setup(GetUart(src), GetUart(dst));			
		} else if (src == P5 && dma_port_port_link5.Instance == 0) {
				MX_DMA_PORT_PORT_LINK5_Init();
				MX_DMA_PORT_PORT_LINK5_Setup(GetUart(src), GetUart(dst));				
		} else if (src == P6 && dma_port_port_link6.Instance == 0) {
				MX_DMA_PORT_PORT_LINK6_Init();
				MX_DMA_PORT_PORT_LINK6_Setup(GetUart(src), GetUart(dst));					
		} else {
				return 0;
		}			
			
		return 1;
								
}











	
/************************ (C) COPYRIGHT OU *****END OF FILE****/
