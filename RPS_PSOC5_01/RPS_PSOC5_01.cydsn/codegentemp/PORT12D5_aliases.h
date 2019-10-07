/*******************************************************************************
* File Name: PORT12D5.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_PORT12D5_ALIASES_H) /* Pins PORT12D5_ALIASES_H */
#define CY_PINS_PORT12D5_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define PORT12D5_0			(PORT12D5__0__PC)
#define PORT12D5_0_INTR	((uint16)((uint16)0x0001u << PORT12D5__0__SHIFT))

#define PORT12D5_INTR_ALL	 ((uint16)(PORT12D5_0_INTR))

#endif /* End Pins PORT12D5_ALIASES_H */


/* [] END OF FILE */
