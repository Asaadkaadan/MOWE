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
uint8_t streamTest = 0, streamLoose = 0, code = 0, LEDCode = 0, sensor = 0;
uint8_t syncCodeMode0 = 0, syncCodeMode1 = 1, syncCodeMode2 = 2, syncCodeMode3 = 3, syncCodeMode4 = 4;
uint8_t testCodeMode0 = 5, testCodeMode1 = 6, testCodeMode2 = 7, testCodeMode3 = 8, testCodeMode4 = 9; 
uint16_t adc = 0; uint8_t output = 0; uint8_t setInt_thres = 50, ledModeThresCounter = 40;
uint16_t int_thres = 200; uint8_t demoMode = 0;
float moduleLight = 0, backgroundLightAve = 0, backgroundLightMax = 0, backgroundLightAveTotal = 0, backgroundLightMaxTotal = 0;
extern TaskHandle_t FrontEndTaskHandle;

uint8_t dataBuffer[ArraySize] = {	\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,			\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,	\
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

/* IMU sesnor variables */
#if (_module == _master)
	extern TaskHandle_t IMUTaskHandle;

	uint8_t result = 0, aveFlag = 0;
	int16_t AccelGyro[6] = {0}; 
	float gyroX = 0, acclX = 0, gyroY = 0, acclY = 0, gyroZ = 0, acclZ = 0;
	float gyroXold = 0, acclXold = 0, gyroYold = 0, acclYold = 0, gyroZold = 0, acclZold = 0;
	float angXaccl = 0, angYaccl = 0, angZaccl = 0;
	float angXgyro = 0, angYgyro = 0, angZgyro = 0;
	float aveXG = 0, aveYG = 0, aveZG = 0, aveXA = 0, aveYA = 0, aveZA = 0;
	float angX = 0, angY = 0, angZ = 0;

	float sampleRate = 4.33;		// Hz
	float gyroSense = 16.4;   	// +- 2000 deg/sec
	float acclSense = 2048;   	// +- 16 g

	#define PI 3.14159265
#endif

/* Private function prototypes -----------------------------------------------*/
void RunDemos(void);

/* Main ----------------------------------------------------------------------*/
int main(void)
{
	/* --- AOS Initialization Section --- */	
	AOS_init();	

	/* --- User Initialization Section --- */		
	// Do something
#if (_module == _master)
	MPU6050_Initialize();
	MPU6050_SetFullScaleGyroRange(MPU6050_GYRO_FS_2000);
	MPU6050_SetFullScaleAccelRange(MPU6050_ACCEL_FS_16);
	
	result = MPU6050_TestConnection();
#endif	
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
	uint8_t temp = 0, flag = 0;
#endif	
	
	/* Example TX LED code */
	//LEDCode = LED_ALL_ON;
	
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
			
			RunDemos();
			
			/* Master transmits a sync code */
			if(!streamTest) {
				if (ledStatus == 0) {
					code = syncCodeMode0 + LEDCode*10;
				} else if (ledStatus == 1) {
					code = syncCodeMode1 + LEDCode*10;		
				} else if (ledStatus == 2) {
					code = syncCodeMode2 + LEDCode*10;
				} else if (ledStatus == 3) {
					code = syncCodeMode3 + LEDCode*10;				
				} else if (ledStatus == 4) {
					code = syncCodeMode4 + LEDCode*10;	
				}  	
			} else {
				if (ledStatus == 0) {
					code = testCodeMode0 + LEDCode*10;
				} else if (ledStatus == 1) {
					code = testCodeMode1 + LEDCode*10;			
				} else if (ledStatus == 2) {
					code = testCodeMode2 + LEDCode*10;	
				} else if (ledStatus == 3) {
					code = testCodeMode3 + LEDCode*10;
				} else if (ledStatus == 4) {
					code = testCodeMode4 + LEDCode*10;						
				}
			}		
			writePx(P2, (const char *) &code, 1, HAL_MAX_DELAY);		/* Route A */
			writePx(P3, (const char *) &code, 1, HAL_MAX_DELAY);		/* Route B */
			writePx(P4, (const char *) &code, 1, HAL_MAX_DELAY);		/* Route C */
			
			/* Synchronization - Wait some time to compensate for propagation delay */
			Delay_us(6*4.2);					/* Max array depth is 6 hops */
			
			
		#ifndef TX
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
		#else
			adc = 0;
		#endif
			
			/* LED status mode 1: Measure streaming rate */
			if (ledStatus == 1)		LED_toggle();
			
			/* Write samples into buffer */
			if (!streamTest) {
				if (adc >= int_thres)		
					dataBuffer[0] = (1<<7) | _module;
				else	
					dataBuffer[0] = (0<<7) | _module;
			} else {
				dataBuffer[0] = _module;
			}
			
			/* Wait some time to download the data from previous module */
			Delay_us(250);
						
			/* LED status mode 3: Measure download delay */
			if (ledStatus == 3)		LED_on();
			
			/* Transmit the samples out */
			for(char i=0 ; i < ArraySize ; i++)
			{
				temp = dataBuffer[i];
				/* Send samples */
				if ( (temp!=0xFF) && (temp>>7) ) {
					writePxMutex(masterOutPort, (char *) &dataBuffer[i], 1, cmd50ms, HAL_MAX_DELAY);		/* x usec */
					if (!flag)	flag = 1;
				}
			}
			/* Send end of packet */
			if (flag) {
				writePxMutex(masterOutPort, "\n\r", 2, cmd50ms, HAL_MAX_DELAY);	
				flag = 0;
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
				
				int_thres = backgroundLightMaxTotal + 200;
				
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
			}	
			
			/* Reset the buffer */
			memset(dataBuffer, 0xFF, ArraySize);			
					
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


#ifndef TX
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
#else
	adc = 0;
#endif
	
	/* LED status mode 1: Measure streaming rate */
	if (ledStatus == 1)		LED_toggle();
	
	/* Write samples into buffer */
	if (!streamTest) {
		if (adc >= int_thres)		
			output = (1<<7) | _module;
		else	
			output = (0<<7) | _module;
	} else {
		output = _module;
	}
				
#if (_module == 2)	
	/* Write samples into buffer */
	dataBuffer[1] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(160);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char *) &dataBuffer[1], 10, cmd50ms, HAL_MAX_DELAY);		/* 50 usec */

#elif (_module == 3)
	/* Write samples into buffer */
	dataBuffer[2] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(110);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[2], 9, cmd50ms, HAL_MAX_DELAY);		/* 40 usec */
	
#elif (_module == 4)
	/* Write samples into buffer */
	dataBuffer[3] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(75);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[3], 4, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 5)
	/* Write samples into buffer */
	dataBuffer[4] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(45);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[4], 3, cmd50ms, HAL_MAX_DELAY);		/* 20 usec */
	
#elif (_module == 6)
	/* Write samples into buffer */
	dataBuffer[5] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(20);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[5], 2, cmd50ms, HAL_MAX_DELAY);		/* 15 usec */
	
#elif (_module == 7)
	/* Write samples into buffer */
	dataBuffer[6] = output;
	/* Wait some time to download the data from previous module */

	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[6], 1, cmd50ms, HAL_MAX_DELAY);		/* 10 usec */
	
#elif (_module == 8)
	/* Write samples into buffer */
	dataBuffer[7] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(75);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[7], 4, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 9)
	/* Write samples into buffer */
	dataBuffer[8] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(45);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[8], 3, cmd50ms, HAL_MAX_DELAY);		/* 20 usec */
	
#elif (_module == 10)
	/* Write samples into buffer */
	dataBuffer[9] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(20);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[9], 2, cmd50ms, HAL_MAX_DELAY);		/* 15 usec */
	
#elif (_module == 11)	
	/* Write samples into buffer */
	dataBuffer[10] = output;
	/* Wait some time to download the data from previous module */

	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[10], 1, cmd50ms, HAL_MAX_DELAY);		/* 10 usec */

#elif (_module == 12)
	/* Write samples into buffer */
	dataBuffer[11] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(140);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[11], 11, cmd50ms, HAL_MAX_DELAY);		/* 45 usec */
	
#elif (_module == 13)
	/* Write samples into buffer */
	dataBuffer[12] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(100);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[12], 5, cmd50ms, HAL_MAX_DELAY);		/* 30 usec */
	
#elif (_module == 14)
	/* Write samples into buffer */
	dataBuffer[13] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(70);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[13], 4, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 15)
	/* Write samples into buffer */
	dataBuffer[14] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(45);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[14], 3, cmd50ms, HAL_MAX_DELAY);		/* 20 usec */
	
#elif (_module == 16)
	/* Write samples into buffer */
	dataBuffer[15] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(20);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[15], 2, cmd50ms, HAL_MAX_DELAY);		/* 15 usec */
	
#elif (_module == 17)
	/* Write samples into buffer */
	dataBuffer[16] = output;
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[16], 1, cmd50ms, HAL_MAX_DELAY);		/* 10 usec */
	
#elif (_module == 18)
	/* Write samples into buffer */
	dataBuffer[17] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(100);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[17], 5, cmd50ms, HAL_MAX_DELAY);		/* 30 usec */
	
#elif (_module == 19)
	/* Write samples into buffer */
	dataBuffer[18] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(70);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[18], 4, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 20)	
	/* Write samples into buffer */
	dataBuffer[19] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(45);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[19], 3, cmd50ms, HAL_MAX_DELAY);		/* 20 usec */

#elif (_module == 21)
	/* Write samples into buffer */
	dataBuffer[20] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(20);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[20], 2, cmd50ms, HAL_MAX_DELAY);		/* 15 usec */
	
#elif (_module == 22)
	/* Write samples into buffer */
	dataBuffer[21] = output;
	/* Wait some time to download the data from previous module */

	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[21], 1, cmd50ms, HAL_MAX_DELAY);		/* 10 usec */
	
#elif (_module == 23)
	/* Write samples into buffer */
	dataBuffer[22] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(150);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[22], 15, cmd50ms, HAL_MAX_DELAY);		/* 90 usec */
	
#elif (_module == 24)
	/* Write samples into buffer */
	dataBuffer[23] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(100);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[23], 9, cmd50ms, HAL_MAX_DELAY);		/* 40 usec */
	
#elif (_module == 25)
	/* Write samples into buffer */
	dataBuffer[24] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(70);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[24], 4, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 26)
	/* Write samples into buffer */
	dataBuffer[25] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(40);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[25], 3, cmd50ms, HAL_MAX_DELAY);		/* 20 usec */
	
#elif (_module == 27)
	/* Write samples into buffer */
	dataBuffer[26] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(20);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[26], 2, cmd50ms, HAL_MAX_DELAY);		/* 15 usec */
		
#elif (_module == 28)
	/* Write samples into buffer */
	dataBuffer[27] = output;
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[27], 1, cmd50ms, HAL_MAX_DELAY);		/* 10 usec */
	
#elif (_module == 29)	
	/* Write samples into buffer */
	dataBuffer[28] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(65);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[28], 4, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 30)
	/* Write samples into buffer */
	dataBuffer[29] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(45);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[29], 3, cmd50ms, HAL_MAX_DELAY);		/* 20 usec */
	
#elif (_module == 31)
	/* Write samples into buffer */
	dataBuffer[30] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(20);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[30], 2, cmd50ms, HAL_MAX_DELAY);		/* 15 usec */
	
#elif (_module == 32)
	/* Write samples into buffer */
	dataBuffer[31] = output;
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[31], 1, cmd50ms, HAL_MAX_DELAY);		/* 10 usec */
	
#elif (_module == 33)
	/* Write samples into buffer */
	dataBuffer[32] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(110);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[32], 5, cmd50ms, HAL_MAX_DELAY);		/* 30 usec */
	
#elif (_module == 34)
	/* Write samples into buffer */
	dataBuffer[33] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(75);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[33], 4, cmd50ms, HAL_MAX_DELAY);		/* 25 usec */
	
#elif (_module == 35)
	/* Write samples into buffer */
	dataBuffer[34] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(45);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[34], 3, cmd50ms, HAL_MAX_DELAY);		/* 20 usec */
	
#elif (_module == 36)
	/* Write samples into buffer */
	dataBuffer[35] = output;
	/* Wait some time to download the data from previous module */
	Delay_us(20);
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[35], 2, cmd50ms, HAL_MAX_DELAY);		/* 15 usec */
	
#elif (_module == 37)
	/* Write samples into buffer */
	dataBuffer[36] = output;
	/* Wait some time to download the data from previous module */
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_on();
	/* Transmit the samples back */
	writePxMutex(inport, (char*) &dataBuffer[36], 1, cmd50ms, HAL_MAX_DELAY);		/* 10 usec */
			
#endif
	
	/* LED status mode 3: Measure download delay */
	if (ledStatus == 3)		LED_off();	
	
	/* Reset the buffer */
	memset(dataBuffer, 0xFF, ArraySize);	

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
	DownlinkDMA1_Setup(GetUart(P2), 10, 1);
	DownlinkDMA2_Setup(GetUart(P3), 11, 11);
	DownlinkDMA3_Setup(GetUart(P4), 15, 22);
#elif (_module == 2)
	DownlinkDMA1_Setup(GetUart(P2), 9, 2);
#elif (_module == 3)
	DownlinkDMA1_Setup(GetUart(P2), 4, 3);
	DownlinkDMA2_Setup(GetUart(P3), 4, 7);
#elif (_module == 4)
	DownlinkDMA1_Setup(GetUart(P3), 3, 4);
#elif (_module == 5)
	DownlinkDMA1_Setup(GetUart(P3), 2, 5);
#elif (_module == 6)
	DownlinkDMA1_Setup(GetUart(P3), 1, 6);
#elif (_module == 7)
	
#elif (_module == 8)	
	DownlinkDMA1_Setup(GetUart(P3), 3, 8);
#elif (_module == 9)
	DownlinkDMA1_Setup(GetUart(P3), 2, 9);
#elif (_module == 10)
	DownlinkDMA1_Setup(GetUart(P3), 1, 10);
#elif (_module == 11)	

#elif (_module == 12)
	DownlinkDMA1_Setup(GetUart(P2), 5, 12);
	DownlinkDMA2_Setup(GetUart(P3), 5, 17);
#elif (_module == 13)	
	DownlinkDMA1_Setup(GetUart(P3), 4, 13);
#elif (_module == 14)
	DownlinkDMA1_Setup(GetUart(P3), 3, 14);
#elif (_module == 15)
	DownlinkDMA1_Setup(GetUart(P3), 2, 15);
#elif (_module == 16)	
	DownlinkDMA1_Setup(GetUart(P3), 1, 16);
#elif (_module == 17)	

#elif (_module == 18)	
	DownlinkDMA1_Setup(GetUart(P3), 4, 18);
#elif (_module == 19)
	DownlinkDMA1_Setup(GetUart(P3), 3, 19);
#elif (_module == 20)
	DownlinkDMA1_Setup(GetUart(P3), 2, 20);
#elif (_module == 21)	
	DownlinkDMA1_Setup(GetUart(P3), 1, 21);
#elif (_module == 22)	

#elif (_module == 23)
	DownlinkDMA1_Setup(GetUart(P3), 9, 23);
	DownlinkDMA2_Setup(GetUart(P4), 5, 32);
#elif (_module == 24)
	DownlinkDMA1_Setup(GetUart(P3), 4, 24);
	DownlinkDMA2_Setup(GetUart(P4), 4, 28);
#elif (_module == 25)	
	DownlinkDMA1_Setup(GetUart(P3), 3, 25);
#elif (_module == 26)
	DownlinkDMA1_Setup(GetUart(P3), 2, 26);
#elif (_module == 27)
	DownlinkDMA1_Setup(GetUart(P3), 1, 27);
#elif (_module == 28)	
	
#elif (_module == 29)	
	DownlinkDMA1_Setup(GetUart(P3), 3, 29);
#elif (_module == 30)
	DownlinkDMA1_Setup(GetUart(P3), 2, 30);
#elif (_module == 31)
	DownlinkDMA1_Setup(GetUart(P3), 1, 31);
#elif (_module == 32)	

#elif (_module == 33)	
	DownlinkDMA1_Setup(GetUart(P4), 4, 33);
#elif (_module == 34)	
	DownlinkDMA1_Setup(GetUart(P3), 3, 34);
#elif (_module == 35)
	DownlinkDMA1_Setup(GetUart(P3), 2, 35);
#elif (_module == 36)
	DownlinkDMA1_Setup(GetUart(P3), 1, 36);
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
#if (_module == _master)

/* IMUTask function */
void IMUTask(void * argument)
{
	uint8_t temp = 0;
	
  /* Infinite loop */
  for(;;)
  {
		
		/* Read the sensor - takes 10-15 ms */
		MPU6050_GetRawAccelGyro(AccelGyro);
		
    osDelay(50);
		
//		writePxMutex(P1, (char *) &AccelGyro[0], 12, cmd50ms, HAL_MAX_DELAY);		
//		writePxMutex(P1, "\n", 1, cmd50ms, HAL_MAX_DELAY);
		
		//LED_on();
		
		/* Processing section takes about 1.5 ms*/
		
		/* Extract sensors values */
		acclX = AccelGyro[0] / acclSense;
		acclY = AccelGyro[1] / acclSense;
		acclZ = AccelGyro[2] / acclSense;
		gyroX = AccelGyro[3] / gyroSense;
		gyroY = AccelGyro[4] / gyroSense;
		gyroZ = AccelGyro[5] / gyroSense;
				
		
		/* Filter gyro noise */
		if (abs(gyroX-gyroXold) < 3)
			//gyroX = gyroXold;
			gyroX = 0;
		else {
			gyroXold = gyroX;
		}		
		if (abs(gyroY-gyroYold) < 3)
			//gyroY = gyroYold;
			gyroY = 0;
		else {
			gyroYold = gyroY;
		}		
		if (abs(gyroZ-gyroZold) < 3)
			//gyroZ = gyroZold;
			gyroZ = 0;
		else {
			gyroZold = gyroZ;
		}				
		
		/* Angle estimation via gyro */
		angXgyro = angXgyro + gyroX/sampleRate; 
    angYgyro = angYgyro - gyroY/sampleRate; 
    angZgyro = angZgyro + gyroZ/sampleRate;
		
		/* Remove Gyro mean */
		if (aveFlag < 10) {
			aveXG = (aveXG+angXgyro)/2;   
			aveYG = (aveYG+angYgyro)/2;
			aveZG = (aveZG+angZgyro)/2;
		} else {
			angXgyro = angXgyro - aveXG;
			angYgyro = angYgyro - aveYG;
			angZgyro = angZgyro - aveZG;
		}		
		
		/* Angle estimation via accel */
		/* Small angles >> approximate atan(x) = x */
		angXaccl = atan2(acclY,acclZ)*180/PI;
		angYaccl = atan2(acclX,acclZ)*180/PI;
		angZaccl = atan2(acclX,acclY)*180/PI;
		
		/* Remove accel mean */
		if (aveFlag < 10) {
			aveXA = (aveXA+angXaccl)/2;   
			aveYA = (aveYA+angYaccl)/2;
			aveZA = (aveZA+angZaccl)/2;
			++aveFlag;
		} else {
			angXaccl = angXaccl - aveXA;
			angYaccl = angYaccl - aveYA;
			angZaccl = angZaccl - aveZA;
		}
		
		/* Zeroforce gyro drift */
		if (abs(angXgyro-angXaccl) > 50)
			angXgyro = 0;
		if (abs(angYgyro-angYaccl) > 50)
			angYgyro = 0;
		if (abs(angZgyro-angZaccl) > 50)
			angZgyro = 0;		
		
		/* Complementary filter */
		angX = 0.02 * angXgyro + 0.98 * angXaccl; 
    angY = 0.02 * angYgyro + 0.98 * angYaccl; 
    angZ = 0.02 * angZgyro + 0.98 * angZaccl; 
//		angX = angXgyro; 
//    angY = angYgyro; 
//    angZ = angZgyro; 
//		angX = 0.45 * angXgyro + 0.55 * angXaccl; 
//    angY = 0.45 * angYgyro + 0.55 * angYaccl; 
//    angZ = 0.45 * angZgyro + 0.55 * angZaccl; 
		
		/* Select TX */
		if (sensor) 
		{
			/* Translate from mid to straight position */
			angX -= 26; angY -= 42;
			
			if (abs(angX) <= 3 && abs(angY) <= 3) {
				result = 19;
				LEDCode = LED_19;
			} else if (angX > 14 && angY > 35) {
				result = 9;
				LEDCode = LED_9;
			} else if (angX < -14 && angY < -35) {
				result = 30;
				LEDCode = LED_30;
			} else if ((angX > 2 && angX < 5) && (angY > 10 && angY < 15)) {
				temp = rand()%10;
				if (temp <= 4) {
					result = 16;
					LEDCode = LED_16;
				} else {
					result = 13;
					LEDCode = LED_13;					
				}
			} else if ((angX > -10 && angX < -5) && (angY > -25 && angY < -15)) {
				temp = rand()%10;
				if (temp <= 4) {
					result = 27;
					LEDCode = LED_27;
				} else {
					result = 24;
					LEDCode = LED_24;					
				}	
			}
		}		
			
		//LED_off();
		
  }
}

/*-----------------------------------------------------------*/

/* Run TR demonstrations */
void RunDemos(void)
{
	//uint8_t temp = 0; 
	static long counter = 0; static uint8_t LEDLit = 1;
	
	switch (demoMode)
	{
		
		case 3 :	/* Demo3 */
			
			++counter;

			switch (LEDLit)
			{
				case 1 : LEDCode = LED_19;						/* 5 : Turn LED 19 on */
					break;
				case 2 : LEDCode = LED_9;							/* 6 : Turn LED 9 on */
					break;
				case 3 : LEDCode = LED_13;						/* 7 : Turn LED 13 on */
					break;
				case 4 : LEDCode = LED_24;						/* 8 : Turn LED 24 on */
					break;
				case 5 : LEDCode = LED_30;						/* 9 : Turn LED 30 on */
					break;
				case 6 : LEDCode = LED_27;						/* 10 : Turn LED 27 on */
					break;
				case 7 : LEDCode = LED_16;						/* 11 : Turn LED 16 on */
					break;
				case 8 : LEDCode = LED_UPPER_GRP;			/* 12 : Turn LED_UPPER_GRP on */
					break;
				case 9 : LEDCode = LED_LEFT_GRP;			/* 13 : Turn LED_LEFT_GRP on */
					break;
				case 10 : LEDCode = LED_LOWER_GRP;		/* 14 : Turn LED_LOWER_GRP on */
					break;
				case 11 : LEDCode = LED_RIGHT_GRP;		/* 15 : Turn LED_RIGHT_GRP on */
					break;
				case 12 : LEDCode = LED_RING_GRP;			/* 16 : Turn LED_RING_GRP on */
					break;
				case 13 :	LEDCode = LED_ALL_ON;				/* 17 : Turn LED_ALL_ON on */
					break;
				case 14 :	LEDCode = LED_ALL_OFF;			/* 18 : Turn LED_ALL_OFF on */
					break;				
				default:
					break;
			}
		
			if (counter == 250) {			/* 5 sec delay */
				counter = 0;
				++LEDLit;
				if (LEDLit > 14)	
					LEDLit = 1;
			}  		
			
			break;
		
		default:
			break;
	}	
	
}

/*-----------------------------------------------------------*/


#endif

/************************ (C) COPYRIGHT OU *****END OF FILE****/
