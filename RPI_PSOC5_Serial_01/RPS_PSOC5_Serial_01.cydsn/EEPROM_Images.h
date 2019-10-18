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

enum EEPROM_Images {
    UNSELECTED_CARD,
    RPPUIO16,
    RPPGVSCFG,
    RPPSOC
};

#include <project.h>

#define RPP_UIO16_EEPROM_LENGTH 109
extern const uint8 rpp_uio_16_eeprom[RPP_UIO16_EEPROM_LENGTH];

#define RPPGVSCFG_EEPROM_LENGTH 117
extern const uint8 raspi_plus_gvs_cfg_eeprom[RPPGVSCFG_EEPROM_LENGTH];

#define RPPSOC_EEPROM_LENGTH 105
extern const uint8 rppsoc_eeprom[RPPSOC_EEPROM_LENGTH];

/* [] END OF FILE */
