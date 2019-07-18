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

// �֐�: CA378_RESULT CA378_AUTO_config(void)
// ����: �Ȃ�
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́AUVC�Ŏw�肵���𑜓x�̐ݒ�ɏ]���܂��B
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

// �֐�: CA378_RESULT CA378_12MP_config(void)
// ����: �Ȃ�
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A9fps�̃t���[�����[�g��12MP�i4056x3040�j�𑜓x�̐ݒ���C���[�W�Z���T�[�ɏ������݂܂��B
//       ���̋@�\�����s�����ƁA�C���[�W�Z���T��9fps��12MP�̉𑜓x�̃r�f�I�o�͂��J�n���܂��B
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

// �֐�: CA378_RESULT CA378_3MP_config(void)
// ����: �Ȃ�
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A 30fps�̃t���[�����[�g��3MP�i2028x1520�j�𑜓x�̐ݒ���C���[�W�Z���T�[�ɏ������݂܂��B
//       ���̋@�\�����s�����ƁA�C���[�W�Z���T��30fps��3MP�̉𑜓x�̃r�f�I�o�͂��J�n���܂��B
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

// �֐�: CA378_RESULT CA378_760P_config(void)
// ����: �Ȃ�
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A 60fps�̃t���[�����[�g��760P�i1012x760�j�𑜓x�̐ݒ���C���[�W�Z���T�[�ɏ������݂܂��B
//       ���̋@�\�����s�����ƁA�C���[�W�Z���T��60fps��760P�̉𑜓x�̃r�f�I�o�͂��J�n���܂��B
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

// �֐�: CA378_RESULT CA378_SetExposure(uint16_t value)
// ����: uint16_t value     �I���l
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A�d�q�V���b�^�[�̘I�����Ԃ�ݒ肵�܂��B
//       3M�i2028x1520�j��760P�i1012x760�j�𑜓x�ł�1580��1/60�b�ɑ������܂��B
//           �I�����ԁi�b�j= (value + 20) / 96000
//       12M�i4056x3040�j�𑜓x�ł�1580��1/30�b�ɑ������܂��B
//           �I�����ԁi�b�j= (value + 20) / 48000
// �t�H�[�}�b�g: 16 �r�b�g�����Ȃ������l
// �͈�: 1 - 65515
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

// �֐�: CA378_RESULT CA378_GetExposure(uint16_t *value)
// ����: uint16_t *value    �I���l
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����:���̋@�\�́A�d�q�V���b�^�[�̘I�����Ԃ��擾���܂��B
// �t�H�[�}�b�g: 16 �r�b�g�����Ȃ������l
// �͈�: 1 - 65515
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

// �֐�: CA378_RESULT CA378_SetAnalogGain(uint16_t value)
// ����: uint16_t value     �A�i���O�Q�C���l
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A�A�i���O�Q�C���l��ݒ肵�܂��B
// �t�H�[�}�b�g: 16 �r�b�g�����Ȃ��Œ菬���_Q8.8
// �͈�: 1.0(0x100) - 22.262(0x1643)
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

// �֐�: CA378_RESULT CA378_GetAnalogGain(uint16_t *value)
// ����: uint16_t *value    �A�i���O�Q�C���l
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����:���̋@�\�́A�A�i���O�Q�C���l���擾���܂��B
// �t�H�[�}�b�g: 16 �r�b�g�����Ȃ��Œ菬���_Q8.8
// �͈�: 1.0(0x100) - 22.262 (0x1643)
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

// �֐�: CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value)
// ����: uint16_t r_value   R�l
//       uint16_t g_value   G�l
//       uint16_t b_value   B�l
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A�z���C�g�o�����X�̒l��ݒ肵�܂��B
// �t�H�[�}�b�g: 16 �r�b�g�����Ȃ��Œ菬���_Q8.8
// �͈�: 1.0 (0x100) - 15.996 (0xFFF)
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


// �֐�: CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value)
// ����: uint16_t *r_value  R�l
//       uint16_t *g_value  G�l
//       uint16_t *b_value  B�l
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����:���̋@�\�́A�z���C�g�o�����X�̒l���擾���܂��B
// �t�H�[�}�b�g: 16 �r�b�g�����Ȃ��Œ菬���_Q8.8
// �͈�: 1.0 (0x100) - 15.996 (0xFFF)
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


// �֐�: CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value)
// ����: uint8_t enable     0:�����A1:�L��
//       uint16_t value     �u���b�N���x���␳�l
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A�u���b�N���x���␳��ݒ肵�܂��B
// �t�H�[�}�b�g: 16 �r�b�g�����Ȃ������l
// �����l: �L�� / 64 (RAW10 ��)
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

// �֐�: CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value)
// ����: uint8_t *enable    0:�����A1:�L��
//       uint16_t *value    �u���b�N���x���␳�l
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����:���̋@�\�́A�u���b�N���x���␳���擾���܂��B
// �t�H�[�}�b�g: 16 �r�b�g�����Ȃ������l
// �����l: �L�� / 64 (RAW10 ��)
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


// �֐�: CA378_RESULT CA378_SetDefectPixel(uint8_t enable)
// ����: uint8_t enable     ���ו␳�L��/����
//       0: �␳����
//       1: �}�b�v�h�J�v���b�g�␳�L��
//       2: ���I�V���O���b�g�␳�L��
//       3: �}�b�v�h�J�v���b�g�␳/���I�V���O���b�g�␳�L��
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A���ו␳�̗L��/������ݒ肵�܂��B
// �����l: 3 (�}�b�v�h�J�v���b�g�␳/���I�V���O���b�g�␳�L��)
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

// �֐�: CA378_RESULT CA378_GetDefectPixel(uint8_t *enable)
// ����: uint8_t *enable    ���ו␳�L��/����
//       0: �␳����
//       1: �}�b�v�h�J�v���b�g�␳�L��
//       2: ���I�V���O���b�g�␳�L��
//       3: �}�b�v�h�J�v���b�g�␳/���I�V���O���b�g�␳�L�� 
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A���ו␳�̗L��/������ݒ肵�܂��B
// �����l: 3 (�}�b�v�h�J�v���b�g�␳/���I�V���O���b�g�␳�L��)
// �⑫: �}�b�v�h�J�v���b�g�␳: �H��o�׎��ɂ��炩���߃}�b�s���O���ꂽ���׃s�N�Z����␳���܂��B
//       ���I�V���O���b�g�␳�L��: ���I�Ɏ��Ӄs�N�Z���̒l���g�p���ĕ␳���܂��B
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

// �֐�: CA378_RESULT CA378_SetLensShading(uint8_t table_number)
// ����: uint8_t table_number   �V�F�[�f�B���O�␳�L������
//       0: �V�F�[�f�B���O�␳����
//       1-3: �w��e�[�u������ǂݏo���A�V�F�[�f�B���O�␳�L��
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A�����Y�V�F�[�f�B���O�␳��L��/�����ɂ��܂��B
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


// �֐�: CA378_RESULT CA378_GetLensShading(uint8_t *table_number)
// ����: uint8_t *table_number  �V�F�[�f�B���O�␳�L������
//       0: �V�F�[�f�B���O�␳�����A
//       1-3: �I�𒆂̃V�F�[�f�B���O�␳�̃e�[�u��
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A�����Y�V�F�[�f�B���O�␳���擾���܂��B
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


// �֐�: CA378_RESULT CA378_SaveEEPROM(uint8_t table_number)
// ����: uint8_t table      �e�[�u���ԍ�(1-15)
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́AEEPROM �ɒ����l��ۑ����܂��B
// �����l:
//         ADDR  (Bytes):
//         00-01 (2Byte): �I���l
//         02-03 (2Byte): �A�i���O�Q�C��
//         04-09 (6Byte): �z���C�g�o�����X
//         0A-0C (3Byte): �u���b�N���x���␳
//         0D    (1Byte): ���׃s�N�Z���␳
//         0E    (1Byte): �V�F�[�f�B���O�␳
// �e�[�u����: 16
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

// �֐�: CA378_RESULT CA378_LoadEEPROM(uint8_t table_number, uint8_t *table_value)
// ����: uint8_t table          �e�[�u���ԍ�(1-15)
//       uint16_t *table_value  �����l(16�o�C�g)
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́AEEPROM ���璲���l��ǂݏo���܂��B
// �����l: 15�o�C�g
//         ADDR  (Bytes):
//         00-01 (2Byte): �I���l
//         02-03 (2Byte): �A�i���O�Q�C��
//         04-09 (6Byte): �z���C�g�o�����X
//         0A-0C (3Byte): �u���b�N���x���␳
//         0D    (1Byte): ���׃s�N�Z���␳
//         0E    (1Byte): �V�F�[�f�B���O�␳
// �e�[�u����: 16
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

// �֐�: CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
// ����: uint8_t table_number   �e�[�u���ԍ�(1-3)
//       uint8_t bayer          �x�C���z��(0:R, 1:Gr, 2:Gb, 3:B)
//       uint16_t *table_value  �V�F�[�f�B���O�␳�l(48�_)
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́AEEPROM�Ƀ����Y�V�F�[�f�B���O�␳��ۑ����܂��B
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

// �֐�: CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
// ����: uint8_t table_number   �e�[�u���ԍ�(1-3)
//       uint8_t bayer          �x�C���z��(0:R, 1:Gr, 2:Gb, 3:B)
//       uint16_t *table_value  �V�F�[�f�B���O�␳�l(48�_)
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́AEEPROM���烌���Y�V�F�[�f�B���O�␳�l��ǂݏo���܂��B
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

// �֐�: CA378_RESULT CA378_ClearEEPROM(void)
// ����: �Ȃ�
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́AEEPROM ���璲���l���N���A���܂��B
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

// �֐�: CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number)
// ����: uint8_t table_number   �e�[�u���ԍ�
//       0: �ǂݏo������
//       1-16: �N�����ɓǂݏo���e�[�u���ԍ�
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́AEEPROM����N�����ɓǂݏo���e�[�u���ԍ����w�肵�܂��B
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

// �֐�: CA378_RESULT CA378_EraseSPI(void)
// ����: �Ȃ�
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́ASPI���������܂��B
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

// �֐�: CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size)
// ����: uint8_t *data          �������݃f�[�^
//       uint32_t size          �T�C�Y
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́ASPI��FW���A�b�v�f�[�g���܂��B
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

// �֐�: CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
// ����: uint8_t devAddr
//       uint16_t addr
//       uint8_t* data
//       uint8_t size
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A�f�o�b�O�p�Ƀ��W�X�^�������݂��s���܂��B
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

// �֐�: CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
// ����: uint8_t devAddr
//       uint16_t addr
//       uint8_t* data
//       uint8_t size
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́A�f�o�b�O�p�Ƀ��W�X�^�ǂݏo�����s���܂��B
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

// �֐�: CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision)
// ����: uint8_t* majorVersion  ���W���[�o�[�W����
//       uint8_t* minorVersion  �}�C�i�[�o�[�W����
//       uint8_t* revision      ���r�W����
// �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
// ����: ���̋@�\�́AFW�̃o�[�W�������擾���܂��B
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

// Set Focus Position
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

// Set OIS Mode
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

// Get Focus Position
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

// Get OIS Mode
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
