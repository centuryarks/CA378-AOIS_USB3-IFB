/*
Copyright (c) 2017-2024, CenturyArks
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

#include "stdafx.h"
#include "USBCAExtensionUnit.h"
#include "demo_control.h"
#include "capture.h"
#include "af_control.h"
#include "tool.hpp"
#include "mode.h"

using namespace cv;
using std::cout; using std::cerr; using std::endl;


/***************************************************************
 *  Defines for Demo Control
 **************************************************************/
#define SETTING_FILENAME    "CA_ViewER.ini"

/***************************************************************
 *  Global variable for Demo Control
 **************************************************************/
CUSBCAExtensionUnit* g_pExpUnit = NULL;     // USBCA Extension Unit DLL


/*******************************************************************************
 * @brief   Initialize Demo software
 *
 * @param   param       Parameters
 *
 * @return  0 Success, other Failure
 ******************************************************************************/
int DemoInit(ST_PARAM* param)
{
    int ret;
    uint8_t enable, table_number;
    uint16_t value, r_value, g_value, b_value;

    ReadSettingFile(SETTING_FILENAME, param);
    param->select_mode = CheckSelectMode(param->select_mode);
    SetSelectMode(param->select_mode);

    // Load USBCA Extension Unit DLL
    if (g_pExpUnit == NULL)
    {
        if (param->usb_connected)
        {
            g_pExpUnit = new CUSBCAExtensionUnit();
        }
    }

    if (g_pExpUnit)
    {
        // Exposure
        ret = g_pExpUnit->CA378_GetExposure(&value);
        if (ret == 0)
        {
            param->exposure = value;
        }

        // Analog Gain
        ret = g_pExpUnit->CA378_GetAnalogGain(&value);
        if (ret == 0)
        {
            param->analog_gain = value / 256.0;
        }

        // White Balance
        ret = g_pExpUnit->CA378_GetWhiteBalance(&r_value, &g_value, &b_value);
        if (ret == 0)
        {
            param->white_balance_r = r_value / 256.0;
            param->white_balance_g = g_value / 256.0;
            param->white_balance_b = b_value / 256.0;
        }

        // Black Level
        ret = g_pExpUnit->CA378_GetBlackLevel(&enable, &value);
        if (ret == 0)
        {
            param->enable_black_level = (bool)enable;
            param->black_level = value;
        }

        // Lens Shading Correction
        ret = g_pExpUnit->CA378_GetLensShading(&table_number);
        if (ret == 0)
        {
            switch (table_number)
            {
            case 1:
                param->enable_lsc = true;
                param->lsc_table_number = 1;
                break;
            case 2:
                param->enable_lsc = true;
                param->lsc_table_number = 2;
                break;
            case 3:
                param->enable_lsc = true;
                param->lsc_table_number = 3;
                break;
            default:
                param->enable_lsc = false;
                break;
            }
        }

        // Defect Pixel
        ret = g_pExpUnit->CA378_GetDefectPixel(&enable);
        if (ret == 0)
        {
            switch (enable)
            {
            case 1:
                param->defect_pixel_mapped = true;
                param->defect_pixel_dynamic = false;
                break;
            case 2:
                param->defect_pixel_mapped = false;
                param->defect_pixel_dynamic = true;
                break;
            case 3:
                param->defect_pixel_mapped = true;
                param->defect_pixel_dynamic = true;
                break;
            default:
                param->defect_pixel_mapped = false;
                param->defect_pixel_dynamic = false;
                break;
            }
        }

#ifdef CA378_AOIS
        // Focus Position
        switch (param->focus_mode)
        {
        case FOCUS_MODE_DIRECT:
            param->focus_position = param->manual_focus_position;
            break;
        case FOCUS_MODE_INFINITY:
            param->focus_position = 0;
            break;
        case FOCUS_MODE_MACRO:
            param->focus_position = 1024;
            break;
        default:
            break;
        }
        DemoControl(param, CMD_MANUAL_FOCUS);

        // OIS Mode
        uint8_t mode;
        ret = g_pExpUnit->CA378_GetOISMode(&mode);
        if (ret == 0)
        {
            param->ois_mode = mode;
        }
#endif
    }

    return 0;
}

/*******************************************************************************
 * @brief   Exit Demo software
 *
 * @param   param       Parameters
 *
 * @return  0 Success, other Failure
 ******************************************************************************/
int DemoExit(ST_PARAM* param)
{
    // Stop Capture
    StopCapture();

    WriteSettingFile(SETTING_FILENAME, param);

    // Free USBCA Extension Unit DLL
    if (g_pExpUnit)
    {
        delete g_pExpUnit;
    }

    return 0;
}

/*******************************************************************************
 * @brief   Control Demo software
 *
 * @param   param       Parameters
 * @param   cmd         Command
 *
 * @return  0 Success, other Failure
 ******************************************************************************/
int DemoControl(ST_PARAM* param, E_DEMO_CMD cmd)
{
    if (!param->usb_connected)
    {
        return -1;
    }

    int ret = 0;

    switch (cmd)
    {
    case CMD_SELECT_MODE:       // Select Mode
        {
            StopCapture();
            SetSelectMode(param->select_mode);

            EnableGamma(param->enable_gamma);
            if (param->enable_gamma)
            {
                if (param->enable_srgb_gamma)
                {
                   UpdateSRGBGammaTable();
                }
                else
                {
                   UpdateGammaTable(param->gamma_value);
                }
            }

            EnableCCM(param->enable_ccm);
            if (param->enable_ccm)
            {
                UpdateCCMTable(param->ccm);
            }

            EnableContrast(param->enable_contrast, param->contrast_value);

            StartCapture();
        }
        break;

    case CMD_EXPOSURE:          // Exposure
        {
            uint16_t value;

            value = param->exposure;
            ret = g_pExpUnit->CA378_SetExposure(value);

            cout <<
                format("CA378_SetExposure: value = %d, ret = %X", value, ret)
            << endl;
        }
        break;

    case CMD_ANALOG_GAIN:       // Analog Gain
        {
            uint16_t value;

            value = (uint16_t)(param->analog_gain * 0x100 + 0.5);
            ret = g_pExpUnit->CA378_SetAnalogGain(value);

            cout <<
                format("CA378_SetAnalogGain: value = %.3lf (0x%04X), ret = %X", value / 256.0, value, ret)
                << endl;
        }
        break;

    case CMD_WHITE_BALANCE:     // White Balance
        {
            uint16_t r_value, g_value, b_value;

            r_value = (uint16_t)(param->white_balance_r * 0x100 + 0.5);
            g_value = (uint16_t)(param->white_balance_g * 0x100 + 0.5);
            b_value = (uint16_t)(param->white_balance_b * 0x100 + 0.5);

            ret = g_pExpUnit->CA378_SetWhiteBalance(r_value, g_value, b_value);

            cout <<
                format("CA378_SetWhiteBalance: R = %.3lf (0x%04X), G = %.3lf (0x%04X), B = %.3lf (0x%04X), ret = %X",
                    r_value / 256.0, r_value, g_value / 256.0, g_value, b_value / 256.0, b_value, ret)
                << endl;
        }
        break;

    case CMD_BLACK_LEVEL:       // Black Level
        {
            uint8_t enable;
            uint16_t value;

            enable = param->enable_black_level;
            value = param->black_level;

            ret = g_pExpUnit->CA378_SetBlackLevel(enable, value);

            cout <<
                format("CA378_SetBlackLevel: enable = %d, value = %d, ret = %X", enable, value, ret)
                << endl;
        }
        break;

    case CMD_LSC:               // LSC(Lens Shading Correction)
        {
            uint8_t table_number;

            if (param->enable_lsc)
            {
                table_number = param->lsc_table_number;
            }
            else
            {
                table_number = 0;
            }

            ret = g_pExpUnit->CA378_SetLensShading(table_number);

            cout <<
            format("CA378_SetLensShading: table_number = %d, ret = %X", table_number, ret)
                << endl;
        }
        break;

    case CMD_DEFECT_PIXEL:      // Defect Pixel
        {
            uint8_t enable;

            if (param->defect_pixel_mapped  == true &&
                param->defect_pixel_dynamic == true)
            {
                enable = 3;
            }
            else
            if (param->defect_pixel_mapped  == false &&
                param->defect_pixel_dynamic == true)
            {
                enable = 2;
            }
            else
            if (param->defect_pixel_mapped  == true &&
                param->defect_pixel_dynamic == false)
            {
                enable = 1;
            }
            else
            {
                enable = 0;
            }

            ret = g_pExpUnit->CA378_SetDefectPixel(enable);

            cout <<
                format("CA378_SetDefectPixel: enable = %d, ret = %X", enable, ret)
                << endl;
        }
        break;

    case CMD_GAMMA:             // Gamma Correction
        {
            EnableGamma(param->enable_gamma);
            if (param->enable_gamma)
            {
                if (param->enable_srgb_gamma)
                {
                   UpdateSRGBGammaTable();
                }
                else
                {
                   UpdateGammaTable(param->gamma_value);
                }
            }
        }
        break;

    case CMD_CCM:               // CCM(Color Corretion Matrix)
        {
            if (param->enable_ccm)
            {
                cout << "Enable Color Correction Matrix" << endl;
                UpdateCCMTable(param->ccm);
                EnableCCM(true);
            }
            else
            {
                cout << "Disable Color Correction Matrix" << endl;
                EnableCCM(false);
            }
        }
        break;

    case CMD_HISTOGRAM:         // Histogram
        {
            DisplayHistogram(param->display_histogram);
        }
        break;

    case CMD_CONTRAST:          // Contrast
        {
            EnableContrast(param->enable_contrast, param->contrast_value);
        }
        break;

#ifdef CA378_AOIS
    case CMD_MANUAL_FOCUS:      // Manual Focus
        {
            uint16_t value;

            switch (param->focus_mode)
            {
            case FOCUS_MODE_DIRECT:
                param->manual_focus_position = param->focus_position;
                break;
            case FOCUS_MODE_INFINITY:
                param->focus_position = 0x0;
                break;
            case FOCUS_MODE_MACRO:
                param->focus_position = 0x3FF;
                break;
            default:
                break;
            }

            value = param->focus_position;
            ret = g_pExpUnit->CA378_SetFocusPosition(value);

            cout <<
                format("CA378_SetFocusPosition: position = %d, ret = %X", value, ret)
                << endl;
        }
        break;

    case CMD_AUTO_FOCUS:        // Auto Focus
        {
            if (param->enable_auto_focus)
            {
                // Set first AF position
                int position = GetFocusPosition();
                SetAfPosition(position);
            }
            SetAutoFocus(param);
        }
        break;

    case CMD_GET_AF_POSITION:   // Get AF position
        {
            param->focus_position = GetAfPosition();
        }
        break;

    case CMD_OIS:               // OIS
        {
            uint8_t mode;

            mode = param->ois_mode;
            ret = g_pExpUnit->CA378_SetOISMode(mode);

            cout <<
                format("CA378_SetOISMode: mode = %d, ret = %X", mode, ret)
                << endl;
        }
        break;
#endif

    case CMD_EEPROM_SAVE:       // Save EEPROM
        {
            uint8_t table_number;

            table_number = param->eeprom_table_number;
            ret = g_pExpUnit->CA378_SaveEEPROM(table_number);

            cout <<
                format("CA378_SaveEEPROM: table_number = %d, ret = %X", table_number, ret)
                << endl;
        }
        break;

    case CMD_EEPROM_LOAD:       // Load EEPROM
        {
            uint8_t table_number;
            uint8_t table_value[16] = { 0 };
            uint16_t value;
            uint16_t r_value, g_value, b_value;

            table_number = param->eeprom_table_number;
            ret = g_pExpUnit->CA378_LoadEEPROM(table_number, table_value);
            cout <<
                format("CA378_LoadEEPROM: table_number = %d, ret = %X", table_number, ret)
                << endl;

            if (ret == CA378_SUCCESS)
            {
                value = (table_value[0] << 8) + table_value[1];
                param->exposure = value;
                cout <<
                    format(" Exposure    : %d", value)
                    << endl;

                value = (table_value[2] << 8) + table_value[3];
                param->analog_gain = value / 256.0;
                cout <<
                    format(" AnalogGain  : %.3lf (0x%04X)", value / 256.0, value)
                    << endl;

                r_value = (table_value[4] << 8) + table_value[5];
                g_value = (table_value[6] << 8) + table_value[7];
                b_value = (table_value[8] << 8) + table_value[9];
                param->white_balance_r = r_value / 256.0;
                param->white_balance_g = g_value / 256.0;
                param->white_balance_b = b_value / 256.0;
                cout <<
                    format(" WhiteBalance: R = %.3lf (0x%04X), G = %.3lf (0x%04X), B = %.3lf (0x%04X)",
                            r_value / 256.0, r_value, g_value / 256.0, g_value, b_value / 256.0, b_value)
                    << endl;

                value = (table_value[11] << 8) + table_value[12];
                param->enable_black_level   = table_value[10];
                param->black_level          = value;
                param->defect_pixel_mapped  = (table_value[13] & 0x1);
                param->defect_pixel_dynamic = (table_value[13] & 0x2) >> 1;
                param->enable_lsc = table_value[14] ? true : false;
                param->lsc_table_number     = table_value[14];
                cout <<
                    format(" BlackLevel  : enable = %d, value = %d", table_value[10], value)
                    << endl;
                cout <<
                    format(" DefectPixel : enable = %d", table_value[13])
                    << endl;
                cout <<
                    format(" LensShading : table_number = %d", table_value[14])
                    << endl;
            }
        }
        break;

    case CMD_EEPROM_UPLOAD_LSC: // Upload LSC to EEPROM
        {
            uint8_t table_number;
            uint8_t bayer_num = 4;
            uint8_t data[96] = { 0 };
            char bayer_id[][4] = { "R", "GR", "GB", "B" };
            char* pPath = NULL;

            table_number = param->save_lsc_table_number;

            switch (table_number)
            {
            case 1:
                pPath = (char*)param->lsc_table1_path.c_str();
                break;
            case 2:
                pPath = (char*)param->lsc_table2_path.c_str();
                break;
            case 3:
                pPath = (char*)param->lsc_table3_path.c_str();
                break;
            default:
                break;
            }

            for (int bayer_index = 0; bayer_index < bayer_num; bayer_index++)
            {
                ret = LoadLensShadingTable(pPath, bayer_index, data);
                if (ret == 0)
                {
                    ret = g_pExpUnit->CA378_SaveEEPROM_LSC(table_number, bayer_index, data);

                    cout <<
                        format("CA378_SaveEEPROM_LSC: table_number = %d, bayer = %s, ret = %X",
                        table_number, bayer_id[bayer_index], ret)
                        << endl;

                    cout << format(" LSC data: ");
                    for (int i = 0; i < sizeof(data); i += 2)
                    {
                        if (i % 16 == 0) cout << endl;
                        cout << format("%3d(0x%03X) ", (data[i] << 8) + data[i + 1], (data[i] << 8) + data[i + 1]);
                    }
                    cout << endl << endl;
                }
                else
                {
                    cout << format("Failed to Load LSC Table.(%s)") << endl;
                    break;
                }
            }
        }
        break;

    case CMD_EEPROM_DOWNLOAD_LSC:   // Download LSC from EEPROM
        {
            uint8_t table_number;
            uint8_t bayer_num = 4;
            uint8_t data[96] = { 0 };
            char bayer_id[][4] = { "R", "GR", "GB", "B" };
            char* pPath = NULL;

            table_number = param->save_lsc_table_number;

            switch (table_number)
            {
            case 1:
                pPath = (char*)param->lsc_table1_path.c_str();
                break;
            case 2:
                pPath = (char*)param->lsc_table2_path.c_str();
                break;
            case 3:
                pPath = (char*)param->lsc_table3_path.c_str();
                break;
            default:
                break;
            }

            for (int bayer_index = 0; bayer_index < bayer_num; bayer_index++)
            {
                ret = g_pExpUnit->CA378_LoadEEPROM_LSC(table_number, bayer_index, data);
                if (ret == CA378_SUCCESS)
                {
                    SaveLensShadingTable(pPath, bayer_index, data);
                }

                cout <<
                    format("CA378_LoadEEPROM_LSC: table_number = %d, bayer = %s, ret = %X",
                    table_number, bayer_id[bayer_index], ret)
                    << endl;

                cout << format(" LSC data: ");
                for (int i = 0; i < sizeof(data); i += 2)
                {
                    if (i % 16 == 0) cout << endl;
                    cout << format("%3d(0x%03X) ", (data[i] << 8) + data[i + 1], (data[i] << 8) + data[i + 1]);
                }
                cout << endl << endl;
            }
        }
        break;

    case CMD_EEPROM_DEFAULT:    // Set Default Table number for EEPROM
        {
            uint8_t table_number;

            table_number = param->eeprom_table_number;
            ret = g_pExpUnit->CA378_DefaultEEPROM(table_number);

            cout <<
                format("CA378_DefaultEEPROM: table_number = %d, ret = %X", table_number, ret)
                << endl;
        }
        break;

    case CMD_EEPROM_CLEAR:      // Clear EEPROM
        {
            ret = g_pExpUnit->CA378_ClearEEPROM();
            cout <<
                format("CA378_ClearEEPROM: ret = %X", ret)
                << endl;
        }
        break;

    case CMD_CAPTURE:           // Still Capture
        StillCapture();
        break;

    case CMD_MOVIE:             // Movie Record
        MovieRecord(param->record_frame);
        break;

    case CMD_MAX_RECORD_FRAME:  // Max Record Frame
        param->max_record_frame = GetMaxRecordFrame(param->select_mode);
        break;

    case CMD_RECORD_PROGRESS:
        param->record_progress = GetRecordProgress();
        break;

    case CMD_CONVERT_PROGRESS:
        param->convert_progress = GetConvertProgress();
        break;

    case CMD_GET_AVERAGE_FPS:   // Get Average FPS
        param->average_fps = GetAverageFps();
        break;

    case CMD_GET_CAPTURED_FRAMES:   // Get Captured Frames
        param->captured_frames = GetCapturedFrames();
        break;

    default:
        cout << "Bad Command I/F!" << endl;
        ret = -1;
        break;
    }

    return ret;
}

/*******************************************************************************
 * @brief   Focus direct moving
 *
 * @param   position    Focus target position
 *
 * @return  void
 ******************************************************************************/
void DirectMove(int position)
{
    if (g_pExpUnit)
    {
        g_pExpUnit->CA378_SetFocusPosition(position);
    }
}

/*******************************************************************************
 * @brief   Get Focus position
 *
 * @param   void
 *
 * @return  Focus position
 ******************************************************************************/
int GetFocusPosition(void)
{
    uint16_t position = 0x200;

    if (g_pExpUnit)
    {
        g_pExpUnit->CA378_GetFocusPosition(&position);
    }

    return position;
}
