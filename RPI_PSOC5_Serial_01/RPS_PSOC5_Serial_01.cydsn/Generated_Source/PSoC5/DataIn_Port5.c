/*******************************************************************************
* File Name: DataIn_Port5.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware to read the value of a Status 
*  Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "DataIn_Port5.h"

#if !defined(DataIn_Port5_sts_sts_reg__REMOVED) /* Check for removal by optimization */


/*******************************************************************************
* Function Name: DataIn_Port5_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The current value in the Status Register.
*
*******************************************************************************/
uint8 DataIn_Port5_Read(void) 
{ 
    return DataIn_Port5_Status;
}


/*******************************************************************************
* Function Name: DataIn_Port5_InterruptEnable
********************************************************************************
*
* Summary:
*  Enables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DataIn_Port5_InterruptEnable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    DataIn_Port5_Status_Aux_Ctrl |= DataIn_Port5_STATUS_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: DataIn_Port5_InterruptDisable
********************************************************************************
*
* Summary:
*  Disables the Status Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void DataIn_Port5_InterruptDisable(void) 
{
    uint8 interruptState;
    interruptState = CyEnterCriticalSection();
    DataIn_Port5_Status_Aux_Ctrl &= (uint8)(~DataIn_Port5_STATUS_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: DataIn_Port5_WriteMask
********************************************************************************
*
* Summary:
*  Writes the current mask value assigned to the Status Register.
*
* Parameters:
*  mask:  Value to write into the mask register.
*
* Return:
*  None.
*
*******************************************************************************/
void DataIn_Port5_WriteMask(uint8 mask) 
{
    #if(DataIn_Port5_INPUTS < 8u)
    	mask &= ((uint8)(1u << DataIn_Port5_INPUTS) - 1u);
	#endif /* End DataIn_Port5_INPUTS < 8u */
    DataIn_Port5_Status_Mask = mask;
}


/*******************************************************************************
* Function Name: DataIn_Port5_ReadMask
********************************************************************************
*
* Summary:
*  Reads the current interrupt mask assigned to the Status Register.
*
* Parameters:
*  None.
*
* Return:
*  The value of the interrupt mask of the Status Register.
*
*******************************************************************************/
uint8 DataIn_Port5_ReadMask(void) 
{
    return DataIn_Port5_Status_Mask;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
