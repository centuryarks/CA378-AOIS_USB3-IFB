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

    // �֐�: CA378_RESULT CA378_AUTO_config(void)
    // ����: �Ȃ�
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́AUVC�Ŏw�肵���𑜓x�̐ݒ�ɏ]���܂��B
    CA378_RESULT CA378_AUTO_config(void);

    // �֐�: CA378_RESULT CA378_12MP_config(void)
    // ����: �Ȃ�
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A9fps�̃t���[�����[�g��12MP�i4056x3040�j�𑜓x�̐ݒ���C���[�W�Z���T�[�ɏ������݂܂��B
    //       ���̋@�\�����s�����ƁA�C���[�W�Z���T��9fps��12MP�̉𑜓x�̃r�f�I�o�͂��J�n���܂��B
    CA378_RESULT CA378_12MP_config(void);

    // �֐�: CA378_RESULT CA378_3MP_config(void)
    // ����: �Ȃ�
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A 30fps�̃t���[�����[�g��3MP�i2028x1520�j�𑜓x�̐ݒ���C���[�W�Z���T�[�ɏ������݂܂��B
    //       ���̋@�\�����s�����ƁA�C���[�W�Z���T��30fps��3MP�̉𑜓x�̃r�f�I�o�͂��J�n���܂��B
    CA378_RESULT CA378_3MP_config(void);

    // �֐�: CA378_RESULT CA378_760P_config(void)
    // ����: �Ȃ�
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A 60fps�̃t���[�����[�g��760P�i1012x760�j�𑜓x�̐ݒ���C���[�W�Z���T�[�ɏ������݂܂��B
    //       ���̋@�\�����s�����ƁA�C���[�W�Z���T��60fps��760P�̉𑜓x�̃r�f�I�o�͂��J�n���܂��B
    CA378_RESULT CA378_760P_config(void);

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
    CA378_RESULT CA378_SetExposure(uint16_t value);

    // �֐�: CA378_RESULT CA378_GetExposure(uint16_t *value)
    // ����: uint16_t *value    �I���l
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����:���̋@�\�́A�d�q�V���b�^�[�̘I�����Ԃ��擾���܂��B
    // �t�H�[�}�b�g: 16 �r�b�g�����Ȃ������l
    // �͈�: 1 - 65515
    CA378_RESULT CA378_GetExposure(uint16_t *value);

    // �֐�: CA378_RESULT CA378_SetAnalogGain(uint16_t value)
    // ����: uint16_t value     �A�i���O�Q�C���l
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A�A�i���O�Q�C���l��ݒ肵�܂��B
    // �t�H�[�}�b�g: 16 �r�b�g�����Ȃ��Œ菬���_Q8.8
    // �͈�: 1.0(0x100) - 22.262(0x1643)
    CA378_RESULT CA378_SetAnalogGain(uint16_t value);

    // �֐�: CA378_RESULT CA378_GetAnalogGain(uint16_t *value)
    // ����: uint16_t *value    �A�i���O�Q�C���l
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����:���̋@�\�́A�A�i���O�Q�C���l���擾���܂��B
    // �t�H�[�}�b�g: 16 �r�b�g�����Ȃ��Œ菬���_Q8.8
    // �͈�: 1.0(0x100) - 22.262 (0x1643)
    CA378_RESULT CA378_GetAnalogGain(uint16_t *value);

    // �֐�: CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value)
    // ����: uint16_t r_value   R�l
    //       uint16_t g_value   G�l
    //       uint16_t b_value   B�l
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A�z���C�g�o�����X�̒l��ݒ肵�܂��B
    // �t�H�[�}�b�g: 16 �r�b�g�����Ȃ��Œ菬���_Q8.8
    // �͈�: 1.0 (0x100) - 15.996 (0xFFF)
    CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value);

    // �֐�: CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value)
    // ����: uint16_t *r_value  R�l
    //       uint16_t *g_value  G�l
    //       uint16_t *b_value  B�l
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����:���̋@�\�́A�z���C�g�o�����X�̒l���擾���܂��B
    // �t�H�[�}�b�g: 16 �r�b�g�����Ȃ��Œ菬���_Q8.8
    // �͈�: 1.0 (0x100) - 15.996 (0xFFF)
    CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value);

    // �֐�: CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value)
    // ����: uint8_t enable     0:�����A1:�L��
    //       uint16_t value     �u���b�N���x���␳�l
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A�u���b�N���x���␳��ݒ肵�܂��B
    // �t�H�[�}�b�g: 16 �r�b�g�����Ȃ������l
    // �����l: �L�� / 64 (RAW10 ��)
    CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value);

    // �֐�: CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value)
    // ����: uint8_t *enable    0:�����A1:�L��
    //       uint16_t *value    �u���b�N���x���␳�l
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����:���̋@�\�́A�u���b�N���x���␳���擾���܂��B
    // �t�H�[�}�b�g: 16 �r�b�g�����Ȃ������l
    // �����l: �L�� / 64 (RAW10 ��)
    CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value);

    // �֐�: CA378_RESULT CA378_SetDefectPixel(uint8_t enable)
    // ����: uint8_t enable     ���ו␳�L��/����
    //       0: �␳����
    //       1: �}�b�v�h�J�v���b�g�␳�L��
    //       2: ���I�V���O���b�g�␳�L��
    //       3: �}�b�v�h�J�v���b�g�␳/���I�V���O���b�g�␳�L��
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A���ו␳�̗L��/������ݒ肵�܂��B
    // �����l: 3 (�}�b�v�h�J�v���b�g�␳/���I�V���O���b�g�␳�L��)
    CA378_RESULT CA378_SetDefectPixel(uint8_t enable);

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
    CA378_RESULT CA378_GetDefectPixel(uint8_t *enable);

    // �֐�: CA378_RESULT CA378_SetLensShading(uint8_t table_number)
    // ����: uint8_t table_number   �V�F�[�f�B���O�␳�L������
    //       0: �V�F�[�f�B���O�␳����
    //       1-3: �w��e�[�u������ǂݏo���A�V�F�[�f�B���O�␳�L��
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A�����Y�V�F�[�f�B���O�␳��L��/�����ɂ��܂��B
    CA378_RESULT CA378_SetLensShading(uint8_t table_number);

    // �֐�: CA378_RESULT CA378_GetLensShading(uint8_t *table_number)
    // ����: uint8_t *table_number  �V�F�[�f�B���O�␳�L������
    //       0: �V�F�[�f�B���O�␳�����A
    //       1-3: �I�𒆂̃V�F�[�f�B���O�␳�̃e�[�u��
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A�����Y�V�F�[�f�B���O�␳���擾���܂��B
    CA378_RESULT CA378_GetLensShading(uint8_t *table_number);

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
    CA378_RESULT CA378_SaveEEPROM(uint8_t table_number);

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
    CA378_RESULT CA378_LoadEEPROM(uint8_t table_number, uint8_t *table_value);

    // �֐�: CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
    // ����: uint8_t table_number   �e�[�u���ԍ�(1-3)
    //       uint8_t bayer          �x�C���z��(0:R, 1:Gr, 2:Gb, 3:B)
    //       uint16_t *table_value  �V�F�[�f�B���O�␳�l(48�_)
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́AEEPROM�Ƀ����Y�V�F�[�f�B���O�␳��ۑ����܂��B
    CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value);

    // �֐�: CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
    // ����: uint8_t table_number   �e�[�u���ԍ�(1-3)
    //       uint8_t bayer          �x�C���z��(0:R, 1:Gr, 2:Gb, 3:B)
    //       uint16_t *table_value  �V�F�[�f�B���O�␳�l(48�_)
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́AEEPROM���烌���Y�V�F�[�f�B���O�␳�l��ǂݏo���܂��B
    CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value);

    // �֐�: CA378_RESULT CA378_ClearEEPROM(void)
    // ����: �Ȃ�
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́AEEPROM ���璲���l���N���A���܂��B
    CA378_RESULT CA378_ClearEEPROM(void);

    // �֐�: CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number)
    // ����: uint8_t table_number   �e�[�u���ԍ�
    //       0: �ǂݏo������
    //       1-16: �N�����ɓǂݏo���e�[�u���ԍ�
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́AEEPROM����N�����ɓǂݏo���e�[�u���ԍ����w�肵�܂��B
    CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number);

    // �֐�: CA378_RESULT CA378_EraseSPI(void)
    // ����: �Ȃ�
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́ASPI���������܂��B
    CA378_RESULT CA378_EraseSPI(void);

    // �֐�: CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size)
    // ����: uint8_t *data          �������݃f�[�^
    //       uint32_t size          �T�C�Y
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́ASPI��FW���A�b�v�f�[�g���܂��B
    CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size);

    // �֐�: CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
    // ����: uint8_t devAddr
    //       uint16_t addr
    //       uint8_t* data
    //       uint8_t size
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A�f�o�b�O�p�Ƀ��W�X�^�������݂��s���܂��B
    CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size);

    // �֐�: CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
    // ����: uint8_t devAddr
    //       uint16_t addr
    //       uint8_t* data
    //       uint8_t size
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́A�f�o�b�O�p�Ƀ��W�X�^�ǂݏo�����s���܂��B
    CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size);

    // �֐�: CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision)
    // ����: uint8_t* majorVersion  ���W���[�o�[�W����
    //       uint8_t* minorVersion  �}�C�i�[�o�[�W����
    //       uint8_t* revision      ���r�W����
    // �Ԃ�l: ����/���s(���^�[���R�[�h�Q��)
    // ����: ���̋@�\�́AFW�̃o�[�W�������擾���܂��B
    CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision);

    // AF/OIS Control
    CA378_RESULT CA378_SetFocusPosition(uint16_t position);
    CA378_RESULT CA378_SetOISMode(uint8_t mode);
    CA378_RESULT CA378_GetFocusPosition(uint16_t* position);
    CA378_RESULT CA378_GetOISMode(uint8_t* mode);
};
