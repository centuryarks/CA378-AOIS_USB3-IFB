// USBCAExtensionUnit.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "USBCAExtensionUnit.h"
#include "ExtensionManager.h"
#include "command.h"


#define EXTENSION_UNIT_NUMBER       (2)     // Extension Unit Number
#define EXTENSION_UNIT_MAX_PACKET   (34)    // Extension Unit Max Packet

ExtensionManager* g_pExtensionManager = NULL;


// This is the constructor of a class that has been exported.
// see USBCAExtensionUnit.h for the class definition
CUSBCAExtensionUnit::CUSBCAExtensionUnit()
{
    if (g_pExtensionManager == NULL)
    {
        g_pExtensionManager = new ExtensionManager();
    }

    return;
}

CUSBCAExtensionUnit::~CUSBCAExtensionUnit(void)
{
    if (g_pExtensionManager)
    {
        delete g_pExtensionManager;
        g_pExtensionManager = NULL;
    }

    return;
}

// transfer extension unit
static int TransferExtensionUnit(uint8_t *buf)
{
    int ret = CA378_SUCCESS;

#if _DEBUG
    printf("  Send: ");
    for (int i = 0; i < buf[0]; i++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\r\n");
#endif

    // send packet
    ret = g_pExtensionManager->SendExtensionUnit(buf,
            EXTENSION_UNIT_MAX_PACKET, EXTENSION_UNIT_NUMBER);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_ERR_SEND | ret);
    }

    // receive packet
    ret = g_pExtensionManager->RecvExtensionUnit(buf,
            EXTENSION_UNIT_MAX_PACKET, EXTENSION_UNIT_NUMBER);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_ERR_RECV | ret);
    }

    if (buf[0] < 2 || buf[0] > EXTENSION_UNIT_MAX_PACKET)
    {
        return CA378_ERR_EXT_UNIT_BAD_SIZE;
    }

#if _DEBUG
    printf("  Recv: ");
    for (int i = 0; i < buf[0]; i++)
    {
        printf("%02X ", buf[i]);
    }
    printf("\r\n");
#endif

    return ret;
}

// Function: CA378_RESULT CA378_AUTO_config(void)
// Argument: None
// Return value: success / failure (see return code)
// Description: This function follows the resolution setting specified in UVC.
CA378_RESULT CUSBCAExtensionUnit::CA378_AUTO_config(void)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = CONFIG_AUTO;               // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_12MP_config(void)
// Argument: None
// Return value: success / failure (see return code)
// Description: This function writes the 12MP (4056x3040) resolution setting of 9fps frame rate to the image sensor.
//              When this function is executed, the image sensor will start to output 12MP resolution video at 9fps.
CA378_RESULT CUSBCAExtensionUnit::CA378_12MP_config(void)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = CONFIG_12M;                // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_3MP_config(void)
// Argument: None
// Return value: success / failure (see return code)
// Description: This function writes the 3MP (2028x1520) resolution setting at 30fps frame rate to the image sensor.
//              When this function is executed, the image sensor starts to output 3MP resolution video at 30fps.
CA378_RESULT CUSBCAExtensionUnit::CA378_3MP_config(void)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = CONFIG_3M;                 // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_760P_config(void)
// Argument: None
// Return value: success / failure (see return code)
// Description: This function writes the 760P (1012x760) resolution setting at 60fps frame rate to the image sensor.
//              When this function is executed, the image sensor starts to output 760P resolution video at 60fps.
CA378_RESULT CUSBCAExtensionUnit::CA378_760P_config(void)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = CONFIG_760P;               // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

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
CA378_RESULT CUSBCAExtensionUnit::CA378_SetExposure(uint16_t value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 4;                         // total packet size
    buf[1] = SET_EXPOSURE;              // Extension unit comamnd
    buf[2] = (uint8_t)(value >> 8);     // value high byte
    buf[3] = (uint8_t)value;            // value low byte

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_GetExposure(uint16_t *value)
// Argument: uint16_t *value   Exposure value
// Return value: success / failure (see return code)
// Description: This function gets the exposure time of the electronic shutter.
// Format: 16-bit unsigned integer value
// Range: 1-65515
CA378_RESULT CUSBCAExtensionUnit::CA378_GetExposure(uint16_t *value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_EXPOSURE;              // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];
    *value     = (buf[2] << 8) + buf[3];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_SetAnalogGain(uint16_t value)
// Argument: uint16_t value     Analog gain value
// Return value: success / failure (see return code)
// Description: This function sets the analog gain value.
// Format: 16-bit unsigned fixed point Q8.8
// Range: 1.0 (0x100)-22.262 (0x1643)
CA378_RESULT CUSBCAExtensionUnit::CA378_SetAnalogGain(uint16_t value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 4;                         // total packet size
    buf[1] = SET_ANALOG_GAIN;           // Extension unit comamnd
    buf[2] = (uint8_t)(value >> 8);     // value high byte
    buf[3] = (uint8_t)value;            // value low byte

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_GetAnalogGain(uint16_t *value)
// Argument: uint16_t *value   Analog gain value
// Return value: success / failure (see return code)
// Description: This function gets the analog gain value.
// Format: 16-bit unsigned fixed point Q8.8
// Range: 1.0 (0x100)-22.262 (0x1643)
CA378_RESULT CUSBCAExtensionUnit::CA378_GetAnalogGain(uint16_t *value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_ANALOG_GAIN;           // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];
    *value     = (buf[2] << 8) + buf[3];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value)
// Argument: uint16_t r_value   R value
//           uint16_t g_value   G value
//           uint16_t b_value   B value
// Return value: success / failure (see return code)
// Description: This function sets the white balance value.
// Format: 16-bit unsigned fixed point Q8.8
// Range: 1.0 (0x100)-15.996 (0xFFF)
CA378_RESULT CUSBCAExtensionUnit::CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 8;                         // total packet size
    buf[1] = SET_WHITE_BALANCE;         // Extension unit comamnd
    buf[2] = (uint8_t)(r_value >> 8);   // r_value high byte
    buf[3] = (uint8_t)r_value;          // r_value low byte
    buf[4] = (uint8_t)(g_value >> 8);   // g_value high byte
    buf[5] = (uint8_t)g_value;          // g_value low byte
    buf[6] = (uint8_t)(b_value >> 8);   // b_value high byte
    buf[7] = (uint8_t)b_value;          // b_value low byte

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}


// Function: CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value)
// Argument: uint16_t *r_value     R value
//           uint16_t *g_value     G value
//           uint16_t *b_value     B value
// Return value: success / failure (see return code)
// Description: This function gets the white balance value.
// Format: 16-bit unsigned fixed point Q8.8
// Range: 1.0 (0x100)-15.996 (0xFFF)
CA378_RESULT CUSBCAExtensionUnit::CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_WHITE_BALANCE;         // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];
    *r_value   = (buf[2] << 8) + buf[3];
    *g_value   = (buf[4] << 8) + buf[5];
    *b_value   = (buf[6] << 8) + buf[7];

    return (CA378_RESULT)recvResult;
}


// Function: CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value)
// Argument: uint8_t enable     0: Disable, 1: Enable
//           uint16_t value     Black level correction value
// Return value: success / failure (see return code)
// Description: This function sets the black level correction.
// Format: 16-bit unsigned integer value
// Initial value: Enabled / 64 (when RAW10)
CA378_RESULT CUSBCAExtensionUnit::CA378_SetBlackLevel(uint8_t enable, uint16_t value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 5;                         // total packet size
    buf[1] = SET_BLACK_LEVEL;           // Extension unit comamnd
    buf[2] = enable;                    // enable
    buf[3] = (uint8_t)(value >> 8);     // value high byte
    buf[4] = (uint8_t)value;            // value low byte

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value)
// Argument: uint8_t *enable   0: Disable, 1: Enable
//           uint16_t *value   Black level correction value
// Return value: success / failure (see return code)
// Description: This function gets black level correction.
// Format: 16-bit unsigned integer value
// Initial value: Enabled / 64 (when RAW10)
CA378_RESULT CUSBCAExtensionUnit::CA378_GetBlackLevel(uint8_t *enable, uint16_t *value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_BLACK_LEVEL;           // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];
    *enable    = buf[2];
    *value     = (buf[3] << 8) + buf[4];

    return (CA378_RESULT)recvResult;
}


// Function: CA378_RESULT CA378_SetDefectPixel(uint8_t enable)
// Argument: uint8_t enable     Defect correction enabled / disabled
//              0: Correction disabled
//              1: Mapped couplet correction enabled
//              2: Dynamic singlet correction enabled
//              3: Mapped couplet correction / dynamic singlet correction enabled
// Return value: success / failure (see return code)
// Description: This function enables / disables defect correction.
// Initial value: 3 (mapped couplet correction / dynamic singlet correction enabled)
CA378_RESULT CUSBCAExtensionUnit::CA378_SetDefectPixel(uint8_t enable)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 3;                         // total packet size
    buf[1] = SET_DEFECT_PIXEL;          // Extension unit comamnd
    buf[2] = enable;                    // enable

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

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
CA378_RESULT CUSBCAExtensionUnit::CA378_GetDefectPixel(uint8_t *enable)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_DEFECT_PIXEL;          // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];
    *enable    = buf[2];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_SetLensShading(uint8_t table_number)
// Argument: uint8_t table_number   Shading correction enabled / disabled
//              0: Shading correction disabled
//              1-3: Read from specified table, shading correction enabled
// Return value: success / failure (see return code)
// Description: This function enables / disables lens shading correction.
CA378_RESULT CUSBCAExtensionUnit::CA378_SetLensShading(uint8_t table_number)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 3;                         // total packet size
    buf[1] = SET_LENS_SHADING;          // Extension unit comamnd
    buf[2] = table_number;              // table number

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}


// Function: CA378_RESULT CA378_GetLensShading(uint8_t *table_number)
// Argument: uint8_t *table_number  Shading correction enabled / disabled
//              0: Shading correction disabled,
//              1-3: Selected shading correction table
// Return value: success / failure (see return code)
// Description: This function gets the lens shading correction.
CA378_RESULT CUSBCAExtensionUnit::CA378_GetLensShading(uint8_t *table_number)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_LENS_SHADING;          // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize      = buf[0];
    recvResult    = buf[1];
    *table_number = buf[2];

    return (CA378_RESULT)recvResult;
}


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
CA378_RESULT CUSBCAExtensionUnit::CA378_SaveEEPROM(uint8_t table_number)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 3;                         // total packet size
    buf[1] = EEPROM_SAVE;               // Extension unit comamnd
    buf[2] = table_number;              // table number

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

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
CA378_RESULT CUSBCAExtensionUnit::CA378_LoadEEPROM(uint8_t table_number, uint8_t *table_value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 4;                         // total packet size
    buf[1] = EEPROM_LOAD;               // Extension unit comamnd
    buf[2] = table_number;              // table number
    buf[3] = 15;                        // load size;

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    if (recvResult == 0)
    {
        // store data
        for (int n = 0; n < recvSize - 2; n++)
        {
            table_value[n] = buf[n + 2];
        }
    }
    else
    {
        return (CA378_RESULT)recvResult;
    }

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
// Argument: uint8_t table_number   Table number (1-3)
//           uint8_t bayer          Bayer array (0: R, 1: Gr, 2: Gb, 3: B)
//           uint16_t *table_value  Shading correction value (48 points)
// Return value: success / failure (see return code)
// Description: This function saves lens shading correction in EEPROM.
CA378_RESULT CUSBCAExtensionUnit::CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;
    int index = 0;
    int size = 16;

    // transfer every 16 bytes
    for (index = 0; index < 48 * 2; index += size)
    {
        // send receive request
        buf[0] = size + 6;              // total packet size
        buf[1] = EEPROM_SAVE_LSC;       // Extension unit comamnd
        buf[2] = table_number;          // table number
        buf[3] = bayer;                 // bayer
        buf[4] = index;                 // index
        buf[5] = size;                  // request size

        // set write data
        for (int n = 0; n < size; n++)
        {
            buf[n + 6] = table_value[index + n];
        }

        // transfer extension unit
        ret = TransferExtensionUnit(buf);
        if (ret != CA378_SUCCESS)
        {
            return (CA378_RESULT)ret;
        }

        // set result
        recvSize   = buf[0];
        recvResult = buf[1];

        if (recvResult != 0)
        {
            return (CA378_RESULT)recvResult;
        }
    }

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
// Argument: uint8_t table_number   Table number (1-3)
//           uint8_t bayer          Bayer array (0: R, 1: Gr, 2: Gb, 3: B)
//           uint16_t *table_value  Shading correction value (48 points)
// Return value: success / failure (see return code)
// Description: This function reads the lens shading correction value from EEPROM.
CA378_RESULT CUSBCAExtensionUnit::CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;
    int index = 0;
    int size = 16;

    // transfer every 16 bytes
    for (index = 0; index < 48 * 2; index += size)
    {
        // send receive request
        buf[0] = 6;                     // total packet size
        buf[1] = EEPROM_LOAD_LSC;       // Extension unit comamnd
        buf[2] = table_number;          // table number
        buf[3] = bayer;                 // bayer
        buf[4] = index;                 // index
        buf[5] = size;                  // request size

        // transfer extension unit
        ret = TransferExtensionUnit(buf);
        if (ret != CA378_SUCCESS)
        {
            return (CA378_RESULT)ret;
        }

        // set result
        recvSize   = buf[0];
        recvResult = buf[1];

        if (recvResult == 0)
        {
            // store data
            for (int n = 0; n < recvSize - 2; n++)
            {
                table_value[index + n] = buf[n + 2];
            }
        }
        else
        {
            return (CA378_RESULT)recvResult;
        }
    }

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_ClearEEPROM(void)
// Argument: None
// Return value: success / failure (see return code)
// Description: This function clears the adjustment value from EEPROM.
CA378_RESULT CUSBCAExtensionUnit::CA378_ClearEEPROM(void)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = EEPROM_CLEAR;              // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number)
// Argument: uint8_t table_number   Table number
//           0: Invalid read
//           1-16: Table number read at startup
// Return value: success / failure (see return code)
// Description: This function specifies the table number to be read from EEPROM at startup.
CA378_RESULT CUSBCAExtensionUnit::CA378_DefaultEEPROM(uint8_t table_number)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 3;                         // total packet size
    buf[1] = EEPROM_DEFAULT;            // Extension unit comamnd
    buf[2] = table_number;              // table number

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_EraseSPI(void)
// Argument: None
// Return value: success / failure (see return code)
// Description: This function clears the SPI.
CA378_RESULT CUSBCAExtensionUnit::CA378_EraseSPI(void)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = SPI_ERASE;                 // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size)
// Argument: uint8_t *data      Write data
//           uint32_t size      Size
// Return value: success / failure (see return code)
// Description: This feature updates the SPI FW.
CA378_RESULT CUSBCAExtensionUnit::CA378_UpdateSPI(uint8_t *data, uint32_t size)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int pageSize = 0x100;
    int tranSize = 0x10;

    printf("FW erasing...\r\n");

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = SPI_ERASE;                 // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // Update one page (256 bytes) at a time
    for (uint32_t addr = 0; addr < (size / pageSize); addr++)
    {
        // transfer every 16 bytes
        for (int index = 0; index < pageSize; index += tranSize)
        {
            // send receive request
            buf[0] = 5 + tranSize;              // total packet size
            buf[1] = SPI_UPDATE;                // Extension unit comamnd
            buf[2] = (uint8_t)(addr >> 8);      // address high byte
            buf[3] = (uint8_t)addr;             // address low byte
            buf[4] = index;

            for (int i = 0; i < tranSize; i++)
            {
                buf[5 + i] = data[addr * pageSize + index + i];
            }

            // transfer extension unit
            ret = TransferExtensionUnit(buf);
            if (ret != CA378_SUCCESS)
            {
                return (CA378_RESULT)ret;
            }
        }

        // print progress
        printf("FW updating... %6d / %6d ", (addr * pageSize + pageSize), size);
        int percent = 32 * (addr * pageSize + pageSize) / size;
        for (int n = 0; n < percent; n++)
        {
            printf("*");
        }
        for (int n = 0; n < 32 - percent; n++)
        {
            printf("-");
        }
        printf("\r");
    }
    printf("\r\n");

    return CA378_SUCCESS;
}

// Function: CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
// Argument: uint8_t devAddr
//           uint16_t addr
//           uint8_t* data
//           uint8_t size
// Return value: success / failure (see return code)
// Description: This function writes a register for debugging.
CA378_RESULT CUSBCAExtensionUnit::CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = size + 5;                  // total packet size
    buf[1] = REGISTER_WRITE;            // Extension unit comamnd
    buf[2] = (uint8_t)devAddr;          // device address (slave address)
    buf[3] = (uint8_t)(addr >> 8);      // register address high byte
    buf[4] = (uint8_t)addr;             // register address low byte

    // set write data
    for (int i = 0; i < size; i++)
    {
        buf[i + 5] = data[i];
    }

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
// Argument: uint8_t devAddr
//           uint16_t addr
//           uint8_t* data
//           uint8_t size
// Return value: success / failure (see return code)
// Description: This function reads a register for debugging.
CA378_RESULT CUSBCAExtensionUnit::CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 6;                         // total packet size
    buf[1] = REGISTER_READ;             // Extension unit comamnd
    buf[2] = (uint8_t)devAddr;          // device address (slave address)
    buf[3] = (uint8_t)(addr >> 8);      // register address high byte
    buf[4] = (uint8_t)addr;             // register address low byte
    buf[5] = size;                      // read size

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    // store data
    for (int i = 0; i < recvSize - 2; i++)
    {
        data[i] = buf[i + 2];
    }

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision)
// Argument: uint8_t* majorVersion  Major version
//           uint8_t* minorVersion  Minor version
//           uint8_t* revision      Revision
// Return value: success / failure (see return code)
// Description: This function gets the FW version.
CA378_RESULT CUSBCAExtensionUnit::CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_FW_VERSION;               // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize      = buf[0];
    recvResult    = buf[1];
    *majorVersion = buf[2];
    *minorVersion = buf[3];
    *revision     = buf[4];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_SetFocusPosition(uint16_t position)
// Argument: uint16_t position      Focus position
// Return value: success / failure (see return code)
// Description: This function sets the focus position.
CA378_RESULT CUSBCAExtensionUnit::CA378_SetFocusPosition(uint16_t position)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 4;                         // total packet size
    buf[1] = SET_FOCUS_POSITION;        // Extension unit comamnd
    buf[2] = (uint8_t)(position >> 8);  // focus position high byte
    buf[3] = (uint8_t)position;         // focus position low byte

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_SetOISMode(uint8_t mode)
// Argument: uint8_t mode   OIS mode
// Return value: success / failure (see return code)
// Description: This function sets the OIS mode.
CA378_RESULT CUSBCAExtensionUnit::CA378_SetOISMode(uint8_t mode)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 3;                         // total packet size
    buf[1] = SET_OIS_MODE;              // Extension unit comamnd
    buf[2] = mode;                      // OIS mode

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_GetFocusPosition(uint16_t* position)
// Argument: uint16_t* position     Focus position
// Return value: success / failure (see return code)
// Description: This function gets the focus position.
CA378_RESULT CUSBCAExtensionUnit::CA378_GetFocusPosition(uint16_t* position)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_FOCUS_POSITION;        // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];
    *position  = (buf[2] << 8) + buf[3];

    return (CA378_RESULT)recvResult;
}

// Function: CA378_RESULT CA378_GetOISMode(uint8_t* mode)
// Argument: uint8_t* mode  OIS mode
// Return value: success / failure (see return code)
// Description: This function gets the OIS mode.
CA378_RESULT CUSBCAExtensionUnit::CA378_GetOISMode(uint8_t* mode)
{
    uint8_t buf[64] = { 0 };
    int ret;
    int recvSize;
    int recvResult;

    // send receive request
    buf[0] = 2;                         // total packet size
    buf[1] = GET_OIS_MODE;              // Extension unit comamnd

    // transfer extension unit
    ret = TransferExtensionUnit(buf);
    if (ret != CA378_SUCCESS)
    {
        return (CA378_RESULT)ret;
    }

    // set result
    recvSize   = buf[0];
    recvResult = buf[1];
    *mode      = buf[2];

    return (CA378_RESULT)recvResult;
}
