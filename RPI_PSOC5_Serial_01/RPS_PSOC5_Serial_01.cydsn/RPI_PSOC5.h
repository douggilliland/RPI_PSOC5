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

#include <I2C_EEPROM.h>

#include "Test_RPP-UIO_16.h"
#include "Test_RASPI_GVS.h"
#include "Test_RASPI_PLUS_GVS.h"
#include "RasPi_Pins.h"
#include "ExtEEPROM.h"
#include "EEPROM_Images.h"
#include "PSoC_Menu.h"

enum Card_Types_Enum {
    UNSELECTED_CARD,
    RPPUIO16,
    RPPGVSCFG,
    RPPSOC,
    RASPIGVS,
    RASPIPLUSGVS,
    RPII2CHUB
};

/* [] END OF FILE */
