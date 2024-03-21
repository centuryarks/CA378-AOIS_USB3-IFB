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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include "demo_control.h"


/***************************************************************
 *  Defines for Main window
 **************************************************************/
#define VERSION             "CA_ViewER  ver 1.3.2"

#define PIXEL_RATE      (840000000)
#define LINE_LENGTH     (4375)

/***************************************************************
 *  Declare class for Main window
 **************************************************************/
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_Mode_activated(int index);
    void on_pushButton_StillCapture_clicked();
    void on_pushButton_MovieRecord_clicked();
    void on_spinBox_Exposure_valueChanged(int arg1);
    void on_horizontalSlider_Exposure_valueChanged(int value);
    void on_doubleSpinBox_AnalogGain_valueChanged(double arg1);
    void on_horizontalSlider_AnalogGain_valueChanged(int value);
    void on_horizontalSlider_WhiteBalance_R_valueChanged(int value);
    void on_horizontalSlider_WhiteBalance_G_valueChanged(int value);
    void on_horizontalSlider_WhiteBalance_B_valueChanged(int value);
    void on_doubleSpinBox_WhiteBalance_R_valueChanged(double arg1);
    void on_doubleSpinBox_WhiteBalance_G_valueChanged(double arg1);
    void on_doubleSpinBox_WhiteBalance_B_valueChanged(double arg1);
    void on_groupBox_BlackLevel_toggled(bool arg1);
    void on_horizontalSlider_BlackLevel_valueChanged(int value);
    void on_spinBox_BlackLevel_valueChanged(int arg1);
    void on_groupBox_LSC_toggled(bool arg1);
    void on_radioButton_LSC_Table1_clicked();
    void on_radioButton_LSC_Table2_clicked();
    void on_radioButton_LSC_Table3_clicked();
    void on_checkBox_DefectPixel_Mapped_toggled(bool checked);
    void on_checkBox_DefectPixel_Dynamic_toggled(bool checked);
    void on_groupBox_Gamma_toggled(bool arg1);
    void on_horizontalSlider_Gamma_valueChanged(int value);
    void on_doubleSpinBox_Gamma_valueChanged(double arg1);
    void on_checkBox_sRGB_Gamma_toggled(bool checked);
    void on_checkBox_Histogram_toggled(bool checked);
    void on_groupBox_Contrast_toggled(bool arg1);
    void on_horizontalSlider_Contrast_valueChanged(int value);
    void on_spinBox_Contrast_valueChanged(int arg1);
    void on_groupBox_CCM_toggled(bool arg1);
    void on_doubleSpinBox_CCM1_valueChanged(double arg1);
    void on_doubleSpinBox_CCM2_valueChanged(double arg1);
    void on_doubleSpinBox_CCM3_valueChanged(double arg1);
    void on_doubleSpinBox_CCM4_valueChanged(double arg1);
    void on_doubleSpinBox_CCM5_valueChanged(double arg1);
    void on_doubleSpinBox_CCM6_valueChanged(double arg1);
    void on_doubleSpinBox_CCM7_valueChanged(double arg1);
    void on_doubleSpinBox_CCM8_valueChanged(double arg1);
    void on_doubleSpinBox_CCM9_valueChanged(double arg1);
    void on_radioButton_Direct_clicked();
    void on_radioButton_Infinity_clicked();
    void on_radioButton_Macro_clicked();
    void on_horizontalSlider_Focus_valueChanged(int value);
    void on_spinBox_Focus_valueChanged(int arg1);
    void on_checkBox_AutoFocus_toggled(bool checked);
    void on_horizontalSlider_AutoFocusGain_valueChanged(int value);
    void on_doubleSpinBox_AutoFocusGain_valueChanged(double arg1);
    void on_radioButton_OIS_Mode0_clicked();
    void on_radioButton_OIS_Mode1_clicked();
    void on_radioButton_OIS_Mode2_clicked();
    void on_radioButton_OIS_Mode3_clicked();
    void on_radioButton_OIS_Mode4_clicked();
    void on_radioButton_OIS_Mode5_clicked();
    void on_comboBox_EEPROM_Table_activated(int index);
    void on_pushButton_EEPROM_Save_clicked();
    void on_pushButton_EEPROM_Load_clicked();
    void on_pushButton_EEPROM_Default_clicked();
    void on_pushButton_EEPROM_Clear_clicked();
    void on_pushButton_LSC_Upload1_clicked();
    void on_pushButton_LSC_Upload2_clicked();
    void on_pushButton_LSC_Upload3_clicked();
    void on_pushButton_LSC_Download1_clicked();
    void on_pushButton_LSC_Download2_clicked();
    void on_pushButton_LSC_Download3_clicked();
    void on_fontComboBox_activated(const QString &arg1);
    void on_spinBox_FontPoints_valueChanged(int arg1);
    void on_pushButton_Apply_clicked();
    void update();

private:
    void CheckWindowArea();
    void ChangeFontAllWidget();

protected:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    ST_PARAM param;
    bool initialized;
    bool recording;
};

#endif // MAINWINDOW_H
