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

#ifndef MODE_H
#define MODE_H

#define MODE_LIST_LENGTH(array) (sizeof(array) / sizeof(array[0]))

typedef struct                          // mode list structure
{
    int pack_width;                     // RAW10 pack width
    int pack_height;                    // RAW10 pack height
    int width;                          // RGB width
    int height;                         // RGB height
    double fps;                         // fps
} ST_MODE_LIST;

namespace
{
    ST_MODE_LIST MODE_LIST[][6] =           // mode list definition
    {
        {                                   // for USB3
            1700, 3042, 4056, 3040, 10.0,   // mode0 (4056x3040@10)4K3K
            1600, 2162, 3840, 2160, 12.0,   // mode1 (3840x2160@12)4K2K
             800, 1082, 1920, 1080, 48.0,   // mode2 (1920x1080@48)FHD
             //800, 1082, 1920, 1080, 48.0,   // mode3 (1280x720@??)HD
             //800, 1082, 1920, 1080, 48.0,   // mode4 (1024x768@??)XGA
             340,  482,  640,  480, 60.0,   // mode5 (640x480@60)VGA
        },
        {                                   // for USB2
             //800, 1082, 1920, 1080, 48.0,   // mode0 (1280x720@??)HD
             //800, 1082, 1920, 1080, 48.0,   // mode1 (1024x768@??)XGA
             340,  482,  640,  480, 30.0,   // mode2 (640x480@30)VGA
        }
    };
}

enum USB_TYPE{
    USB3,
    USB2
};

/***************************************************************
 *  Global variable
 **************************************************************/
extern USB_TYPE usbtype;

/***************************************************************
 *  Declare function
 **************************************************************/
int GetModeListCount(int usbtype);
int CheckSelectMode(int mode);

#endif // MODE_H
