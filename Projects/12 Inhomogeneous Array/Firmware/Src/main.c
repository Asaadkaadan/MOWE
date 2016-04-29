/**
  ******************************************************************************
  * File Name          : main.c
  * Created            : 01/05/2016 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : MOWE main source file.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	

/* Includes ------------------------------------------------------------------*/
#include "aos.h"				// AOS

/* Private variables ---------------------------------------------------------*/

uint8_t masterOutPort = 0; float streamingRate = 0; uint8_t ledStatus = 1;
uint8_t streamTest = 0, streamLoose = 0;
uint8_t syncCodeMode0 = 250, syncCodeMode1 = 240, syncCodeMode2 = 235, syncCodeMode3 = 230;
uint8_t testCodeMode0 = 130, testCodeMode1 = 125, testCodeMode2 = 120, testCodeMode3 = 115; 
uint16_t adc = 0;
extern TaskHandle_t FrontEndTaskHandle;

uint8_t dataBuffer[ArraySize*2] = {	\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	\
0xff,0xff,0xff,0xff};



/* Private function prototypes -----------------------------------------------*/


/* Main ----------------------------------------------------------------------*/
int main(void)
{
	/* --- AOS Initialization Section --- */	
	AOS_init();	

	/* --- User Initialization Section --- */		
	// Do something

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
			
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  while (1)
  {
  }
	
}

/*-----------------------------------------------------------*/

/* FrontEndTask function */
void FrontEndTask(void * argument)
{
	
#if (_module == _master)
	/* Wait indefinitly until the stream is activated by the master */
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#else
	/* Setup the slaves */
	StreamSetup(0, 0);
#endif	
			
  /* Infinite loop */
  for(;;)
  {
		
	#if (_module == _master)
		if ( (streamingRate && myTrig) || (!streamingRate) )
		{		
			if (myTrig) 	myTrig = 0;
			
			/* Master transmits a sync code */
			if(!streamTest) {
				if (ledStatus == 0) {
					writePx(P2, (const char *) &syncCodeMode0, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &syncCodeMode0, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &syncCodeMode0, 1, HAL_MAX_DELAY);		/* Route C */
				} else if (ledStatus == 1) {
					writePx(P2, (const char *) &syncCodeMode1, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &syncCodeMode1, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &syncCodeMode1, 1, HAL_MAX_DELAY);		/* Route C */					
				} else if (ledStatus == 2) {
					writePx(P2, (const char *) &syncCodeMode2, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &syncCodeMode2, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &syncCodeMode2, 1, HAL_MAX_DELAY);		/* Route C */					
				} else if (ledStatus == 3) {
					writePx(P2, (const char *) &syncCodeMode3, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &syncCodeMode3, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &syncCodeMode3, 1, HAL_MAX_DELAY);		/* Route C */	
				}
			} else {
				if (ledStatus == 0) {
					writePx(P2, (const char *) &testCodeMode0, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &testCodeMode0, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &testCodeMode0, 1, HAL_MAX_DELAY);		/* Route C */
				} else if (ledStatus == 1) {
					writePx(P2, (const char *) &testCodeMode1, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &testCodeMode1, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &testCodeMode1, 1, HAL_MAX_DELAY);		/* Route C */					
				} else if (ledStatus == 2) {
					writePx(P2, (const char *) &testCodeMode2, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &testCodeMode2, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &testCodeMode2, 1, HAL_MAX_DELAY);		/* Route C */				
				} else if (ledStatus == 3) {
					writePx(P2, (const char *) &testCodeMode3, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &testCodeMode3, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &testCodeMode3, 1, HAL_MAX_DELAY);		/* Route C */					
				}
			}
			
			/* Synchronization - Wait some time to compensate for propagation delay */
			Delay_us(3*4.2);					/* Max array depth is 3 hops */
			
			/* Take ADC sample */
			//adc = HAL_ADC_GetValue(&hadc);
			adc = SamplePD();
			
			/* LED status mode 1: Measure streaming rate */
			if (ledStatus == 1)		LED_toggle();
			
			/* Write samples into buffer */
			if (!streamTest) {
				dataBuffer[1] = (uint8_t) adc; 				/* Low byte */
				dataBuffer[0] = (uint8_t) (adc>>8);		/* High byte */
			} else {
				dataBuffer[1] = _module;
				dataBuffer[0] = _module+100;
			}
			
			/* Wait some time to download the data from previous module */
			Delay_us(150);
			
			/* LED status mode 3: Measure download delay */
			if (ledStatus == 3)		LED_on();
			/* Transmit the samples out */
			if (!streamLoose){
				writePxMutex(masterOutPort, (char*) dataBuffer, ArraySize*2, cmd50ms, HAL_MAX_DELAY);		/* 88 usec */
			} else {				
				for(char i=0 ; i < ArraySize ; i++)
				{
					writePxMutex(masterOutPort, (char*) &dataBuffer[2*i], 2, cmd50ms, HAL_MAX_DELAY);
					writePxMutex(masterOutPort, " ", 1, cmd50ms, HAL_MAX_DELAY);
				}
				writePxMutex(masterOutPort, "\n\r", 2, cmd50ms, HAL_MAX_DELAY);
			}
			/* LED status mode 3: Measure download delay */
			if (ledStatus == 3)		LED_off();
			
			/* Reset the buffer */
			memset(dataBuffer, 0xFF, ArraySize*2);
		}
	#else 
		
		osDelay(1);

	#endif	

  }	

}

/*-----------------------------------------------------------*/

/* Sample the slaves and forward data to the master */
void SampleAndForward(void)
{	
	/* LED status mode 2: Measure upload delay */
	if (ledStatus == 2)		LED_off();		
	
	/* Synchronization - Wait some time to compensate for propagation delay */
	/* slave sync delay = max depth - # of hops to the master */
#if (_module == 2)					
	Delay_us(2*4.2);				/* Wait for 2 hops */	
#elif (_module == 3)				
	Delay_us(1*4.2);				/* Wait for 1 hop */		
#elif (_module == 4)				
													/* Wait for 0 hops */		
#elif (_module == 5)				
	Delay_us(2*4.2);				/* Wait for 2 hops */		
#elif (_module == 6)				
	Delay_us(1*4.2);				/* Wait for 1 hop */		
#elif (_module == 7)				
													/* Wait for 0 hops */		
#elif (_module == 8)				
	Delay_us(2*4.2);				/* Wait for 2 hops */	
#elif (_module == 9)				
	Delay_us(1*4.2);				/* Wait for 1 hop */					
#elif (_module == 10)				
													/* Wait for 0 hops */	
#elif (_module == 11)				
	Delay_us(1*4.2);				/* Wait for 1 hop */	
#elif (_module == 12)					
													/* Wait for 0 hops */						
#endif	


	/* Take ADC sample */
	//adc = HAL_ADC_GetValue(&hadc);
	adc = SamplePD();
	
	/* LED status mode 1: Measure streaming rate */
	if (ledStatus == 1)		LED_toggle();

	
#if (_module == 2)	
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[3] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[2] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[3] = _module;
		dataBuffer[2] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(80);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char *) &dataBuffer[2], 6, cmd50ms, HAL_MAX_DELAY);		/* 39 usec */

#elif (_module == 3)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[5] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[4] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[5] = _module;
		dataBuffer[4] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[4], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
#elif (_module == 4)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[7] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[6] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[7] = _module;
		dataBuffer[6] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[6], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 5)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[9] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[8] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[9] = _module;
		dataBuffer[8] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(80);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[8], 6, cmd50ms, HAL_MAX_DELAY);		/* 39 usec */
	
#elif (_module == 6)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[11] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[10] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[11] = _module;
		dataBuffer[10] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[10], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
#elif (_module == 7)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[13] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[12] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[13] = _module;
		dataBuffer[12] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[12], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 8)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[15] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[14] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[15] = _module;
		dataBuffer[14] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(80);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[14], 10, cmd50ms, HAL_MAX_DELAY);		/* 56 usec */
	
#elif (_module == 9)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[17] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[16] = (uint8_t) (adc>>8);		/* High byte */
	} else {
		dataBuffer[17] = _module;
		dataBuffer[16] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[16], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
#elif (_module == 10)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[19] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[18] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[19] = _module;
		dataBuffer[18] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[18], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 11)	
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[21] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[20] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[21] = _module;
		dataBuffer[20] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[20], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */

#elif (_module == 12)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[23] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[22] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[23] = _module;
		dataBuffer[22] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[22], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
			
#endif

	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_off();	

	/* Reset the buffer */
	memset(dataBuffer, 0xFF, ArraySize*2);
}

/*-----------------------------------------------------------*/

/* Stream setup function */
void StreamSetup(uint8_t port, float rate)
{
	//HAL_ADC_Start(&hadc);
	
	/* Setup master output rate and port status	*/
#if (_module == _master)
	masterOutPort = port; streamingRate = rate;
	portStatus[P2] = STREAMING; portStatus[P3] = STREAMING; portStatus[P4] = STREAMING;
#endif
	
	/* Setup slaves uplink DMAs and port status */
#if (_module == 2)
	UplinkDMA1_Setup(GetUart(P4), GetUart(P6));
	portStatus[P4] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 3)
	UplinkDMA1_Setup(GetUart(P3), GetUart(P6));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 4)
	UplinkDMA1_Setup(GetUart(P3), GetUart(P6));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 5)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 6)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 7)
	UplinkDMA1_Setup(GetUart(P3), GetUart(P6));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 8)	
	UplinkDMA1_Setup(GetUart(P5), GetUart(P2));
	UplinkDMA2_Setup(GetUart(P5), GetUart(P3));
	portStatus[P2] = STREAMING; portStatus[P3] = STREAMING; portStatus[P5] = STREAMING;
#elif (_module == 9)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 10)
	UplinkDMA1_Setup(GetUart(P5), GetUart(P2));
	portStatus[P2] = STREAMING; portStatus[P5] = STREAMING;
#elif (_module == 11)	
	UplinkDMA1_Setup(GetUart(P5), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P5] = STREAMING;
#elif (_module == 12)
	UplinkDMA1_Setup(GetUart(P5), GetUart(P2));
	portStatus[P2] = STREAMING; portStatus[P5] = STREAMING;
#endif
	
	/* Setup downlink DMAs	*/
#if (_module == 1)	
	DownlinkDMA1_Setup(GetUart(P2), 10, 14);
	DownlinkDMA2_Setup(GetUart(P3), 6, 8);
	DownlinkDMA3_Setup(GetUart(P4), 6, 2);
#elif (_module == 2)
	DownlinkDMA1_Setup(GetUart(P6), 4, 4);
#elif (_module == 3)
	DownlinkDMA1_Setup(GetUart(P6), 2, 6);
#elif (_module == 4)

#elif (_module == 5)
	DownlinkDMA1_Setup(GetUart(P3), 4, 10);
#elif (_module == 6)
	DownlinkDMA1_Setup(GetUart(P3), 2, 12);
#elif (_module == 7)

#elif (_module == 8)	
	DownlinkDMA1_Setup(GetUart(P3), 4, 16);
	DownlinkDMA2_Setup(GetUart(P2), 4, 20);
#elif (_module == 9)
	DownlinkDMA1_Setup(GetUart(P3), 2, 18);
#elif (_module == 10)

#elif (_module == 11)	
	DownlinkDMA1_Setup(GetUart(P3), 2, 22);
#elif (_module == 12)

#endif

	/* Start the stream */
#if (_module == _master)	
	if (streamingRate) {
		startPerEvent( floor((48*1000000)/streamingRate) );
	}
	/* Activate the FrontEnd task */
	xTaskNotifyGive(FrontEndTaskHandle);
#endif
}

/************************ (C) COPYRIGHT OU *****END OF FILE****/
