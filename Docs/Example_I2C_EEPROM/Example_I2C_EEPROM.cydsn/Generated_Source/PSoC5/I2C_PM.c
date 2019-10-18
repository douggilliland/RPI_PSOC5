/*******************************************************************************
* File Name: I2C_PM.c
* Version 2.0
*
* Description:
*  This file provides Sleep APIs for I2C component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "I2C.h"

#if (I2C_ADDR_DECODE == I2C_HDWR_DECODE)
    #define  I2C_DEFAULT_XCFG      (I2C_XCFG_HDWR_ADDR_EN | I2C_XCFG_CLK_EN)
#else
    #define  I2C_DEFAULT_XCFG      I2C_XCFG_CLK_EN
#endif  /* End (I2C_ADDR_DECODE == I2C_HDWR_DECODE) */

#define I2C_DEFAULT_CFG     (I2C_ENABLE_SLAVE | I2C_ENABLE_MASTER | \
                                          I2C_DEFAULT_CLK_RATE)
                                                                                    
/* Define active state */
#if(I2C_IMPLEMENTATION == I2C_FF)
    #define I2C_I2C_ENABLE_REG     I2C_ACT_PWRMGR_REG
    #define I2C_IS_I2C_ENABLE(reg) ( ((reg) & I2C_ACT_PWR_EN) != 0u )
#else
    #define I2C_I2C_ENABLE_REG     I2C_CFG_REG
    #define I2C_IS_I2C_ENABLE(reg) ( (((reg) & I2C_ENABLE_MASTER) != 0u) ? 1u : \
                                                  (((reg)  & I2C_ENABLE_SLAVE) != 0u) ? 1u : 0u )
#endif  /* End (I2C_IMPLEMENTATION == I2C_FF)*/

I2C_BACKUP_STRUCT I2C_backup =
{   
    0x00u, /* enableState; */
        
    #if ((I2C_IMPLEMENTATION == I2C_FF) && (I2C_ENABLE_WAKEUP == 0u))
        I2C_DEFAULT_XCFG, /* xcfg */
        I2C_DEFAULT_CFG,  /* cfg */

        #if (I2C_MODE & I2C_MODE_SLAVE)
            (I2C_DEFAULT_ADDR & I2C_SADDR_MASK), /* addr */
        #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
        
        #if (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)
           I2C_DEFAULT_DIVIDE_FACTOR,
        #else
            LO8(I2C_DEFAULT_DIVIDE_FACTOR), /*  clk_div1 */
            HI8(I2C_DEFAULT_DIVIDE_FACTOR), /*  clk_div2 */
        #endif  /* End  (I2C_PSOC3_ES2 || I2C_PSOC5_ES1) */
        
    #else
        #if (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)
            I2C_INT_ENABLE_MASK, /* int_mask */
            
            #if (I2C_MODE & I2C_MODE_SLAVE)
                (I2C_DEFAULT_ADDR & I2C_SADDR_MASK), /* addr */
            #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
        #else
            /* ES3 Saves: 
                - Status Int mask: int_mask;
                - D0 register: addr;
                - Auxiliary Control: aux_ctl;
                - Period Register;
            */
        #endif  /* End (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)*/
        
    #endif  /* End (I2C_IMPLEMENTATION == I2C_FF)*/
};


/*******************************************************************************
* Function Name: I2C_SaveConfig
********************************************************************************
*
* Summary:
*  Save I2C configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  No
*
*******************************************************************************/
void I2C_SaveConfig(void)
{     
    #if (I2C_IMPLEMENTATION == I2C_FF)
       
        #if (I2C_ENABLE_WAKEUP)
            uint8 enableInterrupts;
    
            /* Need to disable Master */
            #if (I2C_MODE & I2C_MODE_MASTER)
                I2C_CFG_REG &= ~I2C_CFG_EN_MSTR;
            #endif  /* End (I2C_MODE & I2C_MODE_MASTER)*/
            
            enableInterrupts = CyEnterCriticalSection();
            /* Enable the I2C regulator backup */
            I2C_PWRSYS_CR1_REG |= I2C_PWRSYS_CR1_I2C_REG_BACKUP;
            CyExitCriticalSection(enableInterrupts);
            
            /* 1) Set force NACK to ignore I2C transactions 
               2) Wait while I2C will be ready to go Sleep 
               3) This bits clears on wake up */
            I2C_XCFG_REG  |= I2C_XCFG_FORCE_NACK;
            while ((I2C_XCFG_REG  & I2C_XCFG_RDY_TO_SLEEP) == 0u);
                        
        #else
            I2C_backup.xcfg = I2C_XCFG_REG ;
            I2C_backup.cfg  = I2C_CFG_REG;
            
            #if (I2C_MODE & I2C_MODE_SLAVE)
                I2C_backup.addr = I2C_ADDR_REG;
            #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
            
            #if (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)
                I2C_backup.clk_div  = I2C_CLKDIV_REG;
            
            #else
                I2C_backup.clk_div1  = I2C_CLKDIV1_REG;
                I2C_backup.clk_div2  = I2C_CLKDIV2_REG;
                    
            #endif  /* End (I2C_PSOC3_ES2 || I2C_PSOC5_ES1) */
            
        #endif  /* End ((I2C_ENABLE_WAKEUP) && (I2C_I2C_PAIR_SELECTED)) */
        
    #else        
        #if (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)

            I2C_backup.int_mask = I2C_INT_MASK_REG;
        
            #if (I2C_MODE & I2C_MODE_SLAVE)
                I2C_backup.addr = I2C_ADDR_REG;
            
            #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
            
        #else
        
            /* Saves: 
                - Status Int mask: int_mask;
                - D0 register: addr;
                - Auxiliary Control: aux_ctl;
                - Period Register;
                - D0, D1 register: mclk_gen;  
            */
        #endif  /* End (I2C_PSOC3_ES2 || I2C_PSOC5_ES1) */
        
    #endif  /* End (I2C_IMPLEMENTATION == I2C_FF) */
}


/*******************************************************************************
* Function Name: I2C_Sleep
********************************************************************************
*
* Summary:
*  Stops I2C computation and saves CRC configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  No
*
*******************************************************************************/
void I2C_Sleep(void)
{
    #if (I2C_ENABLE_WAKEUP)
        /* The I2C block should be always enabled if used as 
           wakeup source. */
    
    #else
        /* Store PRS enable state */
        if(I2C_IS_I2C_ENABLE(I2C_I2C_ENABLE_REG))
        {
            I2C_backup.enableState = 1u;
            I2C_Stop();
        }
        else
        {
            I2C_backup.enableState = 0u;
        }
    #endif  /* End  (I2C_ENABLE_WAKEUP) */
    
    I2C_SaveConfig();
}


/*******************************************************************************
* Function Name: I2C_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore I2C computation.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void I2C_RestoreConfig(void) 
{
    #if (I2C_IMPLEMENTATION == I2C_FF)
    
        #if ((I2C_ENABLE_WAKEUP) && (I2C_I2C_PAIR_SELECTED))
            uint8 enableInterrupts;
            
            enableInterrupts = CyEnterCriticalSection();
            /* Disable the I2C regulator backup */
            I2C_PWRSYS_CR1_REG &= ~I2C_PWRSYS_CR1_I2C_REG_BACKUP;
            CyExitCriticalSection(enableInterrupts);
            
            /* Need to re-enable Master */
            #if (I2C_MODE & I2C_MODE_MASTER)
                I2C_CFG_REG |= I2C_CFG_EN_MSTR;
            #endif  /* End (I2C_MODE & I2C_MODE_MASTER)*/
            
        #else
            I2C_XCFG_REG = I2C_backup.xcfg;
            I2C_CFG_REG = I2C_backup.cfg;
        
            #if (I2C_MODE & I2C_MODE_SLAVE)
                I2C_ADDR_REG = I2C_backup.addr;
            #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */ 
                
            #if (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)
                I2C_CLKDIV_REG =I2C_backup.clk_div;
            
            #else   
                I2C_CLKDIV1_REG = I2C_backup.clk_div1;
                I2C_CLKDIV2_REG = I2C_backup.clk_div2;
                    
            #endif  /* End (I2C_PSOC3_ES2 || I2C_PSOC5_ES1) */
        
        #endif  /* End (I2C_ENABLE_WAKEUP == 0u) */
        
    #else
        
        #if (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)
            I2C_INT_MASK_REG = I2C_backup.int_mask;
        
            #if (I2C_MODE & I2C_MODE_MASTER)
            
                /* Restore Master Clock generator */
                I2C_MCLK_PRD_REG = I2C_MCLK_PERIOD_VALUE;
                I2C_MCLK_CMP_REG = I2C_MCLK_COMPARE_VALUE;
           
            #endif /* End (I2C_MODE & I2C_MODE_MASTER) */

            #if (I2C_MODE & I2C_MODE_SLAVE)
                I2C_ADDR_REG = I2C_backup.addr;
                
                /* Restore Slave bit counter */
                I2C_PERIOD_REG = I2C_PERIOD_VALUE;
            
            #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
            
        #else
            /* Saves: 
                - Status Int mask: int_mask;
                - D0 register: addr;
                - Auxiliary Control: aux_ctl;
                - Period Register: always 7;
            */
        #endif  /* End (I2C_PSOC3_ES2 || I2C_PSOC5_ES1) */
        
        /* Restore Control register */
        #if (I2C_MODE & I2C_MODE_SLAVE)
            #if (I2C_ADDR_DECODE == I2C_HDWR_DECODE)
                /* Turn off any address match */
                I2C_CFG_REG &= ~(I2C_CTRL_ANY_ADDRESS_MASK);
                
            #else
                /* Turn on any address match */
                I2C_CFG_REG |= I2C_CTRL_ANY_ADDRESS_MASK;
                
            #endif  /* End (I2C_ADDR_DECODE == I2C_HDWR_DECODE) */

        #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
        
    #endif  /* End (I2C_IMPLEMENTATION == I2C_FF) */
}


/*******************************************************************************
* Function Name: I2C_Wakeup
********************************************************************************
*
* Summary:
*  Restores I2C configuration and starts CRC computation on rising edge of 
*  input clock.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Reentrant:
*  No/YES
*
*******************************************************************************/
void I2C_Wakeup(void) 
{
    I2C_RestoreConfig();
    
    /* Restore I2C Enable state */
    if (I2C_backup.enableState != 0u)
    {
        I2C_Enable();
    }
}


/* [] END OF FILE */
