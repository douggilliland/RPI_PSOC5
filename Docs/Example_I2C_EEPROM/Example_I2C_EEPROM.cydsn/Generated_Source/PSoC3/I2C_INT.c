/*******************************************************************************
* File Name: I2C_INT.c  
* Version 2.0
*
*  Description:
*    This file contains the code that operates during the interrupt service
*    routine.  
*
*   Note:
*
*******************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include "cytypes.h"
#include "I2C.h"  


/**********************************
*      System variables
**********************************/

volatile uint8 I2C_State;               /* Current state of I2C state machine */
volatile uint8 I2C_Status;              /* Status byte */

/* Master variables */
#if (I2C_MODE & I2C_MODE_MASTER)
    volatile uint8 I2C_mstrStatus;         /* Master Status byte */
    volatile uint8 I2C_mstrControl;        /* Master Control byte */
    
    /* Transmit buffer variables */
    uint8 * I2C_mstrRdBufPtr;              /* Pointer to Master Tx/Rx buffer */       
    volatile uint8 I2C_mstrRdBufSize;     /* Master buffer size  */
    volatile uint8  I2C_mstrRdBufIndex;     /* Master buffer Index */
    
    /* Receive buffer variables */
    uint8 * I2C_mstrWrBufPtr;              /* Pointer to Master Tx/Rx buffer */       
    volatile uint8 I2C_mstrWrBufSize;      /* Master buffer size  */
    volatile uint8 I2C_mstrWrBufIndex;     /* Master buffer Index */

#endif  /* End (I2C_MODE & I2C_MODE_MASTER) */

/* Slave variables */
#if (I2C_MODE & I2C_MODE_SLAVE)
    volatile uint8 I2C_slStatus;          /* Slave Status byte */

    #if (I2C_ADDR_DECODE == I2C_SW_DECODE)
        volatile uint8 I2C_Address;       /* Software address variable */
    #endif  /* End (I2C_ADDR_DECODE == I2C_SW_DECODE) */

    /* Transmit buffer variables */
    uint8 * I2C_readBufPtr;                /* Pointer to Transmit buffer */       
    volatile uint8 I2C_readBufSize;        /* Slave Transmit buffer size */
    volatile uint8 I2C_readBufIndex;       /* Slave Transmit buffer Index */

    /* Receive buffer variables */
    uint8 * I2C_writeBufPtr;               /* Pointer to Receive buffer */       
    volatile uint8 I2C_writeBufSize;       /* Slave Receive buffer size */
    volatile uint8 I2C_writeBufIndex;      /* Slave Receive buffer Index */

#endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */


/*******************************************************************************
* Function Name: I2C_ISR
********************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine.  
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
CY_ISR(I2C_ISR)
{
    #if (I2C_MODE & I2C_MODE_SLAVE)
       static uint8  tmp8;    /* Making these static so not wasting time allocating */
    #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */

    static uint8  tmpCsr;  /* on the stack each time and no one else can see them */

    /* Entry from interrupt */
    /* In hardware address compare mode, we can assume we only get interrupted when */
    /* a valid address is recognized. In software address compare mode, we have to */
    /* check every address after a start condition.                                 */

    tmpCsr = I2C_CSR_REG;          /* Make temp copy so that we can check */
                                                /* for stop condition after we are done */
    
    #if (I2C_MODE & I2C_MODE_MULTI_MASTER_ENABLE)
       
        /* Check for loss of arbitration  */
        if(I2C_CHECK_LOST_ARB(tmpCsr))
        {
            /* Clear CSR to release the bus, if no Slave */
            #if ((I2C_MODE & I2C_MODE_SLAVE) == 0u)
                I2C_READY_TO_READ;
            #endif  /* (I2C_MODE == I2C_MULTI_MASTER_ENABLE) */    

            /* Arbitration has been lost, reset state machine to Idle */
            I2C_State = I2C_SM_IDLE;
  
            /* Set status transfer complete and arbitration lost */
            I2C_mstrStatus |= (I2C_MSTAT_ERR_ARB_LOST | I2C_MSTAT_ERR_XFER);
        }
    #endif  /* End (I2C_MODE & I2C_MODE_MULTI_MASTER_ENABLE) */    
     
    /* Check for Master operation mode  */
    if(I2C_State & I2C_SM_MASTER)    /*******  Master *******/
    { 
        #if (I2C_MODE & I2C_MODE_MASTER)
        /* Enter Master state machine */
      
            if(I2C_CHECK_BYTE_COMPLETE(tmpCsr))
            {
                switch(I2C_State)
                {
                    case I2C_SM_MSTR_WR_ADDR:    /* After address is sent, Write some data */
                    case I2C_SM_MSTR_RD_ADDR:    /* After address is sent, Read some data */
                
                        /* Check for Slave address ACK */
                        if(I2C_CHECK_ADDR_ACK(tmpCsr))  /* Check ACK/NAK */
                        {
                            /* Setup for transmit or receive of data */
                            if((I2C_State & I2C_SM_MSTR_ADDR) == I2C_SM_MSTR_WR_ADDR)  /* Write data */
                            {
                                if(I2C_mstrWrBufSize > 0)    /* Check if at least one byte is transfered */
                                {
                                    I2C_DATA_REG = I2C_mstrWrBufPtr[0u];   /* Load first data byte  */
                                    I2C_TRANSMIT_DATA;                                  /* Transmit data */
                                    I2C_mstrWrBufIndex = 1u;                            /* Set index to 2nd location */
                                    I2C_State = I2C_SM_MSTR_WR_DATA;       /* Set transmit state until done */
                                }
                                else   /* No data to tranfer */
                                {
                                    /* Handles 0 bytes transfer */
                                    #if (I2C_PSOC3_ES2 || I2C_PSOC5_ES1)
                                        I2C_GENERATE_STOP;
                                        I2C_mstrStatus |= I2C_MSTAT_WR_CMPLT;  /* Set status to Transfer complete */
                                        I2C_State  = I2C_SM_IDLE;              /* Reset State Machine to idle */
                                        
                                    #else  /* The PSoC3 ES3 only handles this well */
                                        if(I2C_CHECK_NO_STOP(I2C_mstrControl))
                                        {
                                            /* Don't do stop, just halt */
                                            I2C_State  = I2C_SM_MSTR_HALT;    /* Reset State Machine to Halt, expect ReStart */
                                            I2C_mstrStatus |= I2C_MSTAT_XFER_HALT; 
                                            CyIntClearPending(I2C_ISR_NUMBER);
                                            I2C_DisableInt();
                                        }
                                        else  /* Do normal Stop */
                                        {
                                            I2C_GENERATE_STOP;
                                            I2C_State = I2C_SM_IDLE;    /* Reset State Machine to idle */
                                        }
                                        
                                    #endif  /* End (I2C_PSOC3_ES2 || I2C_PSOC5_ES1) */
                                }
                            }
                            else  /* Master Receive data */
                            {                                
                                I2C_READY_TO_READ;                              /* Ready to Read data */
                                I2C_State  = I2C_SM_MSTR_RD_DATA;  /* Set state machine to Read data */
                            }
                        }
                        /* The Address was NAKed */
                        else if(I2C_CHECK_ADDR_NAK(tmpCsr))
                        {
                            if(I2C_CHECK_NO_STOP(I2C_mstrControl))
                            {
                                /* Don't do stop, just halt */
                                I2C_State  = I2C_SM_MSTR_HALT;    /* Reset State Machine to Halt, expect ReStart */
                                I2C_mstrStatus |= I2C_MSTAT_ERR_ADDR_NAK | \
                                                               I2C_MSTAT_XFER_HALT; 
                                CyIntClearPending(I2C_ISR_NUMBER);
                                I2C_DisableInt();
                            }
                            else  /* Do normal Stop */
                            {
                                I2C_GENERATE_STOP;
                                I2C_State = I2C_SM_IDLE;    /* Reset State Machine to idle */
                                /* Set Address NAK Error */
                                I2C_mstrStatus |= I2C_MSTAT_ERR_ADDR_NAK; 
                            }
                        }
                        else   
                        {
                            /* Bogus */
                            CyIntClearPending(I2C_ISR_NUMBER);
                            I2C_DisableInt();
                        }
                        break;
                
                    case I2C_SM_MSTR_WR_DATA:                                             /* Write data to slave */
                
                        if(I2C_CHECK_DATA_ACK(tmpCsr))       /* Check ACK */
                        {
                            if(I2C_mstrWrBufIndex  < I2C_mstrWrBufSize)    /* Check if end of buffer */
                            {
                                 /* Load first data byte  */
                                I2C_DATA_REG = I2C_mstrWrBufPtr[I2C_mstrWrBufIndex];
                                I2C_TRANSMIT_DATA;                                        /* Transmit */
                                
                                I2C_mstrWrBufIndex++;                                     /* Advance to data location */
                            }
                            else   /* Last byte was transmitted, send STOP */
                            {
                                if(I2C_CHECK_NO_STOP(I2C_mstrControl))
                                {
                                    /* Don't do stop, just halt */
                                    I2C_State  = I2C_SM_MSTR_HALT;    /* Reset State Machine to Halt, expect ReStart */
                                    I2C_mstrStatus |= (I2C_MSTAT_WR_CMPLT | \
                                                                    I2C_MSTAT_XFER_HALT);
                                    CyIntClearPending(I2C_ISR_NUMBER);
                                    I2C_DisableInt();
                                }
                                else  /* Do normal Stop */
                                {
                                    I2C_Workaround(); /* Workaround for CDT 78083 */
                                    I2C_GENERATE_STOP;
                                    I2C_State = I2C_SM_IDLE;    /* Reset State Machine to idle */
                                    I2C_mstrStatus |= I2C_MSTAT_WR_CMPLT;
                                }
                            }
                        }
                        else /* If last byte NAKed, stop transmit and send STOP. */
                        {
                            if(I2C_CHECK_NO_STOP(I2C_mstrControl))
                            {
                                /* Don't do stop, just halt */
                                I2C_State  = I2C_SM_MSTR_HALT;    /* Reset State Machine to Halt, expect ReStart */
                                I2C_mstrStatus |= (I2C_MSTAT_WR_CMPLT | \
                                                                I2C_MSTAT_XFER_HALT | \
                                                                I2C_MSTAT_ERR_SHORT_XFER);
                                CyIntClearPending(I2C_ISR_NUMBER);  
                                I2C_DisableInt();
                            }
                            else  /* Do normal Stop */
                            {
                                I2C_GENERATE_STOP;
                                I2C_State = I2C_SM_IDLE;    /* Reset State Machine to idle */
                                I2C_mstrStatus |= (I2C_MSTAT_WR_CMPLT | \
                                                                I2C_MSTAT_ERR_SHORT_XFER);
                            }
                        }
                        break;
                
                    case I2C_SM_MSTR_RD_DATA:  /* Data received */  
                
                        I2C_mstrRdBufPtr[I2C_mstrRdBufIndex] = I2C_DATA_REG; 
                        I2C_mstrRdBufIndex++;
                        if(I2C_mstrRdBufIndex  < I2C_mstrRdBufSize)    /* Check if end of buffer */
                        {
                            I2C_ACK_AND_RECEIVE;
                        }
                        else   /* End of data, generate a STOP */
                        {
                            if(I2C_CHECK_NO_STOP(I2C_mstrControl))
                            {
                                I2C_State = I2C_SM_MSTR_HALT;    /* Reset State Machine to Halt, expect ReStart */
                                I2C_mstrStatus |= (I2C_MSTAT_RD_CMPLT | \
                                                                I2C_MSTAT_XFER_HALT );
                            }
                            else   /* Do normal Stop */
                            {
                                I2C_NAK_AND_RECEIVE;
                                I2C_State = I2C_SM_IDLE;                /* Set state to idle */
                                I2C_mstrStatus |= I2C_MSTAT_RD_CMPLT;   /* Set status to complete read */
                            }
                        }
                        break;
                
                    case I2C_SM_MSTR_WAIT_STOP:
                        I2C_mstrStatus |= I2C_MSTAT_ERR_XFER;
                        break;
                
                    /* This case used for NO STOP condition, ready for a restart */
                    case I2C_SM_MSTR_HALT:   /* Do one transfer and halt, used for polling or single stepping */
                        I2C_mstrStatus |= I2C_MSTAT_WR_CMPLT;
                        I2C_DisableInt();
                        break;
                
                    default:
                        /* Invalid state, reset state machine  to a known state */
                        I2C_State = I2C_SM_IDLE;
                
                        /* Set transfer complete with error */
                        I2C_mstrStatus |= I2C_MSTAT_ERR_XFER;
                        break;
                }
            }
            
        #endif  /* End (I2C_MODE & I2C_MODE_MASTER) */
    }
    else  /******** Slave Mode ********/                                        
    {
        #if (I2C_MODE & I2C_MODE_SLAVE)
            /* Check to see if a Start/Address is detected */
            if((tmpCsr & I2C_CSR_ADDRESS) != 0u)
            {
                /* CSR bit _STOP_STATUS clears when Read/Write opearion */
                tmpCsr &=  ~I2C_CSR_STOP_STATUS;  /* Clear Stop bit */
                
                /* This is a Start or ReStart, So Reset the state machine, Check for a Read/Write condition */
                    
                #if (I2C_ADDR_DECODE == I2C_SW_DECODE)  /* Check for software address detection */
                    /******************* Software address detection ************************/
                    tmp8 = ((I2C_DATA_REG >> 1u) & I2C_SADDR_MASK);
                    if(tmp8 == I2C_Address)   /* Check for address match  */
                    {
                        if((I2C_DATA_REG & I2C_READ_FLAG) != 0u)  /* Check for read or write command */
                        {
                            /*******************************************/
                            /*  Place code to prepare read buffer here */
                            /*******************************************/
                            /* `#START SW_PREPARE_READ_BUF`  */

                            /* `#END`  */
                
                            /* Prepare next opeation to read, Get data and place in data register */
                            if(I2C_readBufIndex < I2C_readBufSize)  
                            {
                                I2C_DATA_REG = I2C_readBufPtr[I2C_readBufIndex];   /* Load first data byte  */
                                I2C_ACK_AND_TRANSMIT;
                                I2C_readBufIndex++;                                                      /* Advance to data location */
                                I2C_slStatus |= I2C_SSTAT_RD_BUSY;                          /* Set Read activity */
                            }
                            else   /* Data overflow */
                            {
                                I2C_DATA_REG = 0xFFu;    /* Out of range, send 0xFF  */
                                I2C_ACK_AND_TRANSMIT;
                                I2C_slStatus  |= (I2C_SSTAT_RD_BUSY | \
                                                               I2C_SSTAT_RD_ERR_OVFL); /* Set Read activity */
                            }
                            
                            I2C_State = I2C_SM_SL_RD_DATA;                                /* Prepare for read transaction */
                        }
                        else  /* Start of a Write transaction, reset pointers, first byte is address */
                        {
                            /* Prepare next opeation to write offset */
                            I2C_ACK_AND_RECEIVE;
                            I2C_slStatus |= I2C_SSTAT_WR_BUSY;       /* Set Write activity */
                            I2C_State     = I2C_SM_SL_WR_DATA;       /* Prepare for read transaction */
                
                            /* Enable interrupt on Stop */
                            I2C_ENABLE_INT_ON_STOP;
                        } 
                    }
                    /**********************************************/
                    /* Place compare for additional address here  */
                    /**********************************************/
                    /* `#START ADDR_COMPARE`  */

                    /* `#END`  */
                
                    else   /* No address match */
                    {
                        /* NAK address Match  */
                        I2C_NAK_AND_RECEIVE;
                    }
            
                #else  /* Hardware address detection */
                    if((I2C_DATA_REG & I2C_READ_FLAG) != 0u)  /* Check for read or write command */
                    {
                        /*******************************************/
                        /*  Place code to prepare read buffer here */
                        /*******************************************/
                        /* `#START SW_PREPARE_READ_BUF`  */

                        /* `#END`  */
                
                        /* Prepare next opeation to read, Get data and place in data register */
                        if(I2C_readBufIndex < I2C_readBufSize)  
                        {
                            I2C_DATA_REG = I2C_readBufPtr[I2C_readBufIndex];   /* Load first data byte  */
                            I2C_ACK_AND_TRANSMIT;                                                    /* ACK and transmit */
                            I2C_readBufIndex++;                                                      /* Advance to data location */
                            I2C_slStatus  |= I2C_SSTAT_RD_BUSY;                         /* Set Read activity */
                        }
                        else   /* Data overflow */
                        {
                            #if defined(CYDEV_BOOTLOADER_IO_COMP) && (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_I2C)
                                I2C_slStatus |= (I2C_SSTAT_RD_BUSY | \
                                                              I2C_SSTAT_RD_ERR_OVFL);      /* Set Read activity and Start clock stretching,
                                                                                                           SCL = 0, till the CyBtldrCommWrite() gives the 
                                                                                                           ACK response and data */	
                            #else
                                I2C_DATA_REG = 0xFFu;    /* Out of range, send 0xFF  */
                                I2C_ACK_AND_TRANSMIT;
                                I2C_slStatus  |= (I2C_SSTAT_RD_BUSY | \
                                                               I2C_SSTAT_RD_ERR_OVFL); /* Set Read activity */
                            
                            #endif  /* End defined(CYDEV_BOOTLOADER_IO_COMP) && 
                                       (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_I2C) */
                        }
                        
                        I2C_State = I2C_SM_SL_RD_DATA;    /* Prepare for read transaction */
                    }
                    else  /* Start of a Write transaction, reset pointers, first byte is address */
                    {
                        /* Prepare next opeation to write offset */
                        I2C_ACK_AND_RECEIVE; /* ACK and ready to receive addr */
                        I2C_slStatus |= I2C_SSTAT_WR_BUSY;       /* Set Write activity */
                        I2C_State     = I2C_SM_SL_WR_DATA;       /* Prepare for read transaction */
                        I2C_ENABLE_INT_ON_STOP;                               /* Enable interrupt on Stop */
                    }
                    
                #endif  /* End (I2C_ADDR_DECODE == I2C_SW_DECODE) */
                
            }
            else if(I2C_CHECK_BYTE_COMPLETE(tmpCsr))    /* Check for data transfer */
            {
                if (I2C_State == I2C_SM_SL_WR_DATA)    /* Data write from Master to Slave. */
                {
                    if(I2C_writeBufIndex < I2C_writeBufSize)       /* Check for valid range */
                    {
                        tmp8 = I2C_DATA_REG;                                        /* Get data, to ACK quickly */
                        I2C_ACK_AND_RECEIVE;                                    /* ACK and ready to receive sub addr */
                        I2C_writeBufPtr[I2C_writeBufIndex] = tmp8; /* Write data to array */
                        I2C_writeBufIndex++;                                    /* Inc pointer */
                    }
                    else
                    {
                        /* NAK cause beyond write area */
                        I2C_NAK_AND_RECEIVE;
                        I2C_slStatus &= ~I2C_SSTAT_WR_BUSY;        /* Set Write activity */
                        I2C_slStatus |= (I2C_SSTAT_WR_CMPT | \
                                                      I2C_SSTAT_WR_ERR_OVFL);   /* Set Write activity */
                    }
                }
                else if (I2C_State == I2C_SM_SL_RD_DATA)    /* Data Read from Slave to Master */
                {
                    if(I2C_CHECK_DATA_ACK(tmpCsr))
                    {
                        if(I2C_readBufIndex < I2C_readBufSize) 
                        {
                             /* Get data from array */
                            I2C_DATA_REG = I2C_readBufPtr[I2C_readBufIndex];
            
                            /* Send Data */
                            I2C_TRANSMIT_DATA;
                            I2C_readBufIndex++;                                  /* Inc pointer */
                        }
                        else   /* Over flow */
                        {
                            /* Send 0xFF at the end of the buffer */
                            I2C_DATA_REG = 0xFFu;
            
                            /* Send Data */
                            I2C_TRANSMIT_DATA;
                            I2C_slStatus  |= I2C_SSTAT_RD_ERR_OVFL; /* Set overflow */
                        }
                    }
                    else  /* Last byte NAKed, done */
                    {
                        /* End of read transaction */
                        I2C_DATA_REG = 0xFFu;
                         
                        /* Clear transmit bit at the end of read transaction */
                        I2C_NAK_AND_TRANSMIT;
                        I2C_slStatus &= ~I2C_SSTAT_RD_BUSY;   /* Clear Busy Flag */
                        I2C_slStatus |= I2C_SSTAT_RD_CMPT;    /* Set complete Flag */
                        I2C_State = I2C_SM_IDLE;              /* Return to idle state */
                    }
                }
                else  /* This is an invalid state and should not occur  */
                {
                    /* Invalid state, Reset */
                    I2C_State = I2C_SM_IDLE;
                    I2C_NAK_AND_RECEIVE;
                }   /* End Transfer mode */
            }  
            else if ((tmpCsr & I2C_CSR_BUS_ERROR) != 0u)    /* Quick check for Error */
            {
                if((I2C_CSR_REG & I2C_CSR_BUS_ERROR) != 0u)
                {
                    /* May want to reset bus here CHECK */
                }
            } /* End if without else */
            
            if((tmpCsr & I2C_CSR_STOP_STATUS) != 0u)    /* Check if Stop was detected */
            {
                /* 1) The Write transaction only IE on Stop, so Read never gets here */
                /* 2) The WR_BUSY flag will be cleared at the end of "Write-ReStart-Read-Stop" transaction */
                I2C_slStatus &= ~I2C_SSTAT_WR_BUSY;   /* Clear Busy Flag */
                I2C_slStatus |= I2C_SSTAT_WR_CMPT;    /* Set complete Flag */
                I2C_DISABLE_INT_ON_STOP;
                I2C_State = I2C_SM_IDLE;
            }       
        
        #endif  /* End (I2C_MODE & I2C_MODE_SLAVE) */
    }
    
    #if (I2C_PSOC3_ES2 && (I2C_I2C_IRQ__ES2_PATCH))
        I2C_ISR_PATCH();
    #endif  /* End (I2C_PSOC3_ES2 && (I2C_I2C_IRQ__ES2_PATCH)) */
}


/* [] END OF FILE */
