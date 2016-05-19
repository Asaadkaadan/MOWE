/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
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
  * Modified           : 01/05/2016 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "aos.h"



/* External variables --------------------------------------------------------*/
extern SemaphoreHandle_t PxSemaphoreHandle[7];
extern TaskHandle_t FrontEndTaskHandle;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;
extern uint16_t adc;


/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
	
	HAL_IncTick();
  osSystickHandler();  

}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles TIM2 global interrupt. (periodic event timebase)
*/
void TIM2_IRQHandler(void)
{
	myTrig = 1;
			
	HAL_TIM_IRQHandler(&htim2);
}

/**
* @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
*/
#if (HO01R2 || HO02R1 || HO01R3 || HO02R2)
void USART1_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
  HAL_UART_IRQHandler(&huart1);
	
	/* If lHigherPriorityTaskWoken is now equal to pdTRUE, then a context
	switch should be performed before the interrupt exists.  That ensures the
	unblocked (higher priority) task is returned to immediately. */
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	
}
#endif
/*-----------------------------------------------------------*/

/**
* @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
*/
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0 || HO01R3 || HO02R2)
void USART2_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
  HAL_UART_IRQHandler(&huart2);
	
	/* If lHigherPriorityTaskWoken is now equal to pdTRUE, then a context
	switch should be performed before the interrupt exists.  That ensures the
	unblocked (higher priority) task is returned to immediately. */
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	
}
#endif
/*-----------------------------------------------------------*/

/**
* @brief This function handles USART3 to USART8 global interrupts / USART3 wake-up interrupt through EXTI line 28.
*/
void USART3_8_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0 || HO01R3 || HO02R2)
	HAL_UART_IRQHandler(&huart3);
#endif
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0 || HO01R3 || HO02R2)
	HAL_UART_IRQHandler(&huart4);
#endif
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || PO01R0 || PO02R0)
	HAL_UART_IRQHandler(&huart5);
#endif
#if (HO01R2 || HO02R1)
	HAL_UART_IRQHandler(&huart6);
#endif
#if (HO01R1 || HO02R0 || HO01R3 || HO02R2)
	HAL_UART_IRQHandler(&huart7);
#endif
#if (HO01R1 || HO02R0 || PO01R0 || PO02R0 || HO01R3 || HO02R2)
	HAL_UART_IRQHandler(&huart8);
#endif
	
	/* If lHigherPriorityTaskWoken is now equal to pdTRUE, then a context
	switch should be performed before the interrupt exists.  That ensures the
	unblocked (higher priority) task is returned to immediately. */
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
	
}

/*-----------------------------------------------------------*/

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	// Give back the mutex.
	xSemaphoreGiveFromISR( PxSemaphoreHandle[GetPort(huart)], &( xHigherPriorityTaskWoken ) );
	
	UartTxReady = SET;
}

/*-----------------------------------------------------------*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	// Give back the mutex.
	xSemaphoreGiveFromISR( PxSemaphoreHandle[GetPort(huart)], &( xHigherPriorityTaskWoken ) );
	
	UartRxReady = SET;
}

/*-----------------------------------------------------------*/

/**
* @brief This function handles DMA1 channel 1 interrupt (Uplink DMA 1).
*/
void DMA1_Ch1_IRQHandler(void)
{
	static uint8_t msb, lsb;
	uint8_t code = 0, led = 0;
	
	HAL_DMA_IRQHandler(&uplinkDMA1);

	if (ledStatus < 5)
	{	
		led = GetUart(inport)->Instance->RDR;
		/* Read message code */
		if (led == setInt_thres)
			code = led;
		else
			code = led % 10;
	
	#ifdef TX	
		/* Read TX LEDs */
		led = led / 10;
	
		switch (led)
		{
			case LED_ALL_OFF :						/* All TX LEDs off */
				FE_LED_off(); LED_off();
				break;
		#if (_module == 19)
			case LED_19 :
				FE_LED_on(); LED_on();
				break;
		#endif
		#if (_module == 9)
			case LED_9 :
				FE_LED_on(); LED_on();
				break;
		#endif
		#if (_module == 13)
			case LED_13 :
				FE_LED_on(); LED_on();
				break;
		#endif
		#if (_module == 24)
			case LED_24 :
				FE_LED_on(); LED_on();
				break;
		#endif
		#if (_module == 30)
			case LED_30 :
				FE_LED_on(); LED_on();
				break;
		#endif
		#if (_module == 27)
			case LED_27 :
				FE_LED_on(); LED_on();
				break;
		#endif
		#if (_module == 16)
			case LED_16 :
				FE_LED_on(); LED_on();
				break;
		#endif							
			case LED_ALL_ON :						/* All TX LEDs on */
				FE_LED_on(); LED_on();
				break;
		#if (_module == 19 || _module == 13 || _module == 9 || _module == 16)
			case LED_UPPER_GRP :				/* Upper group */
				FE_LED_on(); LED_on();
				break;
		#endif		
		#if (_module == 19 || _module == 24 || _module == 30 || _module == 27)
			case LED_LOWER_GRP :				/* Lower group */
				FE_LED_on(); LED_on();
				break;
		#endif	
		#if (_module == 19 || _module == 16 || _module == 27)
			case LED_RIGHT_GRP :				/* Right group */
				FE_LED_on(); LED_on();
				break;
		#endif	
		#if (_module == 19 || _module == 13 || _module == 24)
			case LED_LEFT_GRP :					/* Left group */
				FE_LED_on(); LED_on();
				break;
		#endif	
		#if (_module == 9 || _module == 13 || _module == 24 || _module == 30 || _module == 27 || _module == 16)
			case LED_RING_GRP :					/* Ring */
				FE_LED_on(); LED_on();
				break;
		#endif				
			default:
				FE_LED_off(); LED_off();			/* Turn off all other TX LEDs */
				break;
		}
	#endif
		
		/* Process message codes */
		if ( code == syncCodeMode0 || code == testCodeMode0 ) 
		{
			ledStatus = 0;
			
			/* Stream test */
			if ( code == testCodeMode0 )	streamTest = 1;
			
			SampleAndForward();
		} 
		else if ( code == syncCodeMode1 || code == testCodeMode1 )	
		{
			ledStatus = 1;

			/* Stream test */
			if ( code == testCodeMode1 )	streamTest = 1;
			
			SampleAndForward();		
		}
		else if ( code == syncCodeMode2 || code == testCodeMode2 )	
		{
			ledStatus = 2;
			
			/* LED status mode 2: Measure upload delay */
			LED_on();		
			
			/* Stream test */
			if ( code == testCodeMode2 )	streamTest = 1;
			
			SampleAndForward();		
		}
		else if ( code == syncCodeMode3 || code == testCodeMode3 )	
		{
			ledStatus = 3;
			
			/* Stream test */
			if ( code == testCodeMode3 )	streamTest = 1;
			
			SampleAndForward();		
		}
		else if ( code == syncCodeMode4 || code == testCodeMode4 )	
		{
			ledStatus = 4;
			
			/* Stream test */
			if ( code == testCodeMode4 )	streamTest = 1;
			
			SampleAndForward();		
		}	
		else if ( code == setInt_thres )	
		{
			ledStatus = 5;
		}
	}	
	else
	{
		if (ledStatus == 5) {
			lsb = GetUart(inport)->Instance->RDR;
			ledStatus = 6;
		} else {
			msb = GetUart(inport)->Instance->RDR;
			int_thres = msb*256 + lsb;
			msb = 0; lsb = 0; ledStatus = 4;
		}
	}	
}

/*-----------------------------------------------------------*/

/**
* @brief This function handles DMA1 channel 2 to 3 and DMA2 channel 1 to 2 interrupts.
*/
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void)
{
	/* Uplink DMA 2 */
	if (HAL_DMA_GET_IT_SOURCE(DMA1,DMA_ISR_TCIF3) == SET) 
		HAL_DMA_IRQHandler(&uplinkDMA2);
	/* Downlink DMA 3 */
	else if (HAL_DMA_GET_IT_SOURCE(DMA2,DMA_ISR_TCIF2) == SET)
		HAL_DMA_IRQHandler(&downlinkDMA3);
}

/*-----------------------------------------------------------*/

/**
* @brief This function handles DMA1 channel 4 to 7 and DMA2 channel 3 to 5 interrupts.
*/
void DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler(void)
{
	/* Downlink DMA 1 */
	if (HAL_DMA_GET_IT_SOURCE(DMA1,DMA_ISR_TCIF5) == SET)
		HAL_DMA_IRQHandler(&downlinkDMA1);
	/* Downlink DMA 2 */
	else if (HAL_DMA_GET_IT_SOURCE(DMA1,DMA_ISR_TCIF6) == SET)
		HAL_DMA_IRQHandler(&downlinkDMA2);
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
