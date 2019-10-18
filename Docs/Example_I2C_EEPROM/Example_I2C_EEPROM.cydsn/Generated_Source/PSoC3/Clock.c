/*******************************************************************************
* File Name: Clock.c
* Version 1.0
*
*  Description:
*    API for controlling a clock.
*
*    A clock has an input source and a divider.  These are assigned 
*    by CyDesigner.  Durring code generation, CyDesigner creates
*    the necessary defines for manipulating the registers associated
*    with the assigned clock.  This API uses those register definitions
*    to control the clock.  
*
*   Note:
*
*
********************************************************************************
* Copyright 2008-2009, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/


#include <CYDEVICE.H>
#include "Clock.H"







/* Clock Distribution registers. */
#define CLK_DIST_CR                  ((reg8 *) CYDEV_CLKDIST_CR)
#define CLK_DIST_LD                  ((reg8 *) CYDEV_CLKDIST_LD)

#define CLK_DIST_BCFG0               ((reg8 *) CYDEV_CLKDIST_BCFG0)
#define CLK_DIST_BCFG1               ((reg8 *) CYDEV_CLKDIST_BCFG1)
#define CLK_DIST_BCFG2               ((reg8 *) CYDEV_CLKDIST_BCFG2)

#define BCFG2_SHADOW_MASK            (0x80)

#define CLK_DIST_WRK0                ((reg8 *) CYDEV_CLKDIST_WRK0)
#define CLK_DIST_WRK1                ((reg8 *) CYDEV_CLKDIST_WRK1)

#define CLK_DIST_DMASK               ((reg8 *) CYDEV_CLKDIST_DMASK)
#define CLK_DIST_AMASK               ((reg8 *) CYDEV_CLKDIST_AMASK)


/*******************************************************************************
* Function Name: Clock_Start
********************************************************************************
* Summary:
*   Enables the clock by setting the enable bit in the 'Active Power Mode
*   Configuration Register' corresponding to this clock.
*
*
* Parameters:
*   void.
*
* Return:
*   void.
*   
*
*******************************************************************************/
void Clock_Start(void)
{
    /* Set the bit to enable the clock. */
    *Clock_CLKEN |= Clock_CLKEN_MASK;
}


/*******************************************************************************
* Function Name: Clock_Stop
********************************************************************************
* Summary:
*   Disables the clock by clearing the enable bit in the 'Active Power Mode
*   Configuration Register' corresponding to this clock.
*
*   if the clock is disabled, the output will be held at logic 0 when the
*   signal transitions to a logic 0.
*
* Parameters:
*   void.
*
*
* Return:
*   void.
*   
*
*******************************************************************************/
void Clock_Stop(void)
{
    /* Clear the bit to enable the clock. */
    *Clock_CLKEN &= ~Clock_CLKEN_MASK;
}


/*******************************************************************************
* Function Name: Clock_StandbyPower
********************************************************************************
* Summary:
*   Selects the power state for this clock when in standby power mode.
*
* Parameters:
*   state:
*       state of this clock during standby power mode. 1 is active, 0 inactive.
*
*   
* Return:
*   void.
*
*
*******************************************************************************/
void Clock_StandbyPower(uint8 state)
{
    if(state == 0)
    {
        *Clock_CLKSTBY &= ~Clock_CLKSTBY_MASK;
    }
    else
    {
        *Clock_CLKSTBY |= Clock_CLKSTBY_MASK;
    }
}


/*******************************************************************************
* Function Name: Clock_SetDivider
********************************************************************************
* Summary:
*   Sets the divider of the clock.
*
*
* Parameters:
* clkDivider:
*   Value to device the input clock by.
*
*
* Return:
*   void.
*   
*
* Theory:
*   Any value other than zero is acceptable.
*
*
*******************************************************************************/
void Clock_SetDivider(uint16 clkDivider)
{
    // Clear all the mask bits except ours.
#if defined(Clock__CFG3)
    *CLK_DIST_AMASK = Clock_CLKEN_MASK;
    *CLK_DIST_DMASK = 0;
#else
    *CLK_DIST_DMASK = Clock_CLKEN_MASK;
    *CLK_DIST_AMASK = 0;
#endif

    /* Set the mask bit to enable shadow loads. */
    *CLK_DIST_BCFG2 |= BCFG2_SHADOW_MASK;

    *CLK_DIST_WRK0 = LO8(clkDivider);
    *CLK_DIST_WRK1 = HI8(clkDivider);

    /* Load mask and restart together. */
    *CLK_DIST_LD = 3;
}


/*******************************************************************************
* Function Name: Clock_SetMode
********************************************************************************
* Summary:
*   Sets the operating mode of the clock.
*
*
* Parameters:
* clkMode: The following optional bits or'ed together.
*
*   CYCLK_PIPE 
*   CYCLK_SSS  
*   CYCLK_EARLY
*   CYCLK_DUTY 
*   CYCLK_SYNC 
*
* Return:
*   void.
*   
*
*******************************************************************************/
void Clock_SetMode(uint8 clkMode)
{
    uint8 value;


    /* Get the mode and source for the clock. */
    value = *Clock_MOD_SRC;

    /* Clear out the mode. */
    value &= 0x07u;

    /* Write the new mode. */
    value |= clkMode;

    /* Set the new mode of the clock. */
    *Clock_MOD_SRC = value;
}


/*******************************************************************************
* Function Name: Clock_SetSource
********************************************************************************
* Summary:
*   Sets the input source of the clock.
*
* Parameters:
* clkSource:
*   One of the following input sources.
*
*   CYCLK_SRC_SEL_SYNC_DIG
*   CYCLK_SRC_SEL_IMO  
*   CYCLK_SRC_SEL_XTALM
*   CYCLK_SRC_SEL_ILO  
*   CYCLK_SRC_SEL_PLL  
*   CYCLK_SRC_SEL_XTALK
*   CYCLK_SRC_SEL_DSI_G
*   CYCLK_SRC_SEL_DSI_D
*
* Return:
*   void.
*   
*
*******************************************************************************/
void Clock_SetSource(uint8 clkSource)
{
    uint8 value;


    /* Get the mode and source for the clock. */
    value = *Clock_MOD_SRC;

    /* Clear out the source. */
    value &= 0xF8u;

    /* Write the new source. */
    value |= clkSource;

    /* Set the new mode of the clock. */
    *Clock_MOD_SRC = value;
}


#if defined(Clock__CFG3)

/*******************************************************************************
* Function Name: Clock_SetPhase
********************************************************************************
* Summary:
*   Sets the phase delay of the analog clock.
*
*   0x01      2.5ns delay
*   0x02      3.5ns delay
*
*   ...
*
*   0x0a      11.5ns delay
*   0x0b      12.5ns delay
*
*
* Parameters:
* clkPhase: Amount to delay the phase of the clock, in 1.0 ns increments
*           up to 0x0B (12.5ns).
*
* Return:
*   void.
*   
*
*******************************************************************************/
void Clock_SetPhase(uint8 clkPhase)
{

    *Clock_PHASE = 0x0Fu & clkPhase;

}

#endif





