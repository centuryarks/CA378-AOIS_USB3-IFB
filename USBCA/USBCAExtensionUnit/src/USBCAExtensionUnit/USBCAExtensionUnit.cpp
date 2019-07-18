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

// 関数: CA378_RESULT CA378_AUTO_config(void)
// 引数: なし
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、UVCで指定した解像度の設定に従います。
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

// 関数: CA378_RESULT CA378_12MP_config(void)
// 引数: なし
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、9fpsのフレームレートの12MP（4056x3040）解像度の設定をイメージセンサーに書き込みます。
//       この機能が実行されると、イメージセンサは9fpsで12MPの解像度のビデオ出力を開始します。
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

// 関数: CA378_RESULT CA378_3MP_config(void)
// 引数: なし
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、 30fpsのフレームレートの3MP（2028x1520）解像度の設定をイメージセンサーに書き込みます。
//       この機能が実行されると、イメージセンサは30fpsで3MPの解像度のビデオ出力を開始します。
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

// 関数: CA378_RESULT CA378_760P_config(void)
// 引数: なし
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、 60fpsのフレームレートの760P（1012x760）解像度の設定をイメージセンサーに書き込みます。
//       この機能が実行されると、イメージセンサは60fpsで760Pの解像度のビデオ出力を開始します。
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

// 関数: CA378_RESULT CA378_SetExposure(uint16_t value)
// 引数: uint16_t value     露光値
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、電子シャッターの露光時間を設定します。
//       3M（2028x1520）と760P（1012x760）解像度では1580で1/60秒に相当します。
//           露光時間（秒）= (value + 20) / 96000
//       12M（4056x3040）解像度では1580で1/30秒に相当します。
//           露光時間（秒）= (value + 20) / 48000
// フォーマット: 16 ビット符号なし整数値
// 範囲: 1 - 65515
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

// 関数: CA378_RESULT CA378_GetExposure(uint16_t *value)
// 引数: uint16_t *value    露光値
// 返り値: 成功/失敗(リターンコード参照)
// 説明:この機能は、電子シャッターの露光時間を取得します。
// フォーマット: 16 ビット符号なし整数値
// 範囲: 1 - 65515
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

// 関数: CA378_RESULT CA378_SetAnalogGain(uint16_t value)
// 引数: uint16_t value     アナログゲイン値
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、アナログゲイン値を設定します。
// フォーマット: 16 ビット符号なし固定小数点Q8.8
// 範囲: 1.0(0x100) - 22.262(0x1643)
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

// 関数: CA378_RESULT CA378_GetAnalogGain(uint16_t *value)
// 引数: uint16_t *value    アナログゲイン値
// 返り値: 成功/失敗(リターンコード参照)
// 説明:この機能は、アナログゲイン値を取得します。
// フォーマット: 16 ビット符号なし固定小数点Q8.8
// 範囲: 1.0(0x100) - 22.262 (0x1643)
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

// 関数: CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value)
// 引数: uint16_t r_value   R値
//       uint16_t g_value   G値
//       uint16_t b_value   B値
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、ホワイトバランスの値を設定します。
// フォーマット: 16 ビット符号なし固定小数点Q8.8
// 範囲: 1.0 (0x100) - 15.996 (0xFFF)
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


// 関数: CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value)
// 引数: uint16_t *r_value  R値
//       uint16_t *g_value  G値
//       uint16_t *b_value  B値
// 返り値: 成功/失敗(リターンコード参照)
// 説明:この機能は、ホワイトバランスの値を取得します。
// フォーマット: 16 ビット符号なし固定小数点Q8.8
// 範囲: 1.0 (0x100) - 15.996 (0xFFF)
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


// 関数: CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value)
// 引数: uint8_t enable     0:無効、1:有効
//       uint16_t value     ブラックレベル補正値
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、ブラックレベル補正を設定します。
// フォーマット: 16 ビット符号なし整数値
// 初期値: 有効 / 64 (RAW10 時)
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

// 関数: CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value)
// 引数: uint8_t *enable    0:無効、1:有効
//       uint16_t *value    ブラックレベル補正値
// 返り値: 成功/失敗(リターンコード参照)
// 説明:この機能は、ブラックレベル補正を取得します。
// フォーマット: 16 ビット符号なし整数値
// 初期値: 有効 / 64 (RAW10 時)
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


// 関数: CA378_RESULT CA378_SetDefectPixel(uint8_t enable)
// 引数: uint8_t enable     欠陥補正有効/無効
//       0: 補正無効
//       1: マップドカプレット補正有効
//       2: 動的シングレット補正有効
//       3: マップドカプレット補正/動的シングレット補正有効
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、欠陥補正の有効/無効を設定します。
// 初期値: 3 (マップドカプレット補正/動的シングレット補正有効)
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

// 関数: CA378_RESULT CA378_GetDefectPixel(uint8_t *enable)
// 引数: uint8_t *enable    欠陥補正有効/無効
//       0: 補正無効
//       1: マップドカプレット補正有効
//       2: 動的シングレット補正有効
//       3: マップドカプレット補正/動的シングレット補正有効 
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、欠陥補正の有効/無効を設定します。
// 初期値: 3 (マップドカプレット補正/動的シングレット補正有効)
// 補足: マップドカプレット補正: 工場出荷時にあらかじめマッピングされた欠陥ピクセルを補正します。
//       動的シングレット補正有効: 動的に周辺ピクセルの値を使用して補正します。
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

// 関数: CA378_RESULT CA378_SetLensShading(uint8_t table_number)
// 引数: uint8_t table_number   シェーディング補正有効無効
//       0: シェーディング補正無効
//       1-3: 指定テーブルから読み出し、シェーディング補正有効
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、レンズシェーディング補正を有効/無効にします。
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


// 関数: CA378_RESULT CA378_GetLensShading(uint8_t *table_number)
// 引数: uint8_t *table_number  シェーディング補正有効無効
//       0: シェーディング補正無効、
//       1-3: 選択中のシェーディング補正のテーブル
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、レンズシェーディング補正を取得します。
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


// 関数: CA378_RESULT CA378_SaveEEPROM(uint8_t table_number)
// 引数: uint8_t table      テーブル番号(1-15)
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、EEPROM に調整値を保存します。
// 調整値:
//         ADDR  (Bytes):
//         00-01 (2Byte): 露光値
//         02-03 (2Byte): アナログゲイン
//         04-09 (6Byte): ホワイトバランス
//         0A-0C (3Byte): ブラックレベル補正
//         0D    (1Byte): 欠陥ピクセル補正
//         0E    (1Byte): シェーディング補正
// テーブル数: 16
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

// 関数: CA378_RESULT CA378_LoadEEPROM(uint8_t table_number, uint8_t *table_value)
// 引数: uint8_t table          テーブル番号(1-15)
//       uint16_t *table_value  調整値(16バイト)
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、EEPROM から調整値を読み出します。
// 調整値: 15バイト
//         ADDR  (Bytes):
//         00-01 (2Byte): 露光値
//         02-03 (2Byte): アナログゲイン
//         04-09 (6Byte): ホワイトバランス
//         0A-0C (3Byte): ブラックレベル補正
//         0D    (1Byte): 欠陥ピクセル補正
//         0E    (1Byte): シェーディング補正
// テーブル数: 16
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

// 関数: CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
// 引数: uint8_t table_number   テーブル番号(1-3)
//       uint8_t bayer          ベイヤ配列(0:R, 1:Gr, 2:Gb, 3:B)
//       uint16_t *table_value  シェーディング補正値(48点)
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、EEPROMにレンズシェーディング補正を保存します。
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

// 関数: CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
// 引数: uint8_t table_number   テーブル番号(1-3)
//       uint8_t bayer          ベイヤ配列(0:R, 1:Gr, 2:Gb, 3:B)
//       uint16_t *table_value  シェーディング補正値(48点)
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、EEPROMからレンズシェーディング補正値を読み出します。
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

// 関数: CA378_RESULT CA378_ClearEEPROM(void)
// 引数: なし
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、EEPROM から調整値をクリアします。
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

// 関数: CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number)
// 引数: uint8_t table_number   テーブル番号
//       0: 読み出し無効
//       1-16: 起動時に読み出すテーブル番号
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、EEPROMから起動時に読み出すテーブル番号を指定します。
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

// 関数: CA378_RESULT CA378_EraseSPI(void)
// 引数: なし
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、SPIを消去します。
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

// 関数: CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size)
// 引数: uint8_t *data          書き込みデータ
//       uint32_t size          サイズ
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、SPIのFWをアップデートします。
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

// 関数: CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
// 引数: uint8_t devAddr
//       uint16_t addr
//       uint8_t* data
//       uint8_t size
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、デバッグ用にレジスタ書き込みを行います。
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

// 関数: CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
// 引数: uint8_t devAddr
//       uint16_t addr
//       uint8_t* data
//       uint8_t size
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、デバッグ用にレジスタ読み出しを行います。
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

// 関数: CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision)
// 引数: uint8_t* majorVersion  メジャーバージョン
//       uint8_t* minorVersion  マイナーバージョン
//       uint8_t* revision      リビジョン
// 返り値: 成功/失敗(リターンコード参照)
// 説明: この機能は、FWのバージョンを取得します。
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
