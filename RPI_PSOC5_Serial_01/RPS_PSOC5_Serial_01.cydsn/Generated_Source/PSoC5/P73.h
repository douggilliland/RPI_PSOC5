/*******************************************************************************
* File Name: P73.h  
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

#if !defined(CY_PINS_P73_H) /* Pins P73_H */
#define CY_PINS_P73_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "P73_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 P73__PORT == 15 && ((P73__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    P73_Write(uint8 value);
void    P73_SetDriveMode(uint8 mode);
uint8   P73_ReadDataReg(void);
uint8   P73_Read(void);
void    P73_SetInterruptMode(uint16 position, uint16 mode);
uint8   P73_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the P73_SetDriveMode() function.
     *  @{
     */
        #define P73_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define P73_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define P73_DM_RES_UP          PIN_DM_RES_UP
        #define P73_DM_RES_DWN         PIN_DM_RES_DWN
        #define P73_DM_OD_LO           PIN_DM_OD_LO
        #define P73_DM_OD_HI           PIN_DM_OD_HI
        #define P73_DM_STRONG          PIN_DM_STRONG
        #define P73_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define P73_MASK               P73__MASK
#define P73_SHIFT              P73__SHIFT
#define P73_WIDTH              1u

/* Interrupt constants */
#if defined(P73__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in P73_SetInterruptMode() function.
     *  @{
     */
        #define P73_INTR_NONE      (uint16)(0x0000u)
        #define P73_INTR_RISING    (uint16)(0x0001u)
        #define P73_INTR_FALLING   (uint16)(0x0002u)
        #define P73_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define P73_INTR_MASK      (0x01u) 
#endif /* (P73__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define P73_PS                     (* (reg8 *) P73__PS)
/* Data Register */
#define P73_DR                     (* (reg8 *) P73__DR)
/* Port Number */
#define P73_PRT_NUM                (* (reg8 *) P73__PRT) 
/* Connect to Analog Globals */                                                  
#define P73_AG                     (* (reg8 *) P73__AG)                       
/* Analog MUX bux enable */
#define P73_AMUX                   (* (reg8 *) P73__AMUX) 
/* Bidirectional Enable */                                                        
#define P73_BIE                    (* (reg8 *) P73__BIE)
/* Bit-mask for Aliased Register Access */
#define P73_BIT_MASK               (* (reg8 *) P73__BIT_MASK)
/* Bypass Enable */
#define P73_BYP                    (* (reg8 *) P73__BYP)
/* Port wide control signals */                                                   
#define P73_CTL                    (* (reg8 *) P73__CTL)
/* Drive Modes */
#define P73_DM0                    (* (reg8 *) P73__DM0) 
#define P73_DM1                    (* (reg8 *) P73__DM1)
#define P73_DM2                    (* (reg8 *) P73__DM2) 
/* Input Buffer Disable Override */
#define P73_INP_DIS                (* (reg8 *) P73__INP_DIS)
/* LCD Common or Segment Drive */
#define P73_LCD_COM_SEG            (* (reg8 *) P73__LCD_COM_SEG)
/* Enable Segment LCD */
#define P73_LCD_EN                 (* (reg8 *) P73__LCD_EN)
/* Slew Rate Control */
#define P73_SLW                    (* (reg8 *) P73__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define P73_PRTDSI__CAPS_SEL       (* (reg8 *) P73__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define P73_PRTDSI__DBL_SYNC_IN    (* (reg8 *) P73__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define P73_PRTDSI__OE_SEL0        (* (reg8 *) P73__PRTDSI__OE_SEL0) 
#define P73_PRTDSI__OE_SEL1        (* (reg8 *) P73__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define P73_PRTDSI__OUT_SEL0       (* (reg8 *) P73__PRTDSI__OUT_SEL0) 
#define P73_PRTDSI__OUT_SEL1       (* (reg8 *) P73__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define P73_PRTDSI__SYNC_OUT       (* (reg8 *) P73__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(P73__SIO_CFG)
    #define P73_SIO_HYST_EN        (* (reg8 *) P73__SIO_HYST_EN)
    #define P73_SIO_REG_HIFREQ     (* (reg8 *) P73__SIO_REG_HIFREQ)
    #define P73_SIO_CFG            (* (reg8 *) P73__SIO_CFG)
    #define P73_SIO_DIFF           (* (reg8 *) P73__SIO_DIFF)
#endif /* (P73__SIO_CFG) */

/* Interrupt Registers */
#if defined(P73__INTSTAT)
    #define P73_INTSTAT            (* (reg8 *) P73__INTSTAT)
    #define P73_SNAP               (* (reg8 *) P73__SNAP)
    
	#define P73_0_INTTYPE_REG 		(* (reg8 *) P73__0__INTTYPE)
#endif /* (P73__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_P73_H */


/* [] END OF FILE */
