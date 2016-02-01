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
  * Modified           : 07/22/2015 
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

DMA_HandleTypeDef dma_port_fe;
DMA_HandleTypeDef dma_mem_fe;
DMA_HandleTypeDef dma_port_mem1;
DMA_HandleTypeDef dma_port_mem2;
DMA_HandleTypeDef dma_port_mem3;
DMA_HandleTypeDef dma_port_mem4;
DMA_HandleTypeDef dma_port_mem5;
DMA_HandleTypeDef dma_port_mem6;
DMA_HandleTypeDef dma_port_port_link1;
DMA_HandleTypeDef dma_port_port_link2;
DMA_HandleTypeDef dma_port_port_link3;
DMA_HandleTypeDef dma_port_port_link4;
DMA_HandleTypeDef dma_port_port_link5;
DMA_HandleTypeDef dma_port_port_link6;

uint8_t DMAbuffer[6][10] = {0};
uint8_t LED_TX_Buf = 0;
UART_HandleTypeDef* DMAdst[6] = {0};


/* Private function prototypes -----------------------------------------------*/
void MX_DMA_PORT_FE_Init(void);
void MX_DMA_MEM_FE_Init(void);



/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) 
{
		/* DMA controller clock enable */
		__DMA1_CLK_ENABLE();
		__DMA2_CLK_ENABLE();

	
		/* Peripheral DMA init*/
		MX_DMA_PORT_FE_Init();
	
//		MX_DMA_PORT_PORT_LINK1_Init();
//		MX_DMA_PORT_PORT_LINK2_Init();
//		MX_DMA_PORT_PORT_LINK3_Init();
//		MX_DMA_PORT_PORT_LINK4_Init();
//		MX_DMA_PORT_PORT_LINK5_Init();
	
		MX_DMA_PORT_MEM1_Init();
		MX_DMA_PORT_MEM2_Init();
		MX_DMA_PORT_MEM3_Init();
		MX_DMA_PORT_MEM4_Init();
		MX_DMA_PORT_MEM5_Init();
		#if (HO01R1 || HO02R0 || HO01R2 || HO02R1)
		MX_DMA_PORT_MEM6_Init();
		#endif
	
		// Setup array DMAs
		MX_DMA_PORT_MEM1_Setup(getUart(P1), 10, 0);
		MX_DMA_PORT_MEM2_Setup(getUart(P2), 10, 10);
		MX_DMA_PORT_MEM3_Setup(getUart(P3), 10, 20);
		MX_DMA_PORT_MEM4_Setup(getUart(P4), 10, 30);
		MX_DMA_PORT_MEM5_Setup(getUart(P5), 10, 40);
		#if (HO01R1 || HO02R0 || HO01R2 || HO02R1)
		MX_DMA_PORT_MEM6_Setup(getUart(P6), 10, 50);
		#endif
	
}

/* USER CODE BEGIN 2 */

/* Port - Front-end DMA init function */
void MX_DMA_PORT_FE_Init(void)
{
#ifdef TX	
		// Port to Memory / Memory to Front-end (GPIO)
		dma_port_fe.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_fe.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_fe.Init.MemInc = DMA_MINC_DISABLE;
    dma_port_fe.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_fe.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_fe.Init.Mode = DMA_CIRCULAR;
    dma_port_fe.Init.Priority = DMA_PRIORITY_HIGH;	
#else	
		// Front-end (ADC) to Memory / Memory to Port   
		dma_port_fe.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dma_port_fe.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_fe.Init.MemInc = DMA_MINC_DISABLE;
    dma_port_fe.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_fe.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_fe.Init.Mode = DMA_CIRCULAR;
    dma_port_fe.Init.Priority = DMA_PRIORITY_HIGH;
#endif    
		
}

/* Port - Front-end DMA setup function */
void MX_DMA_PORT_FE_Setup(UART_HandleTypeDef* huart)
{	
#ifdef TX
		// Configure DMA - Port to Front-end on TX module
		// UART RX DMA (DMA2 Ch2)
		dma_port_fe.Instance = DMA2_Channel2;
		HAL_DMA_Init(&dma_port_fe);	
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
		__HAL_LINKDMA(huart,hdmarx,dma_port_fe);
		
		/* DMA interrupt init */
		HAL_NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
		
		// Setup streaming baudrate	
		huart->Init.BaudRate = 921600;
		HAL_UART_Init(huart);
		
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, &LED_TX_Buf, 1);
		//HAL_UART_Receive_DMA(huart, (uint8_t *)(&(GPIOB->ODR)), 1);
		// Lock the port
		portStatus[getPort(huart)] = STREAMING;
		
#else
		
		// Configure DMA - Front-end to Port on RX module
		// UART TX DMA (DMA2 Ch1)
		dma_port_fe.Instance = DMA2_Channel1;		
		HAL_DMA_Init(&dma_port_fe);
		if (huart->Instance == USART1) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH1_USART1_TX);
		} else if (huart->Instance == USART2) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH1_USART2_TX);
		} else if (huart->Instance == USART3) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH1_USART3_TX);
		} else if (huart->Instance == USART4) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH1_USART4_TX);
		} else if (huart->Instance == USART5) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH1_USART5_TX);
		} else if (huart->Instance == USART6) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH1_USART6_TX);
		} else if (huart->Instance == USART7) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH1_USART7_TX);
		} else if (huart->Instance == USART8) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH1_USART8_TX);
		}			
    __HAL_LINKDMA(huart,hdmatx,dma_port_fe);
		
		// Configure ADC in continuous mode	
		hadc.Init.ContinuousConvMode = ENABLE;
		hadc.Init.DMAContinuousRequests = ENABLE;
		HAL_ADC_Init(&hadc);
		HAL_ADC_Start(&hadc);

		// Setup streaming baudrate	
		huart->Init.BaudRate = 921600;
		HAL_UART_Init(huart);
		
		// Start DMA stream		
		HAL_UART_Transmit_DMA(huart, (uint8_t *)(&(hadc.Instance->DR)), 1);
		// Lock the port
		portStatus[getPort(huart)] = STREAMING;
		
#endif	
		
}

/* Port - Memory 1 DMA init function */
void MX_DMA_PORT_MEM1_Init(void)
{
		// Port to Memory 
		dma_port_mem1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_mem1.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_mem1.Init.MemInc = DMA_MINC_ENABLE;
    dma_port_mem1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_mem1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_mem1.Init.Mode = DMA_CIRCULAR;
    dma_port_mem1.Init.Priority = DMA_PRIORITY_HIGH;		
}

/* Port - Memory 1 DMA setup function */
void MX_DMA_PORT_MEM1_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
{	
		// Configure DMA - Port to Memory on RX module
		// UART RX DMA (DMA1 Ch1)
		dma_port_mem1.Instance = DMA1_Channel1;
		HAL_DMA_Init(&dma_port_mem1);	
		if (huart->Instance == USART1) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART1_RX);
		} else if (huart->Instance == USART2) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART2_RX);
		} else if (huart->Instance == USART3) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART3_RX);
		} else if (huart->Instance == USART4) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART4_RX);
		} else if (huart->Instance == USART5) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART5_RX);
		} else if (huart->Instance == USART6) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART6_RX);
		} else if (huart->Instance == USART7) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART7_RX);
		} else if (huart->Instance == USART8) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH1_USART8_RX);
		}		
		__HAL_LINKDMA(huart,hdmarx,dma_port_mem1);
		
		// Setup streaming baudrate	
		huart->Init.BaudRate = 115200;
		HAL_UART_Init(huart);
		
		/* DMA interrupt init */
		HAL_NVIC_SetPriority(DMA1_Ch1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch1_IRQn);
		
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, (uint8_t *)&DMAbuffer+start, num);			
}

/* Port - Memory 2 DMA init function */
void MX_DMA_PORT_MEM2_Init(void)
{
		// Port to Memory 
		dma_port_mem2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_mem2.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_mem2.Init.MemInc = DMA_MINC_ENABLE;
    dma_port_mem2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_mem2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_mem2.Init.Mode = DMA_CIRCULAR;
    dma_port_mem2.Init.Priority = DMA_PRIORITY_HIGH;		
}

/* Port - Memory 2 DMA setup function */
void MX_DMA_PORT_MEM2_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
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
		HAL_NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
				
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, (uint8_t *)&DMAbuffer+start, num);			
}

/* Port - Memory 3 DMA init function */
void MX_DMA_PORT_MEM3_Init(void)
{
		// Port to Memory 
		dma_port_mem3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_mem3.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_mem3.Init.MemInc = DMA_MINC_ENABLE;
    dma_port_mem3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_mem3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_mem3.Init.Mode = DMA_CIRCULAR;
    dma_port_mem3.Init.Priority = DMA_PRIORITY_HIGH;		
}

/* Port - Memory 3 DMA setup function */
void MX_DMA_PORT_MEM3_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
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
		HAL_NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
				
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, (uint8_t *)&DMAbuffer+start, num);			
}

/* Port - Memory 4 DMA init function */
void MX_DMA_PORT_MEM4_Init(void)
{
		// Port to Memory 
		dma_port_mem4.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_mem4.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_mem4.Init.MemInc = DMA_MINC_ENABLE;
    dma_port_mem4.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_mem4.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_mem4.Init.Mode = DMA_CIRCULAR;
    dma_port_mem4.Init.Priority = DMA_PRIORITY_HIGH;		
}

/* Port - Memory 4 DMA setup function */
void MX_DMA_PORT_MEM4_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
{	
		// Configure DMA - Port to Memory on RX module
		// UART RX DMA (DMA1 Ch6)
		dma_port_mem4.Instance = DMA1_Channel6;
		HAL_DMA_Init(&dma_port_mem4);	
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
		__HAL_LINKDMA(huart,hdmarx,dma_port_mem4);
		
		// Setup streaming baudrate	
		huart->Init.BaudRate = 115200;
		HAL_UART_Init(huart);
		
		/* DMA interrupt init */
		HAL_NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
				
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, (uint8_t *)&DMAbuffer+start, num);			
}

/* Port - Memory 5 DMA init function */
void MX_DMA_PORT_MEM5_Init(void)
{
		// Port to Memory 
		dma_port_mem5.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_mem5.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_mem5.Init.MemInc = DMA_MINC_ENABLE;
    dma_port_mem5.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_mem5.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_mem5.Init.Mode = DMA_CIRCULAR;
    dma_port_mem5.Init.Priority = DMA_PRIORITY_HIGH;		
}

/* Port - Memory 5 DMA setup function */
void MX_DMA_PORT_MEM5_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
{	
		// Configure DMA - Port to Memory on RX module
		// UART RX DMA (DMA2 Ch2)
		dma_port_mem5.Instance = DMA2_Channel2;
		HAL_DMA_Init(&dma_port_mem5);	
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
		__HAL_LINKDMA(huart,hdmarx,dma_port_mem5);
		
		// Setup streaming baudrate	
		huart->Init.BaudRate = 115200;
		HAL_UART_Init(huart);
		
		/* DMA interrupt init */
		HAL_NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
				
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, (uint8_t *)&DMAbuffer+start, num);			
}

/* Port - Memory 6 DMA init function */
void MX_DMA_PORT_MEM6_Init(void)
{
		// Port to Memory 
		dma_port_mem6.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_mem6.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_mem6.Init.MemInc = DMA_MINC_ENABLE;
    dma_port_mem6.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_mem6.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_mem6.Init.Mode = DMA_CIRCULAR;
    dma_port_mem6.Init.Priority = DMA_PRIORITY_HIGH;		
}

/* Port - Memory 6 DMA setup function */
void MX_DMA_PORT_MEM6_Setup(UART_HandleTypeDef* huart, uint8_t num, uint8_t start)
{	
		// Configure DMA - Port to Memory on RX module
		// UART RX DMA (DMA2 Ch3)
		dma_port_mem6.Instance = DMA2_Channel3;
		HAL_DMA_Init(&dma_port_mem6);	
		if (huart->Instance == USART1) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART1_RX);
		} else if (huart->Instance == USART2) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART2_RX);
		} else if (huart->Instance == USART3) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART3_RX);
		} else if (huart->Instance == USART4) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART4_RX);
		} else if (huart->Instance == USART5) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART5_RX);
		} else if (huart->Instance == USART6) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART6_RX);
		} else if (huart->Instance == USART7) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART7_RX);
		} else if (huart->Instance == USART8) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART8_RX);
		}		
		__HAL_LINKDMA(huart,hdmarx,dma_port_mem6);
		
		// Setup streaming baudrate	
		huart->Init.BaudRate = 115200;
		HAL_UART_Init(huart);
		
		/* DMA interrupt init */
		HAL_NVIC_SetPriority(DMA1_Ch4_7_DMA2_Ch3_5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
				
		// Start DMA stream		
		huart->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huart, (uint8_t *)&DMAbuffer+start, num);			
}

/* Memory - Front-end DMA init function */
void MX_DMA_MEM_FE_Init(void)
{
	
}

/* Port - Port DMA init function - Link 1 */
void MX_DMA_PORT_PORT_LINK1_Init(void)
{
		// Port to Port
		dma_port_port_link1.Instance = DMA1_Channel1;
		dma_port_port_link1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_port_link1.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_port_link1.Init.MemInc = DMA_MINC_DISABLE;
    dma_port_port_link1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_port_link1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_port_link1.Init.Mode = DMA_CIRCULAR;
    dma_port_port_link1.Init.Priority = DMA_PRIORITY_MEDIUM;		
			
		HAL_DMA_Init(&dma_port_port_link1);	
}

/* Port - Port DMA setup function - Link 1 */
void MX_DMA_PORT_PORT_LINK1_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst)
{		
		// UART RX DMA (DMA1 Ch1)
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
		__HAL_LINKDMA(huartSrc,hdmarx,dma_port_port_link1);

		// Setup streaming baudrate	
		huartSrc->Init.BaudRate = 921600;
		HAL_UART_Init(huartSrc);
		huartDst->Init.BaudRate = 921600;
		HAL_UART_Init(huartDst);
		
		// Start DMA stream		
		DMAdst[0] = huartDst;
		huartSrc->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huartSrc, (uint8_t *)(&(huartDst->Instance->TDR)), 1);
		// Lock the ports
		portStatus[getPort(huartSrc)] = STREAMING;
		portStatus[getPort(huartDst)] = STREAMING;
}

/* Port - Port DMA init function - Link 2 */
void MX_DMA_PORT_PORT_LINK2_Init(void)
{
		// Port to Port
		dma_port_port_link2.Instance = DMA1_Channel3;
		dma_port_port_link2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_port_link2.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_port_link2.Init.MemInc = DMA_MINC_DISABLE;
    dma_port_port_link2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_port_link2.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_port_link2.Init.Mode = DMA_CIRCULAR;
    dma_port_port_link2.Init.Priority = DMA_PRIORITY_MEDIUM;		
			
		HAL_DMA_Init(&dma_port_port_link2);		
}

/* Port - Port DMA setup function - Link 2 */
void MX_DMA_PORT_PORT_LINK2_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst)
{
		// UART RX DMA (DMA1 Ch3)
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
		__HAL_LINKDMA(huartSrc,hdmarx,dma_port_port_link2);

		// Setup streaming baudrate	
		huartSrc->Init.BaudRate = 921600;
		HAL_UART_Init(huartSrc);
		huartDst->Init.BaudRate = 921600;
		HAL_UART_Init(huartDst);
		
		// Start DMA stream		
		DMAdst[1] = huartDst;
		huartSrc->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huartSrc, (uint8_t *)(&(huartDst->Instance->TDR)), 1);	
		// Lock the ports
		portStatus[getPort(huartSrc)] = STREAMING;
		portStatus[getPort(huartDst)] = STREAMING;
}

/* Port - Port DMA init function - Link 3 */
void MX_DMA_PORT_PORT_LINK3_Init(void)
{
		// Port to Port
		dma_port_port_link3.Instance = DMA1_Channel5;
		dma_port_port_link3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_port_link3.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_port_link3.Init.MemInc = DMA_MINC_DISABLE;
    dma_port_port_link3.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_port_link3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_port_link3.Init.Mode = DMA_CIRCULAR;
    dma_port_port_link3.Init.Priority = DMA_PRIORITY_MEDIUM;		
			
		HAL_DMA_Init(&dma_port_port_link3);
}

/* Port - Port DMA setup function - Link 3 */
void MX_DMA_PORT_PORT_LINK3_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst)
{		
		// UART RX DMA (DMA1 Ch5)
		if (huartSrc->Instance == USART1) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART1_RX);
		} else if (huartSrc->Instance == USART2) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART2_RX);
		} else if (huartSrc->Instance == USART3) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART3_RX);
		} else if (huartSrc->Instance == USART4) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART4_RX);
		} else if (huartSrc->Instance == USART5) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART5_RX);
		} else if (huartSrc->Instance == USART6) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART6_RX);
		} else if (huartSrc->Instance == USART7) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART7_RX);
		} else if (huartSrc->Instance == USART8) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH5_USART8_RX);
		}		
		__HAL_LINKDMA(huartSrc,hdmarx,dma_port_port_link3);

		// Setup streaming baudrate	
		huartSrc->Init.BaudRate = 921600;
		HAL_UART_Init(huartSrc);
		huartDst->Init.BaudRate = 921600;
		HAL_UART_Init(huartDst);
		
		// Start DMA stream		
		DMAdst[2] = huartDst;
		huartSrc->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huartSrc, (uint8_t *)(&(huartDst->Instance->TDR)), 1);	
		// Lock the ports
		portStatus[getPort(huartSrc)] = STREAMING;
		portStatus[getPort(huartDst)] = STREAMING;
}

/* Port - Port DMA init function - Link 4 */
void MX_DMA_PORT_PORT_LINK4_Init(void)
{
		// Port to Port
		dma_port_port_link4.Instance = DMA1_Channel6;
		dma_port_port_link4.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_port_link4.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_port_link4.Init.MemInc = DMA_MINC_DISABLE;
    dma_port_port_link4.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_port_link4.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_port_link4.Init.Mode = DMA_CIRCULAR;
    dma_port_port_link4.Init.Priority = DMA_PRIORITY_MEDIUM;		
			
		HAL_DMA_Init(&dma_port_port_link4);
}

/* Port - Port DMA setup function - Link 4 */
void MX_DMA_PORT_PORT_LINK4_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst)
{		
		// UART RX DMA (DMA1 Ch6)
		if (huartSrc->Instance == USART1) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART1_RX);
		} else if (huartSrc->Instance == USART2) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART2_RX);
		} else if (huartSrc->Instance == USART3) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART3_RX);
		} else if (huartSrc->Instance == USART4) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART4_RX);
		} else if (huartSrc->Instance == USART5) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART5_RX);
		} else if (huartSrc->Instance == USART6) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART6_RX);
		} else if (huartSrc->Instance == USART7) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART7_RX);
		} else if (huartSrc->Instance == USART8) {
			__HAL_DMA1_REMAP(HAL_DMA1_CH6_USART8_RX);
		}		
		__HAL_LINKDMA(huartSrc,hdmarx,dma_port_port_link4);

		// Setup streaming baudrate	
		huartSrc->Init.BaudRate = 921600;
		HAL_UART_Init(huartSrc);
		huartDst->Init.BaudRate = 921600;
		HAL_UART_Init(huartDst);
		
		// Start DMA stream		
		DMAdst[3] = huartDst;
		huartSrc->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huartSrc, (uint8_t *)(&(huartDst->Instance->TDR)), 1);	
		// Lock the ports
		portStatus[getPort(huartSrc)] = STREAMING;
		portStatus[getPort(huartDst)] = STREAMING;
}

/* Port - Port DMA init function - Link 5 */
void MX_DMA_PORT_PORT_LINK5_Init(void)
{
		// Port to Port
		dma_port_port_link5.Instance = DMA2_Channel2;
		dma_port_port_link5.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_port_link5.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_port_link5.Init.MemInc = DMA_MINC_DISABLE;
    dma_port_port_link5.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_port_link5.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_port_link5.Init.Mode = DMA_CIRCULAR;
    dma_port_port_link5.Init.Priority = DMA_PRIORITY_MEDIUM;		
			
		HAL_DMA_Init(&dma_port_port_link5);
}

/* Port - Port DMA setup function - Link 5 */
void MX_DMA_PORT_PORT_LINK5_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst)
{		
		// UART RX DMA (DMA2 Ch2)
		if (huartSrc->Instance == USART1) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART1_RX);
		} else if (huartSrc->Instance == USART2) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART2_RX);
		} else if (huartSrc->Instance == USART3) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART3_RX);
		} else if (huartSrc->Instance == USART4) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART4_RX);
		} else if (huartSrc->Instance == USART5) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART5_RX);
		} else if (huartSrc->Instance == USART6) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART6_RX);
		} else if (huartSrc->Instance == USART7) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART7_RX);
		} else if (huartSrc->Instance == USART8) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH2_USART8_RX);
		}		
		__HAL_LINKDMA(huartSrc,hdmarx,dma_port_port_link5);
		
		// Setup streaming baudrate	
		huartSrc->Init.BaudRate = 921600;
		HAL_UART_Init(huartSrc);
		huartDst->Init.BaudRate = 921600;
		HAL_UART_Init(huartDst);
		
		// Start DMA stream		
		DMAdst[4] = huartDst;
		huartSrc->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huartSrc, (uint8_t *)(&(huartDst->Instance->TDR)), 1);	
		// Lock the ports
		portStatus[getPort(huartSrc)] = STREAMING;
		portStatus[getPort(huartDst)] = STREAMING;
}

/* Port - Port DMA init function - Link 6 */
void MX_DMA_PORT_PORT_LINK6_Init(void)
{
		// Port to Port
		dma_port_port_link6.Instance = DMA2_Channel3;
		dma_port_port_link6.Init.Direction = DMA_PERIPH_TO_MEMORY;
    dma_port_port_link6.Init.PeriphInc = DMA_PINC_DISABLE;
    dma_port_port_link6.Init.MemInc = DMA_MINC_DISABLE;
    dma_port_port_link6.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    dma_port_port_link6.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    dma_port_port_link6.Init.Mode = DMA_CIRCULAR;
    dma_port_port_link6.Init.Priority = DMA_PRIORITY_MEDIUM;		
			
		HAL_DMA_Init(&dma_port_port_link6);
}

/* Port - Port DMA setup function - Link 6 */
void MX_DMA_PORT_PORT_LINK6_Setup(UART_HandleTypeDef* huartSrc, UART_HandleTypeDef* huartDst)
{		
		// UART RX DMA (DMA2 Ch3)
		if (huartSrc->Instance == USART1) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART1_RX);
		} else if (huartSrc->Instance == USART2) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART2_RX);
		} else if (huartSrc->Instance == USART3) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART3_RX);
		} else if (huartSrc->Instance == USART4) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART4_RX);
		} else if (huartSrc->Instance == USART5) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART5_RX);
		} else if (huartSrc->Instance == USART6) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART6_RX);
		} else if (huartSrc->Instance == USART7) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART7_RX);
		} else if (huartSrc->Instance == USART8) {
			__HAL_DMA2_REMAP(HAL_DMA2_CH3_USART8_RX);
		}		
		__HAL_LINKDMA(huartSrc,hdmarx,dma_port_port_link6);
		
		// Setup streaming baudrate	
		huartSrc->Init.BaudRate = 921600;
		HAL_UART_Init(huartSrc);
		huartDst->Init.BaudRate = 921600;
		HAL_UART_Init(huartDst);
		
		// Start DMA stream		
		DMAdst[5] = huartDst;
		huartSrc->State = HAL_UART_STATE_READY;
		HAL_UART_Receive_DMA(huartSrc, (uint8_t *)(&(huartDst->Instance->TDR)), 1);	
		// Lock the ports
		portStatus[getPort(huartSrc)] = STREAMING;
		portStatus[getPort(huartDst)] = STREAMING;
}

// --- Disable DMA stream on one of the module ports ---
void disableDMAport(uint8_t port)
{
		switch (port)
		{
				case P1 : 
						HAL_DMA_Abort(&dma_port_mem1);		
						break;
				case P2 :
						HAL_DMA_Abort(&dma_port_mem2);
						break;
				case P3 :
						HAL_DMA_Abort(&dma_port_mem3);
						break;
				case P4 :
						HAL_DMA_Abort(&dma_port_mem4);
						break;
				case P5 :
						HAL_DMA_Abort(&dma_port_mem5);
						break;
				#if (HO01R1 || HO02R0 || HO01R2 || HO02R1)
				case P6 :
						HAL_DMA_Abort(&dma_port_mem6);
				#endif
				default:
						break;
		}			
}

// --- Disable DMA UART interrupts ---
void disableDMAISR(uint8_t port)
{
		switch (port)
		{
				case P1 : 
						HAL_NVIC_DisableIRQ(DMA1_Ch1_IRQn);		
						break;
				case P2 :
						HAL_NVIC_DisableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
						break;
				case P3 :
						HAL_NVIC_DisableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
						break;
				case P4 :
						HAL_NVIC_DisableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
						break;
				case P5 :
						HAL_NVIC_DisableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
						break;
				#if (HO01R1 || HO02R0 || HO01R2 || HO02R1)
				case P6 :
						HAL_NVIC_DisableIRQ(DMA1_Ch4_7_DMA2_Ch3_5_IRQn);
				#endif
				default:
						break;
		}			
}

// --- Start a DMA stream on one of the module ports ---
void startDMAport(uint8_t port, uint8_t size)
{
		switch (port)
		{
				case P1 : 
						MX_DMA_PORT_MEM1_Setup(getUart(P1), size, 0);		
						break;
				case P2 :
						MX_DMA_PORT_MEM2_Setup(getUart(P2), size, 10);	
						break;
				case P3 :
						MX_DMA_PORT_MEM3_Setup(getUart(P3), size, 20);	
						break;
				case P4 :
						MX_DMA_PORT_MEM4_Setup(getUart(P4), size, 30);	
						break;
				case P5 :
						MX_DMA_PORT_MEM5_Setup(getUart(P5), size, 40);	
						break;
				#if (HO01R1 || HO02R0 || HO01R2 || HO02R1)
				case P6 :
						MX_DMA_PORT_MEM6_Setup(getUart(P6), size, 50);	
				#endif
				default:
						break;
		}			
}




/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
