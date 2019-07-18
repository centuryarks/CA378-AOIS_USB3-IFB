/*
Copyright (c) 2017-2019, CenturyArks
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

#ifndef DEMO_CONTROL_H
#define DEMO_CONTROL_H


/***************************************************************
 *  Defines for Demo Control
 **************************************************************/
enum E_DEMO_CMD
{
    CMD_SELECT_MODE = 0,
    CMD_CAPTURE,
    CMD_MOVIE,
    CMD_MAX_RECORD_FRAME,
    CMD_RECORD_PROGRESS,
    CMD_CONVERT_PROGRESS,
    CMD_EXPOSURE,
    CMD_ANALOG_GAIN,
    CMD_WHITE_BALANCE,
    CMD_BLACK_LEVEL,
    CMD_LSC,
    CMD_DEFECT_PIXEL,
    CMD_GAMMA,
    CMD_CCM,
    CMD_CONTRAST,
    CMD_HISTOGRAM,
#ifdef CA378_AOIS
    CMD_MANUAL_FOCUS,
    CMD_AUTO_FOCUS,
    CMD_GET_AF_POSITION,
    CMD_OIS,
#endif
    CMD_EEPROM_SAVE,
    CMD_EEPROM_LOAD,
    CMD_EEPROM_UPLOAD_LSC,
    CMD_EEPROM_DOWNLOAD_LSC,
    CMD_EEPROM_DEFAULT,
    CMD_EEPROM_CLEAR,
    CMD_GET_AVERAGE_FPS,
    CMD_GET_CAPTURED_FRAMES
};

#ifdef CA378_AOIS
enum E_FOCUS_MODE
{
    FOCUS_MODE_DIRECT = 0,
    FOCUS_MODE_INFINITY,
    FOCUS_MODE_MACRO
};
#endif

struct ST_PARAM
{
    int select_mode             = 0;

    // Sensor
    uint16_t exposure           = 1000;
    double analog_gain          = 1.000;
    double white_balance_r      = 1.000;
    double white_balance_g      = 1.000;
    double white_balance_b      = 1.000;
    bool enable_black_level     = true;
    uint16_t black_level        = 64;
    bool enable_lsc             = false;
    int lsc_table_number        = 1;
    bool defect_pixel_mapped    = true;
    bool defect_pixel_dynamic   = true;

    // Expansion
    bool enable_gamma           = true;
    bool enable_srgb_gamma      = true;
    double gamma_value          = 0.45;
    bool enable_ccm             = true;
    bool enable_contrast        = true;
    int contrast_value          = 16;
    bool display_histogram      = false;
    double ccm[3][3]
    {
        {  1.922, -0.649, -0.269},
        { -0.589,  1.897, -0.322},
        { -0.161, -0.967,  2.138},
    };

    // Capture
    int record_frame            = 100;
    int record_progress         = 0;
    int convert_progress        = 0;
    int max_record_frame        = 100;

#ifdef CA378_AOIS
    // AF & OIS
    int focus_position          = 512;
    int manual_focus_position   = 512;
    int focus_mode              = FOCUS_MODE_DIRECT;
    int ois_mode                = 0;
    int enable_auto_focus       = 0;
    double auto_focus_gain      = 1.0;
#endif

    int eeprom_table_number     = 1;
    int save_lsc_table_number   = 1;
    std::string lsc_table1_path = "";
    std::string lsc_table2_path = "";
    std::string lsc_table3_path = "";

    // Layout
    std::string font_family     = "Arial";
    int font_points             = 9;
    int scale                   = 100;
    int window_left             = 0;
    int window_top              = 0;
    int window_width            = 480;
    int window_height           = 500;

    // (Status Bar)
    double average_fps = 0.000;
    size_t captured_frames = 0;

    // USB Connected
    bool usb_connected = false;
};

/***************************************************************
 *  Declare function for Demo Control
 **************************************************************/
int DemoInit(ST_PARAM* param);
int DemoExit(ST_PARAM* param);
int DemoControl(ST_PARAM* param, E_DEMO_CMD cmd);
void DirectMove(int position);
int GetFocusPosition(void);

#endif // DEMO_CONTROL_H
