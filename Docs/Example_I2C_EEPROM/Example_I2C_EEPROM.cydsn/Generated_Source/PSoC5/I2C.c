/*******************************************************************************
* File Name: I2C.c  
* Version 2.0
*
*  Description:
*    This file contains the setup, control and status commands for the I2C
*    component.  Actual protocol and operation code resides in the interrupt
*    service routine file.
*
*   Note: 
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "I2C.h"  


/**********************************
*      System variables
**********************************/

uint8 I2C_initVar = 0u;
extern volatile uint8 I2C_State;       /* Current state of I2C state machine */
extern volatile uint8 I2C_Status;      /* Status byte */

/* Master variables */
#if (I2C_MODE & I2C_MODE_MASTER)
   extern volatile uint8 I2C_mstrStatus;       /* Master Status byte */
   extern volatile uint8 I2C_mstrControl;      /* Master Control byte */
   
   /* Transmit buffer variables */
   extern uint8 * I2C_mstrRdBufPtr;            /* Pointer to Master Read buffer */       
   extern volatile uint8 I2C_mstrRdBufSize;    /* Master Read buffer size */
   extern volatile uint8 I2C_mstrRdBufIndex;   /* Master Read buffer Index */
    
   /* Receive buffer variables */
   extern uint8 * I2C_mstrWrBufPtr;            /* Pointer to Master Write buffer */       
   extern volatile uint8 I2C_mstrWrBufSize;    /* Master Write buffer size */
   extern volatile uint8 I2C_mstrWrBufIndex;   /* Master Write buffer Index */
   
#endif  /* End (I2C_MODE & I2C_MODE_MASTER) */

/* Slave variables */
#if (I2C_MODE & I2C_MODE_SLAVE)
   extern volatile uint8 I2C_slStatus;         /* Slave Status  */   
   
   #if (I2C_ADDR_DECODE == I2C_SW_DECODE)
      extern volatile uint8 I2C_Address;       /* Software address variable */
   #endif   /* End (I2C_ADDR_DECODE == I2C_SW_DECODE) */    
   
   /* Transmit buffer variables */
   extern uint8 * I2C_readBufPtr;              /* Pointer to Transmit buffer */       
   extern volatile uint8 I2C_readBufSize;      /* Slave Transmit buffer size */
   extern volatile uint8 I2C_readBufIndex;     /* Slave Transmit buffer Index */

   /* Receive buffer variables */
   extern uint8 * I2C_writeBufPtr;             /* Pointer to Receive buffer */       
   extern volatile uint8 I2C_writeBufSize;     /* Slave Receive buffer size */
   extern volatile uint8 I2C_writeBufIndex;    /* Slave Receive buffer Index */

#endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */


/*******************************************************************************
* Function Name: I2C_Init
********************************************************************************
*
* Summary:
*  Initializes I2C component with initial values.
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
void I2C_Init(void)
{
    #if (I2C_IMPLEMENTATION == I2C_FF)           
        
        /* Enable Master or Slave */
        I2C_CFG_REG = (I2C_ENABLE_SLAVE | I2C_ENABLE_MASTER);
                                
        /* 50 kHz - 32 samples/bit */
        I2C_CFG_REG |= I2C_DEFAULT_CLK_RATE;
         
        /* Set devide factor */
        #if (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)
            I2C_CLKDIV_REG = I2C_DEFAULT_DIVIDE_FACTOR;
        #else
            I2C_CLKDIV1_REG = LO8(I2C_DEFAULT_DIVIDE_FACTOR);
            I2C_CLKDIV2_REG = HI8(I2C_DEFAULT_DIVIDE_FACTOR);
        #endif /* End (I2C_PSOC3_ES2 || I2C_PSOC5_ES1) */
        
        /* if I2C block will be used as wake up source */
        #if (I2C_ENABLE_WAKEUP)
            /* I2C block as wake-up source */
            I2C_XCFG_REG  = I2C_XCFG_I2C_ON;
        
            /* Process sio_select and pselect */                
            #if(I2C_I2C_PAIR_SELECTED == I2C_I2C_PAIR0)
                /* Set I2C0 SIO pair P12[0,1] */
                I2C_CFG_REG |= I2C_CFG_SIO_SELECT;
            #else
                /* Do nothing for I2C1 SIO pair P12[4,5] */
            #endif /* End (I2C_ENABLE_WAKEUP) */
            
            I2C_CFG_REG |= I2C_CFG_PSELECT;
        
        #endif  /* End ((I2C_ENABLE_WAKEUP)*/
        
        /* Clear Status register */
        I2C_CSR_REG = 0x00u;
        
    #else
        /* Enable Byte Complete for interrupt in the mask register */
        I2C_INT_MASK_REG |= I2C_BYTE_COMPLETE_IE_MASK;

        /* Clear the status register before starting */
        I2C_initVar = I2C_CSR_REG;
        
    #endif  /* End (I2C_IMPLEMENTATION == I2C_FF) */

    /* Set address detection type */
    #if (I2C_IMPLEMENTATION == I2C_FF)
        #if (I2C_ADDR_DECODE == I2C_HDWR_DECODE)
            /* Turn on hardware address detection and enable the clock */
            I2C_XCFG_REG  |= (I2C_XCFG_HDWR_ADDR_EN | I2C_XCFG_CLK_EN);
            
        #else
            /* Enable the clock */
            I2C_XCFG_REG  |= I2C_XCFG_CLK_EN;
                            
        #endif  /* End (I2C_ADDR_DECODE == I2C_HDWR_DECODE) */
        
    #else
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
              
    /* Disable Interrupt */
    CyIntDisable(I2C_ISR_NUMBER);
    
    /* Set the ISR to point to the RTC_SUT_isr Interrupt */
    CyIntSetVector(I2C_ISR_NUMBER, I2C_ISR);
    
    /* Set the priority */
    CyIntSetPriority(I2C_ISR_NUMBER, I2C_ISR_PRIORITY);
    
    /* Clear all status flags */
    I2C_Status = 0x00u; 

    /* Put state machine in idle state */
    I2C_State = I2C_SM_IDLE; 

    #if (I2C_MODE & I2C_MODE_SLAVE)
        /* Set default status */
        I2C_SlaveClearReadBuf();
        I2C_SlaveClearWriteBuf();
        I2C_SlaveClearReadStatus();
        I2C_SlaveClearWriteStatus();

        /* Set default address */
        I2C_SlaveSetAddress(I2C_DEFAULT_ADDR);
    
    #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */

    #if (I2C_MODE & I2C_MODE_MASTER)
    
        /* Set default status */
        I2C_MasterClearReadBuf();
        I2C_MasterClearWriteBuf();
        I2C_MasterClearStatus();
        
    #endif  /* End (I2C_MODE & I2C_MODE_MASTER) */
}


/*******************************************************************************
* Function Name: I2C_Enable
********************************************************************************
*
* Summary:
*  Starts I2C component operation.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void I2C_Enable(void) 
{          
    #if ( (I2C_IMPLEMENTATION != I2C_UDB) || \
          (I2C_MODE & I2C_MODE_SLAVE) )
        uint8 enableInterrupts;
    #endif  /* End ( (I2C_IMPLEMENTATION != I2C_FF) || \
                   (I2C_MODE & I2C_MODE_SLAVE) ) */
                   
    #if(I2C_IMPLEMENTATION == I2C_FF)
        enableInterrupts = CyEnterCriticalSection();
        /* Enable power to I2C Module */
        I2C_ACT_PWRMGR_REG  |= I2C_ACT_PWR_EN;
        I2C_STBY_PWRMGR_REG |= I2C_STBY_PWR_EN;
        CyExitCriticalSection(enableInterrupts);
        
    #else
        /* Enable the I2C */
        I2C_CFG_REG = (I2C_ENABLE_MASTER | I2C_ENABLE_SLAVE);
        
        /* Enable bit counter */
        #if (I2C_MODE & I2C_MODE_SLAVE)
            enableInterrupts = CyEnterCriticalSection();
            I2C_COUNTER_AUX_CTL_REG |= I2C_COUNTER_ENABLE_MASK;
            CyExitCriticalSection(enableInterrupts);
        #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
        
    #endif  /* End (I2C_IMPLEMENTATION == I2C_FF) */   
}


/*******************************************************************************
* Function Name: I2C_Start
********************************************************************************
*
* Summary:
*  Starts the component and enables the interupt.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects:
*   This component automatically enables it's interrupt.  If I2C is enabled
*   without the interrupt enabled, it could lock up the I2C bus.
*
* Reentrant:
*  No
*
*******************************************************************************/
void I2C_Start(void)
{
    /* Initialize I2C registers, reset I2C buffer index and clears status */
    if (I2C_initVar == 0u)
    {
        I2C_Init();
        I2C_initVar = 1u;
    }
      
    I2C_Enable();
}


/*******************************************************************************
* Function Name: I2C_Stop
********************************************************************************
*
* Summary:
*  Disable the component and disable the interrupt.
*
* Parameters:
*  void 
*
* Return: 
*  void 
*
*******************************************************************************/
void I2C_Stop(void)  
{   
    #if ( (I2C_IMPLEMENTATION != I2C_UDB) || \
          (I2C_MODE & I2C_MODE_SLAVE) )
        uint8 enableInterrupts;
    #endif  /* End ( (I2C_IMPLEMENTATION != I2C_FF) || \
                   (I2C_MODE & I2C_MODE_SLAVE) ) */
    
    /* Disable Interrupt */
    I2C_DisableInt();
    
    #if (I2C_IMPLEMENTATION == I2C_FF)
        enableInterrupts = CyEnterCriticalSection();
        /* Disable power to I2C block */
        I2C_ACT_PWRMGR_REG  &= ~I2C_ACT_PWR_EN;
        I2C_STBY_PWRMGR_REG &= ~I2C_STBY_PWR_EN;
        CyExitCriticalSection(enableInterrupts);
        
    #else
        /* Reset the I2C before starting */
        I2C_CFG_REG &= ~(I2C_ENABLE_MASTER | I2C_ENABLE_SLAVE);
        
        /* Disable bit counter */
        #if (I2C_MODE & I2C_MODE_SLAVE)
            enableInterrupts = CyEnterCriticalSection();
            I2C_COUNTER_AUX_CTL_REG &= ~I2C_COUNTER_ENABLE_MASK;
            CyExitCriticalSection(enableInterrupts);
	    #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
        
    #endif  /* End (I2C_IMPLEMENTATION == I2C_FF) */
}


/*******************************************************************************
* Function Name: I2C_EnableInt
********************************************************************************
*
* Summary:
*  Enables the interrupt service routine for the component.  This is normally
*  handled with the start command.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void I2C_EnableInt(void)   
{
    #if (I2C_IMPLEMENTATION == I2C_UDB)
        I2C_INT_ENABLE_REG |= I2C_INT_ENABLE_MASK;
    #endif  /* End (I2C_IMPLEMENTATION == I2C_UDB) */
    
    CyIntEnable(I2C_ISR_NUMBER);
}


/*******************************************************************************
* Function Name: I2C_DisableInt
********************************************************************************
*
* Summary:
*  This function disables the interrupt service routine.  Normally this
*  function should never be called, instead use the Stop() function.
*
* Parameters:
*  void
*
* Return: 
*  void
*
* Side Effects:
*   If this function is called during normal operation, it will stop the
*   I2C function from working and it may lock up the I2C bus.
*
*******************************************************************************/
void I2C_DisableInt(void)   
{
    #if (I2C_IMPLEMENTATION == I2C_UDB)
        I2C_INT_ENABLE_REG |= I2C_INT_ENABLE_MASK;
    #endif  /* End (I2C_IMPLEMENTATION == I2C_UDB) */
    
    CyIntDisable(I2C_ISR_NUMBER);
}


#if (I2C_MODE & I2C_MODE_MASTER)

    /*******************************************************************************
    * Function Name: I2C_MasterStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns status of the I2C Master.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns master status register.
    *
    *******************************************************************************/
    uint8 I2C_MasterStatus(void) 
    {
        if(I2C_State == I2C_SM_SL_WR_IDLE)
        {
            return(I2C_mstrStatus);
        }
        else
        {
            return(I2C_mstrStatus | I2C_MSTAT_XFER_INP);
        }
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterClearStatus
    ********************************************************************************
    *
    * Summary:
    *  Clears master status flags.
    *
    * Parameters:
    *  void
    *
    * Return: 
    *  Returns the read status.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_MasterClearStatus(void)
    {
        uint8 status;
        
        status = I2C_mstrStatus ; 
        I2C_mstrStatus  = I2C_MSTAT_CLEAR; 
    
        return (status);
    }    
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterWriteBuf
    ********************************************************************************
    *
    * Summary:
    *  This function initiates a write transaction with an addressed slave.  Writes
    *  one or more bytes (cnt) to the slave I2C device and gets the data from RAM 
    *  or ROM array pointed to by the array pointer.  Once this routine is called, 
    *  the included ISR will handle further data in byte by byte mode.  
    *
    * Parameters:
    *  slaveAddr: 7-bit slave address
    *  xferData:  Pointer to data in array.
    *  cnt:       Count of data to write.
    *  mode:      Mode of operation.  It defines normal start, restart,
    *             stop, no-stop, etc.
    *
    * Return:
    *  void
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_MasterWriteBuf(uint8 slaveAddr, uint8 * xferData, uint8 cnt, uint8 mode)
    {
        uint8 errStatus = I2C_MSTR_SLAVE_BUSY; 
        
        /* Check if I2C in proper state to generate Start/ReStart condition */
        if((I2C_State == I2C_SM_IDLE) || \
           (I2C_State == I2C_SM_MSTR_HALT))
        {
            /* If IDLE, check if bus is free */
            if(I2C_State != I2C_SM_MSTR_HALT)
            {
                /* If Bus is free proceed, no exist with timeout */
                if(I2C_CHECK_BUS_FREE(I2C_MCSR_REG))
                {
                    errStatus = I2C_MSTR_NO_ERROR;
                }
                else
                {
                    errStatus = I2C_MSTR_BUS_BUSY;
                }
            }
            else   /* Bus halted waiting for restart */
            {
                errStatus = I2C_MSTR_NO_ERROR;
                I2C_mstrStatus &= ~I2C_MSTAT_XFER_HALT; 
                CyIntClearPending(I2C_ISR_NUMBER);
            }
    
            /* If no timeout error, generate start */
            if(errStatus == I2C_MSTR_NO_ERROR)
            {
                /* Determine whether or not to automatically generate a stop condition */
                if((mode & I2C_MODE_NO_STOP) != 0u)
                {
                    /* Do not generate a Stop at the end of transfer */
                    I2C_mstrControl |= I2C_MSTR_NO_STOP;
                }
                else  /* Generate a Stop */
                {
                    I2C_mstrControl &= ~I2C_MSTR_NO_STOP;
                }
        
                slaveAddr = (slaveAddr << 1u);
                I2C_State = I2C_SM_MSTR_WR_ADDR;
    
                I2C_mstrWrBufPtr   = xferData; /* Set buffer pointer */
                I2C_mstrWrBufIndex = 0u;       /* Start buffer at zero */
                I2C_mstrWrBufSize  = cnt;      /* Set buffer size */
    
                I2C_DATA_REG = slaveAddr;          /* Write address to data reg */
    
                /* Generate a Start or ReStart depending on flag passed */
                if((mode & I2C_MODE_REPEAT_START) != 0u)
                {
                    I2C_GENERATE_RESTART;  /* Generate a ReStart */
                }
                else
                {
                    I2C_GENERATE_START;    /* Generate a Start */
                }
                
                /* Enable interrupts to process transfer */
                I2C_EnableInt();
    
                /* Clear write complete flag */
                I2C_mstrStatus &= ~I2C_MSTAT_WR_CMPLT; 
            }
        }
   
        return (errStatus);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterReadBuf
    ********************************************************************************
    *
    * Summary:
    *   The function intiates a read transaction with an addressed slave.  Reads
    *   one or more bytes (cnt) from the slave I2C device and writes data to the
    *   array.  Once this routine is called, the included ISR will handle further
    *   data in byte by byte mode.
    *
    * Parameters:
    *  slaveAddr: 7-bit slave address
    *  xferData:  Pointer to data in array.
    *  cnt:       Count of data to write.
    *  mode:      Mode of operation.  It defines normal start, restart,
    *             stop, no-stop, etc.
    *
    * Return:
    *  void
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_MasterReadBuf(uint8 slaveAddr, uint8 * xferData, uint8 cnt, uint8 mode )
    {
        uint8 errStatus = I2C_MSTR_SLAVE_BUSY;
    
        /* Check if I2C in proper state to generate Start/ReStart condition */
        if((I2C_State == I2C_SM_IDLE) || \
           (I2C_State == I2C_SM_MSTR_HALT))
        {
            /* If IDLE, check if bus is free */
            if(I2C_State != I2C_SM_MSTR_HALT)
            {
                 /* If Bus is free proceed, no exist with timeout */
                if(I2C_CHECK_BUS_FREE(I2C_MCSR_REG))
                {
                    errStatus = I2C_MSTR_NO_ERROR;
                }
                else
                {
                    errStatus = I2C_MSTR_BUS_BUSY;
                }
            }
            else   /* Bus halted waiting for restart */
            {
                errStatus = I2C_MSTR_NO_ERROR;
                I2C_mstrStatus &= ~I2C_MSTAT_XFER_HALT; 
                CyIntClearPending(I2C_ISR_NUMBER);
            }
    
            /* If no timeout error, generate Start/ReStart condition */
            if(errStatus == I2C_MSTR_NO_ERROR)
            {
                /* Determine whether or not to automatically generate a stop condition */
                if((mode & I2C_MODE_NO_STOP) != 0u)
                {
                    /* Do not generate a Stop at the end of transfer */
                    I2C_mstrControl |= I2C_MSTR_NO_STOP;
                }
                else  /* Generate a Stop */
                {
                    I2C_mstrControl &= ~I2C_MSTR_NO_STOP;
                }
                
                slaveAddr = (slaveAddr << 1);
                slaveAddr |= I2C_READ_FLAG;   /* Set the Read flag */
                I2C_State = I2C_SM_MSTR_RD_ADDR;
    
                I2C_mstrRdBufPtr    = xferData;
                I2C_mstrRdBufIndex  = 0u;
                I2C_mstrRdBufSize   = cnt;    /* Set buffer size */
    
                I2C_DATA_REG = slaveAddr;         /* Write address to data reg */
    
                /* Generate a Start or ReStart depending on flag passed */
                if((mode & I2C_MODE_REPEAT_START) != 0u)
                {
                    I2C_GENERATE_RESTART;  /* Generate a ReStart */
                }
                else
                {
                    I2C_GENERATE_START;    /* Generate a Start */
                }

                /* Enable interrupts to process transfer */
                I2C_EnableInt();
    
                /* Clear read complete flag */
                I2C_mstrStatus &= ~I2C_MSTAT_RD_CMPLT; 
            }
        }

        return (errStatus);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterSendStart
    ********************************************************************************
    *
    * Summary:
    *  Sends a start with address and R/W bit.
    *
    * Parameters:  
    *  slaveAddress: Address of slave recipiant. 
    *  R_nW:         Send or recieve mode.
    *
    * Return: 
    *  Returns a non-zero value if an error is detected
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_MasterSendStart(uint8 slaveAddress, uint8 R_nW)
    {
        uint8 errStatus = I2C_MSTR_SLAVE_BUSY;
    
        if(I2C_State == I2C_SM_IDLE)
        {
            /* If Bus is free proceed, no exist with timeout */
            if(I2C_CHECK_BUS_FREE(I2C_MCSR_REG))
            {
                /* If no timeout error, generate Start/ReStart condition */
                CyIntDisable(I2C_ISR_NUMBER);
                slaveAddress = (slaveAddress << 1u);
                if(R_nW != 0u)
                {
                    slaveAddress |= I2C_READ_FLAG;   /* Set the Read flag */
                    I2C_State = I2C_SM_MSTR_RD_ADDR;
                }
                else
                {
                    I2C_State = I2C_SM_MSTR_WR_ADDR;
                }
                
                I2C_DATA_REG = slaveAddress;    /* Write address to data reg */
                
                /* Generates a Start */
                I2C_GENERATE_START;
                
                /* Wait for the address to be transfered */
                while(I2C_WAIT_BYTE_COMPLETE(I2C_CSR_REG));
                
                #if (I2C_MODE & I2C_MODE_MULTI_MASTER_ENABLE)
                    /* Check for loss of arbitration */
                    if(I2C_CHECK_LOST_ARB(I2C_CSR_REG))
                    {
                        /* Clear CSR to release the bus, if no Slave */
                        #if ((I2C_MODE & I2C_MODE_SLAVE) == 0u)
                            I2C_READY_TO_READ;
                        #endif  /* (I2C_MODE & I2C_MODE_SLAVE) == 0u) */    
    
                        /* Arbitration has been lost, reset state machine to Idle */
                        I2C_State = I2C_SM_IDLE;
                    
                        errStatus = I2C_MSTR_ERR_ARB_LOST;
                    }
                    else if(I2C_CHECK_ADDR_NAK(I2C_CSR_REG))
                    {
                        errStatus = I2C_MSTR_ERR_LB_NAK;    /* No device ACKed the Master */
                    }
                    else
                    {
                        errStatus = I2C_MSTR_NO_ERROR;     /* Send Start witout errors */
                    }
                    
                #else
                    /* Check ACK address if Master mode */
                    if(I2C_CHECK_ADDR_NAK(I2C_CSR_REG))
                    {
                        errStatus = I2C_MSTR_ERR_LB_NAK;    /* No device ACKed the Master */
                    }
                    else    
                    {
                        errStatus = I2C_MSTR_NO_ERROR;     /* Send Start witout errors */
                    }
                #endif  /* End (I2C_MODE & I2C_MODE_MULTI_MASTER_ENABLE) */
            }
            else
            {
                errStatus = I2C_MSTR_BUS_BUSY;     /* Bus is busy */
            } 
        }

        return (errStatus);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterSendRestart
    ********************************************************************************
    *
    * Summary:
    *   Sends a restart with address and R/W bit.
    *  
    * Parameters:
    *  slaveAddress: Address of slave recipiant. 
    *  R_nW:         Send or recieve mode.
    *
    * Return: 
    *  Returns a non-zero value if an error is detected
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_MasterSendRestart(uint8 slaveAddress, uint8 R_nW)
    {
        uint8 errStatus = I2C_MSTR_SLAVE_BUSY;
        
        /* Check if Start condition was generated */
        if(I2C_CHECK_MASTER_MODE(I2C_MCSR_REG))
        {
            slaveAddress = (slaveAddress << 1u);
            if(R_nW != 0u)
            {
                slaveAddress |= I2C_READ_FLAG;    /* Set the Read flag */
                I2C_State = I2C_SM_MSTR_RD_ADDR;
            }
            else
            {
                I2C_State = I2C_SM_MSTR_WR_ADDR;
            }
            
            I2C_DATA_REG = slaveAddress;    /* Write address to data reg */
            
            /* Generates restart */
            I2C_GENERATE_RESTART;
            #if (I2C_IMPLEMENTATION == I2C_UDB)
                while(I2C_CHECK_BYTE_COMPLETE(I2C_CSR_REG));
            #endif /* End (I2C_IMPLEMENTATION == I2C_UDB) */

            /* Wait for the address to be transfered  */
            while(I2C_WAIT_BYTE_COMPLETE(I2C_CSR_REG));

            if(I2C_CHECK_ADDR_NAK(I2C_CSR_REG))
            {
                errStatus = I2C_MSTR_ERR_LB_NAK;   /* No device ACKed the Master */
            }
            else
            {
                errStatus = I2C_MSTR_NO_ERROR;     /* Send Start witout errors */
            }
        }
        
        return (errStatus);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterSendStop
    ********************************************************************************
    *
    * Summary:
    *  Sends stop condition.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns a non-zero value if an error is detected 
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_MasterSendStop(void)
    {
        /* Always return the success */
        uint8 errStatus = I2C_MSTR_NO_ERROR;

        /* Generates stop */
        if(I2C_CHECK_MASTER_MODE(I2C_MCSR_REG))
        {
            I2C_GENERATE_STOP; 
        }
        
        /* Reset state to IDLE */
        I2C_State  = I2C_SM_IDLE;
                
        return (errStatus);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterWriteByte
    ********************************************************************************
    *
    * Summary:
    *  This function sends a single-byte I2C bus write and ACK.  This function does
    *  not generate a start or stop condition.  This routine should ONLY be called
    *  when a prevous start and address has been generated on the I2Cbus.
    *
    * Parameters:
    *  data:  Byte to be sent to the I2C slave
    *
    * Return:
    *  The return value is non-zero, if the slave acknowledged the master.
    *  The return value is zero, if the slave did not acknoledge the
    *  master.  If the slave failed to acknowledged the master, the
    *  value will be 0xFF.
    *
    *******************************************************************************/
    uint8  I2C_MasterWriteByte(uint8 theByte) 
    {
        uint8 errStatus = I2C_MSTR_SLAVE_BUSY;
        
        /* Check if Start condition was generated */
        if(I2C_CHECK_MASTER_MODE(I2C_MCSR_REG))
        {
            I2C_DATA_REG = theByte;
            I2C_TRANSMIT_DATA;
            I2C_State = I2C_SM_MSTR_WR_DATA;
            #if(I2C_IMPLEMENTATION == I2C_UDB)
                while(I2C_CHECK_BYTE_COMPLETE(I2C_CSR_REG));
            #endif
            
            /* Make sure the last byte has been transfered first. */
            while(I2C_WAIT_BYTE_COMPLETE(I2C_CSR_REG));
            
            if(I2C_CHECK_DATA_ACK(I2C_CSR_REG))
            {    
                errStatus = I2C_MSTR_NO_ERROR;     /* Send Start witout errors */
            }
            else
            {
                errStatus = I2C_MSTR_ERR_LB_NAK;   /* The last bit was NACKed */
            }
            
            I2C_State = I2C_SM_MSTR_HALT;
        }

        return (errStatus);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterReadByte
    ********************************************************************************
    *
    * Summary:
    *  This function sends a single-byte I2C bus read and ACK phase.  This function 
    *  does not generate a start or stop condition.  This routine should ONLY be 
    *  called when a prevous start and address has been generated on the I2Cbus.
    *
    * Parameters:
    *  acknNak:  If non-zero an ACK will be the response, else a zero will
    *            cause a NAK to be sent.
    *
    * Return:
    *  Returns the data received from the I2C slave.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_MasterReadByte(uint8 acknNak)
    {
        uint8 theByte = 0u;
        
        /* Check if Start condition was generated */
        if(I2C_CHECK_MASTER_MODE(I2C_MCSR_REG))
        {
            /* When address phase need release the bus and receive the byte,
            than decide ACK or NACK */
            if (I2C_SM_MSTR_RD_ADDR == I2C_State)
            {
                I2C_READY_TO_READ;
                I2C_State = I2C_SM_MSTR_DATA;
                #if (I2C_IMPLEMENTATION == I2C_UDB)
                    while(I2C_CHECK_BYTE_COMPLETE(I2C_CSR_REG));
                #endif /* End (I2C_IMPLEMENTATION == I2C_UDB) */
            }
            
            while(I2C_WAIT_BYTE_COMPLETE(I2C_CSR_REG));

            theByte = I2C_DATA_REG;
        
            /* Now if the ACK flag was set, Ack the data which will release the bus and start the next byte in
               otherwise do NOTHING to the CSR reg.  
               This will allow the calling routine to generate a repeat start or a stop depending on it's preference. */
            if(acknNak != 0u)   /* Do ACK */
            {
                I2C_ACK_AND_RECEIVE;
                #if (I2C_IMPLEMENTATION == I2C_UDB)
                    while(I2C_CHECK_BYTE_COMPLETE(I2C_CSR_REG));
                #endif /* End (I2C_IMPLEMENTATION == I2C_UDB) */
            }
            else                /* Do NACK */
            {
                /* Do nothing to be able work with ReStart */
                I2C_State = I2C_SM_MSTR_HALT;
            }
        }
        
        return (theByte);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterGetReadBufSize
    ********************************************************************************
    *
    * Summary:
    *  Determines the number of bytes used in the RX buffer. Empty returns 0.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Number of bytes in buffer until full.
    *
    *******************************************************************************/
    uint16 I2C_MasterGetReadBufSize(void) 
    {
        return (I2C_mstrRdBufIndex);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterGetWriteBufSize
    ********************************************************************************
    * 
    * Summary:
    *  Determine the number of bytes used in the TX buffer.  Empty returns 0.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    uint16 I2C_MasterGetWriteBufSize(void) 
    {
        return (I2C_mstrWrBufIndex);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterClearReadBuf
    ********************************************************************************
    *
    * Summary:
    *  Sets the buffer read and write pointers to 0.
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
    void I2C_MasterClearReadBuf(void)
    {
        I2C_mstrRdBufIndex = 0u;
        I2C_mstrStatus &= ~I2C_MSTAT_RD_CMPLT;
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_MasterClearWriteBuf
    ********************************************************************************
    *
    * Summary:
    *  Sets the buffer read and write pointers to 0.
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
    void I2C_MasterClearWriteBuf(void)
    {
        I2C_mstrWrBufIndex = 0u;
        I2C_mstrStatus &= ~I2C_MSTAT_WR_CMPLT;
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_Workaround
    ********************************************************************************
    *
    * Summary:
    *  Do nothing. This fake fuction use as workaround for CDT 78083.
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
    void I2C_Workaround(void)
    {

    }
    
#endif  /* End (I2C_MODE & I2C_MODE_MASTER) */


#if (I2C_MODE & I2C_MODE_SLAVE)

    /*******************************************************************************
    * Function Name: I2C_SlaveStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns status of the I2C status register. 
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Returns status of I2C slave status register.
    *
    *******************************************************************************/
    uint8 I2C_SlaveStatus(void)   
    {
        return (I2C_slStatus);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveClearReadStatus
    ********************************************************************************
    *
    * Summary:
    *  Clears the read status bits in the I2C_RsrcStatus register and returns read
    *  status.  No other bits are affected.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Return the read status.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_SlaveClearReadStatus(void) 
    {
        uint8 status;
    
        status = I2C_slStatus & I2C_SSTAT_RD_MASK;
        
        /* Mask of transfer complete flag and Error status */
        I2C_slStatus &= ~I2C_SSTAT_RD_CLEAR;
        
        return (status);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveClearWriteStatus
    ********************************************************************************
    *
    * Summary:
    *  Clears the write status bits in the I2C_Status register and returns write
    *  status. No other bits are affected.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  Return the write status.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    uint8 I2C_SlaveClearWriteStatus(void)
    {
        uint8 status;
    
        status = I2C_slStatus & I2C_SSTAT_WR_MASK;
    
        /* Mask of transfer complete flag and Error status */
        I2C_slStatus &= ~I2C_SSTAT_WR_CLEAR;
        
        return (status);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveSetAddress
    ********************************************************************************
    *
    * Summary:
    *  Sets the address for the first device.
    *
    * Parameters:
    *  (uint8) address:  The slave adderss for the first device.          
    *
    * Return:
    *  void
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    #if (I2C_ADDR_DECODE == I2C_HDWR_DECODE)
        void I2C_SlaveSetAddress(uint8 address) 
        { 
            I2C_ADDR_REG = address & I2C_SADDR_MASK;     /* Set I2C Address register */
        }
        
    #else
        void I2C_SlaveSetAddress(uint8 address)
        {  
            I2C_Address = address & I2C_SADDR_MASK;  /* Set Address variable */ 
        }
        
    #endif  /* End (I2C_ADDR_DECODE == I2C_HDWR_DECODE) */ 
    
    /*******************************************************************************
    * Function Name: I2C_SlavePutReadByte
    ********************************************************************************
    *
    * Summary:
    *  For Master Read, sends 1 byte out Slave transmit buffer.
    *  Wait to send byte until buffer has room.  Used to preload
    *  the transmit buffer.
    *
    *  In byte by byte mode if the last byte was ACKed, stall the master
    *  (on the first bit of the next byte) if needed until the next byte
    *  is PutChared.  If the last byte was NAKed it does not stall the bus
    *  because the master will generate a stop or restart condition.
    *
    * Parameters:
    *  (uint8) transmitDataByte: Byte containing the data to transmit.
    *
    * Return:
    *  void
    *
    *******************************************************************************/
    void I2C_SlavePutReadByte(uint8 transmitDataByte) 
    {
        I2C_DATA_REG = transmitDataByte;
        I2C_ACK_AND_TRANSMIT;
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveGetWriteByte
    ********************************************************************************
    *
    * Summary:
    *  For a Master Write, ACKs or NAKs the previous byte and reads out the last
    *  byte tranmitted.  The first byte read of a packet is the Address byte in 
    *  which case there is no previous data so no ACK or NAK is generated.  The
    *  bus is stalled until the next GetByte, therefore a GetByte must be executed
    *  after the last byte in order to send the final ACK or NAK before the Master
    *  can send a Stop or restart condition.
    *
    * Parameters:
    *  ackNak:  1 = ACK, 0 = NAK for the previous byte received.
    *
    * Return: 
    *  Last byte transmitted or last byte in buffer from Master.
    *
    *******************************************************************************/
    uint8 I2C_SlaveGetWriteByte(uint8 ackNak) 
    {
        uint8 dataByte;
    
        dataByte = I2C_DATA_REG;
        
        if(ackNak == I2C_ACK_DATA)
        {
            I2C_ACK_AND_RECEIVE;
        }
        else
        {
            I2C_NAK_AND_RECEIVE;
        }            
        
        return (dataByte);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveInitReadBuf
    ********************************************************************************
    *
    * Summary:
    *  This function sets up the buffer in which data will be read by the 
    *  Master.  The buffer index will be reset to zero and the status flags
    *  will be cleared with this command.
    *
    * Parameters:
    *  readBuf:  Pointer to the array to be sent to the Slave transmit register.
    *  bufSize:  Size of the buffer to transfer.
    *
    * Return:
    *  void
    *
    * Side Effects:
    *     If this function is called during a bus transaction, data from the 
    *     previous buffer location and the beginning of this buffer may be
    *     transmitted.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void I2C_SlaveInitReadBuf(uint8 * readBuf, uint8 bufSize)
    {
        I2C_readBufPtr   = readBuf;
        I2C_readBufIndex = 0u;
        I2C_readBufSize  = bufSize;  /* Set buffer size */
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveInitWriteBuf
    ********************************************************************************
    *
    * Summary:
    *  This function initializes the write buffer.  The write buffer is the array
    *  that is written to when the master performs a write operation.
    *
    * Parameters:
    *  writeBuf:  Pointer to the array used to store the data written by the Master 
    *             and read by the Slave.
    *  bufSize:   Size of buffer to receive data from master.
    *
    * Return: 
    *  void
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    void I2C_SlaveInitWriteBuf(uint8 * writeBuf, uint8 bufSize)
    {
        I2C_writeBufPtr   = writeBuf;
        I2C_writeBufIndex = 0u;
        I2C_writeBufSize  = bufSize;  /* Set buffer size */
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveGetReadBufSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the count of bytes read by the Master since the buffer was reset.
    *  The maximum return value will be the size of the buffer.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  (uint8) Bytes read by Master.
    *
    *******************************************************************************/
    uint8 I2C_SlaveGetReadBufSize(void) 
    {
        return (I2C_readBufIndex);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveGetWriteBufSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the count of bytes written by the I2C Master. The maximum value
    *  that will be returned in the buffer size itself.
    *
    * Parameters:
    *  void
    *
    * Return:
    *  The valid number of bytes in Tx buffer.
    *
    *******************************************************************************/
    uint8 I2C_SlaveGetWriteBufSize(void) 
    {
        return (I2C_writeBufIndex);
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveClearReadBuf
    ********************************************************************************
    *
    * Summary:
    *  Sets the buffer read buffer index back to zero.
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
    void I2C_SlaveClearReadBuf(void)
    {
        I2C_readBufIndex = 0u;
    }
    
    
    /*******************************************************************************
    * Function Name: I2C_SlaveClearRxBuf
    ********************************************************************************
    *
    * Summary:
    *  Sets the I2C write buffer index to 0.
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
    void I2C_SlaveClearWriteBuf(void)
    {
        I2C_writeBufIndex = 0u;
    }
    
    
#endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */


/* [] END OF FILE */
