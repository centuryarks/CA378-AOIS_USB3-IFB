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

#ifndef AF_CONTROL_H
#define AF_CONTROL_H


/***************************************************************
 *  Defines for AF Control
 **************************************************************/
#define PDAF_WMAX   (16)
#define PDAF_HMAX   (12)
#define PDAF_PMAX   (PDAF_WMAX * PDAF_HMAX)
#define PDAF_HEADER (9)
#define PDAF_ID     (0x15)

/***************************************************************
 *  Declare function for AF Control
 **************************************************************/
int GetAfPosition();
void SetAfPosition(int position);
double Q6_4(int data);
double CalcDCC(unsigned char * data, int pdafWidth, int pdafHeight);
void PDAF_Control(unsigned char *pdData, int pdSize, int pdafWidth, int pdafHeight);
void SetAutoFocus(ST_PARAM* param);

#endif // AF_CONTROL_H
