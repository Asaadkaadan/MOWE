/**
  ******************************************************************************
  * File Name          : relay.c
  * Created            : 03/31/2016 
  * Version            : 1.0 
 	* Author             : Will Nichols
  * Description        : Relay setup functions.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "relay.h"


/* Variables -----------------------------------------------------------------*/
//For switching code
#ifdef SWITCH 
	#ifdef TX
		uint8_t data1=0;
		uint8_t data2=0;
		uint8_t* currStream = &data1;
		uint8_t dataDone=0;
		
		char receiveBuffer[MESSAGE_LENGTH]={0};
		char *currPos=receiveBuffer;
		char currBitPos=0;
	#endif
#endif

/* Functions -----------------------------------------------------------------*/
/**
		This function initalizes the relay modules. This function automatically adjusts
		for the type of module and its position in the relay if TX,RX,SWITCH, and SPILTTER
		are properly defined for the current module. Uses P1 and possibly P2 by default
		(P2 used for splitter and switch).
		
		A reciver will enter an infinite loop in this function, as will a switch.
		A transmitter will exit this function, but the front-end LED will be in use, as will the P1 port.
		A switch node will print status information to P1 if a computer is connected to the Tx side, which is otherwise unused.
		*/
void relaySetup(void){
	/*osDelay(100);
	RTOS_blink(100);*/
	#ifndef TX
		//Reciver module
	
		osDelay(100);
		RTOS_blink(100);
	
		uint32_t value = 0;
		uint8_t binValue = 0;
		
		//HAL_UART_Transmit_DMA(GetUart(P1),&binValue,1);
		while(1){
			
			HAL_ADC_Start(&hadc);

		while(HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY) != HAL_OK) {}
		
			// Digital
			
			value = HAL_ADC_GetValue(&hadc);
			//RTOS_blink(200);
			//Threshold used here
			if (value >= THRESHOLD){
				binValue = 1;//Chars work too actually
				LED_on(); //Displays current state
			}		
			else{
				binValue = 0;
				LED_off(); //Displays current state
			}
			
			//LED_on();
			HAL_ADC_Stop(&hadc);
			//LED_off();
			
			//The port used can be changed here:
			HAL_UART_Transmit(GetUart(P1),&binValue,1,HAL_MAX_DELAY);
			/*HAL_UART_Transmit_DMA(GetUart(P1),&binValue,1)*/ //Couldn't quite figure out how to get this to send repeatedly
			#ifdef SPLITTER
			HAL_UART_Transmit(GetUart(P2),&binValue,1,HAL_MAX_DELAY);//Second destination
			#endif
			osDelay(1);
		}
		
		
	#endif
		
	#ifdef TX
		
		//Transmitter
		
		#ifndef SWITCH
			//This is the only needed code for most transmitters.
			//The port used can be changed here:
			relayDMASetup(GetUart(P1),(uint8_t *)&(GPIOB->ODR), 1);	//FE LED memory mapped IO location
			osDelay(1);
		
		#else
		//Switch code
		
		
		char activeMessage[150];
		
		//RTOS_blink(1500);
		
		//The ports used can be changed here:
		relayDMASetup(GetUart(P1), &data1, 1);
		relayDMASetup2(GetUart(P2), &data2, 1);
		osDelay(1);
		
		//RTOS_blink(1500);
		//Timers
		TimerHandle_t dataLostTimer= xTimerCreate
			 ( /* Just a text name, not used by the RTOS kernel. */
				 "Data Loss",
				 /* The timer period in ticks, must be greater than 0. */
				 pdMS_TO_TICKS( STREAM_TIMEOUT ), 
				 /* The timers will *not* auto-reload themselves when they expire. */
				 pdFALSE,
				 /* The ID is unused here */
				 ( void * ) 0,
				 /* Each timer calls the same callback when
				 it expires. */
				 vTimerCallbackSwitch
			 );
				 
		TimerHandle_t datarateTimer= xTimerCreate
			 ( /* Just a text name, not used by the RTOS kernel. */
				 "Datarate Timer",
				 /* The timer period in ticks, must be greater than 0. */
				 pdMS_TO_TICKS( MSDATARATE ), //timing
				 /* The timers will auto-reload themselves when they expire. */
				 pdTRUE,
				 /* A value to identify the start of a message. */
				 ( void * ) 1,
				 /* Each timer calls the same callback when
				 it expires. */
				 vTimerCallbackReceive
			 );
		
		while(1){
			//Used for testing transmission modes
			//HAL_UART_Transmit(GetUart(P1),hello,9, HAL_MAX_DELAY);
			//HAL_UART_Transmit_DMA(GetUart(P1),hello,9);
			//HAL_UART_Transmit_IT(GetUart(P1),hello,9);
			
			//Debug and status messages
			//sprintf(activeMessage, "Active data stream is %d. Status 1: %d Status 2: %d\r",(currStream == &data1) ? 1 : 2,data1,data2);
			sprintf(activeMessage, "Active data stream is %d. Status 1: %d Status 2: %d Character: %d \r",
					(currStream == &data1) ? 1 : 2,data1,data2,currPos-receiveBuffer);
			
			if(dataDone){
				sprintf(activeMessage, "\n\rMessage Received: %s \n\r",receiveBuffer);
				//Reset for next message
				dataDone=0;
				LED_off();
				osDelay(400);
			}
			
			HAL_UART_Transmit(GetUart(P1),(uint8_t *) activeMessage,strlen(activeMessage),HAL_MAX_DELAY);
			
			
			//Switch logic
			if((*currStream)){
				FE_LED_on();
				LED_on();
				if(xTimerIsTimerActive( dataLostTimer ) != pdFALSE){
					//Timer is active, need to stop it since data is still coming
					xTimerStop(dataLostTimer, 0 );
				}
			}else{
				FE_LED_off();
				LED_off();
				if(xTimerIsTimerActive( dataLostTimer ) == pdFALSE){
					//Timer inactive, need to start
					xTimerStart(dataLostTimer, 0 );
				}
				if(xTimerIsTimerActive( datarateTimer ) == pdFALSE){
					//Timer inactive, this is start bit
					xTimerStart(datarateTimer, 0 );
				}
				
				
			}
			
		}
		
		#endif
	#endif
}


/**
	Available if _debug_threshold is defined. Repeatedly prints the current value of the 
	receiver to P1 for debugging purposes. This should be used to determine the needed 
	threshold values in the intended lighting conditions.
*/
#ifdef _debug_threshold
void debugThreshold(void){
	#ifndef TX
		
	
		char message[30];
	
		osDelay(100);
		RTOS_blink(100);
	
		uint32_t value = 0;
		//uint8_t binValue = 0;
		
		/*LED_on();
		GetUart(P1)->hdmatx->Instance->CCR = GetUart(P1)->hdmatx->Instance->CCR | DMA_CCR_CIRC;//Sets up repeated DMA transfers?
		RTOS_blink(100);
		HAL_UART_Transmit_DMA(GetUart(P1),(uint8_t *) message, 25);
		LED_off();*/
	
		while(1){
			
			//RTOS_blink(100);
			HAL_ADC_Start(&hadc);

		while(HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY) != HAL_OK) {}
		
			// Digital
			
			value = HAL_ADC_GetValue(&hadc);
			//RTOS_blink(200);
			if (value >= THRESHOLD){
				
				LED_on();
			}		
			else{
				
				LED_off();
			}
			
			
			
			HAL_ADC_Stop(&hadc);
			
			sprintf(message,"Curr Value: %d\n\r",value);
			
			//HAL_UART_Transmit(GetUart(P1),(uint8_t *) message, strlen(message),HAL_MAX_DELAY);
			/*HAL_UART_Transmit_DMA(GetUart(P1),&binValue,1)*/
			
			osDelay(1);
		}
		
		
	#endif
}
#endif

#ifdef _data_test
char message[MESSAGE_LENGTH]=MESSAGE;
char *currPos=message;
char debugMessage[100];

//Timer callback used to send data at a constant rate
/**
	Available if _data_test is defined. Timer interrupt service routine for sending data bits at reliable intervals. 
	Not strictly necessary for the operation of the relay system,but used to test data communications. Should only be 
	called as an ISR. See http://www.freertos.org/RTOS-software-timer.html and 
	http://www.freertos.org/FreeRTOS-Software-Timer-API-Functions.html for a more detailed explaination of the use of timers.

*/
void vTimerCallbackSend( TimerHandle_t xTimer )
{
	char currentChar=(*currPos);
	signed int count = ( signed int ) pvTimerGetTimerID( xTimer );
	
	if(count==-2){
		//Start bit (low)
		FE_LED_off();
		count++;
		vTimerSetTimerID( xTimer, ( void * ) count );
		return;
	}else if(count==-1){
		//Start bit (high)
		FE_LED_on();
		count++;
		vTimerSetTimerID( xTimer, ( void * ) count );
		return;
	}
	
	char currentBit=(currentChar>>count)&0x01;
	
	sprintf(debugMessage, "Now sending %c, bit %d, value %d",currentChar,count,currentBit);
	HAL_UART_Transmit_IT(GetUart(P1),(uint8_t *) debugMessage,strlen(debugMessage));
	
	
	
	
	if(currentBit){
		FE_LED_on();//Bit is a 1
	}else{
		FE_LED_off();
	}
	count++;
	if(count>7){
		currPos++;
		count=0;
		if(currPos>(message+strlen(message))){
			xTimerStop(xTimer,0);
			LED_on();
			FE_LED_on();
		}
	}
	vTimerSetTimerID( xTimer, ( void * ) count );
	
}
/**
	Available if _data_test is defined. Initializes the timer to send data via the front-end LED. 
	The rate of transmission is defined by MSDATARATEin relay.h. Enters an infinite loop and does not return. 
	Used to test transmitting data, and is not required for the function of the relays.
*/

void dataTest(){
	TimerHandle_t datarateTimer= xTimerCreate
			 ( /* Just a text name, not used by the RTOS kernel. */
				 "Datarate Timer",
				 /* The timer period in ticks, must be greater than 0. */
				 pdMS_TO_TICKS( MSDATARATE ), //timing
				 /* The timers will auto-reload themselves when they expire. */
				 pdTRUE,
				 /* The bit being sent, which
				 is initialised to -2. */
				 ( void * ) -2,
				 /* Each timer calls the same callback when
				 it expires. */
				 vTimerCallbackSend
			 );
	FE_LED_on();
	RTOS_blink(2000);
	xTimerStart(datarateTimer,0);
	while(1);//May not be nessesary, probably can continue to do other things
}
#endif

#ifdef SWITCH
#ifdef TX
//Timer callback to switch active data stream
/**
	Available if TX and SWITCH are defined. Timer interrupt service routine for changing active stream 
	if the currently active stream has gone silent for some time. The amount of time is defined by STREAM_TIMEOUT 
	in relay.h. This function is intended for internal use only, and is only intended to be called as an ISR. 
	See http://www.freertos.org/RTOS-software-timer.html and http://www.freertos.org/FreeRTOS-Software-Timer-API-Functions.html 
	for a more detailed explaination of the use of timers.
*/
void vTimerCallbackSwitch( TimerHandle_t xTimer )
{
		
		/* Optionally do something if the pxTimer parameter is NULL. */
		//configASSERT( pxTimer );
		currStream = (currStream == &data1) ? &data2 : &data1;
		
}



//Timer callback used to recive data at a constant rate.
/**
	Available if TX and SWITCH are defined. Timer interrupt service routine for receiving data at a constant rate 
	defined by MSDATARATE in relay.h. This function is intended for internal use only, and is only intended to be called as an ISR. 
	See http://www.freertos.org/RTOS-software-timer.html and http://www.freertos.org/FreeRTOS-Software-Timer-API-Functions.html 
	for a more detailed explaination of the use of timers.

*/
void vTimerCallbackReceive( TimerHandle_t xTimer )
{
	uint32_t idTemp=(uint32_t)pvTimerGetTimerID( xTimer );
	if(idTemp==2){
		//Multiple 0s at start, noise.
		
		//1 now reviced, stop timer and set Id value back to 1
		if(*currStream){
			xTimerStop(xTimer , 0);
			vTimerSetTimerID( xTimer, ( void * ) 1 );
			//Now ready for the real start bit
			return;
		}
		else{
			//Still just noise.
			return;
		}
	}
	else if(idTemp==1){
		//Start bit 2
		if(*currStream){
			//Correct, progress to next state
			vTimerSetTimerID( xTimer, ( void * ) 0 );
			return;
		}else{
			//Noise, no point in stopping the timer, it will just restart.
			//Instead, indicate that multiple 0s have been recived, and this is not the start bit.
			vTimerSetTimerID( xTimer, ( void * ) 2 );
			return;
		}
	}
	
	
	int currBit= (*currStream)<<currBitPos;
	*currPos=(*currPos)|currBit;
	currBitPos++;
	if(currBitPos>7){
		
		currPos++;
		currBitPos=0;
		
		
		//End if the length is exceeded.
		if(currPos>=(receiveBuffer+MESSAGE_LENGTH)){
			xTimerStop(xTimer,0);
			//We're done here, reset internally used values to prepeare for future messages.
			currPos=receiveBuffer;
			vTimerSetTimerID( xTimer, ( void * ) 1 );
			dataDone=1;
		}
	}
	
}
#endif
#endif


/* Port - Memory DMA setup function */
/**
	Sets up a DMA stream from the specified port (huart) to the specified destination address (dest) of the specified size in bytes (num). 
	Used internally to setup the DMA streams for the Tx modules.
*/
void relayDMASetup(UART_HandleTypeDef* huart, uint8_t * dest, uint16_t num)
{	
		// Configure DMA - Port to Memory on RX module
		// UART RX DMA (DMA1 Ch5)
		dma_port_mem3.Instance = DMA1_Channel5;
		HAL_DMA_Init(&dma_port_mem3);	
		if (huart->Instance == USART1) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART1_RX);
		} else if (huart->Instance == USART2) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART2_RX);
		} else if (huart->Instance == USART3) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART3_RX);
		} else if (huart->Instance == USART4) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART4_RX);
		} else if (huart->Instance == USART5) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART5_RX);
		} else if (huart->Instance == USART6) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART6_RX);
		} else if (huart->Instance == USART7) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART7_RX);
		} else if (huart->Instance == USART8) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART8_RX);
		}		
		__HAL_LINKDMA(huart,hdmarx,dma_port_mem3);
		
		// Setup streaming baudrate	
		huart->Init.BaudRate = 115200;
		HAL_UART_Init(huart);
		
		/* DMA interrupt init */
		HAL_NVIC_SetPriority(DMA1_Ch1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch1_IRQn);
		
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, dest, num);
}

/* Port - Memory 2 DMA setup function */
/**
	Sets up a DMA stream from the specified port (huart) to the specified destination address (dest) of the specified size in bytes (num). 
	Used internally to setup the DMA streams for the switch Tx module.
*/
void relayDMASetup2(UART_HandleTypeDef* huart, uint8_t * dest, uint16_t num)
{	
	// Configure DMA - Port to Memory on RX module
		// UART RX DMA (DMA1 Ch3)
		dma_port_mem2.Instance = DMA1_Channel3;
		HAL_DMA_Init(&dma_port_mem2);	
		if (huart->Instance == USART1) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART1_RX);
		} else if (huart->Instance == USART2) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART2_RX);
		} else if (huart->Instance == USART3) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART3_RX);
		} else if (huart->Instance == USART4) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART4_RX);
		} else if (huart->Instance == USART5) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART5_RX);
		} else if (huart->Instance == USART6) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART6_RX);
		} else if (huart->Instance == USART7) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART7_RX);
		} else if (huart->Instance == USART8) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART8_RX);
		}		
		__HAL_LINKDMA(huart,hdmarx,dma_port_mem2);
		
		// Setup streaming baudrate	
		huart->Init.BaudRate = 115200;
		HAL_UART_Init(huart);
		
		/* DMA interrupt init */
		//This part may be incorrect, but it works correctly as is.
		//The function originally had the lines currently commented out.
		//HAL_NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0, 0);
		//HAL_NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
		HAL_NVIC_SetPriority(DMA1_Ch1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch1_IRQn);
				
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, dest, num);
}
