#ifndef __COMMAND_H__
#define __COMMAND_H__

// Extention Uint ƒRƒ}ƒ“ƒhˆê——:
typedef enum E_CA378_COMMAND
{
    CONFIG_AUTO         = 0x80,         // config AUTO (UVC Setting)
    CONFIG_12M,                         // config 12M (4056x3040)
    CONFIG_3M,                          // config 3M (2028x1520)
    CONFIG_760P,                        // config 760P (1012x760)
    SET_EXPOSURE        = 0x90,         // set exposure
    SET_ANALOG_GAIN,                    // set analog gain
    SET_WHITE_BALANCE,                  // set white blance
    SET_BLACK_LEVEL,                    // set black level
    SET_DEFECT_PIXEL,                   // set defect pixel
    SET_LENS_SHADING,                   // set lens shading
    SET_FOCUS_POSITION,                 // set focus position
    SET_OIS_MODE,                       // set OIS mode
    GET_EXPOSURE        = 0xA0,         // get exposure
    GET_ANALOG_GAIN,                    // get analog gain
    GET_WHITE_BALANCE,                  // get white blance
    GET_BLACK_LEVEL,                    // get black level
    GET_DEFECT_PIXEL,                   // get defect pixel
    GET_LENS_SHADING,                   // get lens shading
    GET_FOCUS_POSITION,                 // get focus position
    GET_OIS_MODE,                       // get OIS mode
    EEPROM_SAVE         = 0xB0,         // EEPROM save
    EEPROM_LOAD,                        // EEPROM load
    EEPROM_SAVE_LSC,                    // EEPROM save for LSC
    EEPROM_LOAD_LSC,                    // EEPROM load for LSC
    EEPROM_CLEAR,                       // EEPROM clear
    EEPROM_DEFAULT,                     // EEPROM default
    REGISTER_WRITE      = 0xC0,         // write register (for debug)
    REGISTER_READ       = 0xD0,         // read register (for debug)
    SPI_ERASE           = 0xE0,         // SPI erase
    SPI_UPDATE          = 0xE1,         // SPI update
    GET_FW_VERSION      = 0xF0,         // get firmware version
} CA378_COMMAND;

#endif
