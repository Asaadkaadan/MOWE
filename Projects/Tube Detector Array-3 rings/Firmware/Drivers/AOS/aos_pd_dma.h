/**
  ******************************************************************************
  * File Name          : aos_pd_dma.h
  * Date               : 15/05/2015 19:11:53
  * Description        : This file contains all the function prototypes for
  *                      the dma.c file
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
  * Modified           : 01/07/2016 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __aos_pd_dma_H
#define __aos_pd_dma_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
	 
	 
/* Check which DMA interrupt occured */	 
#define HAL_DMA_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)  ((((__HANDLE__)->ISR & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)


/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef uplinkDMA1;
extern DMA_HandleTypeDef uplinkDMA2;
extern DMA_HandleTypeDef downlinkDMA1;
extern DMA_HandleTypeDef downlinkDMA2;
extern DMA_HandleTypeDef downlinkDMA3;
extern DMA_HandleTypeDef downlinkDMA4;
	 
/* External function prototypes ----------------------------------------------*/
extern void MX_DMA_Init(void);
extern void DownlinkDMA1_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void DownlinkDMA2_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void DownlinkDMA3_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void DownlinkDMA4_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void UplinkDMA1_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst);
extern void UplinkDMA2_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst);
	 

#ifdef __cplusplus
}
#endif

#endif /* __aos_pd_dma_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
