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
#include <I2C_EEPROM.h>

void readEEPROM(uint8 *);
void dumpEEPROM(uint8 *);
void writeEEPROM(uint16);
void writeEEPROMBlock(uint8 *);

#define MAX_ID_IMAGE_SIZE   128
#define NUM_32_BYTE_BLOCKS  4
#define EEPROM_BLOCK_SIZE   32

/* [] END OF FILE */
