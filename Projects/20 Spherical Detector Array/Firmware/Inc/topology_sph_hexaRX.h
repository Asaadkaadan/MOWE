/**
  ******************************************************************************
  * File Name          : topology_sph_hexaRX.h
  * Date               : 01-Feb-2016
  *	Author 			   		 : Asaad Kaadan
  * Description        : Spherical detector array (hexagons only).
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __topology_sph_hexaRX_H
#define __topology_sph_hexaRX_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

// Module Part Numbers:
// HO01R3: Hexagon receiver with VEMT3700 - 30mm height
// HO01R2: Hexagon receiver with VEMT3700 - 20mm height
// HO02R0: Hexagon transmiter with VSMY2850G - 30mm height
// HO02R1: Hexagon transmiter with VSMY2850G - 20mm height
// PO01R0: Pentagon receiver with VEMT3700 compatible with hexagons 30mm height
// PO02R0: Pentagon transmiter with VSMY2850G compatible with hexagons 30mm height

// Enumerations
enum	modulePartNumbers{_HO01R3, _HO01R2, _HO02R0, _HO02R1, _PO01R0, _PO02R0};
enum	portPol{_normal, _reversed};
enum	modulePorts{_P1=1, _P2, _P3, _P4, _P5, _P6};

#define _N	20					// Number of array modules

// Array modules
#define _mod1	1<<3
#define _mod2	2<<3
#define _mod3	3<<3
#define _mod4	4<<3
#define _mod5	5<<3
#define _mod6	6<<3
#define _mod7	7<<3
#define _mod8	8<<3
#define _mod9	9<<3
#define _mod10	10<<3
#define _mod11	11<<3
#define _mod12	12<<3
#define _mod13	13<<3
#define _mod14	14<<3
#define _mod15	15<<3
#define _mod16	16<<3
#define _mod17	17<<3
#define _mod18	18<<3
#define _mod19	19<<3
#define _mod20	20<<3

// Topology
static uint16_t array[_N][7] = {
{ _HO01R3, 0, _mod13|_P4, 0, _mod5|_P2, 0, _mod2|_P6},		// Module 1
{ _HO01R3, 0, _mod3|_P4, 0, _mod15|_P6, 0, _mod1|_P6},		// Module 2
{ _HO01R3, 0, _mod4|_P2, 0, _mod2|_P2, 0, _mod6|_P4},			// Module 3
{ _HO01R3, 0, _mod3|_P2, 0, _mod8|_P6, 0, _mod16|_P4},		// Module 4
{ _HO01R3, 0, _mod1|_P4, 0, _mod9|_P2, 0, _mod6|_P6},			// Module 5
{ _HO01R3, 0, _mod7|_P4, 0, _mod3|_P6, 0, _mod5|_P6},			// Module 6
{ _HO01R3, 0, _mod8|_P2, 0, _mod6|_P2, 0, _mod10|_P4},		// Module 7
{ _HO01R3, 0, _mod7|_P2, 0, _mod12|_P6, 0, _mod4|_P4},		// Module 8
{ _HO01R3, 0, _mod5|_P4, 0, _mod17|_P2, 0, _mod10|_P6},		// Module 9
{ _HO01R3, 0, _mod11|_P4, 0, _mod7|_P6, 0, _mod9|_P6},		// Module 10
{ _HO01R3, 0, _mod12|_P2, 0, _mod10|_P2, 0, _mod18|_P4},	// Module 11
{ _HO01R3, 0, _mod11|_P2, 0, _mod20|_P6, 0, _mod8|_P4},		// Module 12
{ _HO01R3, 0, _mod17|_P4, 0, _mod1|_P2, 0, _mod14|_P6},		// Module 13
{ _HO01R3, 0, _mod15|_P4, 0, _mod19|_P6, 0, _mod13|_P6},	// Module 14
{ _HO01R3, 0, _mod16|_P2, 0, _mod14|_P2, 0, _mod2|_P4},		// Module 15
{ _HO01R3, 0, _mod15|_P2, 0, _mod4|_P6, 0, _mod20|_P4},		// Module 16
{ _HO01R3, 0, _mod9|_P4, 0, _mod13|_P2, 0, _mod18|_P6},		// Module 17
{ _HO01R3, 0, _mod19|_P4, 0, _mod11|_P6, 0, _mod17|_P6},	// Module 18
{ _HO01R3, 0, _mod20|_P2, 0, _mod18|_P2, 0, _mod14|_P4},	// Module 19
{ _HO01R3, 0, _mod19|_P2, 0, _mod16|_P6, 0, _mod12|_P4}		// Module 20
};

// Configurations for duplex serial ports
#ifdef _module_1
	#define	_module	1
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_2
	#define	_module	2
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_3
	#define	_module	3
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_reversed	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_4
	#define	_module	4
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_5
	#define	_module	5
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_6
	#define	_module	6
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_7
	#define	_module	7
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_reversed	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_8
	#define	_module	8
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_9
	#define	_module	9
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_10
	#define	_module	10
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_11
	#define	_module	11
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_reversed	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_12
	#define	_module	12
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_13
	#define	_module	13
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_reversed	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_14
	#define	_module	14
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_15
	#define	_module	15
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_reversed	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_16
	#define	_module	16
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_reversed	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_17
	#define	_module	17
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_18
	#define	_module	18
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_19
	#define	_module	19
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_reversed	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_20
	#define	_module	20
	#define	HO01R3	1
	#define	_P1pol_normal	1
	#define	_P2pol_reversed	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif


#ifdef __cplusplus
}
#endif
#endif /*__topology_sph_hexaRX_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT OU *****END OF FILE****/
