#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// ���^�[���R�[�h�ꗗ:
typedef enum E_CA378_RESULT
{
    CA378_SUCCESS                   = 0x00,     // ��{�֐��̎��s�ɐ���
    CA378_ERR_NOMEM                 = 0x01,     // �������m�ۂɎ��s����
    CA378_ERR_THREAD                = 0x02,     // �X���b�h�̍쐬�Ɏ��s����
    CA378_ERR_EVENT                 = 0x03,     // �C�x���g�̍쐬�Ɏ��s����
    CA378_ERR_FATAL                 = 0x04,     // �v���I�G���[����������
    CA378_ERR_SYSTEM_INIT           = 0x05,     // �V�X�e���̏������Ɏ��s����
    CA378_ERR_CX3_INIT              = 0x06,     // CX3�f�o�C�X�̏������Ɏ��s����
    CA378_ERR_CAMERA_MIPI           = 0x07,     // �J����MIPI�ʐM�Ŏ��s����
    CA378_ERR_CAMERA_SETUP          = 0x08,     // �J�������W���[���̃Z�b�g�A�b�v�Ɏ��s����
    CA378_ERR_PARAM                 = 0x09,     // �p�����[�^�w�肪�Ԉ���Ă���
    CA378_ERR_SENSOR_IF             = 0x0A,     // Image Sensor �̒ʐM�ŃG���[����
    CA378_ERR_EEPROM_IF             = 0x0B,     // EEPROM �̒ʐM�ŃG���[����
    CA378_ERR_EEPROM_NOT_SET        = 0x0C,     // EEPROM �ɒl���Z�b�g����ĂȂ�
    CA378_ERR_SPI_IF                = 0x0D,     // SPI �̒ʐM�ŃG���[����
    CA378_ERR_NOT_USE_USBSPEED      = 0x0E,     // ���݂�USB���x(USB2.0 or USB1.0)�ł͎g�p�ł��Ȃ�
    CA378_ERR_EXT_UNIT_NOT_FOUND    = 0x10,     // Extention Unit ��������Ȃ�
    CA378_ERR_EXT_UNIT_FAIL         = 0x11,     // Extention Unit �ʐM�Ɏ��s
    CA378_ERR_EXT_UNIT_BAD_SIZE     = 0x12,     // Extention Unit �ʐM�ŃT�C�Y���s��
    CA378_ERR_EXT_UNIT_UNKNOWN_CMD  = 0x13,     // Extention Unit �ʐM�ŕs���ȃR�}���h
    CA378_ERR_EXT_UNIT_TIMEOUT      = 0x14,     // Extention Unit �ʐM�Ń^�C���A�E�g
    CA378_ERR_SEND                  = 0x20,     // ���M�G���[ + 0x20
    CA378_ERR_RECV                  = 0x40,     // ��M�G���[ + 0x40
} CA378_RESULT;

#endif
