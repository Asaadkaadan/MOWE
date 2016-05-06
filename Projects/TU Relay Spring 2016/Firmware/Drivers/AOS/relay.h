/**
  ******************************************************************************
  * File Name          : relay.h
  * Created            : 03/31/2016 
  * Version            : 1.0 
 	* Author             : Will Nichols
  * Description        : Relay setup functions.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __relay_H
#define __relay_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "aos.h"
	 
/* External variables ---------------------------------------------------------*/
//figure these out later, if needed
	 
/* External macros ---------------------------------------------------------*/	 
/*#define LOW_THRESHOLD 35
#define HIGH_THRESHOLD 100*/
#define THRESHOLD 100
//Datarate in msec
#define MSDATARATE 100
//Time before swithcing streams (ms)
#define STREAM_TIMEOUT 1000 
#define MESSAGE_LENGTH 10
#define MESSAGE "Red Sox"

//Must configure timers in FreeRTOS.h using the following code:
/*
#ifndef configUSE_TIMERS
	#define configUSE_TIMERS 1
#endif

#ifndef configTIMER_TASK_PRIORITY
	#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#endif
//Not created by interrupts, should not be that large. Really should only be one at a time.
#ifndef configTIMER_QUEUE_LENGTH
	#define configTIMER_QUEUE_LENGTH 3
#endif
//This seems fine
#ifndef configTIMER_TASK_STACK_DEPTH
	#define configTIMER_TASK_STACK_DEPTH configMINIMAL_STACK_SIZE
#endif
*/

	
/* External function prototypes -----------------------------------------------*/
//figure these out later, if needed

/* Internal function prototypes -----------------------------------------------*/
void relaySetup(void);
void relayDMASetup(UART_HandleTypeDef* , uint8_t * , uint16_t );
void relayDMASetup2(UART_HandleTypeDef*, uint8_t * , uint16_t );
#ifdef _debug_threshold
void debugThreshold(void);
#endif
#ifdef _data_test
void dataTest(void);
#endif
#ifdef SWITCH
	#ifdef TX
	void vTimerCallbackSwitch( TimerHandle_t xTimer );
	void vTimerCallbackReceive( TimerHandle_t xTimer );
	#endif
#endif

#ifdef __cplusplus
}
#endif
#endif /*__relay_H */


/************************ (C) COPYRIGHT OU *****END OF FILE****/
