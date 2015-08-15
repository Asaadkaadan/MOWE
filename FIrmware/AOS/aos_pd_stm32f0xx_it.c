/**
  ******************************************************************************
  * @file    aos_pd_stm32f0xx_it.c
  * @date    05/05/2015 12:12:29
  * @brief   Interrupt Service Routines.
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


/* Variables -----------------------------------------------------------------*/

// Non-blocking timeout in msec
uint32_t nblkTimout = 0;
uint8_t nblkEvent = 0;

/* Private function prototypes -----------------------------------------------*/
void nblkTimoutRoutine(void);



/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* System interrupt init*/
/* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

}


/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{

  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
	
	// Update non-blocking timeout
	if (nblkTimout) 
		--nblkTimout;
	// Timeout
	if (!nblkTimout && (nblkEvent != NoEvent)) 
		nblkTimoutRoutine();
}

/**
* @brief Non-blocking timeout processing.
*/
void nblkTimoutRoutine(void)
{
	switch (nblkEvent)
	{
//		case discoveryEvent: 							// Inform base about neighboring modules
//			outPort = findRoute(myID, baseModule);
//			sprintf(sertemp,"%% #%03d #%03d ST %d %d %d %d %d %d\r",myID,baseModule,ST[0],ST[0],ST[0],ST[0],ST[0],ST[0]); 	
//			sendPacket(outPort, sertemp);	
//			nblkEvent = NoEvent;
//			break;
//		case bcastEvent: 									// After building broadcast routes, discover neighbors again
//			arrDiscover();
//			nblkTimout = 10;
//			nblkEvent = discoveryEvent;
//			break;
//		case bcastBaseEvent: 							// Display discovered modules to the user			
//			nblkEvent = NoEvent;
//			break;
		case samAllEvent:									// Timeout on sampling modules
			modStack = 1;
			pushMessage(1, _MesID_sampleAllDisp);							// Push a sampleAllDisp message	
			nblkEvent = NoEvent;
			break;
		case pingAllEvent:								// Timeout on pinging modules
			modStack = 1;
			pushMessage(1, _MesID_pingAllDisp);							// Push a pingAllDisp message	
			//nblkEvent = NoEvent;			// leave commented for the blinkEvent to occure
			break;
		case blinkEvent:									// Timeout on indicator LED blinking
			LED_off();
			nblkEvent = NoEvent;
			break;
		case FEblinkEvent:								// Timeout on front-end LED blinking
			FE_LED_off();
			nblkEvent = NoEvent;
			break;
		default:
			break;
	}

	
}


/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/
/**
* @brief This function handles DMA1 channel 1 interrupt.
*/
void DMA1_Ch1_IRQHandler(void)
{

		HAL_DMA_IRQHandler(&dma_port_mem1);

}

/**
* @brief This function handles DMA1 channel 2 to 3 and DMA2 channel 1 to 2 interrupts.
*/
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void)
{
		if (HAL_DMA_GET_IT_SOURCE(DMA2,DMA_ISR_TCIF2) == SET && stream2FE)
				HAL_DMA_IRQHandler(&dma_port_fe);
		else if (HAL_DMA_GET_IT_SOURCE(DMA1,DMA_ISR_TCIF3) == SET)
				HAL_DMA_IRQHandler(&dma_port_mem2);	
		else if (HAL_DMA_GET_IT_SOURCE(DMA2,DMA_ISR_TCIF2) == SET)
				HAL_DMA_IRQHandler(&dma_port_mem5);

}

/**
* @brief This function handles DMA1 channel 4 to 7 and DMA2 channel 3 to 5 interrupts.
*/
void DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler(void)
{

		if (HAL_DMA_GET_IT_SOURCE(DMA1,DMA_ISR_TCIF5) == SET)
				HAL_DMA_IRQHandler(&dma_port_mem3);
		else if (HAL_DMA_GET_IT_SOURCE(DMA1,DMA_ISR_TCIF6) == SET)
				HAL_DMA_IRQHandler(&dma_port_mem4);
		else if (HAL_DMA_GET_IT_SOURCE(DMA2,DMA_ISR_TCIF3) == SET)
				HAL_DMA_IRQHandler(&dma_port_mem6);
		
}

/**
* @brief This function handles TIM7 global interrupt. (1 usec timebase)
*/
void TIM7_IRQHandler(void)
{
		//microTick++;
		HAL_TIM_IRQHandler(&htim7);
}

/**
* @brief This function handles TIM2 global interrupt. (periodic event timebase)
*/
void TIM2_IRQHandler(void)
{
		myTrig = 1;
		HAL_TIM_IRQHandler(&htim2);
		
		// Stream for read/write commands
		stream();

}

/**
* @brief This function handles TIM3 global interrupt. (Front-end LED PWM)
*/
void TIM3_IRQHandler(void)
{	
		HAL_TIM_IRQHandler(&htim3);
}

/**
* @brief This function handles USART1 global interrupt (combined with EXTI line 25 interrupt).
*/
#if (HO01R2 || HO02R1)
void USART1_IRQHandler(void)
{
	
  HAL_UART_IRQHandler(&huart1);

}
#endif

/**
* @brief This function handles USART2 global interrupt (combined with EXTI line 26 interrupt).
*/
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
void USART2_IRQHandler(void)
{
	

  HAL_UART_IRQHandler(&huart2);

}
#endif

/**
* @brief This function handles USART3 to USART8 global interrupts (combined with EXTI line 28 interrupt).
*/
void USART3_8_IRQHandler(void)
{
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
		HAL_UART_IRQHandler(&huart3);
#endif
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
		HAL_UART_IRQHandler(&huart4);
#endif
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
		HAL_UART_IRQHandler(&huart5);
#endif
#if (HO01R2 || HO02R1)
		HAL_UART_IRQHandler(&huart6);
#endif
#if (HO01R1 || HO02R0)
		HAL_UART_IRQHandler(&huart7);
#endif
#if (HO01R1 || HO02R0 || PO01R0 || PO02R0)
		HAL_UART_IRQHandler(&huart8);
#endif

}


/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

		/* Set transmission flag: transfer complete*/
		switch (getPort(huart))
		{
				case P1 : 
						P1Ready = SET;		
						break;
				case P2 :
						P2Ready = SET;
						break;
				case P3 :
						P3Ready = SET;
						break;
				case P4 :
						P4Ready = SET;
						break;
				case P5 :
						P5Ready = SET;
						break;
				#if (HO01R1 || HO02R0 || HO01R2 || HO02R1)
				case P6 :
						P6Ready = SET;
						break;
				#endif
				default:
						break;
		}		
		
		// Check if message or command
		if (getPort(huart) == pcPort)
				PCReady = SET;
		else	
				parseMessage(getPort(huart));
		
		
		//LED_toggle();
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
