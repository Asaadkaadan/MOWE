/**
  ******************************************************************************
  * File Name          : aos_pd_dma.c
  * Date               : 15/05/2015 19:11:52
  * Description        : This file provides code for the configuration
  *                      of all the requested memory to memory DMA transfers.
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
	
/* Includes ------------------------------------------------------------------*/
#include "aos.h"



/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/


/* Variables ---------------------------------------------------------*/

DMA_HandleTypeDef uplinkDMA1;
DMA_HandleTypeDef uplinkDMA2;
DMA_HandleTypeDef downlinkDMA1;
DMA_HandleTypeDef downlinkDMA2;
DMA_HandleTypeDef downlinkDMA3;
extern uint8_t dataBuffer[ArraySize*2];

/* Private function prototypes -----------------------------------------------*/
void DownlinkDMA1_Init(void);
void DownlinkDMA2_Init(void);
void DownlinkDMA3_Init(void);
void UplinkDMA1_Init(void);
void UplinkDMA2_Init(void);

/*-----------------------------------------------------------*/

/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) 
{
	/* DMA controller clock enable */
	__DMA1_CLK_ENABLE();
	__DMA2_CLK_ENABLE();
	
	DownlinkDMA1_Init();
	DownlinkDMA2_Init();
	DownlinkDMA3_Init();
	UplinkDMA1_Init();
	UplinkDMA2_Init();
}

/*-----------------------------------------------------------*/

/* Downlink DMA 1 (port-to-memory) initialization */
void DownlinkDMA1_Init(void)
{
	/* UART RX DMA (DMA1 Ch5) */
	downlinkDMA1.Instance = DMA1_Channel5;
	downlinkDMA1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	downlinkDMA1.Init.PeriphInc = DMA_PINC_DISABLE;
	downlinkDMA1.Init.MemInc = DMA_MINC_ENABLE;
	downlinkDMA1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	downlinkDMA1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	downlinkDMA1.Init.Mode = DMA_CIRCULAR;
	downlinkDMA1.Init.Priority = DMA_PRIORITY_HIGH;
	
	HAL_DMA_Init(&downlinkDMA1);		
}

/*-----------------------------------------------------------*/

/* Downlink DMA 1 (port-to-memory) setup */
void DownlinkDMA1_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
{	
	/* UART RX DMA (DMA1 Ch5) */
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
	__HAL_LINKDMA(huart,hdmarx,downlinkDMA1);
	
	/* Setup streaming baudrate	*/
	huart->Init.BaudRate = 3000000;
	HAL_UART_Init(huart);
	
	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
	
	/* Start DMA stream	*/	
	HAL_UART_Receive_DMA(huart, (uint8_t *)&dataBuffer+start, num);			
}

/*-----------------------------------------------------------*/

/* Downlink DMA 2 (port-to-memory) initialization */
void DownlinkDMA2_Init(void)
{
	/* UART RX DMA (DMA1 Ch6) */
	downlinkDMA2.Instance = DMA1_Channel6;
	downlinkDMA2.Init.Direction = DMA_PERIPH_TO_MEMORY;
	downlinkDMA2.Init.PeriphInc = DMA_PINC_DISABLE;
	downlinkDMA2.Init.MemInc = DMA_MINC_ENABLE;
	downlinkDMA2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	downlinkDMA2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	downlinkDMA2.Init.Mode = DMA_CIRCULAR;
	downlinkDMA2.Init.Priority = DMA_PRIORITY_HIGH;
	
	HAL_DMA_Init(&downlinkDMA2);		
}

/*-----------------------------------------------------------*/

/* Downlink DMA 2 (port-to-memory) setup */
void DownlinkDMA2_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
{	
	/* UART RX DMA (DMA1 Ch5) */
	if (huart->Instance == USART1) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART1_RX);
	} else if (huart->Instance == USART2) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART2_RX);
	} else if (huart->Instance == USART3) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART3_RX);
	} else if (huart->Instance == USART4) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART4_RX);
	} else if (huart->Instance == USART5) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART5_RX);
	} else if (huart->Instance == USART6) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART6_RX);
	} else if (huart->Instance == USART7) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART7_RX);
	} else if (huart->Instance == USART8) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART8_RX);
	}		
	__HAL_LINKDMA(huart,hdmarx,downlinkDMA2);
	
	/* Setup streaming baudrate	*/
	huart->Init.BaudRate = 3000000;
	HAL_UART_Init(huart);
	
	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
	
	/* Start DMA stream	*/	
	HAL_UART_Receive_DMA(huart, (uint8_t *)&dataBuffer+start, num);			
}

/*-----------------------------------------------------------*/

/* Downlink DMA 3 (port-to-memory) initialization */
void DownlinkDMA3_Init(void)
{
	/* UART RX DMA (DMA2 Ch2) */
	downlinkDMA3.Instance = DMA2_Channel2;
	downlinkDMA3.Init.Direction = DMA_PERIPH_TO_MEMORY;
	downlinkDMA3.Init.PeriphInc = DMA_PINC_DISABLE;
	downlinkDMA3.Init.MemInc = DMA_MINC_ENABLE;
	downlinkDMA3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	downlinkDMA3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	downlinkDMA3.Init.Mode = DMA_CIRCULAR;
	downlinkDMA3.Init.Priority = DMA_PRIORITY_HIGH;
	
	HAL_DMA_Init(&downlinkDMA3);		
}

/*-----------------------------------------------------------*/

/* Downlink DMA 3 (port-to-memory) setup */
void DownlinkDMA3_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
{	
	/* UART RX DMA (DMA2 Ch2) */
	if (huart->Instance == USART1) {
		__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART1_RX);
	} else if (huart->Instance == USART2) {
		__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART2_RX);
	} else if (huart->Instance == USART3) {
		__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART3_RX);
	} else if (huart->Instance == USART4) {
		__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART4_RX);
	} else if (huart->Instance == USART5) {
		__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART5_RX);
	} else if (huart->Instance == USART6) {
		__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART6_RX);
	} else if (huart->Instance == USART7) {
		__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART7_RX);
	} else if (huart->Instance == USART8) {
		__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART8_RX);
	}		
	__HAL_LINKDMA(huart,hdmarx,downlinkDMA3);
	
	/* Setup streaming baudrate	*/
	huart->Init.BaudRate = 3000000;
	HAL_UART_Init(huart);
	
	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
	
	/* Start DMA stream	*/	
	HAL_UART_Receive_DMA(huart, (uint8_t *)&dataBuffer+start, num);			
}

/*-----------------------------------------------------------*/

/* Uplink DMA 1 (port-to-port) initialization */
void UplinkDMA1_Init(void)
{
	/* UART RX DMA (DMA1 Ch1) */
	uplinkDMA1.Instance = DMA1_Channel1;
	uplinkDMA1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	uplinkDMA1.Init.PeriphInc = DMA_PINC_DISABLE;
	uplinkDMA1.Init.MemInc = DMA_MINC_DISABLE;
	uplinkDMA1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	uplinkDMA1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	uplinkDMA1.Init.Mode = DMA_CIRCULAR;
	uplinkDMA1.Init.Priority = DMA_PRIORITY_HIGH;		
		
	HAL_DMA_Init(&uplinkDMA1);	
}

/*-----------------------------------------------------------*/

/* Uplink DMA 1 (port-to-port) setup */
void UplinkDMA1_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst)
{		
	/* UART RX DMA (DMA1 Ch1) */
	if (huartSrc->Instance == USART1) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART1_RX);
	} else if (huartSrc->Instance == USART2) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART2_RX);
	} else if (huartSrc->Instance == USART3) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART3_RX);
	} else if (huartSrc->Instance == USART4) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART4_RX);
	} else if (huartSrc->Instance == USART5) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART5_RX);
	} else if (huartSrc->Instance == USART6) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART6_RX);
	} else if (huartSrc->Instance == USART7) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART7_RX);
	} else if (huartSrc->Instance == USART8) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART8_RX);
	}		
	__HAL_LINKDMA(huartSrc,hdmarx,uplinkDMA1);

	/* Setup streaming baudrate	*/
	huartSrc->Init.BaudRate = 3000000;
	HAL_UART_Init(huartSrc);
	huartDst->Init.BaudRate = 3000000;
	HAL_UART_Init(huartDst);
	
	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA1_Ch1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Ch1_IRQn);
	
	/* Start DMA stream	*/	
	huartSrc->State = HAL_UART_STATE_READY;
	HAL_UART_Receive_DMA(huartSrc, (uint8_t *)(&(huartDst->Instance->TDR)), 1);

}

/*-----------------------------------------------------------*/

/* Uplink DMA 2 (port-to-port) initialization */
void UplinkDMA2_Init(void)
{
	/* UART RX DMA (DMA1 Ch3) */
	uplinkDMA2.Instance = DMA1_Channel3;
	uplinkDMA2.Init.Direction = DMA_PERIPH_TO_MEMORY;
	uplinkDMA2.Init.PeriphInc = DMA_PINC_DISABLE;
	uplinkDMA2.Init.MemInc = DMA_MINC_DISABLE;
	uplinkDMA2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	uplinkDMA2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	uplinkDMA2.Init.Mode = DMA_CIRCULAR;
	uplinkDMA2.Init.Priority = DMA_PRIORITY_HIGH;		
		
	HAL_DMA_Init(&uplinkDMA2);	
}

/*-----------------------------------------------------------*/

/* Uplink DMA 2 (port-to-port) setup */
void UplinkDMA2_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst)
{		
	/* UART RX DMA (DMA1 Ch3) */
	if (huartSrc->Instance == USART1) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART1_RX);
	} else if (huartSrc->Instance == USART2) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART2_RX);
	} else if (huartSrc->Instance == USART3) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART3_RX);
	} else if (huartSrc->Instance == USART4) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART4_RX);
	} else if (huartSrc->Instance == USART5) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART5_RX);
	} else if (huartSrc->Instance == USART6) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART6_RX);
	} else if (huartSrc->Instance == USART7) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART7_RX);
	} else if (huartSrc->Instance == USART8) {
		__HAL_DMA1_REMAP(HAL_DMA1_CH3_USART8_RX);
	}		
	__HAL_LINKDMA(huartSrc,hdmarx,uplinkDMA2);

	/* Setup streaming baudrate	*/
	huartSrc->Init.BaudRate = 3000000;
	HAL_UART_Init(huartSrc);
	huartDst->Init.BaudRate = 3000000;
	HAL_UART_Init(huartDst);
	
	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
	
	/* Start DMA stream	*/	
	huartSrc->State = HAL_UART_STATE_READY;
	HAL_UART_Receive_DMA(huartSrc, (uint8_t *)(&(huartDst->Instance->TDR)), 1);

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
