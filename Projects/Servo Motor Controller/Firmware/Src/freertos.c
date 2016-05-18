/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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


/* Place holder for calls to ioctl that don't use the value parameter. */
#define cmdPARAMTER_NOT_USED		( ( void * ) 0 )

/* Block times of 50 and 500milliseconds, specified in ticks. */
#define cmd50ms						( 50UL / portTICK_RATE_MS ) 
#define cmd500ms					( 500UL / portTICK_RATE_MS ) 

/* Used in the run time stats calculations. */
static uint32_t ulClocksPer10thOfAMilliSecond = 0UL;

/* Variables -----------------------------------------------------------------*/

/* Tasks */
TaskHandle_t defaultTaskHandle = NULL;
TaskHandle_t FrontEndTaskHandle = NULL;
//TaskHandle_t ContRotationHandle = NULL;

TaskHandle_t P1TaskHandle = NULL;
TaskHandle_t P2TaskHandle = NULL;
TaskHandle_t P3TaskHandle = NULL;
TaskHandle_t P4TaskHandle = NULL;
TaskHandle_t P5TaskHandle = NULL;
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || HO01R3 || HO02R2)
	TaskHandle_t P6TaskHandle = NULL;
#endif

extern xTaskHandle xCommandConsoleTask;

/* Semaphores */
SemaphoreHandle_t PxSemaphoreHandle[7];

/* CLI commands for tasks management ---------------------------------------------*/


/* Function prototypes -------------------------------------------------------*/
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
void StartDefaultTask(void * argument);
void PxTask(void * argument);
void FrontEndTask(void * argument);
//void ContRotationTask(void * argument);
void vRegisterCLICommands(void);

/*-----------------------------------------------------------*/

/* Init FreeRTOS */
void MX_FREERTOS_Init(void) 
{

  /* Create a defaultTask */
  xTaskCreate(StartDefaultTask, (const char *) "DefaultTask", configMINIMAL_STACK_SIZE, NULL, osPriorityNormal, &defaultTaskHandle);	
	
  /* Create tasks for module ports */
  xTaskCreate(PxTask, (const char *) "P1Task", configMINIMAL_STACK_SIZE, (void *) P1, osPriorityNormal, &P1TaskHandle);
	xTaskCreate(PxTask, (const char *) "P2Task", configMINIMAL_STACK_SIZE, (void *) P2, osPriorityNormal, &P2TaskHandle);
	xTaskCreate(PxTask, (const char *) "P3Task", configMINIMAL_STACK_SIZE, (void *) P3, osPriorityNormal, &P3TaskHandle);
	xTaskCreate(PxTask, (const char *) "P4Task", configMINIMAL_STACK_SIZE, (void *) P4, osPriorityNormal, &P4TaskHandle);
	xTaskCreate(PxTask, (const char *) "P5Task", configMINIMAL_STACK_SIZE, (void *) P5, osPriorityNormal, &P5TaskHandle);
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || HO01R3 || HO02R2)
	xTaskCreate(PxTask, (const char *) "P6Task", configMINIMAL_STACK_SIZE, (void *) P6, osPriorityNormal, &P6TaskHandle);
#endif

	/* Create the front-end task */
	xTaskCreate(FrontEndTask, (const char *) "FrontEndTask", configMINIMAL_STACK_SIZE, NULL, osPriorityAboveNormal, &FrontEndTaskHandle);

	/* Create the motor continuous rotation task */
//	xTaskCreate(ContRotationTask, (const char *) "ContRotationTask", configMINIMAL_STACK_SIZE, NULL, osPriorityAboveNormal, &ContRotationHandle);
		
	/* Create semaphores to protect module ports (FreeRTOS vSemaphoreCreateBinary didn't work) */
	osSemaphoreDef(SemaphoreP1); PxSemaphoreHandle[P1] = osSemaphoreCreate(osSemaphore(SemaphoreP1), 1);
	osSemaphoreDef(SemaphoreP2); PxSemaphoreHandle[P2] = osSemaphoreCreate(osSemaphore(SemaphoreP2), 1);	
	osSemaphoreDef(SemaphoreP3); PxSemaphoreHandle[P3] = osSemaphoreCreate(osSemaphore(SemaphoreP3), 1);
	osSemaphoreDef(SemaphoreP4); PxSemaphoreHandle[P4] = osSemaphoreCreate(osSemaphore(SemaphoreP4), 1);	
	osSemaphoreDef(SemaphoreP5); PxSemaphoreHandle[P5] = osSemaphoreCreate(osSemaphore(SemaphoreP5), 1);
#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || HO01R3 || HO02R2)
	osSemaphoreDef(SemaphoreP6); PxSemaphoreHandle[P6] = osSemaphoreCreate(osSemaphore(SemaphoreP6), 1);
#endif

	/* Register command line commands */
	vRegisterCLICommands();
	
	/* Start the tasks that implements the command console on the UART */
	vUARTCommandConsoleStart();
	

	
}

/*-----------------------------------------------------------*/

/* StartDefaultTask function */
void StartDefaultTask(void * argument)
{
  /* Infinite loop */
  for(;;)
  {
		//LED_toggle();
    osDelay(1000);
  }
	
}

/*-----------------------------------------------------------*/

/* PxTask function */
void PxTask(void * argument)
{
char cRxedChar = '\0';
uint8_t port; 
	
	port = (int8_t)(unsigned) argument;
	
  /* Infinite loop */
	for( ;; )
	{

		/* Read one character at a time. */
		if (portStatus[port] == FREE)
			readPxMutex(port, &cRxedChar, sizeof( cRxedChar ), cmd50ms, cmd50ms);
		
		
		if(cRxedChar == '\r')
		{
			cRxedChar = '\0';
			PcPort = port;
			portStatus[port] = CLI;
			
			/* Activate the CLI task */
			xTaskNotifyGive(xCommandConsoleTask);		
		}
		else if (cRxedChar != 0)
		{

					
			cRxedChar = '\0';
		}
		
		
		taskYIELD();
  }

}

/*-----------------------------------------------------------*/

void vMainConfigureTimerForRunTimeStats( void )
{
	/* How many clocks are there per tenth of a millisecond? */
	ulClocksPer10thOfAMilliSecond = configCPU_CLOCK_HZ / 10000UL;
}

/*-----------------------------------------------------------*/

uint32_t ulMainGetRunTimeCounterValue( void )
{
uint32_t ulSysTickCounts, ulTickCount, ulReturn;
const uint32_t ulSysTickReloadValue = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
volatile uint32_t * const pulCurrentSysTickCount = ( ( volatile uint32_t *) 0xe000e018 );
volatile uint32_t * const pulInterruptCTRLState = ( ( volatile uint32_t *) 0xe000ed04 );
const uint32_t ulSysTickPendingBit = 0x04000000UL;

	/* NOTE: There are potentially race conditions here.  However, it is used
	anyway to keep the examples simple, and to avoid reliance on a separate
	timer peripheral. */


	/* The SysTick is a down counter.  How many clocks have passed since it was
	last reloaded? */
	ulSysTickCounts = ulSysTickReloadValue - *pulCurrentSysTickCount;

	/* How many times has it overflowed? */
	ulTickCount = xTaskGetTickCountFromISR();

	/* Is there a SysTick interrupt pending? */
	if( ( *pulInterruptCTRLState & ulSysTickPendingBit ) != 0UL )
	{
		/* There is a SysTick interrupt pending, so the SysTick has overflowed
		but the tick count not yet incremented. */
		ulTickCount++;

		/* Read the SysTick again, as the overflow might have occurred since
		it was read last. */
		ulSysTickCounts = ulSysTickReloadValue - *pulCurrentSysTickCount;
	}

	/* Convert the tick count into tenths of a millisecond.  THIS ASSUMES
	configTICK_RATE_HZ is 1000! */
	ulReturn = ( ulTickCount * 10UL ) ;

	/* Add on the number of tenths of a millisecond that have passed since the
	tick count last got updated. */
	ulReturn += ( ulSysTickCounts / ulClocksPer10thOfAMilliSecond );

	return ulReturn;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
