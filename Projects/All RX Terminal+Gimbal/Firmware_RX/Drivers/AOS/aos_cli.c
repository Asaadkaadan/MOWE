/**
  ******************************************************************************
  * File Name          : aos_cli.c
  * Created            : 01/06/2016 
  * Version            : 1.0 
  * Author             : Asaad Kaadan
  * Description        : AOS Command Line Interface (CLI) commands.
  * License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "aos.h"

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/



/* Prototype commands functions ----------------------------------------------*/
/*
 * Implements the task-stats command.
 */
static portBASE_TYPE prvTaskStatsCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
/*
 * Implements the run-time-stats command.
 */
static portBASE_TYPE prvRunTimeStatsCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
/*
 * Implements the ping command.
 */
static portBASE_TYPE pingCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
/*
 * Implements the sample command.
 */
#ifndef TX
static portBASE_TYPE sampleCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
#endif
/*
 * Implements the update command.
 */
static portBASE_TYPE bootloaderUpdateCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
/*
 * Implements the stream command.
 */
static portBASE_TYPE streamCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
/*
 * Implements the stream test command.
 */
static portBASE_TYPE streamTestCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
/*
 * Implements the set command.
 */
static portBASE_TYPE setCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
/*
 * Implements the start command.
 */
static portBASE_TYPE startCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );


/* Create CLI commands --------------------------------------------------------*/

/* CLI command structure : run-time-stats 
This generates a table that shows how much run time each task has */
static const CLI_Command_Definition_t prvRunTimeStatsCommandDefinition =
{
	( const int8_t * ) "run-time-stats", /* The command string to type. */
	( const int8_t * ) "run-time-stats:\r\n Displays a table showing how much processing time each FreeRTOS task has used\r\n\r\n",
	prvRunTimeStatsCommand, /* The function to run. */
	0 /* No parameters are expected. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : task-stats" 
This generates a table that gives information on each task in the system. */
static const CLI_Command_Definition_t prvTaskStatsCommandDefinition =
{
	( const int8_t * ) "task-stats", /* The command string to type. */
	( const int8_t * ) "task-stats:\r\n Displays a table showing the state of each FreeRTOS task\r\n\r\n",
	prvTaskStatsCommand, /* The function to run. */
	0 /* No parameters are expected. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : ping */
static const CLI_Command_Definition_t pingCommandDefinition =
{
	( const int8_t * ) "ping", /* The command string to type. */
	( const int8_t * ) "ping:\r\n Pings a module\r\n\r\n",
	pingCommand, /* The function to run. */
	1 /* One parameter is expected. */
};
/*-----------------------------------------------------------*/
#ifndef TX
/* CLI command structure : sample */
static const CLI_Command_Definition_t sampleCommandDefinition =
{
	( const int8_t * ) "sample", /* The command string to type. */
	( const int8_t * ) "sample:\r\n Samples a receiver module\r\n\r\n",
	sampleCommand, /* The function to run. */
	1 /* One parameter is expected. */
};
#endif
/*-----------------------------------------------------------*/
/* CLI command structure : update */
static const CLI_Command_Definition_t bootloaderUpdateCommandDefinition =
{
	( const int8_t * ) "update", /* The command string to type. */
	( const int8_t * ) "update:\r\n Puts the module in bootloader mode to update its firmware\r\n\r\n",
	bootloaderUpdateCommand, /* The function to run. */
	1 /* One parameter is expected. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : stream */
static const CLI_Command_Definition_t streamCommandDefinition =
{
	( const int8_t * ) "stream", /* The command string to type. */
	( const int8_t * ) "stream:\r\n Starts the array streaming process from a given port at a given rate. Add the word 'loose'\
to separate samples by whitespace and scans by CRLF\r\n\r\n",
	streamCommand, /* The function to run. */
	-1 /* The user can enter any number of parameters. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : streamtest */
static const CLI_Command_Definition_t streamTestCommandDefinition =
{
	( const int8_t * ) "streamtest", /* The command string to type. */
	( const int8_t * ) "streamtest:\r\n Tests the streaming process from a given port at a given rate. Add the word 'loose'\
to separate samples by whitespace and scans by CRLF\r\n\r\n",
	streamTestCommand, /* The function to run. */
	-1 /* The user can enter any number of parameters. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : set */
static const CLI_Command_Definition_t setCommandDefinition =
{
	( const int8_t * ) "set", /* The command string to type. */
		( const int8_t * ) "set:\r\n Set parameters.\r\nAvailable parameters:\r\n'ledmode' (1st par.)\
\t'off'\t'rate'\t'uploaddelay'\t'downloaddelay'\t'threshold'\r\n\r\n",
	setCommand, /* The function to run. */
	-1 /* The user can enter any number of parameters. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : start */
static const CLI_Command_Definition_t startCommandDefinition =
{
	( const int8_t * ) "start", /* The command string to type. */
		( const int8_t * ) "start:\r\n Start one of the demonstrations.\r\nAvailable parameters:\r\n'demo1'\
\t'demo2'\t'demo3'\t'demo4'\r\n\r\n",
	startCommand, /* The function to run. */
	-1 /* The user can enter any number of parameters. */
};

/* Define long messages -------------------------------------------------------*/

static char * pcBootloaderUpdateMessage = 	\
"\n\rThis module will be forced into bootloader mode.\n\rPlease use the \"STM Flash Loader Demonstrator\" \
utility to update the firmware.\n\r\n\t*** Important ***\n\rIf this module is connected directly to PC please close this port first.\n\r";	


/* Register the commands ----------------------------------------------------*/

void vRegisterCLICommands(void)
{
	/* Register all CLI commands */
	FreeRTOS_CLIRegisterCommand( &prvTaskStatsCommandDefinition );
	FreeRTOS_CLIRegisterCommand( &prvRunTimeStatsCommandDefinition );	
	FreeRTOS_CLIRegisterCommand( &pingCommandDefinition );
#ifndef TX
	FreeRTOS_CLIRegisterCommand( &sampleCommandDefinition );
#endif
	FreeRTOS_CLIRegisterCommand( &bootloaderUpdateCommandDefinition );
	FreeRTOS_CLIRegisterCommand( &streamCommandDefinition );
	FreeRTOS_CLIRegisterCommand( &streamTestCommandDefinition );
	FreeRTOS_CLIRegisterCommand( &setCommandDefinition );
	FreeRTOS_CLIRegisterCommand( &startCommandDefinition );
}

/* CLI functions -----------------------------------------------------------*/

static portBASE_TYPE prvTaskStatsCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
const int8_t *const pcTaskTableHeader = ( int8_t * ) "Task          State  Priority  Stack	#\r\n************************************************\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Generate a table of task stats. */
	strcpy( ( char * ) pcWriteBuffer, ( char * ) pcTaskTableHeader );
	vTaskList( ((char*) pcWriteBuffer) + strlen( ( char * ) pcTaskTableHeader ) );

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
/*-----------------------------------------------------------*/

static portBASE_TYPE prvRunTimeStatsCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
const int8_t * const pcStatsTableHeader = ( int8_t * ) "Task            Abs Time      % Time\r\n****************************************\r\n";

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Generate a table of task stats. */
	strcpy( ( char * ) pcWriteBuffer, ( char * ) pcStatsTableHeader );
	vTaskGetRunTimeStats( ((char*) pcWriteBuffer) + strlen( ( char * ) pcStatsTableHeader ) );

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

static portBASE_TYPE pingCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Respond to the ping */
	sprintf( ( char * ) pcWriteBuffer, "Hi from module %d\r\n", myID);
	RTOS_blink(200);	
	
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/
#ifndef TX
static portBASE_TYPE sampleCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	static const int8_t *pcMessage = ( int8_t * ) "Module %d = %d\n\r";
	uint16_t val = 0;
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Respond to the ping */
	val = SamplePD();
	sprintf( ( char * ) pcWriteBuffer, ( char * ) pcMessage, myID, val);	
	
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
#endif
/*-----------------------------------------------------------*/

static portBASE_TYPE bootloaderUpdateCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	static const int8_t *pcMessage = ( int8_t * ) "Update firmware for module %d\n\r";
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Respond to the update command */
	sprintf( ( char * ) pcWriteBuffer, ( char * ) pcMessage, myID);
	strcat( ( char * ) pcWriteBuffer, ( char * ) pcBootloaderUpdateMessage );
	writePxMutex(PcPort, (char*) pcWriteBuffer, strlen((char*) pcWriteBuffer), cmd50ms, HAL_MAX_DELAY);
	
	/* Address for RAM signature (STM32F09x) - Last 4 words of SRAM */
	*((unsigned long *)0x20007FF0) = 0xDEADBEEF;   

	NVIC_SystemReset();		
	
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

static portBASE_TYPE streamCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	uint8_t port = 0; float rate = 0; 
	int8_t *pcParameterString1, *pcParameterString2, *pcParameterString3; 
	portBASE_TYPE xParameterStringLength1 = 0, xParameterStringLength2 = 0, xParameterStringLength3 = 0;
	
	static const int8_t *pcMessage = ( int8_t * ) "Streaming out port P%d at a rate of %4.2f Hz\n\r";
	static const int8_t *pcMessageMaxRate = ( int8_t * ) "Streaming out port P%d at the maximum possible rate\n\r";
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	/* Obtain the 1st parameter string. */
	pcParameterString1 = ( int8_t * ) FreeRTOS_CLIGetParameter
								(
									pcCommandString,		/* The command string itself. */
									1,						/* Return the first parameter. */
									&xParameterStringLength1	/* Store the parameter string length. */
								);

	/* Obtain the 2nd parameter string. */
	pcParameterString2 = ( int8_t * ) FreeRTOS_CLIGetParameter (pcCommandString, 2, &xParameterStringLength2);

	/* Obtain the 3rd parameter string. */
	pcParameterString3 = ( int8_t * ) FreeRTOS_CLIGetParameter (pcCommandString, 3, &xParameterStringLength3);

	/* Read the format parameter */
	if (!strcmp(( char * ) pcParameterString1, "loose") || !strcmp(( char * ) pcParameterString2, "loose")	\
																											|| !strcmp(( char * ) pcParameterString3, "loose")) {
		streamLoose = 1;
	}
	if( pcParameterString2 != NULL )
	{
		/* Read the port and rate parameters */
		if (pcParameterString1[0] == 'P' || pcParameterString1[0] == 'p') 
		{
			port = ( uint8_t ) atol( ( char * ) (pcParameterString1+1) );
			if (!strcmp(( char * ) pcParameterString2, "loose"))
				rate = ( float ) atof( ( char * ) pcParameterString3 );
			else
				rate = ( float ) atof( ( char * ) pcParameterString2 );
		}
		else if (pcParameterString2[0] == 'P' || pcParameterString2[0] == 'p')
		{
			port = ( uint8_t ) atol( ( char * ) (pcParameterString2+1) );
			if (!strcmp(( char * ) pcParameterString1, "loose"))
				rate = ( float ) atof( ( char * ) pcParameterString3 );
			else
				rate = ( float ) atof( ( char * ) pcParameterString1 );
		}
	}		
	else
	{
		/* Read the port parameter */
		if (pcParameterString1[0] == 'P' || pcParameterString1[0] == 'p') 
		{
			port = ( uint8_t ) atol( ( char * ) (pcParameterString1+1) );
			rate = 0;
		}			
	}
	
	/* Respond to the stream command */
	if (rate)
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcMessage, port, rate);
	else
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcMessageMaxRate, port);
	
	/* Setup the stream */
	StreamSetup(port, rate);

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

static portBASE_TYPE streamTestCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	uint8_t port = 0; float rate = 0; 
	int8_t *pcParameterString1, *pcParameterString2, *pcParameterString3; 
	portBASE_TYPE xParameterStringLength1 = 0, xParameterStringLength2 = 0, xParameterStringLength3 = 0;
	
	static const int8_t *pcMessage = ( int8_t * ) "Testing streams out port P%d at a rate of %4.2f Hz\n\r";
	static const int8_t *pcMessageMaxRate = ( int8_t * ) "Testing streams out port P%d at the maximum possible rate\n\r";
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	streamTest = 1;
	
	/* Obtain the 1st parameter string. */
	pcParameterString1 = ( int8_t * ) FreeRTOS_CLIGetParameter
								(
									pcCommandString,		/* The command string itself. */
									1,						/* Return the first parameter. */
									&xParameterStringLength1	/* Store the parameter string length. */
								);

	/* Obtain the 2nd parameter string. */
	pcParameterString2 = ( int8_t * ) FreeRTOS_CLIGetParameter (pcCommandString, 2, &xParameterStringLength2);

	/* Obtain the 3rd parameter string. */
	pcParameterString3 = ( int8_t * ) FreeRTOS_CLIGetParameter (pcCommandString, 3, &xParameterStringLength3);

	/* Read the format parameter */
	if (!strcmp(( char * ) pcParameterString1, "loose") || !strcmp(( char * ) pcParameterString2, "loose")	\
																											|| !strcmp(( char * ) pcParameterString3, "loose")) {
		streamLoose = 1;
	}
	if( pcParameterString2 != NULL )
	{
		/* Read the port and rate parameters */
		if (pcParameterString1[0] == 'P' || pcParameterString1[0] == 'p') 
		{
			port = ( uint8_t ) atol( ( char * ) (pcParameterString1+1) );
			if (!strcmp(( char * ) pcParameterString2, "loose"))
				rate = ( float ) atof( ( char * ) pcParameterString3 );
			else
				rate = ( float ) atof( ( char * ) pcParameterString2 );
		}
		else if (pcParameterString2[0] == 'P' || pcParameterString2[0] == 'p')
		{
			port = ( uint8_t ) atol( ( char * ) (pcParameterString2+1) );
			if (!strcmp(( char * ) pcParameterString1, "loose"))
				rate = ( float ) atof( ( char * ) pcParameterString3 );
			else
				rate = ( float ) atof( ( char * ) pcParameterString1 );
		}
	}		
	else
	{
		/* Read the port parameter */
		if (pcParameterString1[0] == 'P' || pcParameterString1[0] == 'p') 
		{
			port = ( uint8_t ) atol( ( char * ) (pcParameterString1+1) );
			rate = 0;
		}			
	}
	
	/* Respond to the stream command */
	if (rate)
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcMessage, port, rate);
	else
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcMessageMaxRate, port);
	
	/* Setup the stream */
	StreamSetup(port, rate);

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

static portBASE_TYPE setCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	int8_t *pcParameterString1, *pcParameterString2, *pcParameterString3; 
	portBASE_TYPE xParameterStringLength1, xParameterStringLength2, xParameterStringLength3;
	
	static const int8_t *pcMode0 = ( int8_t * ) "Indicator LED is turned off\n\r";
	static const int8_t *pcMode1 = ( int8_t * ) "Indicator LED is used to measure streaming rate\n\r";
	static const int8_t *pcMode2 = ( int8_t * ) "Indicator LED is used to measure upload delay\n\r";
	static const int8_t *pcMode3 = ( int8_t * ) "Indicator LED is used to measure download delay\n\r";
	static const int8_t *pcMode4 = ( int8_t * ) "Indicator LED is used to detect intensity threshold of %d\n\r";
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	/* Obtain the 1st parameter string. */
	pcParameterString1 = ( int8_t * ) FreeRTOS_CLIGetParameter
								(
									pcCommandString,		/* The command string itself. */
									1,						/* Return the first parameter. */
									&xParameterStringLength1	/* Store the parameter string length. */
								);

	/* Obtain the 2nd parameter string. */
	pcParameterString2 = ( int8_t * ) FreeRTOS_CLIGetParameter (pcCommandString, 2, &xParameterStringLength2);

	/* Obtain the 3rd parameter string. */
	pcParameterString3 = ( int8_t * ) FreeRTOS_CLIGetParameter (pcCommandString, 3, &xParameterStringLength3);
	
	/* Set parameters and respond to the set command */
	/* Indicator LED mode*/
	if (!strncmp(( char * ) pcParameterString1, "ledmode", (size_t) xParameterStringLength1) ) 
	{
			/* LED status mode 0 */
		if (!strcmp(( char * ) pcParameterString2, "off") ) {
			ledStatus = 0;
			strcpy( ( char * ) pcWriteBuffer, ( char * ) pcMode0 );
			/* LED status mode 1 */
		} else if (!strcmp(( char * ) pcParameterString2, "rate") ) {
			ledStatus = 1;
			strcpy( ( char * ) pcWriteBuffer, ( char * ) pcMode1 );
			/* LED status mode 2 */
		} else if (!strcmp(( char * ) pcParameterString2, "uploaddelay") ) {
			ledStatus = 2;
			strcpy( ( char * ) pcWriteBuffer, ( char * ) pcMode2 );
			/* LED status mode 3 */
		} else if (!strcmp(( char * ) pcParameterString2, "downloaddelay") ) {
			ledStatus = 3;
			strcpy( ( char * ) pcWriteBuffer, ( char * ) pcMode3 );
			/* LED status mode 4 */
		} else if (!strncmp(( char * ) pcParameterString2, "threshold", (size_t) xParameterStringLength2) ) {
			ledStatus = 4;
			int_thres = ( uint16_t ) atol( ( char * ) pcParameterString3 );
			sprintf( ( char * ) pcWriteBuffer, ( char * ) pcMode4, int_thres);	
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
	} 

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

static portBASE_TYPE startCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	int8_t *pcParameterString1; uint8_t temp = 0;
	portBASE_TYPE xParameterStringLength1;
	
	static const int8_t *pcDemo = ( int8_t * ) "Demo %d is executing\n\r";
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	/* Obtain the 1st parameter string. */
	pcParameterString1 = ( int8_t * ) FreeRTOS_CLIGetParameter
								(
									pcCommandString,		/* The command string itself. */
									1,						/* Return the first parameter. */
									&xParameterStringLength1	/* Store the parameter string length. */
								);

	/* Obtain the 2nd parameter string. */
	//pcParameterString2 = ( int8_t * ) FreeRTOS_CLIGetParameter (pcCommandString, 2, &xParameterStringLength2);
	
	/* Set parameters and respond to the start command */
	/* Indicator LED mode*/
	if (!strncmp(( char * ) pcParameterString1, "demo1", (size_t) xParameterStringLength1) ) 
	{
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcDemo, 1);
		
		/* 1 : Command gimbal to go straight */
		temp = 7; writePx(P5, (const char *) &temp, 1, HAL_MAX_DELAY);
		
		/* 2 : Set ledmode threshold to automatic */
		ledModeThresCounter = 0;

		/* 3 : Setup a stream */
		streamLoose = 1;
		StreamSetup(P1, 50);
	} 
	else if (!strncmp(( char * ) pcParameterString1, "demo2", (size_t) xParameterStringLength1) ) 
	{
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcDemo, 2);
		
		/* 1 : Command gimbal to go straight */
		temp = 7; writePx(P5, (const char *) &temp, 1, HAL_MAX_DELAY);

		/* 2 : Set ledmode threshold to automatic */
		ledModeThresCounter = 0;

		/* 3 : Setup a stream */
		streamLoose = 1;
		StreamSetup(P1, 50);
	}
	else if (!strncmp(( char * ) pcParameterString1, "demo3", (size_t) xParameterStringLength1) ) 
	{
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcDemo, 3);
		
		/* 1 : Command gimbal to go straight */
		temp = 7; writePx(P5, (const char *) &temp, 1, HAL_MAX_DELAY);

		/* 2 : Set ledmode threshold to automatic */
		ledModeThresCounter = 0;

		/* 3 : Setup a stream */
		streamLoose = 1;
		StreamSetup(P1, 50);
	}
	else if (!strncmp(( char * ) pcParameterString1, "demo4", (size_t) xParameterStringLength1) ) 
	{
		demoMode = 4;
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcDemo, 4);
		
		/* 2 : Set ledmode threshold to automatic */
		ledModeThresCounter = 0;

		/* 1 : Command gimbal to go to middle position */
		temp = 3; writePx(P5, (const char *) &temp, 1, HAL_MAX_DELAY);
		
		/* 3 : Setup a stream */
		streamLoose = 1;
		StreamSetup(P1, 50);
		
	}
	
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}


/************************ (C) COPYRIGHT OU *****END OF FILE****/
