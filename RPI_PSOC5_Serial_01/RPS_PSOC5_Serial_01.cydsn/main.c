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

void putStringToUSB(char * stringToPutOutUSB);

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
    uint8 inBuffer[USBUART_Buffer_SIZE];
    uint8 eepromBuffer[256];
    uint16 cardType = UNSELECTED_CARD;
        
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
                        putStringToUSB("Read from the EEPROM\n\r");
                        readEEPROM(eepromBuffer);
                        dumpEEPROM(eepromBuffer);
                        while (0u == USBUART_CDCIsReady());
                    }
                    else if ((inBuffer[0] == 'w') || (inBuffer[0] == 'W'))
                    {
                        if (cardType == UNSELECTED_CARD)
                        {
                            putStringToUSB("Must first select card type\n\r");
                        }
                        else
                        {
                            putStringToUSB("Write to the EEPROM...");
                            writeEEPROM(cardType);
                            putStringToUSB("Completed EEPROM write\n\r");
                        }
                    }
                    else if ((inBuffer[0] == 'b') || (inBuffer[0] == 'B'))
                    {
                        if (cardType == UNSELECTED_CARD)
                        {
                            putStringToUSB("Must first select card type\n\r");
                        }
                        else if (cardType == RPPUIO16)
                        {
                            putStringToUSB("Blinking the LEDs on the RPP-UIO-16 card, please wait\n\r");
                            testRPPUIO16();
                            putStringToUSB("Completed blinking the LEDs on the RPP-UIO-16 card\n\r");
                        }
                        else
                        {
                            putStringToUSB("Card not yet implemented\n\r");
                        }
                    }
                    else if (inBuffer[0] == '1')
                    {
                        cardType = RPPUIO16;
                        putStringToUSB("Selected RPP-UIO-16 card\n\r");
                    }
                    else if (inBuffer[0] == '2')
                    {
                        cardType = RPPSOC;
                        putStringToUSB("Selected RPPSOC card\n\r");
                    }
                    else if (inBuffer[0] == '3')
                    {
                        cardType = RPPGVSCFG;
                        putStringToUSB("Selected RASPI-PLUS-GVS-CFG card\n\r");
                    }
                    else
                    {
                        putStringToUSB("Land Boards, LLC - RPi Card Test Station\n\r");
                        putStringToUSB("1 - Select RPP-UIO-16 Card\n\r");
                        putStringToUSB("2 - Select RPPSOC Card\n\r");
                        putStringToUSB("3 - Select RASPI-PLUS-GVS-CFG Card\n\r");
                        putStringToUSB("R - Read EEPROM\n\r");
                        putStringToUSB("W - Write EEPROM\n\r");
                        putStringToUSB("B - Bounce LED across Card GPIOs\n\r");
                        putStringToUSB("? - Print this menu\n\r");
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

void putStringToUSB(char * stringToPutOutUSB)
{
    USBUART_PutData((uint8 *)stringToPutOutUSB, strlen(stringToPutOutUSB));
    while (0u == USBUART_CDCIsReady());
}

/* [] END OF FILE */
