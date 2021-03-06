/*******************************************************************************
* File Name: PORT1D0.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_PORT1D0_H) /* Pins PORT1D0_H */
#define CY_PINS_PORT1D0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PORT1D0_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PORT1D0__PORT == 15 && ((PORT1D0__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    PORT1D0_Write(uint8 value);
void    PORT1D0_SetDriveMode(uint8 mode);
uint8   PORT1D0_ReadDataReg(void);
uint8   PORT1D0_Read(void);
void    PORT1D0_SetInterruptMode(uint16 position, uint16 mode);
uint8   PORT1D0_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the PORT1D0_SetDriveMode() function.
     *  @{
     */
        #define PORT1D0_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define PORT1D0_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define PORT1D0_DM_RES_UP          PIN_DM_RES_UP
        #define PORT1D0_DM_RES_DWN         PIN_DM_RES_DWN
        #define PORT1D0_DM_OD_LO           PIN_DM_OD_LO
        #define PORT1D0_DM_OD_HI           PIN_DM_OD_HI
        #define PORT1D0_DM_STRONG          PIN_DM_STRONG
        #define PORT1D0_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define PORT1D0_MASK               PORT1D0__MASK
#define PORT1D0_SHIFT              PORT1D0__SHIFT
#define PORT1D0_WIDTH              1u

/* Interrupt constants */
#if defined(PORT1D0__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in PORT1D0_SetInterruptMode() function.
     *  @{
     */
        #define PORT1D0_INTR_NONE      (uint16)(0x0000u)
        #define PORT1D0_INTR_RISING    (uint16)(0x0001u)
        #define PORT1D0_INTR_FALLING   (uint16)(0x0002u)
        #define PORT1D0_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define PORT1D0_INTR_MASK      (0x01u) 
#endif /* (PORT1D0__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PORT1D0_PS                     (* (reg8 *) PORT1D0__PS)
/* Data Register */
#define PORT1D0_DR                     (* (reg8 *) PORT1D0__DR)
/* Port Number */
#define PORT1D0_PRT_NUM                (* (reg8 *) PORT1D0__PRT) 
/* Connect to Analog Globals */                                                  
#define PORT1D0_AG                     (* (reg8 *) PORT1D0__AG)                       
/* Analog MUX bux enable */
#define PORT1D0_AMUX                   (* (reg8 *) PORT1D0__AMUX) 
/* Bidirectional Enable */                                                        
#define PORT1D0_BIE                    (* (reg8 *) PORT1D0__BIE)
/* Bit-mask for Aliased Register Access */
#define PORT1D0_BIT_MASK               (* (reg8 *) PORT1D0__BIT_MASK)
/* Bypass Enable */
#define PORT1D0_BYP                    (* (reg8 *) PORT1D0__BYP)
/* Port wide control signals */                                                   
#define PORT1D0_CTL                    (* (reg8 *) PORT1D0__CTL)
/* Drive Modes */
#define PORT1D0_DM0                    (* (reg8 *) PORT1D0__DM0) 
#define PORT1D0_DM1                    (* (reg8 *) PORT1D0__DM1)
#define PORT1D0_DM2                    (* (reg8 *) PORT1D0__DM2) 
/* Input Buffer Disable Override */
#define PORT1D0_INP_DIS                (* (reg8 *) PORT1D0__INP_DIS)
/* LCD Common or Segment Drive */
#define PORT1D0_LCD_COM_SEG            (* (reg8 *) PORT1D0__LCD_COM_SEG)
/* Enable Segment LCD */
#define PORT1D0_LCD_EN                 (* (reg8 *) PORT1D0__LCD_EN)
/* Slew Rate Control */
#define PORT1D0_SLW                    (* (reg8 *) PORT1D0__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PORT1D0_PRTDSI__CAPS_SEL       (* (reg8 *) PORT1D0__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PORT1D0_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PORT1D0__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PORT1D0_PRTDSI__OE_SEL0        (* (reg8 *) PORT1D0__PRTDSI__OE_SEL0) 
#define PORT1D0_PRTDSI__OE_SEL1        (* (reg8 *) PORT1D0__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PORT1D0_PRTDSI__OUT_SEL0       (* (reg8 *) PORT1D0__PRTDSI__OUT_SEL0) 
#define PORT1D0_PRTDSI__OUT_SEL1       (* (reg8 *) PORT1D0__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PORT1D0_PRTDSI__SYNC_OUT       (* (reg8 *) PORT1D0__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(PORT1D0__SIO_CFG)
    #define PORT1D0_SIO_HYST_EN        (* (reg8 *) PORT1D0__SIO_HYST_EN)
    #define PORT1D0_SIO_REG_HIFREQ     (* (reg8 *) PORT1D0__SIO_REG_HIFREQ)
    #define PORT1D0_SIO_CFG            (* (reg8 *) PORT1D0__SIO_CFG)
    #define PORT1D0_SIO_DIFF           (* (reg8 *) PORT1D0__SIO_DIFF)
#endif /* (PORT1D0__SIO_CFG) */

/* Interrupt Registers */
#if defined(PORT1D0__INTSTAT)
    #define PORT1D0_INTSTAT            (* (reg8 *) PORT1D0__INTSTAT)
    #define PORT1D0_SNAP               (* (reg8 *) PORT1D0__SNAP)
    
	#define PORT1D0_0_INTTYPE_REG 		(* (reg8 *) PORT1D0__0__INTTYPE)
#endif /* (PORT1D0__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PORT1D0_H */


/* [] END OF FILE */
