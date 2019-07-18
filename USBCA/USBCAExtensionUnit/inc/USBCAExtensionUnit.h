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

    // 関数: CA378_RESULT CA378_AUTO_config(void)
    // 引数: なし
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、UVCで指定した解像度の設定に従います。
    CA378_RESULT CA378_AUTO_config(void);

    // 関数: CA378_RESULT CA378_12MP_config(void)
    // 引数: なし
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、9fpsのフレームレートの12MP（4056x3040）解像度の設定をイメージセンサーに書き込みます。
    //       この機能が実行されると、イメージセンサは9fpsで12MPの解像度のビデオ出力を開始します。
    CA378_RESULT CA378_12MP_config(void);

    // 関数: CA378_RESULT CA378_3MP_config(void)
    // 引数: なし
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、 30fpsのフレームレートの3MP（2028x1520）解像度の設定をイメージセンサーに書き込みます。
    //       この機能が実行されると、イメージセンサは30fpsで3MPの解像度のビデオ出力を開始します。
    CA378_RESULT CA378_3MP_config(void);

    // 関数: CA378_RESULT CA378_760P_config(void)
    // 引数: なし
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、 60fpsのフレームレートの760P（1012x760）解像度の設定をイメージセンサーに書き込みます。
    //       この機能が実行されると、イメージセンサは60fpsで760Pの解像度のビデオ出力を開始します。
    CA378_RESULT CA378_760P_config(void);

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
    CA378_RESULT CA378_SetExposure(uint16_t value);

    // 関数: CA378_RESULT CA378_GetExposure(uint16_t *value)
    // 引数: uint16_t *value    露光値
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明:この機能は、電子シャッターの露光時間を取得します。
    // フォーマット: 16 ビット符号なし整数値
    // 範囲: 1 - 65515
    CA378_RESULT CA378_GetExposure(uint16_t *value);

    // 関数: CA378_RESULT CA378_SetAnalogGain(uint16_t value)
    // 引数: uint16_t value     アナログゲイン値
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、アナログゲイン値を設定します。
    // フォーマット: 16 ビット符号なし固定小数点Q8.8
    // 範囲: 1.0(0x100) - 22.262(0x1643)
    CA378_RESULT CA378_SetAnalogGain(uint16_t value);

    // 関数: CA378_RESULT CA378_GetAnalogGain(uint16_t *value)
    // 引数: uint16_t *value    アナログゲイン値
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明:この機能は、アナログゲイン値を取得します。
    // フォーマット: 16 ビット符号なし固定小数点Q8.8
    // 範囲: 1.0(0x100) - 22.262 (0x1643)
    CA378_RESULT CA378_GetAnalogGain(uint16_t *value);

    // 関数: CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value)
    // 引数: uint16_t r_value   R値
    //       uint16_t g_value   G値
    //       uint16_t b_value   B値
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、ホワイトバランスの値を設定します。
    // フォーマット: 16 ビット符号なし固定小数点Q8.8
    // 範囲: 1.0 (0x100) - 15.996 (0xFFF)
    CA378_RESULT CA378_SetWhiteBalance(uint16_t r_value, uint16_t g_value, uint16_t b_value);

    // 関数: CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value)
    // 引数: uint16_t *r_value  R値
    //       uint16_t *g_value  G値
    //       uint16_t *b_value  B値
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明:この機能は、ホワイトバランスの値を取得します。
    // フォーマット: 16 ビット符号なし固定小数点Q8.8
    // 範囲: 1.0 (0x100) - 15.996 (0xFFF)
    CA378_RESULT CA378_GetWhiteBalance(uint16_t *r_value, uint16_t *g_value, uint16_t *b_value);

    // 関数: CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value)
    // 引数: uint8_t enable     0:無効、1:有効
    //       uint16_t value     ブラックレベル補正値
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、ブラックレベル補正を設定します。
    // フォーマット: 16 ビット符号なし整数値
    // 初期値: 有効 / 64 (RAW10 時)
    CA378_RESULT CA378_SetBlackLevel(uint8_t enable, uint16_t value);

    // 関数: CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value)
    // 引数: uint8_t *enable    0:無効、1:有効
    //       uint16_t *value    ブラックレベル補正値
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明:この機能は、ブラックレベル補正を取得します。
    // フォーマット: 16 ビット符号なし整数値
    // 初期値: 有効 / 64 (RAW10 時)
    CA378_RESULT CA378_GetBlackLevel(uint8_t *enable, uint16_t *value);

    // 関数: CA378_RESULT CA378_SetDefectPixel(uint8_t enable)
    // 引数: uint8_t enable     欠陥補正有効/無効
    //       0: 補正無効
    //       1: マップドカプレット補正有効
    //       2: 動的シングレット補正有効
    //       3: マップドカプレット補正/動的シングレット補正有効
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、欠陥補正の有効/無効を設定します。
    // 初期値: 3 (マップドカプレット補正/動的シングレット補正有効)
    CA378_RESULT CA378_SetDefectPixel(uint8_t enable);

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
    CA378_RESULT CA378_GetDefectPixel(uint8_t *enable);

    // 関数: CA378_RESULT CA378_SetLensShading(uint8_t table_number)
    // 引数: uint8_t table_number   シェーディング補正有効無効
    //       0: シェーディング補正無効
    //       1-3: 指定テーブルから読み出し、シェーディング補正有効
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、レンズシェーディング補正を有効/無効にします。
    CA378_RESULT CA378_SetLensShading(uint8_t table_number);

    // 関数: CA378_RESULT CA378_GetLensShading(uint8_t *table_number)
    // 引数: uint8_t *table_number  シェーディング補正有効無効
    //       0: シェーディング補正無効、
    //       1-3: 選択中のシェーディング補正のテーブル
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、レンズシェーディング補正を取得します。
    CA378_RESULT CA378_GetLensShading(uint8_t *table_number);

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
    CA378_RESULT CA378_SaveEEPROM(uint8_t table_number);

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
    CA378_RESULT CA378_LoadEEPROM(uint8_t table_number, uint8_t *table_value);

    // 関数: CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
    // 引数: uint8_t table_number   テーブル番号(1-3)
    //       uint8_t bayer          ベイヤ配列(0:R, 1:Gr, 2:Gb, 3:B)
    //       uint16_t *table_value  シェーディング補正値(48点)
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、EEPROMにレンズシェーディング補正を保存します。
    CA378_RESULT CA378_SaveEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value);

    // 関数: CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value)
    // 引数: uint8_t table_number   テーブル番号(1-3)
    //       uint8_t bayer          ベイヤ配列(0:R, 1:Gr, 2:Gb, 3:B)
    //       uint16_t *table_value  シェーディング補正値(48点)
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、EEPROMからレンズシェーディング補正値を読み出します。
    CA378_RESULT CA378_LoadEEPROM_LSC(uint8_t table_number, uint8_t bayer, uint8_t *table_value);

    // 関数: CA378_RESULT CA378_ClearEEPROM(void)
    // 引数: なし
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、EEPROM から調整値をクリアします。
    CA378_RESULT CA378_ClearEEPROM(void);

    // 関数: CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number)
    // 引数: uint8_t table_number   テーブル番号
    //       0: 読み出し無効
    //       1-16: 起動時に読み出すテーブル番号
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、EEPROMから起動時に読み出すテーブル番号を指定します。
    CA378_RESULT CA378_DefaultEEPROM(uint8_t table_number);

    // 関数: CA378_RESULT CA378_EraseSPI(void)
    // 引数: なし
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、SPIを消去します。
    CA378_RESULT CA378_EraseSPI(void);

    // 関数: CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size)
    // 引数: uint8_t *data          書き込みデータ
    //       uint32_t size          サイズ
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、SPIのFWをアップデートします。
    CA378_RESULT CA378_UpdateSPI(uint8_t *data, uint32_t size);

    // 関数: CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
    // 引数: uint8_t devAddr
    //       uint16_t addr
    //       uint8_t* data
    //       uint8_t size
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、デバッグ用にレジスタ書き込みを行います。
    CA378_RESULT CA378_WriteRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size);

    // 関数: CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size)
    // 引数: uint8_t devAddr
    //       uint16_t addr
    //       uint8_t* data
    //       uint8_t size
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、デバッグ用にレジスタ読み出しを行います。
    CA378_RESULT CA378_ReadRegister(uint8_t devAddr, uint16_t addr, uint8_t* data, uint8_t size);

    // 関数: CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision)
    // 引数: uint8_t* majorVersion  メジャーバージョン
    //       uint8_t* minorVersion  マイナーバージョン
    //       uint8_t* revision      リビジョン
    // 返り値: 成功/失敗(リターンコード参照)
    // 説明: この機能は、FWのバージョンを取得します。
    CA378_RESULT CA378_GetFwVersion(uint8_t* majorVersion, uint8_t* minorVersion, uint8_t* revision);

    // AF/OIS Control
    CA378_RESULT CA378_SetFocusPosition(uint16_t position);
    CA378_RESULT CA378_SetOISMode(uint8_t mode);
    CA378_RESULT CA378_GetFocusPosition(uint16_t* position);
    CA378_RESULT CA378_GetOISMode(uint8_t* mode);
};
