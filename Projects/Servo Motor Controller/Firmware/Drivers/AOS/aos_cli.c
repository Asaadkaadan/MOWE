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
 * Implements the start command.
 */
static portBASE_TYPE motorCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );


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
/* CLI command structure : motor */
static const CLI_Command_Definition_t motorCommandDefinition =
{
	( const int8_t * ) "motor", /* The command string to type. */
	( const int8_t * ) "motor:\r\n Servo motor commands. \
		\r\n1st. parameter: Motor port 'p3'\t'p4'\t'P3'\t'P4'	\
		\r\n2nd. parameter: 'start'\t'stop'\t'min'\t'maxCW'\t'maxCCW'\t'speed' \
		\r\n3rd. parameter:	Rotation speed in rpm (Max 86 rpm at 6V)\
	\r\n\r\n",
	motorCommand, /* The function to run. */
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
	FreeRTOS_CLIRegisterCommand( &motorCommandDefinition );
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

static portBASE_TYPE motorCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	int8_t *pcParameterString1, *pcParameterString2, *pcParameterString3; 
	portBASE_TYPE xParameterStringLength1 = 0, xParameterStringLength2 = 0, xParameterStringLength3 = 0;
	
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

	/* Read the motor port */
	if( pcParameterString1 != NULL )
	{	
		if (pcParameterString1[0] == 'P' || pcParameterString1[0] == 'p') 
		{
			motor = ( uint8_t ) atol( ( char * ) (pcParameterString1+1) );	
		}
	}	
	
	strcpy( (char *) pcWriteBuffer, "\r\n" );
	
	/* Read the motor command */
	if( pcParameterString2 != NULL )
	{
		if (!strncmp(( char * ) pcParameterString2, "start", (size_t) xParameterStringLength2)) {
			mode = 1;	
		}	else if (!strncmp(( char * ) pcParameterString2, "stop", (size_t) xParameterStringLength2)) {
			mode = 2;	
		}	else if (!strncmp(( char * ) pcParameterString2, "min", (size_t) xParameterStringLength2)) {
			mode = 5;	
		}	else if (!strncmp(( char * ) pcParameterString2, "maxCW", (size_t) xParameterStringLength2)) {
			mode = 6;	
		}	else if (!strncmp(( char * ) pcParameterString2, "maxCCW", (size_t) xParameterStringLength2)) {
			mode = 7;	
//		}	else if (!strncmp(( char * ) pcParameterString2, "middle", (size_t) xParameterStringLength2)) {
//			mode = 4;	
//		}	else if (!strncmp(( char * ) pcParameterString2, "sweep", (size_t) xParameterStringLength2)) {
//			mode = 7;	
//		}	else if (!strncmp(( char * ) pcParameterString2, "angle", (size_t) xParameterStringLength2)) {
//			mode = 3;	
//			if( pcParameterString3 != NULL ) {
//				angle = atoi( ( char * ) (pcParameterString3) );
//			}
		}	else if (!strncmp(( char * ) pcParameterString2, "speed", (size_t) xParameterStringLength2)) {	
			mode = 8;			
			if( pcParameterString3 != NULL ) {
				rate = atof( ( char * ) (pcParameterString3) );
			}
		}
		
	}
	
	

	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/
		
/************************ (C) COPYRIGHT OU *****END OF FILE****/
