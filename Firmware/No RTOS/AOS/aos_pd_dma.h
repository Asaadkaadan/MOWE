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
  * Modified           : 07/22/2015 
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
	 

#define HAL_DMA_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)  ((((__HANDLE__)->ISR & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)
	 
/* External variables --------------------------------------------------------*/
extern uint8_t DMAbuffer[6][10];
extern uint8_t LED_TX_Buf;
extern UART_HandleTypeDef* DMAdst[6];
extern DMA_HandleTypeDef dma_port_fe;;
extern DMA_HandleTypeDef dma_port_mem1;
extern DMA_HandleTypeDef dma_port_mem2;
extern DMA_HandleTypeDef dma_port_mem3;
extern DMA_HandleTypeDef dma_port_mem4;
extern DMA_HandleTypeDef dma_port_mem5;
extern DMA_HandleTypeDef dma_port_mem6;
extern DMA_HandleTypeDef dma_port_port_link1;
extern DMA_HandleTypeDef dma_port_port_link2;
extern DMA_HandleTypeDef dma_port_port_link3;
extern DMA_HandleTypeDef dma_port_port_link4;
extern DMA_HandleTypeDef dma_port_port_link5;
extern DMA_HandleTypeDef dma_port_port_link6;
	 
/* External function prototypes ----------------------------------------------*/
extern void MX_DMA_Init(void);
extern void MX_DMA_PORT_MEM1_Init(void);
extern void MX_DMA_PORT_MEM2_Init(void);
extern void MX_DMA_PORT_MEM3_Init(void);
extern void MX_DMA_PORT_MEM4_Init(void);
extern void MX_DMA_PORT_MEM5_Init(void);
extern void MX_DMA_PORT_MEM6_Init(void);	 
extern void MX_DMA_PORT_PORT_LINK1_Init(void);
extern void MX_DMA_PORT_PORT_LINK2_Init(void);
extern void MX_DMA_PORT_PORT_LINK3_Init(void);
extern void MX_DMA_PORT_PORT_LINK4_Init(void);
extern void MX_DMA_PORT_PORT_LINK5_Init(void);
extern void MX_DMA_PORT_PORT_LINK6_Init(void);
extern void MX_DMA_PORT_FE_Setup(UART_HandleTypeDef* huart);
extern void MX_DMA_PORT_MEM1_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void MX_DMA_PORT_MEM2_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void MX_DMA_PORT_MEM3_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void MX_DMA_PORT_MEM4_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void MX_DMA_PORT_MEM5_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void MX_DMA_PORT_MEM6_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start);
extern void MX_DMA_PORT_PORT_LINK1_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst);
extern void MX_DMA_PORT_PORT_LINK2_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst);	 
extern void MX_DMA_PORT_PORT_LINK3_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst);
extern void MX_DMA_PORT_PORT_LINK4_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst);	 
extern void MX_DMA_PORT_PORT_LINK5_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst);
extern void MX_DMA_PORT_PORT_LINK6_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst);
extern void disableDMAport(uint8_t port);
extern void disableDMAISR(uint8_t port);
extern void startDMAport(uint8_t port, uint8_t size);


#ifdef __cplusplus
}
#endif

#endif /* __aos_pd_dma_H */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
