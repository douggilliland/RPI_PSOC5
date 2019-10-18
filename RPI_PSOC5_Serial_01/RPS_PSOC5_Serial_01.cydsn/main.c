/*******************************************************************************
* File Name: main.c
*
* Version: 2.0
*
* Description:
*   The component is enumerated as a Virtual Com port. Receives data from the 
*   hyper terminal, then sends back the received data.
*
* Related Document:
*  Universal Serial Bus Specification Revision 2.0
*  Universal Serial Bus Class Definitions for Communications Devices
*  Revision 1.2
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>
#include "stdio.h"
#include "Test_RPP-UIO_16.h"
#include "ExtEEPROM.h"
#include "EEPROM_Images.h"

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow usage of the floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
//    asm (".global _printf_float");
#endif

#define USBFS_DEVICE    (0u)

/* The inBuffer size is equal to the maximum packet size of the IN and OUT bulk
* endpoints.
*/
#define USBUART_Buffer_SIZE (64u)

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  The main function performs the following actions:
*   1. Waits until VBUS becomes valid and starts the USBFS component which is
*      enumerated as virtual Com port.
*   2. Waits until the device is enumerated by the host.
*   3. Waits for data coming from the hyper terminal
*   4. Parse command.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{
    uint16 inCount;
    uint16 outCount;
    uint8 inBuffer[USBUART_Buffer_SIZE];
    char outBuffer[USBUART_Buffer_SIZE];
    uint8 eepromBuffer[256];
        
    CyGlobalIntEnable;

    /* Start USBFS operation with 5-V operation. */
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);

    // enable I2C interrupts
	I2C_EEPROM_EnableInt();  
	//Start I2C
	I2C_EEPROM_Start(); 
    
    for(;;)
    {
        /* Host can send double SET_INTERFACE request. */
        if (0u != USBUART_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data 
                 * from host. */
                USBUART_CDC_Init();
            }
        }

        /* Service USB CDC when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Check for input data from host. */
            if (0u != USBUART_DataIsReady())
            {
                /* Read received data and re-enable OUT endpoint. */
                inCount = USBUART_GetAll(inBuffer);

                if (0u != inCount)
                {
                    /* Wait until component is ready to send data to host. */
                    while (0u == USBUART_CDCIsReady());
                    if ((inBuffer[0] == 'r') || (inBuffer[0] == 'R'))
                    {
                        strcpy(outBuffer,"Read from the EEPROM\n\r");
                        outCount = strlen(outBuffer);
                        USBUART_PutData((uint8 *)outBuffer, outCount);
                        while (0u == USBUART_CDCIsReady());
                        readEEPROM(eepromBuffer);
                        dumpEEPROM(eepromBuffer);
                    }
                    else if ((inBuffer[0] == 'w') || (inBuffer[0] == 'W'))
                    {
                        strcpy(outBuffer,"Write to the EEPROM...");
                        outCount = strlen(outBuffer);
                        USBUART_PutData((uint8 *)outBuffer, outCount);
                        while (0u == USBUART_CDCIsReady());
                        writeEEPROM(RPPUIO16);
                        strcpy(outBuffer,"Completed EEPROM write\n\r");
                        outCount = strlen(outBuffer);
                        USBUART_PutData((uint8 *)outBuffer, outCount);
                    }
                    else if ((inBuffer[0] == 'b') || (inBuffer[0] == 'B'))
                    {
                        strcpy(outBuffer,"Blinking the LEDs on the RPP-UIO-16 card, please wait\n\r");
                        outCount = strlen(outBuffer);
                        USBUART_PutData((uint8 *)outBuffer, outCount);
                        while (0u == USBUART_CDCIsReady());
                        testRPPUIO16();
                        strcpy(outBuffer,"Completed blinking the LEDs on the RPP-UIO-16 card\n\r");
                        USBUART_PutData((uint8 *)outBuffer, outCount);
                        while (0u == USBUART_CDCIsReady());
                    }
                    else
                    {
                        strcpy(outBuffer,"Not a valid command, legal values are r, w, b\n\r");
                        outCount = strlen(outBuffer);
                        USBUART_PutData((uint8 *)outBuffer, outCount);
                        while (0u == USBUART_CDCIsReady());
                    }
                    /* If the last sent packet is exactly the maximum packet 
                    *  size, it is followed by a zero-length packet to assure
                    *  that the end of the segment is properly identified by 
                    *  the terminal.
                    */
                    if (USBUART_Buffer_SIZE == inCount)
                    {
                        /* Wait until component is ready to send data to PC. */
                        while (0u == USBUART_CDCIsReady())
                        {
                        }

                        /* Send zero-length packet to PC. */
                        USBUART_PutData(NULL, 0u);
                    }
                }
            }
        }
    }
}


/* [] END OF FILE */
