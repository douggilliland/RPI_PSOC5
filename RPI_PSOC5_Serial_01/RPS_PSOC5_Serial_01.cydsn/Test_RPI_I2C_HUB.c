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

#include <project.h>
#include "RPI_PSOC5.h"

void testRPIHUB(void)
{
    I2C1_Start();
    uint8 SLAVE_ADDR = 0x71;
    uint8 cMessage[2];
    uint8 channel;
    //uint16 cLength = 256;
    
    // initialize the four DIGIO8 cards
    for (channel = 0; channel < 4; channel++)
    {
        cMessage[0] = 0x04 | channel;
        I2C1_MasterWriteBuf(SLAVE_ADDR, cMessage, 1, I2C_EEPROM_MODE_COMPLETE_XFER);
        initDIGIO8Card(0x20);
    }
    for (channel = 0; channel < 4; channel++)
    {
        cMessage[0] = 0x04 | channel;
        I2C1_MasterWriteBuf(SLAVE_ADDR, cMessage, 1, I2C_EEPROM_MODE_COMPLETE_XFER);
        writeDIGIO8Card(0x20, 0x01);
        CyDelay(200);
        writeDIGIO8Card(0x20, 0x02);
        CyDelay(200);
        writeDIGIO8Card(0x20, 0x04);
        CyDelay(200);
        writeDIGIO8Card(0x20, 0x08);
        CyDelay(200);
        writeDIGIO8Card(0x20, 0x00);
    }
}

/* [] END OF FILE */
