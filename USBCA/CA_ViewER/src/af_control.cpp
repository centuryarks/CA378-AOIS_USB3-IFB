/*
Copyright (c) 2017-2018, CenturyArks
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
#include "demo_control.h"
#include "af_control.h"


/***************************************************************
 *  Global variable for AF Control
 **************************************************************/
int TableCount[PDAF_PMAX];
int TableCOF[PDAF_PMAX];
double TablePD[PDAF_PMAX];
int afPosition                   = 0x200;
int dccCount                     = 0;
double dccAve                    = 0.0;
double lastDccAve                = 9999.0;
double AutoFocusGain             = 1.0;
int AutoFocusSlope               = (2048 / 100);
int AutoFocusMoveLimit           = 100;
int AutoFocusConfidenceThreshold = 10;
int AutoFocusAverageNum          = 4;
int EnableAutoFocus              = 0;


/***************************************************************
 *  Property function for AF Control
 **************************************************************/
int GetAfPosition()              { return afPosition;     }
void SetAfPosition(int position) { afPosition = position; }

/*******************************************************************************
 * @brief   Calculate Q6.4
 *
 * @param   data  Input data
 *
 * @return  Calculated value
 ******************************************************************************/
double Q6_4(int data)
{
    double val;

    if (data & 0x400)
    {
        val = -(0x800 - data) / 16.0;
    }
    else
    {
        val = data / 16.0;
    }

    return val;
}

/*******************************************************************************
* @brief    Calculate Defocus Conversion Coefficient (DCC)
 *
 * @param data          Input data
 * @param pdafWidth     PDAF area width
 * @param pdafHeight    PDAF area height
 *
 * @return              Result value
 ******************************************************************************/
double CalcDCC(unsigned char * data, int pdafWidth, int pdafHeight)
{
    int h, w;
    int count  = 0;

    for (h = 0; h < pdafHeight; h++)
    {
        int index = PDAF_HEADER + 0x50 * h;
        for (w = 0; w < pdafWidth; w++)
        {
            int i = index + w * 5;
            int d0 = data[i + 0];
            int d1 = data[i + 1];
            int d2 = data[i + 2];
            int cof = (d0 << 3) + (d1 >> 5);
            int pda = ((d1 & 0x1F) << 6) + (d2 >> 2);

            if (cof >= AutoFocusConfidenceThreshold)
            {
                TablePD[h * pdafWidth + w] += Q6_4(pda);
                TableCOF[h * pdafWidth + w] += cof;
                TableCount[h * pdafWidth + w]++;
                count++;
            }
        }
    }

    return count;
}

/*******************************************************************************
 * @brief   PDAF Control
 *
 * @param   pdData      PDAF data pointer
 * @param   pdSize      PDAF data size
 * @param   pdafWidth   PDAF area width
 * @param   pdafHeight  PDAF area height
 *
 * @return  void
 ******************************************************************************/
void PDAF_Control(unsigned char *pdData, int pdSize, int pdafWidth, int pdafHeight)
{
    if (EnableAutoFocus)
    {
        int i, w, h;

        // Initializing PD table
        if (dccCount == 0)
        {
            for (h = 0; h < pdafHeight; h++)
            {
                for (w = 0; w < pdafWidth; w++)
                {
                    TablePD[h * pdafWidth + w] = 0;
                    TableCOF[h * pdafWidth + w] = 0;
                    TableCount[h * pdafWidth + w] = 0;
                }
            }
        }

        // Calculate Defocus Conversion Coefficient
        for (i = 0; i < pdSize ; i++)
        {
            int ID = pdData[i];
            if (ID == PDAF_ID)
            {
                if (CalcDCC(&pdData[i], pdafWidth, pdafHeight) == 0)
                {
                    return;
                }
                break;
            }
        }
        if (i == pdSize) return;
        dccCount++;

        // Averaging Defocus Conversion Coefficient
        if (dccCount == AutoFocusAverageNum)
        {
            double cofAve = 0.0;

            lastDccAve = dccAve;
            dccCount   = 0;
            dccAve     = 0.0;

            // Calculate PD average
            int count = 0;
            for (h = 0; h < pdafHeight; h++)
            {
                for (w = 0; w < pdafWidth; w++)
                {
                    if (TableCount[h * pdafWidth + w] != 0)
                    {
                        dccAve += TablePD[h * pdafWidth + w]  / TableCount[h * pdafWidth + w];
                        cofAve += TableCOF[h * pdafWidth + w] / TableCount[h * pdafWidth + w];
                        count++;
                    }
                }
            }
            dccAve /= count;
            cofAve /= count;

            // Calculate moving amount of focus
            double defocus = cofAve * dccAve * AutoFocusGain / AutoFocusSlope;
            if (defocus > AutoFocusMoveLimit)  defocus =  AutoFocusMoveLimit;
            if (defocus < -AutoFocusMoveLimit) defocus = -AutoFocusMoveLimit;

            // Calculate AF position
            afPosition -= (int)floor(defocus + 0.5);
            if (afPosition < 0x000) afPosition = 0x000;
            if (afPosition > 0x3FF) afPosition = 0x3FF;

            // Focus direct moving
            DirectMove((uint16_t)afPosition);

#ifdef _DEBUG
            printf("Phase Difference:\n");
            for (h = 0; h < pdafHeight; h++)
            {
                for (w = 0; w < pdafWidth; w++)
                {
                    if (TableCount[h * pdafWidth + w] == 0)
                    {
                        printf("%7.2lf", 0.0);
                    }
                    else
                    {
                        printf("%7.2lf", TablePD[h * pdafWidth + w] / TableCount[h * pdafWidth + w]);
                    }
                }
                printf("\n");
            }

            printf("Confidence:\n");
            for (h = 0; h < pdafHeight; h++)
            {
                for (w = 0; w < pdafWidth; w++)
                {
                    if (TableCount[h * pdafWidth + w] == 0)
                    {
                        printf(" %6d", 0);
                    }
                    else
                    {
                        printf(" %6d", TableCOF[h * pdafWidth + w] / TableCount[h * pdafWidth + w]);
                    }
                }
                printf("\n");
            }

            printf("Average:\r\n  Position:%d, Phase Difference:%6.2lf, Confidence:%6.2lf\r\n", afPosition, dccAve, cofAve);
#endif
        }
    }
}

/*******************************************************************************
 * @brief   Control PDAF
 *
 * @param   param       Parameters
 *
 * @return  void
 ******************************************************************************/
void SetAutoFocus(ST_PARAM* param)
{
    EnableAutoFocus = param->enable_auto_focus;
    AutoFocusGain   = param->auto_focus_gain;
}
