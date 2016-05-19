/**
  ******************************************************************************
  * File Name          : aos_pd_USART.h
  * Date               : 15/05/2015 19:11:53
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/**
  ******************************************************************************
  * Modified           : 07/22/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __aos_pd_usart_H
#define __aos_pd_usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

	 
// Blocking (polling-based) read
#define readPx(port, buffer, n, timeout) while(HAL_UART_Receive(GetUart(port), (uint8_t *)buffer, n, timeout) != HAL_OK) {}
	
// Blocking (polling-based) write
#define writePx(port, buffer, n, timeout) while(HAL_UART_Transmit(GetUart(port), (uint8_t *)buffer, n, timeout) != HAL_OK) {}
	
	
/* External variables -----------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;	 
	
extern FlagStatus UartRxReady;
extern FlagStatus UartTxReady;
extern uint8_t PcPort;
	
/* External function prototypes -----------------------------------------------*/
extern void MX_USART1_UART_Init(void);
extern void MX_USART2_UART_Init(void);
extern void MX_USART3_UART_Init(void);
extern void MX_USART4_UART_Init(void);
extern void MX_USART5_UART_Init(void);
extern void MX_USART6_UART_Init(void);
extern void MX_USART7_UART_Init(void);
extern void MX_USART8_UART_Init(void);
extern void MX_UART_Init(void);
extern HAL_StatusTypeDef readPxMutex(uint8_t port, char *buffer, uint16_t n, uint32_t mutexTimeout, uint32_t portTimeout);
extern HAL_StatusTypeDef writePxMutex(uint8_t port, char *buffer, uint16_t n, uint32_t mutexTimeout, uint32_t portTimeout);
extern HAL_StatusTypeDef readPxITMutex(uint8_t port, char *buffer, uint16_t n, uint32_t mutexTimeout);
extern HAL_StatusTypeDef writePxITMutex(uint8_t port, char *buffer, uint16_t n, uint32_t mutexTimeout);

	

#ifdef __cplusplus
}
#endif
#endif /*__aos_pd_usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
