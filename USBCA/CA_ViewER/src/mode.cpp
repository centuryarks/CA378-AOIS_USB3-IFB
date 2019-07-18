/*
Copyright (c) 2019, CenturyArks
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

#include "mode.h"

/***************************************************************
 *  Global variable
 **************************************************************/
USB_TYPE usbtype = USB3;


/*******************************************************************************
 * @brief   Get Mode List Count
 *
 * @param   usbtype       usb type
 *
 * @return  mode list count
 ******************************************************************************/
int GetModeListCount(int usbtype)
{
    int count = 0;
    int i = 0;
    int length = MODE_LIST_LENGTH(MODE_LIST[usbtype]);

    for (i = 0; i < length; i++)
    {
        if (MODE_LIST[usbtype][i].width > 0)
        {
            count++;
        }
    }
    return count;
}

/*******************************************************************************
 * @brief   Check Select Mode
 *
 * @param   mode       select mode
 *
 * @return  mode
 ******************************************************************************/
int CheckSelectMode(int mode)
{
    int count = GetModeListCount(usbtype);
    if (mode < count)
    {
        return mode;
    }
    else
    {
        return 0;
    }
}

