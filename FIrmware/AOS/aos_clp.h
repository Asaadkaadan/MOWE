/**
  ******************************************************************************
  * File Name          : aos_clp.h
  * Created            : 07/22/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : AOS Command Line Parser (CLP) header file.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __aos_clp_H
#define __aos_clp_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "aos.h"
	 
/* External variables ---------------------------------------------------------*/
extern char cptr;
extern uint8_t pcPort;
extern char sertemp[100];
extern uint16_t modBuf[_N];
extern uint16_t modStack;
extern uint8_t RXcom[10];
extern UART_HandleTypeDef *PCuart;		// Pointer to PC uart
extern uint8_t comFormat;

	 
/* External macros ---------------------------------------------------------*/	 

	
/* External function prototypes -----------------------------------------------*/
extern int parseCommand(void);
extern void checkUser(void);
extern void receiveCommand(void);	 
extern void broadcast(uint16_t originID);	 
extern int isNumeric(char *str);


#ifdef __cplusplus
}
#endif
#endif /*__aos_clp_H */


/************************ (C) COPYRIGHT OU *****END OF FILE****/
