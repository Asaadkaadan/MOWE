/**
  ******************************************************************************
  * File Name          : aos.h
  * Created            : 01/05/2016 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : Array Operating System (AOS) header file.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __aos_H
#define __aos_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
	 
// HAL
#include "stm32f0xx_hal.h" 
	 
// RTOS
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
	 
// RTOS CLI
#include "FreeRTOS_CLI.h"
#include "UART-interrupt-driven-command-console.h"

// Array Topology
//#include "topology_hexaRX.h"		 
//#include "topology_hexaTX.h"
//#include "topology_pentaRX.h"		 
//#include "topology_pentaTX.h"	
//#include "topology_line.h"	
#include "topology_sph_hexaRX.h"	
//#include "topology_txdome.h"	
//#include "topology_line_10_rx.h"
//#include "37_flat_rx.h"
	 
// AOS Peripheral Drivers	(PD) 
#include "aos_pd_usart.h"	
#include "aos_pd_gpio.h"	
#include "aos_pd_adc.h"	
#include "aos_pd_dma.h"	


// AOS Command Line Interface	(CLI) Commands 
#include "aos_cli.h"	

// C STD Library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>	 

// APPs



/* Public Defines -----------------------------------------------------------*/

// Define TX
#if (HO02R0 || HO02R1 || HO02R2 || PO02R0 || HO02R2)		// Transmitters
	#define TX	1
#endif
#if (HO01R1 || HO01R2 || HO01R3 || PO01R0 || HO01R3)		// Receivers
	#define	IR	1			// Using VEMT3700F	 
#endif

	 
// Firmware
#define	_firmVersion		"FOR001"
#define _firmDate				__DATE__
#define _firmTime				__TIME__

// Error flag
static char errFlag = 1;

// ID
extern uint16_t myID;

// Port status
extern uint8_t portStatus[7];

// Delay macros
#define	Delay_us(t)			startMicroDelay(t)		/* RTOS safe */
#define	Delay_ms(t)			HAL_Delay(t)					/* Non-RTOS safe */
#define	Delay_s(t)			HAL_Delay(1000*t)			/* Non-RTOS safe */

// Indicator LED
#define LED_toggle()		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_6)		
#define LED_on()				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)		
#define LED_off()				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)		
#define blink(t)				LED_on();	HAL_Delay(t); LED_off()				/* Use before starting the scheduler */
#define RTOS_blink(t)		LED_on();	osDelay(t); LED_off()					/* Use after starting the scheduler */

// Front-end LED
#define FE_LED_state()			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define FE_LED_toggle()			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0)		
#define FE_LED_on()					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)		
#define FE_LED_off()				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)	
#define FE_blink(t)					FE_LED_on(); HAL_Delay(t); FE_LED_off()			/* Use before starting the scheduler */	
#define RTOS_FE_blink(t)		FE_LED_on();	osDelay(t); FE_LED_off()			/* Use after starting the scheduler */

// Enumerations
enum portNames{PC, P1, P2, P3, P4, P5, P6};
enum portStatus{FREE, STREAMING, CLI};
enum errors{Error, NoErrors, WrongID, ModuleDoesNotExist, WrongAlias, WrongRate, WrongPort, WrongWidth};

// Port-UART mapping
#if (HO01R1 || HO02R0)
	#define P1uart &huart2	
	#define P2uart &huart3
	#define P3uart &huart7
	#define P4uart &huart8
	#define P5uart &huart4
	#define P6uart &huart5
#endif
#if (HO01R2 || HO02R1)
	#ifndef P1uart	
		#define P1uart &huart2
	#endif
	#ifndef P2uart	
		#define P2uart &huart6
	#endif
	#ifndef P3uart
		#define P3uart &huart3
	#endif
	#ifndef P4uart	
		#define P4uart &huart5
	#endif
	#ifndef P5uart	
		#define P5uart &huart1
	#endif
	#ifndef P6uart	
		#define P6uart &huart4
	#endif
#endif
#if (HO01R3 || HO02R2)
	#ifndef P1uart	
		#define P1uart &huart2
	#endif
	#ifndef P2uart	
		#define P2uart &huart3
	#endif
	#ifndef P3uart
		#define P3uart &huart7
	#endif
	#ifndef P4uart	
		#define P4uart &huart8
	#endif
	#ifndef P5uart	
		#define P5uart &huart4
	#endif
	#ifndef P6uart	
		#define P6uart &huart1
	#endif
#endif
#if (PO01R0 || PO02R0)
	#ifndef P1uart
		#define P1uart &huart5
	#endif
	#ifndef P2uart
		#define P2uart &huart2
	#endif
	#ifndef P3uart
		#define P3uart &huart3
	#endif
	#ifndef P4uart
		#define P4uart &huart8
	#endif
	#ifndef P5uart
		#define P5uart &huart4
	#endif
#endif	

/* Place holder for calls to ioctl that don't use the value parameter. */
#define cmdPARAMTER_NOT_USED		( ( void * ) 0 )

/* Block times of 50 and 500milliseconds, specified in ticks. */
#define cmd50ms						( 50UL / portTICK_RATE_MS ) 
#define cmd500ms					( 500UL / portTICK_RATE_MS ) 

/* Streaming definitions */
#define	_master		_module_1
#define	ArraySize		20

#if (_module == 1)
	#define	inport	0
#elif (_module == 2)
	#define	inport	P6
#elif (_module == 3)
	#define	inport	P4
#elif (_module == 4)
	#define	inport	P2
#elif (_module == 5)
	#define	inport	P2
#elif (_module == 6)
	#define	inport	P6
#elif (_module == 7)
	#define	inport	P4
#elif (_module == 8)
	#define	inport	P2
#elif (_module == 9)
	#define	inport	P2
#elif (_module == 10)
	#define	inport	P6
#elif (_module == 11)
	#define	inport	P4
#elif (_module == 12)
	#define	inport	P2
#elif (_module == 13)
	#define	inport	P4
#elif (_module == 14)
	#define	inport	P6
#elif (_module == 15)
	#define	inport	P4
#elif (_module == 16)
	#define	inport	P2
#elif (_module == 17)
	#define	inport	P4
#elif (_module == 18)
	#define	inport	P6
#elif (_module == 19)
	#define	inport	P4
#elif (_module == 20)
	#define	inport	P2
#endif

/* Streaming external variables */
extern uint8_t syncCodeMode0;
extern uint8_t testCodeMode0;
extern uint8_t syncCodeMode1;
extern uint8_t testCodeMode1;
extern uint8_t syncCodeMode2;
extern uint8_t testCodeMode2;
extern uint8_t syncCodeMode3;
extern uint8_t testCodeMode3;
extern uint8_t streamTest;
extern uint8_t streamLoose;
extern uint8_t ledStatus;
extern uint8_t myTrig;												
extern uint8_t dataBuffer[ArraySize*2];

/* External function prototypes -----------------------------------------------*/
extern void AOS_init(void);
extern void SystemClock_Config(void);
extern void MX_FREERTOS_Init(void);
extern UART_HandleTypeDef* GetUart(uint8_t port);
extern int GetPort(UART_HandleTypeDef *huart);
extern void startPerEvent(uint32_t rateFactor);
extern void startMicroDelay(uint16_t Delay);
extern void StreamSetup(uint8_t port, float rate);
extern void SampleAndForward(void);


#ifdef __cplusplus
}
#endif
#endif /*__aos_H */


/************************ (C) COPYRIGHT OU *****END OF FILE****/
