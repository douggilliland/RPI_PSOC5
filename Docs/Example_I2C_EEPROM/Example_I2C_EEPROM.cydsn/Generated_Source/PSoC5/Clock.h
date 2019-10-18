/******************************************************************************
* File Name: Clock.h
* Version 1.0
*
*  Description:
*   Provides the function definitions for a clock component.
*
*
********************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/


#if !defined(CY_CLOCK_Clock_H)
#define CY_CLOCK_Clock_H

#include <CYTYPES.H>
#include <CYFITTER.H>


/***************************************
*        Function Prototypes            
***************************************/

void Clock_Start(void);
void Clock_Stop(void);
void Clock_StandbyPower(uint8 state);
void Clock_SetDivider(uint16 clkDivider);
void Clock_SetMode(uint8 clkMode);
void Clock_SetSource(uint8 clkSource);

#if defined(Clock__CFG3)
void Clock_SetPhase(uint8 clkPhase);
#endif


/* Backward compatibility macros for designs created with PSoC Creator 1.0 Beta 1 */
#define Clock_Enable Clock_Start
#define Clock_Disable Clock_Stop


/***************************************
*           API Constants        
***************************************/

/* Time values to delay the phase of an analog clock. */
#define CYCLK_2_5NS     		0x01 /* 2.5 ns delay. */
#define CYCLK_3_5NS     		0x02 /* 3.5 ns delay. */
#define CYCLK_4_5NS   			0x03 /* 4.5 ns delay. */ 
#define CYCLK_5_5NS   			0x04 /* 5.5 ns delay. */ 
#define CYCLK_6_5NS   			0x05 /* 6.5 ns delay. */ 
#define CYCLK_7_5NS   			0x06 /* 7.5 ns delay. */ 
#define CYCLK_8_5NS   			0x07 /* 8.5 ns delay. */ 
#define CYCLK_9_5NS   			0x08 /* 9.5 ns delay. */ 
#define CYCLK_10_5NS   			0x09 /* 10.5 ns delay. */
#define CYCLK_11_5NS   			0x0A /* 11.5 ns delay. */
#define CYCLK_12_5NS    		0x0B /* 12.5 ns delay. */
 

/***************************************
*             Registers        
***************************************/

/* Register to enable or disable the digital clocks */
#define Clock_CLKEN              ((reg8 *) Clock__PM_ACT_CFG)

/* Clock mask for this clock. */
#define Clock_CLKEN_MASK         Clock__PM_ACT_MSK

/* Register to enable or disable the digital clocks */
#define Clock_CLKSTBY            ((reg8 *) Clock__PM_STBY_CFG)

/* Clock mask for this clock. */
#define Clock_CLKSTBY_MASK       Clock__PM_STBY_MSK

/* Clock LSB divider configuration register. */
#define Clock_DIV_LSB            ((reg8 *) Clock__CFG0)

/* Clock MSB divider configuration register. */
#define Clock_DIV_MSB            ((reg8 *) Clock__CFG1)

/* Mode and source configuration register */
#define Clock_MOD_SRC            ((reg8 *) Clock__CFG2)

#if defined(Clock__CFG3)
/* Analog clock phase configuration register */
#define Clock_PHASE              ((reg8 *) Clock__CFG3)
#endif


/* CY_CLOCK_Clock_H */
#endif

