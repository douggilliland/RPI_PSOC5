/*******************************************************************************
* File Name: DataOut_Port5.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_DataOut_Port5_H) /* CY_CONTROL_REG_DataOut_Port5_H */
#define CY_CONTROL_REG_DataOut_Port5_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} DataOut_Port5_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    DataOut_Port5_Write(uint8 control) ;
uint8   DataOut_Port5_Read(void) ;

void DataOut_Port5_SaveConfig(void) ;
void DataOut_Port5_RestoreConfig(void) ;
void DataOut_Port5_Sleep(void) ; 
void DataOut_Port5_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define DataOut_Port5_Control        (* (reg8 *) DataOut_Port5_Sync_ctrl_reg__CONTROL_REG )
#define DataOut_Port5_Control_PTR    (  (reg8 *) DataOut_Port5_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_DataOut_Port5_H */


/* [] END OF FILE */
