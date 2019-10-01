#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// List of return codes:
typedef enum E_CA378_RESULT
{
    CA378_SUCCESS                   = 0x00,     // Successful execution of basic function
    CA378_ERR_NOMEM                 = 0x01,     // Memory allocation failed
    CA378_ERR_THREAD                = 0x02,     // Thread creation failed
    CA378_ERR_EVENT                 = 0x03,     // Event creation failed
    CA378_ERR_FATAL                 = 0x04,     // A fatal error has occurred
    CA378_ERR_SYSTEM_INIT           = 0x05,     // System initialization failed
    CA378_ERR_CX3_INIT              = 0x06,     // CX3 device initialization failed
    CA378_ERR_CAMERA_MIPI           = 0x07,     // Camera MIPI communication failed
    CA378_ERR_CAMERA_SETUP          = 0x08,     // Camera module setup failed
    CA378_ERR_PARAM                 = 0x09,     // Incorrect parameter specification
    CA378_ERR_SENSOR_IF             = 0x0A,     // An error occurred during image sensor communication
    CA378_ERR_EEPROM_IF             = 0x0B,     // Error occurred in EEPROM communication
    CA378_ERR_EEPROM_NOT_SET        = 0x0C,     // No value is set in EEPROM
    CA378_ERR_SPI_IF                = 0x0D,     // An error occurred during SPI communication
    CA378_ERR_NOT_USE_USBSPEED      = 0x0E,     // Cannot be used at the current USB speed (USB2.0 or USB1.0)
    CA378_ERR_EXT_UNIT_NOT_FOUND    = 0x10,     // Extension unit not found
    CA378_ERR_EXT_UNIT_FAIL         = 0x11,     // Extension unit communication failed
    CA378_ERR_EXT_UNIT_BAD_SIZE     = 0x12,     // The size is incorrect in the extension unit communication
    CA378_ERR_EXT_UNIT_UNKNOWN_CMD  = 0x13,     // Unknown command in extension unit communication
    CA378_ERR_EXT_UNIT_TIMEOUT      = 0x14,     // Timeout on extension unit communication
    CA378_ERR_SEND                  = 0x20,     // Transmission error + 0x20
    CA378_ERR_RECV                  = 0x40,     // Receive error + 0x40
} CA378_RESULT;

#endif
