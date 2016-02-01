/**
  ******************************************************************************
  * File Name          : aos_md_dbase.c
  * Created            : 07/24/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : AOS Module Drivers - Database routines.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "aos_md_dbase.h"


/* Variables -----------------------------------------------------------------*/

// Labeling
uint16_t myID = _module;
char myName[5] = "#xxx";
char moduleAlias[_N+1][10] = {0};

uint16_t numOfRX = 0;


/* Private function prototypes -----------------------------------------------*/



// --- Retrieve module ID from its alias ---
uint16_t getID(char* alias)
{
		for(int i=1 ; i<=_N ; i++)
		{
				if (!strcmp(moduleAlias[i],alias)) {
						return i;
				}
		}	
		
		return 0;
}

// --- Check my Name and ID ---
void checkMyName(void)
{
		sprintf(myName, "#%03d", myID);
}

// --- Check number of receivers in the array ---
void checkNumOfReceivers(void)
{
		for (uint16_t i=1 ; i<=_N ; i++)
		{				
				if (array[i-1][0] == _HO01R1 || array[i-1][0] == _HO01R2 || array[i-1][0] == _PO01R0)
				{
						++numOfRX;
				}
		}		
}





/************************ (C) COPYRIGHT OU *****END OF FILE****/
