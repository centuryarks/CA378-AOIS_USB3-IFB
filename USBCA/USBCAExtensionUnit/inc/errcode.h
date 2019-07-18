#ifndef __ERRCODE_H__
#define __ERRCODE_H__

// リターンコード一覧:
typedef enum E_CA378_RESULT
{
    CA378_SUCCESS                   = 0x00,     // 基本関数の実行に成功
    CA378_ERR_NOMEM                 = 0x01,     // メモリ確保に失敗した
    CA378_ERR_THREAD                = 0x02,     // スレッドの作成に失敗した
    CA378_ERR_EVENT                 = 0x03,     // イベントの作成に失敗した
    CA378_ERR_FATAL                 = 0x04,     // 致命的エラーが発生した
    CA378_ERR_SYSTEM_INIT           = 0x05,     // システムの初期化に失敗した
    CA378_ERR_CX3_INIT              = 0x06,     // CX3デバイスの初期化に失敗した
    CA378_ERR_CAMERA_MIPI           = 0x07,     // カメラMIPI通信で失敗した
    CA378_ERR_CAMERA_SETUP          = 0x08,     // カメラモジュールのセットアップに失敗した
    CA378_ERR_PARAM                 = 0x09,     // パラメータ指定が間違っている
    CA378_ERR_SENSOR_IF             = 0x0A,     // Image Sensor の通信でエラー発生
    CA378_ERR_EEPROM_IF             = 0x0B,     // EEPROM の通信でエラー発生
    CA378_ERR_EEPROM_NOT_SET        = 0x0C,     // EEPROM に値がセットされてない
    CA378_ERR_SPI_IF                = 0x0D,     // SPI の通信でエラー発生
    CA378_ERR_NOT_USE_USBSPEED      = 0x0E,     // 現在のUSB速度(USB2.0 or USB1.0)では使用できない
    CA378_ERR_EXT_UNIT_NOT_FOUND    = 0x10,     // Extention Unit が見つからない
    CA378_ERR_EXT_UNIT_FAIL         = 0x11,     // Extention Unit 通信に失敗
    CA378_ERR_EXT_UNIT_BAD_SIZE     = 0x12,     // Extention Unit 通信でサイズが不正
    CA378_ERR_EXT_UNIT_UNKNOWN_CMD  = 0x13,     // Extention Unit 通信で不明なコマンド
    CA378_ERR_EXT_UNIT_TIMEOUT      = 0x14,     // Extention Unit 通信でタイムアウト
    CA378_ERR_SEND                  = 0x20,     // 送信エラー + 0x20
    CA378_ERR_RECV                  = 0x40,     // 受信エラー + 0x40
} CA378_RESULT;

#endif
