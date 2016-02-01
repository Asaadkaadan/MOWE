/**
  ******************************************************************************
  * File Name          : aos_pd_USART.c
  * Date               : 05/05/2015 12:12:29
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
	
/* Includes ------------------------------------------------------------------*/
#include "aos.h"


#if (HO01R2 || HO02R1)
UART_HandleTypeDef huart1;
#endif
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
UART_HandleTypeDef huart2;
#endif
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
UART_HandleTypeDef huart3;
#endif
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
UART_HandleTypeDef huart4;
#endif
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
UART_HandleTypeDef huart5;
#endif
#if (HO01R2 || HO02R1)
UART_HandleTypeDef huart6;
#endif
#if (HO01R1 || HO02R0)
UART_HandleTypeDef huart7;
#endif
#if (HO01R1 || HO02R0 || PO01R0 || PO02R0)
UART_HandleTypeDef huart8;
#endif


/* Private function prototypes -----------------------------------------------*/


/* USARTs init function */
void MX_UART_Init(void)
{
#if (HO01R1 || HO02R0)
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USART7_UART_Init();
	MX_USART8_UART_Init();
	MX_USART4_UART_Init();
	MX_USART5_UART_Init();
#elif (HO01R2 || HO02R1)
	MX_USART2_UART_Init();
	MX_USART6_UART_Init();
	MX_USART3_UART_Init();
	MX_USART5_UART_Init();
	MX_USART1_UART_Init();
	MX_USART4_UART_Init();
#elif (PO01R0 || PO02R0)
	MX_USART5_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USART8_UART_Init();
	MX_USART4_UART_Init();
#endif	
}

/* USART1 init function */
#if (HO01R2 || HO02R1)
void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	#if (_P5pol == _reversed)	
		huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
		huart1.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
	#endif	
  HAL_UART_Init(&huart1);
}
#endif

/* USART2 init function */
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	#if (PO01R0 || PO02R0) 
		#if (_P2pol_reversed)	
			huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
			huart2.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
		#endif
	#else
		#if (_P1pol_reversed)	
			huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
			huart2.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
		#endif	
	#endif	
  HAL_UART_Init(&huart2);
}
#endif

/* USART3 init function */
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
void MX_USART3_UART_Init(void)
{
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	#if (HO01R1 || HO02R0)
		#if (_P2pol_reversed)	
			huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
			huart3.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
		#endif
	#else	
		#if (_P3pol_reversed)	
			huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
			huart3.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
		#endif	
	#endif	
  HAL_UART_Init(&huart3);
}
#endif

/* USART4 init function */
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
void MX_USART4_UART_Init(void)
{
  huart4.Instance = USART4;
  huart4.Init.BaudRate = 115200;	
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	#if (HO01R1 || HO02R0)
		#if (_P5pol_reversed)	
			huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
			huart4.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
		#endif
	#else	
		#if (_P6pol_reversed)	
			huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
			huart4.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
		#endif	
	#endif	
  HAL_UART_Init(&huart4);
}
#endif

/* USART5 init function */
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
void MX_USART5_UART_Init(void)
{
  huart5.Instance = USART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	#if (HO01R1 || HO02R0) && (_P6pol_reversed)	
		huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
		huart5.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
	#endif
	#if	(HO01R2 || HO02R1) && (_P4pol_reversed)	
		huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
		huart5.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;	
	#endif	
	#if	(PO01R0 || PO02R0) && (_P1pol_reversed)	
		huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
		huart5.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;	
	#endif	
  HAL_UART_Init(&huart5);
}
#endif

/* USART6 init function */
#if (HO01R2 || HO02R1)
void MX_USART6_UART_Init(void)
{
  huart5.Instance = USART6;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	#if (_P2pol_reversed)	
		huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
		huart6.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
	#endif
  HAL_UART_Init(&huart6);
}
#endif

/* USART7 init function */
#if (HO01R1 || HO02R0)
void MX_USART7_UART_Init(void)
{
  huart7.Instance = USART7;
  huart7.Init.BaudRate = 115200;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	#if (_P3pol_reversed)	
		huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
		huart7.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
	#endif
  HAL_UART_Init(&huart7);
}
#endif

/* USART8 init function */
#if (HO01R1 || HO02R0 || PO01R0 || PO02R0)
void MX_USART8_UART_Init(void)
{
  huart8.Instance = USART8;
  huart8.Init.BaudRate = 115200;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  huart8.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	#if (_P4pol_reversed)	
		huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
		huart8.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
	#endif
  HAL_UART_Init(&huart8);
}
#endif

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
	
	if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(huart->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* Peripheral clock enable */
    __USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(huart->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __USART3_CLK_ENABLE();
  
    /**USART3 GPIO Configuration  
		HO01R1, HO02R0, PO01R0, PO02R0
    PC4     ------> USART3_TX
    PC5     ------> USART3_RX 
		HO01R2, HO02R1
    PB_10   ------> USART3_TX
    PB_11   ------> USART3_RX 
    */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		#if (HO01R1 || HO02R0 || PO01R0 || PO02R0)
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
		GPIO_InitStruct.Alternate = GPIO_AF1_USART3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		#elif (HO01R2 || HO02R1)
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
		GPIO_InitStruct.Alternate = GPIO_AF4_USART3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		#endif		

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_8_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(huart->Instance==USART4)
  {
  /* USER CODE BEGIN USART4_MspInit 0 */

  /* USER CODE END USART4_MspInit 0 */
    /* Peripheral clock enable */
    __USART4_CLK_ENABLE();
  
    /**USART4 GPIO Configuration    
		HO01R1, HO02R0, PO01R0, PO02R0
    PC_10     ------> USART4_TX
    PC_11     ------> USART4_RX 
		HO01R2, HO02R1
    PA0     ------> USART4_TX
    PA1     ------> USART4_RX 
    */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		#if (HO01R1 || HO02R0 || PO01R0 || PO02R0)
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Alternate = GPIO_AF0_USART4;		
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		#elif (HO01R2 || HO02R1)
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART4;		
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		#endif
		
    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_8_IRQn);
  /* USER CODE BEGIN USART4_MspInit 1 */

  /* USER CODE END USART4_MspInit 1 */
  }
  else if(huart->Instance==USART5)
  {
  /* USER CODE BEGIN USART5_MspInit 0 */

  /* USER CODE END USART5_MspInit 0 */
    /* Peripheral clock enable */
    __USART5_CLK_ENABLE();
  
    /**USART5 GPIO Configuration  
		HO01R1, HO02R0, PO01R0, PO02R0
    PC12     ------> USART5_TX
    PD2     ------> USART5_RX 
		HO01R2, HO02R1
    PB_3     ------> USART5_TX
    PB_4     ------> USART5_RX 		
    */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		#if (HO01R1 || HO02R0 || PO01R0 || PO02R0)
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Alternate = GPIO_AF2_USART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Alternate = GPIO_AF2_USART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);		
		#elif (HO01R2 || HO02R1)
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART5;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);				
		#endif

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_8_IRQn);
  /* USER CODE BEGIN USART5_MspInit 1 */

  /* USER CODE END USART5_MspInit 1 */
  }
  else if(huart->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* Peripheral clock enable */
    __USART6_CLK_ENABLE();
  
    /**USART6 GPIO Configuration    
    PA_4     ------> USART6_TX
    PA_5     ------> USART6_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_USART6;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_8_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
  else if(huart->Instance==USART7)
  {
  /* USER CODE BEGIN USART7_MspInit 0 */

  /* USER CODE END USART7_MspInit 0 */
    /* Peripheral clock enable */
    __USART7_CLK_ENABLE();
  
    /**USART7 GPIO Configuration    
    PC_6     ------> USART7_TX
    PC_7     ------> USART7_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART7;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_8_IRQn);
  /* USER CODE BEGIN USART7_MspInit 1 */

  /* USER CODE END USART7_MspInit 1 */
  }
  else if(huart->Instance==USART8)
  {
  /* USER CODE BEGIN USART8_MspInit 0 */

  /* USER CODE END USART8_MspInit 0 */
    /* Peripheral clock enable */
    __USART8_CLK_ENABLE();
  
    /**USART8 GPIO Configuration    
    PC8     ------> USART8_TX
    PC9     ------> USART8_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART8;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Peripheral interrupt init*/
    HAL_NVIC_SetPriority(USART3_8_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_8_IRQn);
  /* USER CODE BEGIN USART8_MspInit 1 */

  /* USER CODE END USART8_MspInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void UART_Receive(void)
{
	HAL_UART_Receive_IT(&huart2, (uint8_t *)RXcom, 1);
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
