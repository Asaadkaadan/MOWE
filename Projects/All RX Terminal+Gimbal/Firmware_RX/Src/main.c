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

uint8_t masterOutPort = 0; float streamingRate = 0; uint8_t ledStatus = 4;
uint8_t streamTest = 0, streamLoose = 0;
uint8_t syncCodeMode0 = 250, syncCodeMode1 = 240, syncCodeMode2 = 235, syncCodeMode3 = 230, syncCodeMode4 = 220;
uint8_t testCodeMode0 = 130, testCodeMode1 = 125, testCodeMode2 = 120, testCodeMode3 = 115, testCodeMode4 = 110; 
uint8_t setInt_thres = 50, ledModeThresCounter = 40;
uint16_t adc = 0; uint16_t int_thres = 200; uint8_t demoMode = 0; float yaw = 0, pitch = 0; 
float moduleLight = 0, backgroundLightAve = 0, backgroundLightMax = 0, backgroundLightAveTotal = 0, backgroundLightMaxTotal = 0;
extern TaskHandle_t FrontEndTaskHandle; 
extern TaskHandle_t MoveGimbalTaskHandle;

#define PI 3.14159265

uint8_t dataBuffer[ArraySize*2] = {	\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,			\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

/* Module locations (mm) */
float x_module[ArraySize] = {	\
   15.0000,	30.0000,	45.0000,	60.0000,	90.0000,	120.0000,	150.0000,	75.0000,	\
  105.0000,	135.0000,	 165.0000, 45.0000,	60.0000, 90.0000, 120.0000,	150.0000,		\
  180.0000,	75.0000,	105.0000,	135.0000,	165.0000,	195.0000,	30.0000,	60.0000,	\
   90.0000,	120.0000,	150.0000,	180.0000,	75.0000,	105.0000,	135.0000,	165.0000,	\
   45.0000,	60.0000,	90.0000,	120.0000,	150.0000};	
	
float y_module[ArraySize] = {	\
   95.2628, 121.2436, 147.2243, 173.2051, 173.2051, 173.2051, 173.2051, 147.2243,	\
  147.2243, 147.2243, 147.2243, 95.2628, 121.2436, 121.2436, 121.2436, 121.2436,	\
  121.2436, 95.2628, 95.2628, 95.2628, 95.2628, 95.2628, 69.2820, 69.2820, 				\
	69.2820, 69.2820, 69.2820, 69.2820, 43.3013, 43.3013, 43.3013, 43.3013,					\
  43.3013, 17.3205, 17.3205, 17.3205, 17.3205};


/* Private function prototypes -----------------------------------------------*/
void RunDemos(void);

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
				} else if (ledStatus == 4) {
					writePx(P2, (const char *) &syncCodeMode4, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &syncCodeMode4, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &syncCodeMode4, 1, HAL_MAX_DELAY);		/* Route C */		
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
				} else if (ledStatus == 4) {
					writePx(P2, (const char *) &testCodeMode4, 1, HAL_MAX_DELAY);		/* Route A */
					writePx(P3, (const char *) &testCodeMode4, 1, HAL_MAX_DELAY);		/* Route B */
					writePx(P4, (const char *) &testCodeMode4, 1, HAL_MAX_DELAY);		/* Route C */								
				}
			}
			
			/* Synchronization - Wait some time to compensate for propagation delay */
			Delay_us(6*4.2);					/* Max array depth is 6 hops */
			
			/* Take ADC sample */
			//adc = HAL_ADC_GetValue(&hadc);
			adc = SamplePD();
			
			/* LED status mode 4: Detect beam */
			if (ledStatus == 4) {
				if (adc >= int_thres)		
					LED_on();
				else	
					LED_off();
			}
			
			
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
			Delay_us(850);
			
			/* LED status mode 3: Measure download delay */
			if (ledStatus == 3)		LED_on();
			/* Transmit the samples out */
			if (!streamLoose){
				writePxMutex(masterOutPort, (char*) dataBuffer, ArraySize*2, cmd50ms, HAL_MAX_DELAY);		/* 272 usec */
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
			
			/* Automatic calculation of ledmode threshold */
			if (ledModeThresCounter < 30)
			{
				backgroundLightAve = 0;
				backgroundLightMax = 0;
				for(char i=0 ; i < ArraySize ; i++)
				{
					moduleLight = (dataBuffer[2*i]<<8) + dataBuffer[2*i+1];
					backgroundLightAve = backgroundLightAve + moduleLight;
					if (backgroundLightMax < moduleLight)
						backgroundLightMax = moduleLight;
				}
				backgroundLightAve = backgroundLightAve / ArraySize;			
				backgroundLightAveTotal += backgroundLightAve;
				backgroundLightMaxTotal += backgroundLightMax;
				++ledModeThresCounter;
			}
			else if (ledModeThresCounter == 30)
			{
				ledModeThresCounter = 40;
				backgroundLightAveTotal = backgroundLightAveTotal / 30;
				backgroundLightMaxTotal = backgroundLightMaxTotal / 30;
				
				int_thres = backgroundLightMaxTotal + 10;
				osDelay(10);	
				
				/* Send set threshold command */
				writePx(P2, (const char *) &setInt_thres, 1, HAL_MAX_DELAY);		/* Route A */
				writePx(P3, (const char *) &setInt_thres, 1, HAL_MAX_DELAY);		/* Route B */
				writePx(P4, (const char *) &setInt_thres, 1, HAL_MAX_DELAY);		/* Route C */	
				osDelay(1);
				/* Send the new threshold */		
				writePx(P2, (const char *) &int_thres, 1, HAL_MAX_DELAY);		/* Route A */
				writePx(P3, (const char *) &int_thres, 1, HAL_MAX_DELAY);		/* Route B */
				writePx(P4, (const char *) &int_thres, 1, HAL_MAX_DELAY);		/* Route C */
				osDelay(1);			
				writePx(P2, (const char *) &(int_thres)+1, 1, HAL_MAX_DELAY);		/* Route A */
				writePx(P3, (const char *) &(int_thres)+1, 1, HAL_MAX_DELAY);		/* Route B */
				writePx(P4, (const char *) &(int_thres)+1, 1, HAL_MAX_DELAY);		/* Route C */			
				osDelay(10);
			}	
			
			
			if (ledModeThresCounter == 40)
				RunDemos();
			
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
	Delay_us(5*4.2);				/* Wait for 5 hops */	
#elif (_module == 3)				
	Delay_us(4*4.2);				/* Wait for 4 hops */		
#elif (_module == 4)				
	Delay_us(3*4.2);				/* Wait for 3 hops */		
#elif (_module == 5)				
	Delay_us(2*4.2);				/* Wait for 2 hops */		
#elif (_module == 6)				
	Delay_us(1*4.2);				/* Wait for 1 hop */		
#elif (_module == 7)				
													/* Wait for 0 hops */		
#elif (_module == 8)				
	Delay_us(3*4.2);				/* Wait for 3 hops */	
#elif (_module == 9)				
	Delay_us(2*4.2);				/* Wait for 2 hops */					
#elif (_module == 10)				
	Delay_us(1*4.2);				/* Wait for 1 hop */	
#elif (_module == 11)				
													/* Wait for 0 hops */	
#elif (_module == 12)					
	Delay_us(5*4.2);				/* Wait for 5 hops */	
#elif (_module == 13)				
	Delay_us(4*4.2);				/* Wait for 4 hops */		
#elif (_module == 14)				
	Delay_us(3*4.2);				/* Wait for 3 hops */		
#elif (_module == 15)				
	Delay_us(2*4.2);				/* Wait for 2 hops */		
#elif (_module == 16)				
	Delay_us(1*4.2);				/* Wait for 1 hop */		
#elif (_module == 17)				
													/* Wait for 0 hops */	
#elif (_module == 18)				
	Delay_us(4*4.2);				/* Wait for 4 hops */		
#elif (_module == 19)				
	Delay_us(3*4.2);				/* Wait for 3 hops */		
#elif (_module == 20)				
	Delay_us(2*4.2);				/* Wait for 2 hops */		
#elif (_module == 21)				
	Delay_us(1*4.2);				/* Wait for 1 hop */		
#elif (_module == 22)				
													/* Wait for 0 hops */	
#elif (_module == 23)					
	Delay_us(5*4.2);				/* Wait for 5 hops */	
#elif (_module == 24)				
	Delay_us(4*4.2);				/* Wait for 4 hops */		
#elif (_module == 25)				
	Delay_us(3*4.2);				/* Wait for 3 hops */		
#elif (_module == 26)				
	Delay_us(2*4.2);				/* Wait for 2 hops */		
#elif (_module == 27)				
	Delay_us(1*4.2);				/* Wait for 1 hop */		
#elif (_module == 28)				
													/* Wait for 0 hops */															
#elif (_module == 29)				
	Delay_us(3*4.2);				/* Wait for 3 hops */		
#elif (_module == 30)				
	Delay_us(2*4.2);				/* Wait for 2 hops */		
#elif (_module == 31)				
	Delay_us(1*4.2);				/* Wait for 1 hop */		
#elif (_module == 32)				
													/* Wait for 0 hops */														
#elif (_module == 33)				
	Delay_us(4*4.2);				/* Wait for 4 hops */		
#elif (_module == 34)				
	Delay_us(3*4.2);				/* Wait for 3 hops */		
#elif (_module == 35)				
	Delay_us(2*4.2);				/* Wait for 2 hops */		
#elif (_module == 36)				
	Delay_us(1*4.2);				/* Wait for 1 hop */		
#elif (_module == 37)				
													/* Wait for 0 hops */																										
#endif	


	/* Take ADC sample */
	//adc = HAL_ADC_GetValue(&hadc);
	adc = SamplePD();
	
	/* LED status mode 4: Detect beam */
	if (ledStatus == 4) {
		if (adc >= int_thres)		
			LED_on();
		else	
			LED_off();
	}
	
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
	Delay_us(490);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char *) &dataBuffer[2], 20, cmd50ms, HAL_MAX_DELAY);		/* 85 usec */

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
	Delay_us(264);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[4], 18, cmd50ms, HAL_MAX_DELAY);		/* 78 usec */
	
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
	Delay_us(160);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[6], 8, cmd50ms, HAL_MAX_DELAY);		/* 45 usec */
	
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
	Delay_us(84);
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
	Delay_us(160);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[14], 8, cmd50ms, HAL_MAX_DELAY);		/* 45 usec */
	
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
	Delay_us(84);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[16], 6, cmd50ms, HAL_MAX_DELAY);		/* 39 usec */
	
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
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[18], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
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

	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[20], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */

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
	Delay_us(390);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[22], 22, cmd50ms, HAL_MAX_DELAY);		/* 92 usec */
	
#elif (_module == 13)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[25] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[24] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[25] = _module;
		dataBuffer[24] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(264);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[24], 10, cmd50ms, HAL_MAX_DELAY);		/* 52 usec */
	
#elif (_module == 14)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[27] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[26] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[27] = _module;
		dataBuffer[26] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(160);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[26], 8, cmd50ms, HAL_MAX_DELAY);		/* 45 usec */
	
#elif (_module == 15)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[29] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[28] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[29] = _module;
		dataBuffer[28] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(84);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[28], 6, cmd50ms, HAL_MAX_DELAY);		/* 39 usec */
	
#elif (_module == 16)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[31] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[30] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[31] = _module;
		dataBuffer[30] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[30], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
#elif (_module == 17)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[33] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[32] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[33] = _module;
		dataBuffer[32] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[32], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 18)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[35] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[34] = (uint8_t) (adc>>8);		/* High byte */
	} else {
		dataBuffer[35] = _module;
		dataBuffer[34] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(264);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[34], 10, cmd50ms, HAL_MAX_DELAY);		/* 52 usec */
	
#elif (_module == 19)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[37] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[36] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[37] = _module;
		dataBuffer[36] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(160);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[36], 8, cmd50ms, HAL_MAX_DELAY);		/* 45 usec */

#elif (_module == 20)	
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[39] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[38] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[39] = _module;
		dataBuffer[38] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(84);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[38], 6, cmd50ms, HAL_MAX_DELAY);		/* 39 usec */

#elif (_module == 21)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[41] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[40] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[41] = _module;
		dataBuffer[40] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[40], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
#elif (_module == 22)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[43] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[42] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[43] = _module;
		dataBuffer[42] = _module+100;
	}
	/* Wait some time to download the data from previous module */

	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[42], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 23)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[45] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[44] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[45] = _module;
		dataBuffer[44] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(490);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[44], 30, cmd50ms, HAL_MAX_DELAY);		/* 180 usec */
	
#elif (_module == 24)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[47] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[46] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[47] = _module;
		dataBuffer[46] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(264);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[46], 18, cmd50ms, HAL_MAX_DELAY);		/* 80 usec */
	
#elif (_module == 25)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[49] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[48] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[49] = _module;
		dataBuffer[48] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(160);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[48], 8, cmd50ms, HAL_MAX_DELAY);		/* 45 usec */
	
#elif (_module == 26)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[51] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[50] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[51] = _module;
		dataBuffer[50] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(84);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[50], 6, cmd50ms, HAL_MAX_DELAY);		/* 39 usec */
	
#elif (_module == 27)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[53] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[52] = (uint8_t) (adc>>8);		/* High byte */
	} else {
		dataBuffer[53] = _module;
		dataBuffer[52] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[52], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
#elif (_module == 28)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[55] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[54] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[55] = _module;
		dataBuffer[54] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[54], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 29)	
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[57] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[56] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[57] = _module;
		dataBuffer[56] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(160);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[56], 8, cmd50ms, HAL_MAX_DELAY);		/* 45 usec */

#elif (_module == 30)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[59] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[58] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[59] = _module;
		dataBuffer[58] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(84);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[58], 6, cmd50ms, HAL_MAX_DELAY);		/* 39 usec */
	
#elif (_module == 31)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[61] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[60] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[61] = _module;
		dataBuffer[60] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[60], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
#elif (_module == 32)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[63] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[62] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[63] = _module;
		dataBuffer[62] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[62], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 33)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[65] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[64] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[65] = _module;
		dataBuffer[64] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(264);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[64], 10, cmd50ms, HAL_MAX_DELAY);		/* 52 usec */
	
#elif (_module == 34)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[67] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[66] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[67] = _module;
		dataBuffer[66] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(160);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[66], 8, cmd50ms, HAL_MAX_DELAY);		/* 45 usec */
	
#elif (_module == 35)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[69] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[68] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[69] = _module;
		dataBuffer[68] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(84);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[68], 6, cmd50ms, HAL_MAX_DELAY);		/* 39 usec */
	
#elif (_module == 36)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[71] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[70] = (uint8_t) (adc>>8);		/* High byte */
	} else {
		dataBuffer[71] = _module;
		dataBuffer[70] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	Delay_us(30);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[70], 4, cmd50ms, HAL_MAX_DELAY);		/* 32 usec */
	
#elif (_module == 37)
	/* Write samples into buffer */
	if (!streamTest) {
		dataBuffer[73] = (uint8_t) adc; 				/* Low byte */
		dataBuffer[72] = (uint8_t) (adc>>8);		/* High byte */	
	} else {
		dataBuffer[73] = _module;
		dataBuffer[72] = _module+100;
	}
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[72], 2, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
			
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
	UplinkDMA1_Setup(GetUart(P5), GetUart(P2));
	portStatus[P2] = STREAMING; portStatus[P5] = STREAMING;
#elif (_module == 3)
	UplinkDMA1_Setup(GetUart(P5), GetUart(P2));
	UplinkDMA2_Setup(GetUart(P5), GetUart(P3));
	portStatus[P2] = STREAMING; portStatus[P3] = STREAMING; portStatus[P5] = STREAMING;
#elif (_module == 4)
	UplinkDMA1_Setup(GetUart(P5), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P5] = STREAMING;
#elif (_module == 5)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 6)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 7)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 8)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 9)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 10)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 11)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 12)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	UplinkDMA2_Setup(GetUart(P6), GetUart(P2));
	portStatus[P2] = STREAMING; portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 13)	
	UplinkDMA1_Setup(GetUart(P5), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P5] = STREAMING;
#elif (_module == 14)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 15)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 16)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 17)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 18)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 19)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 20)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 21)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 22)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 23)
	UplinkDMA1_Setup(GetUart(P1), GetUart(P3));
	UplinkDMA2_Setup(GetUart(P1), GetUart(P4));
	portStatus[P1] = STREAMING; portStatus[P3] = STREAMING; portStatus[P4] = STREAMING;
#elif (_module == 24)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	UplinkDMA2_Setup(GetUart(P6), GetUart(P4));
	portStatus[P3] = STREAMING; portStatus[P4] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 25)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 26)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 27)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 28)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 29)	
	UplinkDMA1_Setup(GetUart(P1), GetUart(P3));
	portStatus[P1] = STREAMING; portStatus[P3] = STREAMING;
#elif (_module == 30)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 31)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 32)	
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 33)	
	UplinkDMA1_Setup(GetUart(P1), GetUart(P4));
	portStatus[P1] = STREAMING; portStatus[P4] = STREAMING;
#elif (_module == 34)	
	UplinkDMA1_Setup(GetUart(P1), GetUart(P3));
	portStatus[P1] = STREAMING; portStatus[P3] = STREAMING;
#elif (_module == 35)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 36)
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#elif (_module == 37)		
	UplinkDMA1_Setup(GetUart(P6), GetUart(P3));
	portStatus[P3] = STREAMING; portStatus[P6] = STREAMING;
#endif
	
	/* Setup downlink DMAs	*/
#if (_module == 1)	
	DownlinkDMA1_Setup(GetUart(P2), 20, 2);
	DownlinkDMA2_Setup(GetUart(P3), 22, 22);
	DownlinkDMA3_Setup(GetUart(P4), 30, 44);
#elif (_module == 2)
	DownlinkDMA1_Setup(GetUart(P2), 18, 4);
#elif (_module == 3)
	DownlinkDMA1_Setup(GetUart(P2), 8, 6);
	DownlinkDMA2_Setup(GetUart(P3), 8, 14);
#elif (_module == 4)
	DownlinkDMA1_Setup(GetUart(P3), 6, 8);
#elif (_module == 5)
	DownlinkDMA1_Setup(GetUart(P3), 4, 10);
#elif (_module == 6)
	DownlinkDMA1_Setup(GetUart(P3), 2, 12);
#elif (_module == 7)
	
#elif (_module == 8)	
	DownlinkDMA1_Setup(GetUart(P3), 6, 16);
#elif (_module == 9)
	DownlinkDMA1_Setup(GetUart(P3), 4, 18);
#elif (_module == 10)
	DownlinkDMA1_Setup(GetUart(P3), 2, 20);
#elif (_module == 11)	

#elif (_module == 12)
	DownlinkDMA1_Setup(GetUart(P2), 10, 24);
	DownlinkDMA2_Setup(GetUart(P3), 10, 34);
#elif (_module == 13)	
	DownlinkDMA1_Setup(GetUart(P3), 8, 26);
#elif (_module == 14)
	DownlinkDMA1_Setup(GetUart(P3), 6, 28);
#elif (_module == 15)
	DownlinkDMA1_Setup(GetUart(P3), 4, 30);
#elif (_module == 16)	
	DownlinkDMA1_Setup(GetUart(P3), 2, 32);
#elif (_module == 17)	

#elif (_module == 18)	
	DownlinkDMA1_Setup(GetUart(P3), 8, 36);
#elif (_module == 19)
	DownlinkDMA1_Setup(GetUart(P3), 6, 38);
#elif (_module == 20)
	DownlinkDMA1_Setup(GetUart(P3), 4, 40);
#elif (_module == 21)	
	DownlinkDMA1_Setup(GetUart(P3), 2, 42);
#elif (_module == 22)	

#elif (_module == 23)
	DownlinkDMA1_Setup(GetUart(P3), 18, 46);
	DownlinkDMA2_Setup(GetUart(P4), 10, 64);
#elif (_module == 24)
	DownlinkDMA1_Setup(GetUart(P3), 8, 48);
	DownlinkDMA2_Setup(GetUart(P4), 8, 56);
#elif (_module == 25)	
	DownlinkDMA1_Setup(GetUart(P3), 6, 50);
#elif (_module == 26)
	DownlinkDMA1_Setup(GetUart(P3), 4, 52);
#elif (_module == 27)
	DownlinkDMA1_Setup(GetUart(P3), 2, 54);
#elif (_module == 28)	
	
#elif (_module == 29)	
	DownlinkDMA1_Setup(GetUart(P3), 6, 58);
#elif (_module == 30)
	DownlinkDMA1_Setup(GetUart(P3), 4, 60);
#elif (_module == 31)
	DownlinkDMA1_Setup(GetUart(P3), 2, 62);
#elif (_module == 32)	

#elif (_module == 33)	
	DownlinkDMA1_Setup(GetUart(P4), 8, 66);
#elif (_module == 34)	
	DownlinkDMA1_Setup(GetUart(P3), 6, 68);
#elif (_module == 35)
	DownlinkDMA1_Setup(GetUart(P3), 4, 70);
#elif (_module == 36)
	DownlinkDMA1_Setup(GetUart(P3), 2, 72);
#elif (_module == 37)		
	
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


/*-----------------------------------------------------------*/

/* Run RX demonstrations */
void RunDemos(void)
{
	uint16_t intval[ArraySize] = {0}; static uint8_t counter = 0;
	long intsum = 0; float x_int = 0, y_int = 0, x_err = 0, y_err = 0;
	//uint8_t temp = 0;
	const float arm = 60;		/* mm */
	
	switch (demoMode)
	{
		
		case 4 :	/* Demo4 */
			
			++counter;
		
			if (counter > 5)
			{
				counter = 0;
				
				/* Measured intensity values */
				for(char i=0 ; i < ArraySize ; i++)
				{
					intval[i] = (dataBuffer[2*i]<<8) + dataBuffer[2*i+1];
				}
				
				/* Calculate center of mass (intensity) */
				for(char i=0 ; i < ArraySize ; i++)
				{
					x_int = x_int + intval[i] * x_module[i];
					y_int = y_int + intval[i] * y_module[i];
					intsum = intsum + intval[i];
				}
				x_int = x_int / intsum;
				y_int = y_int / intsum;
				
				/* Distance error from module 19 */
				x_err = x_int - x_module[18];
				y_err = y_int - y_module[18];
				
				/* Calculate yaw and pitch angles */
				yaw = atan2(x_err,arm) * 180 / PI;
				pitch = atan2(y_err,arm) * 180 / PI;
				if (abs(yaw) < 3)	yaw = 0;
				if (abs(pitch) < 3)	pitch = 0;
				
				/* Command gimbal */
				if (yaw != 0 || pitch != 0)
				{
//					temp = 6; writePx(P5, (const char *) &temp, 1, HAL_MAX_DELAY); osDelay(10);
//					temp = 25+90; writePx(P5, (const char *) &temp, 1, HAL_MAX_DELAY); osDelay(10);
//					temp = 25+90; writePx(P5, (const char *) &temp, 1, HAL_MAX_DELAY);
					
//					GetUart(P5)->Instance->TDR = 6;	osDelay(10);;	/* Move at a specific angle */
//					GetUart(P5)->Instance->TDR = 25+90; osDelay(10);	/* yaw */
//					GetUart(P5)->Instance->TDR = 25+90; 		/* pitch */
					
					/* Activate the MoveGimbal task */
					xTaskNotifyGive(MoveGimbalTaskHandle);
					
					
//						/* 1 : Command gimbal to go straight */
//						GetUart(P5)->Instance->TDR = 7;
				}
				
			}
			
			break;
		
		default:
			break;
	}	
	
}

/* MoveGimbalTask function */
void MoveGimbalTask(void * argument)
{

	/* Infinite loop */
  for(;;)
  {
		/* Wait indefinitly until the task is needed */
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	
		GetUart(P5)->Instance->TDR = 6;	osDelay(10);;											/* Move at a specific angle */
		GetUart(P5)->Instance->TDR = 2*yaw + 90; osDelay(10);							/* yaw */
		GetUart(P5)->Instance->TDR = -2*pitch + 90; osDelay(5);						/* pitch */
			
	}
	
}

/************************ (C) COPYRIGHT OU *****END OF FILE****/
