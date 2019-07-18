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

#include <stdafx.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tool.hpp"
#include "mode.h"
#include "usbinfo.h"


/*******************************************************************************
 * @brief   Main window constructor
 *
 * @param   parent  Widget parent
 *
 * @return  void
 ******************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Check USB Version
    USHORT vid = 0x31F7;
    USHORT pid = 0x0001;
    USHORT bcdUSB = 0;
    BOOL ret = GetUsbVersion(vid, pid, &bcdUSB);
    if (ret)
    {
        param.usb_connected = true;
        usbtype = USB3;
        if (bcdUSB < 0x0300)
        {
            usbtype = USB2;
        }
    }
    else
    {
        param.usb_connected = false;
    }

    recording = false;
    initialized = false;

    DemoInit(&param);

    // Setup UI
    ui->setupUi(this);

    // Set Version
    this->setWindowTitle(VERSION);

#ifndef CA378_AOIS
    ui->tabWidget->removeTab(TAB_INDEX_AOIS);
#endif

    //
    // Sensor
    //

    // Mode Select
    int i;
    int max = GetModeListCount(usbtype);
    for (i = 0; i < max; i++)
    {
        QString str;
        str.sprintf("%d x %d @ %.0ffps", MODE_LIST[usbtype][i].width, MODE_LIST[usbtype][i].height, MODE_LIST[usbtype][i].fps);
        ui->comboBox_Mode->addItem(str);
    }

    // Exposure
    double exposure = 1.0 / ((double)(param.exposure + 20) * LINE_LENGTH * 2 / PIXEL_RATE);
    ui->spinBox_Exposure->setValue((int)exposure);
    // Analog Gain
    ui->doubleSpinBox_AnalogGain->setValue(param.analog_gain);
    // White Balance
    ui->doubleSpinBox_WhiteBalance_R->setValue(param.white_balance_r);
    ui->doubleSpinBox_WhiteBalance_G->setValue(param.white_balance_g);
    ui->doubleSpinBox_WhiteBalance_B->setValue(param.white_balance_b);
    // Black Level
    ui->groupBox_BlackLevel->setChecked(param.enable_black_level);
    ui->spinBox_BlackLevel->setValue(param.black_level);
    // Lens Shading Correction
    ui->groupBox_LSC->setChecked(param.enable_lsc);
    switch (param.lsc_table_number)
    {
    case 1:
        ui->radioButton_LSC_Table1->setChecked(true);
        break;
    case 2:
        ui->radioButton_LSC_Table2->setChecked(true);
        break;
    case 3:
        ui->radioButton_LSC_Table2->setChecked(true);
        break;
    default:
        ui->radioButton_LSC_Table1->setChecked(false);
        ui->radioButton_LSC_Table2->setChecked(false);
        ui->radioButton_LSC_Table3->setChecked(false);
        break;
    }
    // Defect Piexel
    ui->checkBox_DefectPixel_Mapped->setChecked(param.defect_pixel_mapped);
    ui->checkBox_DefectPixel_Dynamic->setChecked(param.defect_pixel_dynamic);

    //
    // Expansion
    //

    // Gamma
    ui->groupBox_Gamma->setChecked(param.enable_gamma);
    ui->doubleSpinBox_Gamma->setValue(param.gamma_value);
    ui->checkBox_sRGB_Gamma->setChecked(param.enable_srgb_gamma);
    ui->groupBox_Contrast->setChecked(param.enable_contrast);
    ui->spinBox_Contrast->setValue(param.contrast_value);

    if (param.enable_srgb_gamma)
    {
        ui->horizontalSlider_Gamma->setEnabled(false);
        ui->doubleSpinBox_Gamma->setEnabled(false);
    }

    // CCM(Color Correction Matrix)
    ui->groupBox_CCM->setChecked(param.enable_ccm);
    QDoubleSpinBox* CCM_Table[3][3] =
    {
        ui->doubleSpinBox_CCM1,
        ui->doubleSpinBox_CCM2,
        ui->doubleSpinBox_CCM3,
        ui->doubleSpinBox_CCM4,
        ui->doubleSpinBox_CCM5,
        ui->doubleSpinBox_CCM6,
        ui->doubleSpinBox_CCM7,
        ui->doubleSpinBox_CCM8,
        ui->doubleSpinBox_CCM9,
    };
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            CCM_Table[y][x]->setValue(param.ccm[y][x]);
        }
    }

#ifdef CA378_AOIS
    //
    // AF & OIS
    //

    // Focus
    switch (param.focus_mode)
    {
    case FOCUS_MODE_DIRECT:
        ui->radioButton_Direct->setChecked(true);
        break;
    case FOCUS_MODE_INFINITY:
        ui->radioButton_Infinity->setChecked(true);
        break;
    case FOCUS_MODE_MACRO:
        ui->radioButton_Macro->setChecked(true);
        break;
    default:
        ui->radioButton_Direct->setChecked(false);
        ui->radioButton_Infinity->setChecked(false);
        ui->radioButton_Macro->setChecked(false);
        break;
    }
    ui->spinBox_Focus->setValue(param.focus_position);
    ui->doubleSpinBox_AutoFocusGain->setValue(param.auto_focus_gain);

    // OIS
    switch (param.ois_mode)
    {
    case 0:
        ui->radioButton_OIS_Mode0->setChecked(true);
        break;
    case 1:
        ui->radioButton_OIS_Mode1->setChecked(true);
        break;
    case 2:
        ui->radioButton_OIS_Mode2->setChecked(true);
        break;
    case 3:
        ui->radioButton_OIS_Mode3->setChecked(true);
        break;
    case 4:
        ui->radioButton_OIS_Mode4->setChecked(true);
        break;
    case 5:
        ui->radioButton_OIS_Mode5->setChecked(true);
        break;
    default:
        ui->radioButton_OIS_Mode0->setChecked(false);
        ui->radioButton_OIS_Mode1->setChecked(false);
        ui->radioButton_OIS_Mode2->setChecked(false);
        ui->radioButton_OIS_Mode3->setChecked(false);
        ui->radioButton_OIS_Mode4->setChecked(false);
        ui->radioButton_OIS_Mode5->setChecked(false);
        break;
    }
#endif

    //
    // EEPROM
    //
    ui->comboBox_EEPROM_Table->setCurrentIndex(param.eeprom_table_number - 1);
    QString path1 = param.lsc_table1_path.c_str();
    QString path2 = param.lsc_table2_path.c_str();
    QString path3 = param.lsc_table3_path.c_str();
    QDir dir(NULL);
    ui->textEdit_LSC_Path1->setText(dir.relativeFilePath(path1));
    ui->textEdit_LSC_Path1->setToolTip(dir.absoluteFilePath(path1));
    ui->textEdit_LSC_Path2->setText(dir.relativeFilePath(path2));
    ui->textEdit_LSC_Path2->setToolTip(dir.absoluteFilePath(path2));
    ui->textEdit_LSC_Path3->setText(dir.relativeFilePath(path3));
    ui->textEdit_LSC_Path3->setToolTip(dir.absoluteFilePath(path3));

    // Record Frame
    ui->spinBox_MovieRecord->setValue(param.record_frame);

    // Select Mode
    ui->comboBox_Mode->setCurrentIndex(param.select_mode);
    DemoControl(&param, CMD_SELECT_MODE);
    DemoControl(&param, CMD_MAX_RECORD_FRAME);
    ui->spinBox_MovieRecord->setMaximum(param.max_record_frame);

    //
    // Layout
    //

    // Convert Scale
    //int scale = GetDpiForSystem() * 100 / 96;
    // GetDpiForSystem function is minimum supported client Windows 10, version 1607.
    HDC screen = GetDC(0);
    int scale = GetDeviceCaps(screen, LOGPIXELSX) * 100 / 96;
    ReleaseDC(0, screen);

    if (param.scale != scale)
    {
        param.window_left   = param.window_left   * scale / param.scale;
        param.window_top    = param.window_top    * scale / param.scale;
        param.window_width  = param.window_width  * scale / param.scale;
        param.window_height = param.window_height * scale / param.scale;
        param.scale         = scale;
    }

    // Window
    CheckWindowArea();
    int barHeight = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME) * 2;
    int left   = param.window_left;
    int top    = param.window_top    + barHeight;
    int width  = param.window_width;
    int height = param.window_height - barHeight;
    this->setGeometry(left, top, width, height);

    // Font
    ui->fontComboBox->setCurrentFont(QFont(QString(param.font_family.c_str()), param.font_points));
    ui->spinBox_FontPoints->setValue(param.font_points);

    if (param.usb_connected)
    {
        initialized = true;

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(500);
    }
    else
    {
        ui->statusBar->showMessage("USB camera is not connected.");
        ui->centralWidget->setEnabled(false);
    }
}

/*******************************************************************************
 * @brief   Main window destructor
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
MainWindow::~MainWindow()
{
    DemoExit(&param);

    delete ui;
}

/*******************************************************************************
 * @brief   Mode activated on combo box
 *
 * @param   index           mode
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_comboBox_Mode_activated(int index)
{
    param.select_mode = index;

    DemoControl(&param, CMD_MAX_RECORD_FRAME);
    ui->spinBox_MovieRecord->setMaximum(param.max_record_frame);

    if (initialized)
    {
        DemoControl(&param, CMD_SELECT_MODE);
    }
}

/*******************************************************************************
 * @brief   Still Capture clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_StillCapture_clicked()
{
    if (initialized)
    {
        DemoControl(&param, CMD_CAPTURE);
    }
}

/*******************************************************************************
 * @brief   Movie Record clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_MovieRecord_clicked()
{
    if (initialized)
    {
        if (recording == true)
        {
            ui->pushButton_MovieRecord->setEnabled(false);
            recording = false;

            param.record_frame = 0;
            DemoControl(&param, CMD_MOVIE);
        }
        else
        {
            recording = true;
            ui->pushButton_MovieRecord->setText("Stop");

            param.record_frame = ui->spinBox_MovieRecord->value();
            DemoControl(&param, CMD_MOVIE);

            param.record_progress = 0;
            param.convert_progress = 0;
            ui->progressBar_MovieRecord->setValue(0);
            ui->progressBar_MovieConvert->setValue(0);

            while (recording && param.record_progress < param.record_frame)
            {
                DemoControl(&param, CMD_RECORD_PROGRESS);
                ui->progressBar_MovieRecord->setValue(100 * param.record_progress / param.record_frame);
                QApplication::processEvents();
                Sleep(500);
            }

            while (recording && param.convert_progress < param.record_frame)
            {
                DemoControl(&param, CMD_CONVERT_PROGRESS);
                ui->progressBar_MovieConvert->setValue(100 * param.convert_progress / param.record_frame);
                QApplication::processEvents();
                Sleep(500);
            }

            if (recording == true)
            {
                recording = false;
                ui->progressBar_MovieRecord->setValue(100);
            }
            else
            {
                ui->progressBar_MovieRecord->setValue(0);
            }

            ui->pushButton_MovieRecord->setText("Movie");
            ui->pushButton_MovieRecord->setEnabled(true);
        }
    }
}

/*******************************************************************************
 * @brief   CCM toggled on group box
 *
 * @param   arg1            disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_groupBox_CCM_toggled(bool arg1)
{
    param.enable_ccm = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM1 value changed on spin box(double)
 *
 * @param   arg1            ccm1 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM1_valueChanged(double arg1)
{
    param.ccm[0][0] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM2 value changed on spin box(double)
 *
 * @param   arg1    ccm2 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM2_valueChanged(double arg1)
{
    param.ccm[0][1] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM3 value changed on spin box(double)
 *
 * @param   arg1            ccm3 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM3_valueChanged(double arg1)
{
    param.ccm[0][2] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM4 value changed on spin box(double)
 *
 * @param   arg1            ccm4 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM4_valueChanged(double arg1)
{
    param.ccm[1][0] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM5 value changed on spin box(double)
 *
 * @param   arg1            ccm5 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM5_valueChanged(double arg1)
{
    param.ccm[1][1] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM6 value changed on spin box(double)
 *
 * @param   arg1            ccm6 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM6_valueChanged(double arg1)
{
    param.ccm[1][2] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM7 value changed on spin box(double)
 *
 * @param   arg1            ccm7 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM7_valueChanged(double arg1)
{
    param.ccm[2][0] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM8 value changed on spin box(double)
 *
 * @param   arg1            ccm8 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM8_valueChanged(double arg1)
{
    param.ccm[2][1] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   CCM9 value changed on spin box(double)
 *
 * @param   arg1            ccm9 value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_CCM9_valueChanged(double arg1)
{
    param.ccm[2][2] = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CCM);
    }
}

/*******************************************************************************
 * @brief   Exposure value changed on horizontal slider
 *
 * @param   value           exposure value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_Exposure_valueChanged(int value)
{
    ui->spinBox_Exposure->setValue(value);
}

/*******************************************************************************
 * @brief   Exposure value changed on spin box
 *
 * @param   arg1            exposure value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_spinBox_Exposure_valueChanged(int arg1)
{
    int exposure = (int)((PIXEL_RATE / LINE_LENGTH / arg1 / 2) - 20);
    ui->horizontalSlider_Exposure->setValue(arg1);
    param.exposure = exposure;

    if (initialized)
    {
        DemoControl(&param, CMD_EXPOSURE);
    }
}

/*******************************************************************************
 * @brief   Analog Gain value changed on horizontal slider
 *
 * @param   value           Analog Gain value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_AnalogGain_valueChanged(int value)
{
    ui->doubleSpinBox_AnalogGain->setValue(value/1000.0);
}

/*******************************************************************************
 * @brief   Analog Gain value changed on spin box
 *
 * @param   arg1            Analog Gain value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_AnalogGain_valueChanged(double arg1)
{
    ui->horizontalSlider_AnalogGain->setValue((int)(arg1*1000 + 0.5));
    param.analog_gain = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_ANALOG_GAIN);
    }
}

/*******************************************************************************
 * @brief   White Balance R value changed on horizontal slider
 *
 * @param   value           White Balance R value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_WhiteBalance_R_valueChanged(int value)
{
    ui->doubleSpinBox_WhiteBalance_R->setValue(value/1000.0);
}

/*******************************************************************************
 * @brief   White Balance G value changed on horizontal slider
 *
 * @param   value           White Balance G value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_WhiteBalance_G_valueChanged(int value)
{
    ui->doubleSpinBox_WhiteBalance_G->setValue(value/1000.0);
}

/*******************************************************************************
 * @brief   White Balance B value changed on horizontal slider
 *
 * @param   value           White Balance B value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_WhiteBalance_B_valueChanged(int value)
{
    ui->doubleSpinBox_WhiteBalance_B->setValue(value/1000.0);
}

/*******************************************************************************
 * @brief   White Balance R value changed on spin box(double)
 *
 * @param   arg1            White Balance R value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_WhiteBalance_R_valueChanged(double arg1)
{
    ui->horizontalSlider_WhiteBalance_R->setValue((int)(arg1*1000.0 + 0.5));
    param.white_balance_r = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_WHITE_BALANCE);
    }
}

/*******************************************************************************
 * @brief   White Balance G value changed on spin box(double)
 *
 * @param   arg1            White Balance G value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_WhiteBalance_G_valueChanged(double arg1)
{
    ui->horizontalSlider_WhiteBalance_G->setValue((int)(arg1*1000.0 + 0.5));
    param.white_balance_g = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_WHITE_BALANCE);
    }
}

/*******************************************************************************
 * @brief   White Balance B value changed on spin box(double)
 *
 * @param   arg1            White Balance B value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_WhiteBalance_B_valueChanged(double arg1)
{
    ui->horizontalSlider_WhiteBalance_B->setValue((int)(arg1*1000.0 + 0.5));
    param.white_balance_b = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_WHITE_BALANCE);
    }
}

/*******************************************************************************
 * @brief   Black Level toggled on group box
 *
 * @param   arg1            disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_groupBox_BlackLevel_toggled(bool arg1)
{
    param.enable_black_level = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_BLACK_LEVEL);
    }
}

/*******************************************************************************
 * @brief   Black Level value changed on horizontal slider
 *
 * @param   value           Black Level value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_BlackLevel_valueChanged(int value)
{
    ui->spinBox_BlackLevel->setValue(value);
}

/*******************************************************************************
 * @brief   Black Level value changed on spin box
 *
 * @param   arg1            Black Level value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_spinBox_BlackLevel_valueChanged(int arg1)
{
    ui->horizontalSlider_BlackLevel->setValue(arg1);
    param.black_level = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_BLACK_LEVEL);
    }
}

/*******************************************************************************
 * @brief   LSC(Lens Shading Correction) toggled on group box
 *
 * @param   arg1            disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_groupBox_LSC_toggled(bool arg1)
{
    param.enable_lsc = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_LSC);
    }
}

/*******************************************************************************
 * @brief   LSC(Lens Shading Correction) Table1 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_LSC_Table1_clicked()
{
    param.lsc_table_number = 1;

    if (initialized)
    {
        DemoControl(&param, CMD_LSC);
    }
}

/*******************************************************************************
 * @brief   LSC(Lens Shading Correction) Table2 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_LSC_Table2_clicked()
{
    param.lsc_table_number = 2;

    if (initialized)
    {
        DemoControl(&param, CMD_LSC);
    }
}

/*******************************************************************************
 * @brief   LSC(Lens Shading Correction) Table3 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_LSC_Table3_clicked()
{
    param.lsc_table_number = 3;

    if (initialized)
    {
        DemoControl(&param, CMD_LSC);
    }
}

/*******************************************************************************
 * @brief   Defect Pixel Mapped couplet correction toggled on check box
 *
 * @param   checked         disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_checkBox_DefectPixel_Mapped_toggled(bool checked)
{
    param.defect_pixel_mapped = checked;

    if (initialized)
    {
        DemoControl(&param, CMD_DEFECT_PIXEL);
    }
}

/*******************************************************************************
 * @brief   Defect Pixel Dynamic singlet correction toggled on check box
 *
 * @param   checked         disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_checkBox_DefectPixel_Dynamic_toggled(bool checked)
{
    param.defect_pixel_dynamic = checked;

    if (initialized)
    {
        DemoControl(&param, CMD_DEFECT_PIXEL);
    }
}

/*******************************************************************************
 * @brief   Gamma value toggled on group box
 *
 * @param   arg1            disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_groupBox_Gamma_toggled(bool arg1)
{
    param.enable_gamma = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_GAMMA);
    }
}

/*******************************************************************************
 * @brief   Gamma value changed on horizontal slider
 *
 * @param   value           Gamma value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_Gamma_valueChanged(int value)
{
    ui->doubleSpinBox_Gamma->setValue(value/100.0);
}

/*******************************************************************************
 * @brief   Gamma value changed on spin box(double)
 *
 * @param   arg1            Gamma value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_Gamma_valueChanged(double arg1)
{
    ui->horizontalSlider_Gamma->setValue((int)(arg1*100.0 + 0.5));
    param.gamma_value = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_GAMMA);
    }
}

/*******************************************************************************
 * @brief   sRGB Gamma value toggled on group box
 *
 * @param   checked         disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_checkBox_sRGB_Gamma_toggled(bool checked)
{
    param.enable_srgb_gamma = checked;

    if (checked)
    {
        ui->horizontalSlider_Gamma->setEnabled(false);
        ui->doubleSpinBox_Gamma->setEnabled(false);
    }
    else
    {
        ui->horizontalSlider_Gamma->setEnabled(true);
        ui->doubleSpinBox_Gamma->setEnabled(true);
    }

    if (initialized)
    {
        DemoControl(&param, CMD_GAMMA);
    }
}

/*******************************************************************************
 * @brief   Histogram toggled on check box
 *
 * @param   checked         disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_checkBox_Histogram_toggled(bool checked)
{
    param.display_histogram = checked;

    if (initialized)
    {
        DemoControl(&param, CMD_HISTOGRAM);
    }
}

/*******************************************************************************
 * @brief   Contrast toggled on group box
 *
 * @param   arg1            disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_groupBox_Contrast_toggled(bool arg1)
{
    param.enable_contrast = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CONTRAST);
    }
}

/*******************************************************************************
 * @brief   Contrast value changed on horizontal slider
 *
 * @param   value           Contrast value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_Contrast_valueChanged(int value)
{
    ui->spinBox_Contrast->setValue(value);
}

/*******************************************************************************
 * @brief   Contrast value changed on spin box
 *
 * @param   arg1            Contrast value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_spinBox_Contrast_valueChanged(int arg1)
{
    ui->horizontalSlider_Contrast->setValue(arg1);
    param.contrast_value = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_CONTRAST);
    }
}

/*******************************************************************************
 * @brief   Direct clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_Direct_clicked()
{
#ifdef CA378_AOIS
    param.focus_position = param.manual_focus_position;
    param.focus_mode = FOCUS_MODE_DIRECT;
    QString str;
    str.sprintf("%d", param.focus_position);
    ui->horizontalSlider_Focus->setEnabled(true);
    ui->spinBox_Focus->setReadOnly(false);
    ui->spinBox_Focus->setValue(param.focus_position);
#endif
}

/*******************************************************************************
 * @brief   Infinity clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_Infinity_clicked()
{
#ifdef CA378_AOIS
    param.focus_mode = FOCUS_MODE_INFINITY;
    ui->horizontalSlider_Focus->setEnabled(false);
    ui->spinBox_Focus->setReadOnly(true);
    ui->spinBox_Focus->setValue(0);
#endif
}

/*******************************************************************************
 * @brief   Macro clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_Macro_clicked()
{
#ifdef CA378_AOIS
    param.focus_mode = FOCUS_MODE_MACRO;
    ui->horizontalSlider_Focus->setEnabled(false);
    ui->spinBox_Focus->setReadOnly(true);
    ui->spinBox_Focus->setValue(1023);
#endif
}

/*******************************************************************************
 * @brief   Focus value changed on horizontal slider
 *
 * @param   value           Focus value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_Focus_valueChanged(int value)
{
#ifdef CA378_AOIS
    ui->spinBox_Focus->setValue(value);
#endif
}

/*******************************************************************************
 * @brief   Focus value changed on spin box
 *
 * @param   arg1            Focus value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_spinBox_Focus_valueChanged(int arg1)
{
#ifdef CA378_AOIS
    ui->horizontalSlider_Focus->setValue(arg1);
    param.focus_position = arg1;

    if (initialized)
    {
        if (param.enable_auto_focus == 0)
        {
            DemoControl(&param, CMD_MANUAL_FOCUS);
        }
    }
#endif
}

/*******************************************************************************
 * @brief   Auto Focus toggled on check box
 *
 * @param   checked         disable/enable
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_checkBox_AutoFocus_toggled(bool checked)
{
#ifdef CA378_AOIS
    if (initialized)
    {
        if (checked == true)
        {
            ui->spinBox_Focus->setReadOnly(true);
            ui->horizontalSlider_Focus->setEnabled(false);
            ui->radioButton_Direct->setEnabled(false);
            ui->radioButton_Infinity->setEnabled(false);
            ui->radioButton_Macro->setEnabled(false);
            QApplication::processEvents();
            this->update();

            param.enable_auto_focus = checked;
            DemoControl(&param, CMD_AUTO_FOCUS);

            while (param.enable_auto_focus)
            {
                DemoControl(&param, CMD_GET_AF_POSITION);
                ui->spinBox_Focus->setValue(param.focus_position);
                QApplication::processEvents();
                Sleep(500);
            }

            param.manual_focus_position = param.focus_position;
            DemoControl(&param, CMD_MANUAL_FOCUS);
            ui->horizontalSlider_Focus->setValue(param.focus_position);
        }
        else
        {
            param.enable_auto_focus = checked;
            DemoControl(&param, CMD_AUTO_FOCUS);

            ui->spinBox_Focus->setReadOnly(false);
            ui->horizontalSlider_Focus->setEnabled(true);
            ui->radioButton_Direct->setEnabled(true);
            ui->radioButton_Infinity->setEnabled(true);
            ui->radioButton_Macro->setEnabled(true);
        }
    }
#endif
}

/*******************************************************************************
 * @brief   Auto Focus Gain value changed on horizontal slider
 *
 * @param   value           Auto Focus Gain value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_horizontalSlider_AutoFocusGain_valueChanged(int value)
{
#ifdef CA378_AOIS
    ui->doubleSpinBox_AutoFocusGain->setValue(value/10.0);
#endif
}

/*******************************************************************************
 * @brief   Auto Focus Gain value changed on spin box(double)
 *
 * @param   arg1            Auto Focus Gain value
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_doubleSpinBox_AutoFocusGain_valueChanged(double arg1)
{
#ifdef CA378_AOIS
    ui->horizontalSlider_AutoFocusGain->setValue((int)(arg1*10.0 + 0.5));
    param.auto_focus_gain = arg1;

    if (initialized)
    {
        DemoControl(&param, CMD_AUTO_FOCUS);
    }
#endif
}

/*******************************************************************************
 * @brief   OIS mode 0 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode0_clicked()
{
#ifdef CA378_AOIS
    param.ois_mode = 0;

    if (initialized)
    {
        DemoControl(&param, CMD_OIS);
    }
#endif
}

/*******************************************************************************
 * @brief   OIS mode 1 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode1_clicked()
{
#ifdef CA378_AOIS
    param.ois_mode = 1;

    if (initialized)
    {
        DemoControl(&param, CMD_OIS);
    }
#endif
}

/*******************************************************************************
 * @brief   OIS mode 2 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode2_clicked()
{
#ifdef CA378_AOIS
    param.ois_mode = 2;

    if (initialized)
    {
        DemoControl(&param, CMD_OIS);
    }
#endif
}

/*******************************************************************************
 * @brief   OIS mode 3 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode3_clicked()
{
#ifdef CA378_AOIS
    param.ois_mode = 3;

    if (initialized)
    {
        DemoControl(&param, CMD_OIS);
    }
#endif
}

/*******************************************************************************
 * @brief   OIS mode 4 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode4_clicked()
{
#ifdef CA378_AOIS
    param.ois_mode = 4;

    if (initialized)
    {
        DemoControl(&param, CMD_OIS);
    }
#endif
}

/*******************************************************************************
 * @brief   OIS mode 5 clicked on radio button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_radioButton_OIS_Mode5_clicked()
{
#ifdef CA378_AOIS
    param.ois_mode = 5;

    if (initialized)
    {
        DemoControl(&param, CMD_OIS);
    }
#endif
}

/*******************************************************************************
 * @brief   EEPROM Table number activated on combo box
 *
 * @param   index           EEPROM Table number
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_comboBox_EEPROM_Table_activated(int index)
{
    param.eeprom_table_number = index + 1;
}

/*******************************************************************************
 * @brief   EEPROM Save clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_EEPROM_Save_clicked()
{
    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_SAVE);
    }
}

/*******************************************************************************
 * @brief   EEPROM Load clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_EEPROM_Load_clicked()
{
    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_LOAD);
    }

    initialized = false;

    //
    // Sensor
    //

    // Exposure
    double exposure = 1.0 / ((double)(param.exposure + 20) * LINE_LENGTH * 2 / PIXEL_RATE);
    ui->spinBox_Exposure->setValue((int)exposure);
    // Analog Gain
    ui->doubleSpinBox_AnalogGain->setValue(param.analog_gain);
    // White Balance
    ui->doubleSpinBox_WhiteBalance_R->setValue(param.white_balance_r);
    ui->doubleSpinBox_WhiteBalance_G->setValue(param.white_balance_g);
    ui->doubleSpinBox_WhiteBalance_B->setValue(param.white_balance_b);
    // Black Level
    ui->groupBox_BlackLevel->setChecked(param.enable_black_level);
    ui->spinBox_BlackLevel->setValue(param.black_level);
    // Lens Shading Correction
    ui->groupBox_LSC->setChecked(param.enable_lsc);
    switch (param.lsc_table_number)
    {
    case 1:
        ui->radioButton_LSC_Table1->setChecked(true);
        break;
    case 2:
        ui->radioButton_LSC_Table2->setChecked(true);
        break;
    case 3:
        ui->radioButton_LSC_Table2->setChecked(true);
        break;
    default:
        ui->radioButton_LSC_Table1->setChecked(false);
        ui->radioButton_LSC_Table2->setChecked(false);
        ui->radioButton_LSC_Table3->setChecked(false);
        break;
    }
    // Defect Piexel
    ui->checkBox_DefectPixel_Mapped->setChecked(param.defect_pixel_mapped);
    ui->checkBox_DefectPixel_Dynamic->setChecked(param.defect_pixel_dynamic);

    initialized = true;
}

/*******************************************************************************
 * @brief   EEPROM Default clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_EEPROM_Default_clicked()
{
    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_DEFAULT);
    }
}

/*******************************************************************************
 * @brief   EEPROM Clear clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_EEPROM_Clear_clicked()
{
    if (initialized)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "EEPROM Clear",
            "Are you sure you want to clear all the settings recorded in the EEPROM ?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            DemoControl(&param, CMD_EEPROM_CLEAR);
        }
    }
}

/*******************************************************************************
 * @brief   LSC Upload1 clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_LSC_Upload1_clicked()
{
    QString selFilter = tr("LSC(Lens Shadind Correction) Table(*.csv)");
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open LSC Table"),
        ui->textEdit_LSC_Path1->toPlainText(),
        tr("LSC(Lens Shadind Correction) Table(*.csv);;All(*.*)"),
        &selFilter,
        QFileDialog::DontUseCustomDirectoryIcons
    );
    if (fileName.isEmpty()) {
        // Cancel
        return;
    }

    QDir dir(NULL);

    ui->textEdit_LSC_Path1->setText(dir.relativeFilePath(fileName));
    ui->textEdit_LSC_Path1->setToolTip(fileName);

    param.save_lsc_table_number = 1;
    param.lsc_table1_path = ui->textEdit_LSC_Path1->toPlainText().toStdString();

    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_UPLOAD_LSC);
    }
}

/*******************************************************************************
 * @brief   LSC Upload2 clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_LSC_Upload2_clicked()
{
    QString selFilter = tr("LSC(Lens Shadind Correction) Table(*.csv)");
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open LSC Table"),
        ui->textEdit_LSC_Path2->toPlainText(),
        tr("LSC(Lens Shadind Correction) Table(*.csv);;All(*.*)"),
        &selFilter,
        QFileDialog::DontUseCustomDirectoryIcons
    );
    if (fileName.isEmpty()) {
        // Cancel
        return;
    }

    QDir dir(NULL);

    ui->textEdit_LSC_Path2->setText(dir.relativeFilePath(fileName));
    ui->textEdit_LSC_Path2->setToolTip(fileName);

    param.save_lsc_table_number = 2;
    param.lsc_table1_path = ui->textEdit_LSC_Path1->toPlainText().toStdString();

    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_UPLOAD_LSC);
    }
}

/*******************************************************************************
 * @brief   LSC Upload3 clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_LSC_Upload3_clicked()
{
    QString selFilter = tr("LSC(Lens Shadind Correction) Table(*.csv)");
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open LSC Table"),
        ui->textEdit_LSC_Path3->toPlainText(),
        tr("LSC(Lens Shadind Correction) Table(*.csv);;All(*.*)"),
        &selFilter,
        QFileDialog::DontUseCustomDirectoryIcons
    );
    if (fileName.isEmpty()) {
        // Cancel
        return;
    }

    QDir dir(NULL);

    ui->textEdit_LSC_Path3->setText(dir.relativeFilePath(fileName));
    ui->textEdit_LSC_Path3->setToolTip(fileName);

    param.save_lsc_table_number = 3;
    param.lsc_table1_path = ui->textEdit_LSC_Path1->toPlainText().toStdString();

    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_UPLOAD_LSC);
    }
}

/*******************************************************************************
 * @brief   LSC Download1 clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_LSC_Download1_clicked()
{
    QString selFilter = tr("LSC(Lens Shadind Correction) Table(*.csv)");
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Open LSC Table"),
        ui->textEdit_LSC_Path1->toPlainText(),
        tr("LSC(Lens Shadind Correction) Table(*.csv);;All(*.*)"),
        &selFilter,
        QFileDialog::DontUseCustomDirectoryIcons
    );
    if (fileName.isEmpty()) {
        // Cancel
        return;
    }

    QDir dir(NULL);

    ui->textEdit_LSC_Path1->setText(dir.relativeFilePath(fileName));
    ui->textEdit_LSC_Path1->setToolTip(fileName);

    param.save_lsc_table_number = 1;
    param.lsc_table1_path = ui->textEdit_LSC_Path1->toPlainText().toStdString();

    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_DOWNLOAD_LSC);
    }
}

/*******************************************************************************
 * @brief   LSC Download2 clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_LSC_Download2_clicked()
{
    QString selFilter = tr("LSC(Lens Shadind Correction) Table(*.csv)");
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Open LSC Table"),
        ui->textEdit_LSC_Path2->toPlainText(),
        tr("LSC(Lens Shadind Correction) Table(*.csv);;All(*.*)"),
        &selFilter,
        QFileDialog::DontUseCustomDirectoryIcons
    );
    if (fileName.isEmpty()) {
        // Cancel
        return;
    }

    QDir dir(NULL);

    ui->textEdit_LSC_Path2->setText(dir.relativeFilePath(fileName));
    ui->textEdit_LSC_Path2->setToolTip(fileName);

    param.save_lsc_table_number = 2;
    param.lsc_table1_path = ui->textEdit_LSC_Path2->toPlainText().toStdString();

    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_DOWNLOAD_LSC);
    }
}

/*******************************************************************************
 * @brief   LSC Download3 clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_LSC_Download3_clicked()
{
    QString selFilter = tr("LSC(Lens Shadind Correction) Table(*.csv)");
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Open LSC Table"),
        ui->textEdit_LSC_Path3->toPlainText(),
        tr("LSC(Lens Shadind Correction) Table(*.csv);;All(*.*)"),
        &selFilter,
        QFileDialog::DontUseCustomDirectoryIcons
    );
    if (fileName.isEmpty()) {
        // Cancel
        return;
    }

    QDir dir(NULL);

    ui->textEdit_LSC_Path3->setText(dir.relativeFilePath(fileName));
    ui->textEdit_LSC_Path3->setToolTip(fileName);

    param.save_lsc_table_number = 3;
    param.lsc_table1_path = ui->textEdit_LSC_Path3->toPlainText().toStdString();

    if (initialized)
    {
        DemoControl(&param, CMD_EEPROM_DOWNLOAD_LSC);
    }
}

/*******************************************************************************
 * @brief   Activated on font combo box
 *
 * @param   arg1            Font family
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_fontComboBox_activated(const QString &arg1)
{
    param.font_family = arg1.toStdString();

    ChangeFontAllWidget();
}

/*******************************************************************************
 * @brief   Font points value changed on spin box
 *
 * @param   arg1            Font points
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_spinBox_FontPoints_valueChanged(int arg1)
{
    param.font_points = arg1;

    ChangeFontAllWidget();
}

/*******************************************************************************
 * @brief   Apply clicked on push button
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::on_pushButton_Apply_clicked()
{
    int barHeight = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME) * 2;

    param.window_left   = ui->spinBox_Left->value();
    param.window_top    = ui->spinBox_Top->value();
    param.window_width  = ui->spinBox_Width->value();
    param.window_height = ui->spinBox_Height->value();

    CheckWindowArea();

    int left   = param.window_left;
    int top    = param.window_top    + barHeight;
    int width  = param.window_width;
    int height = param.window_height - barHeight;

    this->setGeometry(left, top, width, height);
}

/*******************************************************************************
 * @brief   Check Window Area
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::CheckWindowArea()
{
    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    int left     = param.window_left;
    int top      = param.window_top;
    int width    = param.window_width;
    int height   = param.window_height;

    if (width > cxScreen)
    {
        width = cxScreen;
    }
    if (height > cyScreen)
    {
        height = cyScreen;
    }
    if (left + width > cxScreen - 1)
    {
        left = cxScreen - 1 - width;
    }
    if ((top + height) > cyScreen - 1)
    {
        top = cyScreen - 1 - height;
    }

    param.window_left   = left;
    param.window_top    = top;
    param.window_width  = width;
    param.window_height = height;
}

/*******************************************************************************
 * @brief   Change Font All Widget
 *
 * @param   void
 *
 * @return  void
 ******************************************************************************/
void MainWindow::ChangeFontAllWidget()
{
    QList<QWidget*> widgets = findChildren<QWidget*>();
    foreach(QWidget *widget, widgets) {
        widget->setFont(QFont(QString(param.font_family.c_str()), param.font_points));
    }
}

/*******************************************************************************
 * @brief   Resize Event
 *
 * @param   event           resize event
 *
 * @return  void
 ******************************************************************************/
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QRect rect    = this->geometry();
    int barHeight = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME) * 2;

    ui->gridLayout_Expansion->setContentsMargins( QMargins(0, 0, 0, rect.height() / 3));
#ifdef CA378_AOIS
    ui->horizontalLayout_AOIS->setContentsMargins(      QMargins(0, 0, 0, rect.height()     / 2));
#endif
    ui->horizontalLayout_FontWindow->setContentsMargins(QMargins(0, 0, 0, rect.height()     / 2));
    ui->centralWidget->setGeometry(0, 0, rect.width(), rect.height() - ui->statusBar->height());
    ui->tabWidget->setGeometry(0, 0, rect.width(), rect.height() - ui->statusBar->height());

    param.window_width  = rect.width();
    param.window_height = rect.height() + barHeight;
    ui->spinBox_Width->setValue(param.window_width);
    ui->spinBox_Height->setValue(param.window_height);
}

/*******************************************************************************
 * @brief   Move Event
 *
 * @param   event           move event
 *
 * @return  void
 ******************************************************************************/
void MainWindow::moveEvent(QMoveEvent *event)
{
    QRect rect    = this->geometry();
    int barHeight = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME) * 2;

    param.window_left = rect.left();
    param.window_top  = (rect.top() - barHeight);
    ui->spinBox_Left->setValue(param.window_left);
    ui->spinBox_Top->setValue(param.window_top);
}

/*******************************************************************************
 * @brief   Close Event
 *
 * @param   event           close event
 *
 * @return  void
 ******************************************************************************/
void MainWindow::closeEvent(QCloseEvent *event)
{
#ifdef CA378_AOIS
    on_checkBox_AutoFocus_toggled(false);
    QApplication::processEvents();
#endif
}

/*******************************************************************************
 * @brief   Update (Timer Event) For Status Bar
 *
 * @param   event           update event
 *
 * @return  void
 ******************************************************************************/
void MainWindow::update()
{
    DemoControl(&param, CMD_GET_AVERAGE_FPS);
    DemoControl(&param, CMD_GET_CAPTURED_FRAMES);
    QString str;
    str.sprintf("Frames captured: %5lld   Average FPS: %4.1f", (long long int)param.captured_frames, param.average_fps);
    ui->statusBar->showMessage(str);
    QApplication::processEvents();
}
