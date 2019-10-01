// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the USBCAEXTENSIONUNIT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// USBCAEXTENSIONUNIT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef USBCAEXTENSIONUNIT_EXPORTS
#define USBCAEXTENSIONUNIT_API __declspec(dllexport)
#else
#define USBCAEXTENSIONUNIT_API __declspec(dllimport)
#endif

#include "types.h"
#include "errcode.h"


// This class is exported from the USBCAExtensionUnit.dll
class USBCAEXTENSIONUNIT_API CUSBCAExtensionUnit {
public:
    CUSBCAExtensionUnit(void);
    ~CUSBCAExtensionUnit(void);

    // Function: CA378_RESULT CA378_AUTO_config(void)
    // Argument: None
    // Return value: success / failure (see return code)
    // Description: This function follows the resolution setting specified in UVC.
    CA378_RESULT CA378_AUTO_config(void);

    // Function: CA378_RESULT CA378_12MP_config(void)
    // Argument: None
    // Return value: success / failure (see return code)
    // Description: This function writes the 12MP (4056x3040) resolution setting of 9fps frame rate to the image sensor.
    //              When this function is executed, the image sensor will start to output 12MP resolution video at 9fps.
    CA378_RESULT CA378_12MP_config(void);

    // Function: CA378_RESULT CA378_3MP_config(void)
    // Argument: None
    // Return value: success / failure (see return code)
    // Description: This function writes the 3MP (2028x1520) resolution setting at 30fps frame rate to the image sensor.
    //              When this function is executed, the image sensor starts to output 3MP resolution video at 30fps.
    CA378_RESULT CA378_3MP_config(void);

    // Function: CA378_RESULT CA378_760P_config(void)
    // Argument: None
    // Return value: success / failure (see return code)
    // Description: This function writes the 760P (1012x760) resolution setting at 60fps frame rate to the image sensor.
    //              When this function is executed, the image sensor starts to output 760P resolution video at 60fps.
    CA378_RESULT CA378_760P_config(void);

    // Function: CA378_RESULT CA378_SetExposure(uint16_t value)
    // Argument: uint16_t value     Exposure value
    // Return value: success / failure (see return code)
    // Description: This function sets the exposure time of the electronic shutter.
    //              At 3M (2028x1520) and 760P (1012x760) resolution, 1580 is equivalent to 1/60 second.
    //                  exposure time (seconds) = (value + 20) / 96000
    //              At 12M (4056x3040) resolution, 1580 corresponds to 1/30 second.
    //                  exposure time (seconds) = (value + 20) / 48000
    // Format: 16-bit unsigned integer value
    // Range: 1-65515
    CA378_RESULT CA378_SetExposure(uint16_t value);

    // Function: CA378_RESULT CA378_GetExposure(uint16_t *value)
    // Argument: uint16_t *value   Exposure value
    // Return value: success / failure (see return code)
    // Description: This function gets the exposure time of the electronic shutter.
    // Format: 16-bit unsigned integer value
    // Range: 1-65515
    CA378_RESULT CA378_GetExposure(uint16_t *value);

    // Function: CA378_RESULT CA378_SetAnalogGain(uint16_t value)
    // Argument: uint16_t value     Analog gain value
    // Return value: success / failure (see return code)
    // Description: This function sets the analog gain value.
    // Format: 16-bit unsigned fixed point Q8.8
    // Range: 1.0 (0x100)-22.262 (0x1643)
    CA378_RESULT CA378_SetAnalogGain(uint16_t value);

    // Function: CA378_RESULT CA378_GetAnalogGain(uint16_t *value)
    // Argument: uint16_t *value   Analog gain value
    // Return value: success / failure (see return code)
    // Description: This function gets the analog gain value.
    // Format: 16-bit unsigned fixed point Q8.8
    // Range: 1.0 (0x100)-22.262 (0x1643)
    CA378_RESULT CA378_GetAnalogGain(uint16_t *value);

    // Function: CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value)
    // Argument: uint16_t r_value   R value
    //           uint16_t g_value   G value
    //           uint16_t b_value   B value
    // Return value: success / failure (see return code)
    // Description: This function sets the white balance value.
    // Format: 16-bit unsigned fixed point Q8.8
    // Range: 1.0 (0x100)-15.996 (0xFFF)
    CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value);

    // Function: CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value)
    // Argument: uint16_t *r_value     R value
    //           uint16_t *g_value     G value
    //           uint16_t *b_value     B value
    // Return value: success / failure (see return code)
    // Description: This function gets the white balance value.
    // Format: 16-bit unsigned fixed point Q8.8
    // Range: 1.0 (0x100)-15.996 (0xFFF)
    CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value);

    // Function: CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value)
    // Argument: uint8_t enable     0: Disable, 1: Enable
    //           uint16_t value     Black level correction value
    // Return value: success / failure (see return code)
    // Description: This function sets the black level correction.
    // Format: 16-bit unsigned integer value
    // Initial value: Enabled / 64 (when RAW10)
    CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value);

    // Function: CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value)
    // Argument: uint8_t *enable   0: Disable, 1: Enable
    //           uint16_t *value   Black level correction value
    // Return value: success / failure (see return code)
    // Description: This function gets black level correction.
    // Format: 16-bit unsigned integer value
    // Initial value: Enabled / 64 (when RAW10)
    CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value);

    // Function: CA378_RESULT CA378_SetDefectPixel(uint8_t enable)
    // Argument: uint8_t enable     Defect correction enabled / disabled
    //              0: Correction disabled
    //              1: Mapped couplet correction enabled
    //              2: Dynamic singlet correction enabled
    //              3: Mapped couplet correction / dynamic singlet correction enabled
    // Return value: success / failure (see return code)
    // Description: This function enables / disables defect correction.
    // Initial value: 3 (mapped couplet correction / dynamic singlet correction enabled)
    CA378_RESULT CA378_SetDefectPixel(uint8_t enable);

    // Function: CA378_RESULT CA378_GetDefectPixel(uint8_t *enable)
    // Argument: uint8_t *enable    Defect correction enabled / disabled
    //              0: Correction disabled
    //              1: Mapped couplet correction enabled
    //              2: Dynamic singlet correction enabled
    //              3: Mapped couplet correction / dynamic singlet correction enabled
    // Return value: success / failure (see return code)
    // Description: This function enables / disables defect correction.
    // Initial value: 3 (mapped couplet correction / dynamic singlet correction enabled)
    // NOTE: Mapped couplet correction: Corrects defective pixels pre-mapped at the factory.
    //       Dynamic singlet correction enabled: Dynamic correction using the values of surrounding pixels.
    CA378_RESULT CA378_GetDefectPixel(uint8_t *enable);

    // Function: CA378_RESULT CA378_SetLensShading(uint8_t table_number)
    // Argument: uint8_t table_number   Shading correction enabled / disabled
    //              0: Shading correction disabled
    //              1-3: Read from specified table, shading correction enabled
    // Return value: success / failure (see return code)
    // Description: This function enables / disables lens shading correction.
    CA378_RESULT CA378_SetLensShading(uint8_t table_number);

    // Function: CA378_RESULT CA378_GetLensShading(uint8_t *table_number)
    // Argument: uint8_t *table_number  Shading correction enabled / disabled
    //              0: Shading correction disabled,
    //              1-3: Selected shading correction table
    // Return value: success / failure (see return code)
    // Description: This function gets the lens shading correction.
    CA378_RESULT CA378_GetLensShading(uint8_t *table_number);

    // Function: CA378_RESULT CA378_SaveEEPROM(uint8_t table_number)
    // Argument: uint8_t table_number   table number (1-15)
    // Return value: success / failure (see return code)
    // Description: This function saves the adjustment value in EEPROM.
    // Adjustment value:
    //          ADDR  (Bytes):
    //          00-01 (2Byte): Exposure value
    //          02-03 (2Byte): Analog gain
    //          04-09 (6Byte): White balance
    //          0A-0C (3Byte): Black level correction
    //          0D    (1Byte): defective pixel correction
    //          0E    (1Byte): Shading correction
    // Number of tables: 16
    CA378_RESULT CA378_SaveEEPROM(uint8_t table_number);

    // Function: CA378_RESULT CA378_LoadEEPROM(uint8_t table_number, uint8_t *table_value)
    // Argument: uint8_t table_number   table number (1-15)
    //           uint16_t *table_value  Adjustment value (16 bytes)
    // Return value: success / failure (see return code)
    // Description: This function reads the adjustment value from the EEPROM.
    // Adjustment value: 15 bytes
    //          ADDR  (Bytes):
    //          00-01 (2Byte): Exposure value
    //          02-03 (2Byte): Analog gain
    //          04-09 (6Byte): White balance
    //          0A-0C (3Byte): Black level correction
    //          0D    (1Byte): defective pixel correction
    //          0E    (1Byte): Shading correction
    // Number of tables: 16
    CA378_RESULT CA378_LoadEEPROM(uint8_t table_number, uint8_t *table_value);

    // Function: CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
    // Argument: uint8_t table_number   Table number (1-3)
    //           uint8_t bayer          Bayer array (0: R, 1: Gr, 2: Gb, 3: B)
    //           uint16_t *table_value  Shading correction value (48 points)
    // Return value: success / failure (see return code)
    // Description: This function saves lens shading correction in EEPROM.
    CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value);

    // Function: CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
    // Argument: uint8_t table_number   Table number (1-3)
    //           uint8_t bayer          Bayer array (0: R, 1: Gr, 2: Gb, 3: B)
    //           uint16_t *table_value  Shading correction value (48 points)
    // Return value: success / failure (see return code)
    // Description: This function reads the lens shading correction value from EEPROM.
    CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value);

    // Function: CA378_RESULT CA378_ClearEEPROM(void)
    // Argument: None
    // Return value: success / failure (see return code)
    // Description: This function clears the adjustment value from EEPROM.
    CA378_RESULT CA378_ClearEEPROM(void);

    // Function: CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number)
    // Argument: uint8_t table_number   Table number
    //           0: Invalid read
    //           1-16: Table number read at startup
    // Return value: success / failure (see return code)
    // Description: This function specifies the table number to be read from EEPROM at startup.
    CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number);

    // Function: CA378_RESULT CA378_EraseSPI(void)
    // Argument: None
    // Return value: success / failure (see return code)
    // Description: This function clears the SPI.
    CA378_RESULT CA378_EraseSPI(void);

    // Function: CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size)
    // Argument: uint8_t *data      Write data
    //           uint32_t size      Size
    // Return value: success / failure (see return code)
    // Description: This feature updates the SPI FW.
    CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size);

    // Function: CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
    // Argument: uint8_t devAddr
    //           uint16_t addr
    //           uint8_t* data
    //           uint8_t size
    // Return value: success / failure (see return code)
    // Description: This function writes a register for debugging.
    CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size);

    // Function: CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
    // Argument: uint8_t devAddr
    //           uint16_t addr
    //           uint8_t* data
    //           uint8_t size
    // Return value: success / failure (see return code)
    // Description: This function reads a register for debugging.
    CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size);

    // Function: CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision)
    // Argument: uint8_t* majorVersion  Major version
    //           uint8_t* minorVersion  Minor version
    //           uint8_t* revision      Revision
    // Return value: success / failure (see return code)
    // Description: This function gets the FW version.
    CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision);

    // Function: CA378_RESULT CA378_SetFocusPosition(uint16_t position)
    // Argument: uint16_t position      Focus position
    // Return value: success / failure (see return code)
    // Description: This function sets the focus position.
    CA378_RESULT CA378_SetFocusPosition(uint16_t position);

    // Function: CA378_RESULT CA378_SetOISMode(uint8_t mode)
    // Argument: uint8_t mode   OIS mode
    // Return value: success / failure (see return code)
    // Description: This function sets the OIS mode.
    CA378_RESULT CA378_SetOISMode(uint8_t mode);

    // Function: CA378_RESULT CA378_GetFocusPosition(uint16_t* position)
    // Argument: uint16_t* position     Focus position
    // Return value: success / failure (see return code)
    // Description: This function gets the focus position.
    CA378_RESULT CA378_GetFocusPosition(uint16_t* position);

    // Function: CA378_RESULT CA378_GetOISMode(uint8_t* mode)
    // Argument: uint8_t* mode  OIS mode
    // Return value: success / failure (see return code)
    // Description: This function gets the OIS mode.
    CA378_RESULT CA378_GetOISMode(uint8_t* mode);
};
