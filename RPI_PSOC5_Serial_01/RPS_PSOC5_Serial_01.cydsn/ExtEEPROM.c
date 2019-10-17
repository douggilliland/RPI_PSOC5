/* ========================================
 *
 * Copyright LAND BOARDS, LLC, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Land Boards.
 *
 * ========================================
*/

#include "ExtEEPROM.h"

void readEEPROM(char *eepromBuffer)
{
    for (uint16 val = 0; val < 0x200; val++)
    {
        eepromBuffer[val] = (char) val;     // Fill with data = lowest 8-bits of address (temporary solution)
    }
}

void dumpEEPROM(char * eepromBuffer)
{
    char lineBuffer[8];
    lineBuffer[0] = 0;
    uint16 linesLoop = 0;
    uint16 charsLoop = 0;
    for (linesLoop = 0; linesLoop < 16; linesLoop++)
    {
        char lineString[65];
        lineString[0] = 0;
        sprintf(lineString,"%04x",linesLoop<<4);
        for (charsLoop = 0; charsLoop < 16; charsLoop++)
        {
            sprintf(lineBuffer, " %02x",eepromBuffer[(linesLoop<<4)+charsLoop]);
            strcat(lineString, lineBuffer);
        }
        strcat(lineString, "\n\r");
        USBUART_PutData(lineString, strlen(lineString));
        while (0u == USBUART_CDCIsReady());
    }
}

/* [] END OF FILE */
