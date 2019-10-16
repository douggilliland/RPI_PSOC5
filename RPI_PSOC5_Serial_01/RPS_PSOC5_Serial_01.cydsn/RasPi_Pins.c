/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "RasPi_Pins.h"

void setRasPiPin(uint8 bcmPinNum, uint8 value)
{
    switch (bcmPinNum)
    {
        case IO_2:
            IO_2_SetDriveMode(PIN_DM_STRONG);
            IO_2_Write(value);
            break;
        case IO_3:
            IO_3_SetDriveMode(PIN_DM_STRONG);
            IO_3_Write(value);
            break;
        case IO_4:
            IO_4_SetDriveMode(PIN_DM_STRONG);
            IO_4_Write(value);
            break;
        case IO_14:
            IO_14_SetDriveMode(PIN_DM_STRONG);
            IO_14_Write(value);
            break;
        case IO_15:
            IO_14_SetDriveMode(PIN_DM_STRONG);
            IO_15_Write(value);
            break;
        case IO_17:
            IO_17_SetDriveMode(PIN_DM_STRONG);
            IO_17_Write(value);
            break;
        case IO_18:
            IO_18_SetDriveMode(PIN_DM_STRONG);
            IO_18_Write(value);
            break;
        case IO_27:
            IO_27_SetDriveMode(PIN_DM_STRONG);
            IO_27_Write(value);
            break;
        case IO_22:
            IO_22_SetDriveMode(PIN_DM_STRONG);
            IO_22_Write(value);
            break;
        case IO_23:
            IO_23_SetDriveMode(PIN_DM_STRONG);
            IO_23_Write(value);
            break;
        case IO_24:
            IO_24_SetDriveMode(PIN_DM_STRONG);
            IO_24_Write(value);
            break;
        case IO_7:
            IO_7_SetDriveMode(PIN_DM_STRONG);
            IO_7_Write(value);
            break;
        case IO_5:
            IO_5_SetDriveMode(PIN_DM_STRONG);
            IO_5_Write(value);
            break;
        case IO_6:
            IO_6_SetDriveMode(PIN_DM_STRONG);
            IO_6_Write(value);
            break;
        case IO_12:
            IO_12_SetDriveMode(PIN_DM_STRONG);
            IO_12_Write(value);
            break;
        case IO_13:
            IO_13_SetDriveMode(PIN_DM_STRONG);
            IO_13_Write(value);
            break;
        case IO_19:
            IO_19_SetDriveMode(PIN_DM_STRONG);
            IO_19_Write(value);
            break;
        case IO_16:
            IO_16_SetDriveMode(PIN_DM_STRONG);
            IO_16_Write(value);
            break;
        case IO_26:
            IO_26_SetDriveMode(PIN_DM_STRONG);
            IO_26_Write(value);
            break;
        case IO_20:
            IO_20_SetDriveMode(PIN_DM_STRONG);
            IO_20_Write(value);
            break;
        case IO_21:
            IO_21_SetDriveMode(PIN_DM_STRONG);
            IO_21_Write(value);
            break;
        }
}


/* [] END OF FILE */
