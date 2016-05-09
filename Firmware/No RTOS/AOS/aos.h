/**
  ******************************************************************************
  * File Name          : aos.h
  * Created            : 07/22/2015 
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
// Array Topology
//#include "topology.h"		 
//#include "topology_line.h"	
//#include "topology_hexaRX.h"	
//#include "topology_hexaTX.h"
//#include "topology_pentaRX.h"	
#include "topology_pentaTX.h"	
// AOS Peripheral Drivers	(PD) 
#include "aos_pd_adc.h"	
#include "aos_pd_gpio.h"	 
#include "aos_pd_usart.h"	
#include "aos_pd_dma.h"
#include "aos_pd_tim.h"
#include "aos_pd_stm32f0xx_it.h"
// AOS Inter-Array Communication (IAC) 	
#include "aos_iac_routing.h"
// AOS Module Drivers (MD)
#include "aos_md_fe.h" 
#include "aos_md_dbase.h" 
// AOS Command Line Parser (CLP)
#include "aos_clp.h"	 
// C STD Library
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>	 
	 
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

// Inaccurate counter macros for delays
#define	count_us(t)			startMicroDelay(t)
#define	count_s(t)			HAL_Delay(1000*t)

// Indicator LED
#define LED_toggle()		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_6)		
#define LED_on()				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)		
#define LED_off()				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)		
#define blink(t)				LED_on();	HAL_Delay(t); LED_off()
#define nblkBlink(t)		LED_on();	nblkTimout=t; nblkEvent=blinkEvent;

// Front-end LED
#define FE_LED_state()			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)
#define FE_LED_toggle()			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0)		
#define FE_LED_on()					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)		
#define FE_LED_off()				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)	
#define FE_blink(t)					FE_LED_on(); HAL_Delay(t); FE_LED_off()
#define FE_nblkBlink(t)			FE_LED_on(); nblkTimout=t; nblkEvent=FEblinkEvent

// Enumerations
enum portNames{PC, P1, P2, P3, P4, P5, P6};
enum portStatus{FREE, STREAMING, ReceivingMessage, ReceivingCommand, ReceivingStream};
enum errors{Error, NoErrors, WrongID, ModuleDoesNotExist, WrongAlias, WrongRate, WrongPort, WrongWidth};
enum nonBlockEvents{NoEvent, discoveryEvent, bcastEvent, bcastBaseEvent, samAllEvent, pingAllEvent, blinkEvent, FEblinkEvent};

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





#ifdef __cplusplus
}
#endif
#endif /*__aos_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT OU *****END OF FILE****/
