/**
  ******************************************************************************
  * File Name          : aos_md_dbase.h
  * Created            : 07/24/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : AOS Module Drivers - Database routines header file.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __aos_md_dbase_H
#define __aos_md_dbase_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "aos.h"
	 
/* External variables ---------------------------------------------------------*/
extern char myName[5];
extern uint16_t myID;
extern char moduleAlias[_N+1][10];
extern uint16_t numOfRX;	 
	 
	 
/* External macros ---------------------------------------------------------*/	 

	
/* External function prototypes -----------------------------------------------*/
extern uint16_t getID(char* alias);	
extern void checkMyName(void);
extern void checkNumOfReceivers(void);
	 
	 

#ifdef __cplusplus
}
#endif
#endif /*__aos_md_dbase_H */


/************************ (C) COPYRIGHT OU *****END OF FILE****/
