/*
Copyright (c) 2019-2024, CenturyArks
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
#include "usbinfo.h"

/***************************************************************
 *  Declare function for Usb info
 **************************************************************/
BOOL EnumerateHub(USHORT vId, USHORT pId, PUSHORT usbVersoin, PCHAR HubName, ULONG HubNameBytes);
BOOL EnumerateHubPorts(USHORT vId, USHORT pId, PUSHORT usbVersoin, HANDLE hHubDevice, ULONG NumPorts);
BOOL EnumerateHostControllers(USHORT vId, USHORT pId, PUSHORT usbVersoin);

/*******************************************************************************
 * @brief   Get usb version(binary coded decimal)
 *
 * @param   vid             Vendor id
 * @param   pid             Product id
 * @param   usbVersoin      Usb version(binary coded decimal) pointer
 *
 * @return  Success, other Failure
 ******************************************************************************/
BOOL GetUsbVersion(USHORT vId, USHORT pId, PUSHORT usbVersoin)
{
    return EnumerateHostControllers(vId, pId, usbVersoin);
}

/*******************************************************************************
 * @brief   Enumerate host controllers
 *
 * @param   vid             Vendor id
 * @param   pid             Product id
 * @param   usbVersoin      Usb version(binary coded decimal) pointer
 *
 * @return  Success, other Failure
 ******************************************************************************/
BOOL EnumerateHostControllers(USHORT vId, USHORT pId, PUSHORT usbVersoin)
{
    char    HCName[16];
    int     HCNum;
    HANDLE  hHCDev;
    PCHAR   rootHubName;
    BOOL    success, ReturnValue = FALSE;
    ULONG   nBytes;
    USB_ROOT_HUB_NAME   rootHubNameS;
    PUSB_ROOT_HUB_NAME  rootHubNameW;

    for (HCNum = 0; HCNum < NUM_HCS_TO_CHECK; HCNum++)
    {
        wsprintf(HCName, "\\\\.\\HCD%d", HCNum);
        hHCDev = CreateFile(HCName,
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
        if (hHCDev != INVALID_HANDLE_VALUE)
        {
            rootHubNameW = NULL;
            rootHubName = NULL;
            success = DeviceIoControl(hHCDev,
                IOCTL_USB_GET_ROOT_HUB_NAME,
                0,
                0,
                &rootHubNameS,
                sizeof(rootHubNameS),
                &nBytes,
                NULL);
            if (success)
            {
                nBytes = rootHubNameS.ActualLength;
                rootHubNameW = (PUSB_ROOT_HUB_NAME)GlobalAlloc(GPTR, nBytes);
                if (rootHubNameW)
                {
                    success = DeviceIoControl(hHCDev,
                        IOCTL_USB_GET_ROOT_HUB_NAME,
                        NULL,
                        0,
                        rootHubNameW,
                        nBytes,
                        &nBytes,
                        NULL);
                    if (success)
                    {
                        nBytes = WideCharToMultiByte(CP_ACP, 0, rootHubNameW->RootHubName, -1, NULL, 0, NULL, NULL);
                        if (nBytes)
                        {
                            rootHubName = (PCHAR)GlobalAlloc(GPTR, nBytes);
                            if (rootHubName)
                            {
                                nBytes = WideCharToMultiByte(CP_ACP, 0, rootHubNameW->RootHubName, -1, rootHubName, nBytes, NULL, NULL);
                                if (!nBytes)
                                {
                                    GlobalFree(rootHubName);
                                }
                                else
                                {
                                    ReturnValue = EnumerateHub(vId, pId, usbVersoin, rootHubName, nBytes);
                                    if (ReturnValue)
                                    {
                                        CloseHandle(hHCDev);
                                        GlobalFree(rootHubNameW);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    GlobalFree(rootHubNameW);
                }
            }
            CloseHandle(hHCDev);
        }
    }
    return ReturnValue;
}

/*******************************************************************************
 * @brief   Enumerate hub
 *
 * @param   vid             Vendor id
 * @param   pid             Product id
 * @param   usbVersoin      Usb version(binary coded decimal) pointer
 * @param   HubName         Hub name
 * @param   HubNameBytes    Hub name byte count
 *
 * @return  Success, other Failure
 ******************************************************************************/
BOOL EnumerateHub(USHORT vId, USHORT pId, PUSHORT usbVersoin, PCHAR HubName, ULONG HubNameBytes)
{
    HANDLE          hHubDevice;
    CHAR            deviceName[256];
    BOOL            success = FALSE;
    ULONG           nBytes;
    USB_NODE_INFORMATION HubInfo;

    hHubDevice = INVALID_HANDLE_VALUE;
    memset(&HubInfo, 0, sizeof(USB_NODE_INFORMATION));
    strcpy_s(deviceName, sizeof("\\\\.\\"),"\\\\.\\");
    strcpy_s(deviceName + sizeof("\\\\.\\") - 1, HubNameBytes, HubName);
    GlobalFree(HubName);
    hHubDevice = CreateFile(deviceName,
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hHubDevice != INVALID_HANDLE_VALUE)
    {
        success = DeviceIoControl(hHubDevice,
            IOCTL_USB_GET_NODE_INFORMATION,
            &HubInfo,
            sizeof(USB_NODE_INFORMATION),
            &HubInfo,
            sizeof(USB_NODE_INFORMATION),
            &nBytes,
            NULL);
        if (success)
            success = EnumerateHubPorts(vId, pId, usbVersoin, hHubDevice, HubInfo.u.HubInformation.HubDescriptor.bNumberOfPorts);
        CloseHandle(hHubDevice);
    }
    return success;
}

/*******************************************************************************
 * @brief   Enumerate hub ports
 *
 * @param   vid             Vendor id
 * @param   pid             Product id
 * @param   usbVersoin      Usb version(binary coded decimal) pointer
 * @param   hHubDevice      Hub device handle
 * @param   NumPorts        Number of ports
 *
 * @return  Success, other Failure
 ******************************************************************************/
BOOL EnumerateHubPorts(USHORT vId, USHORT pId, PUSHORT usbVersoin, HANDLE hHubDevice, ULONG NumPorts)
{
    ULONG   index;
    BOOL    success, ReturnValue = FALSE;
    ULONG   nBytes;
    PCHAR   extHubName;
    USB_NODE_CONNECTION_INFORMATION    connectionInfo;
    nBytes = sizeof(USB_NODE_CONNECTION_INFORMATION);

    for (index = 1; index <= NumPorts; index++)
    {
        memset(&connectionInfo, 0, sizeof(USB_NODE_CONNECTION_INFORMATION));
        connectionInfo.ConnectionIndex = index;
        success = DeviceIoControl(hHubDevice,
            IOCTL_USB_GET_NODE_CONNECTION_INFORMATION,
            &connectionInfo,
            sizeof(USB_NODE_CONNECTION_INFORMATION),
            &connectionInfo,
            sizeof(USB_NODE_CONNECTION_INFORMATION),
            &nBytes,
            NULL);
        if (!success) continue;
        USHORT PID, VID;
        PID = connectionInfo.DeviceDescriptor.idProduct;
        VID = connectionInfo.DeviceDescriptor.idVendor;

        if ((VID == vId) && (PID == pId))
        {
            *usbVersoin = connectionInfo.DeviceDescriptor.bcdUSB;
            ReturnValue = TRUE;
            break;
        }

        if (connectionInfo.DeviceIsHub)
        {
            USB_NODE_CONNECTION_NAME    extHubNameS;
            PUSB_NODE_CONNECTION_NAME   extHubNameW;
            extHubNameW = NULL;
            extHubNameS.ConnectionIndex = index;
            success = DeviceIoControl(hHubDevice,
                IOCTL_USB_GET_NODE_CONNECTION_NAME,
                &extHubNameS,
                sizeof(extHubNameS),
                &extHubNameS,
                sizeof(extHubNameS),
                &nBytes,
                NULL);
            if (success)
            {
                nBytes = extHubNameS.ActualLength;
                if (nBytes > sizeof(extHubNameS))
                {
                    extHubNameW = (PUSB_NODE_CONNECTION_NAME)GlobalAlloc(GPTR, nBytes);
                    if (extHubNameW)
                    {
                        extHubNameW->ConnectionIndex = index;
                        success = DeviceIoControl(hHubDevice,
                            IOCTL_USB_GET_NODE_CONNECTION_NAME,
                            extHubNameW,
                            nBytes,
                            extHubNameW,
                            nBytes,
                            &nBytes,
                            NULL);
                        if (success)
                        {
                            nBytes = WideCharToMultiByte(CP_ACP, 0, extHubNameW->NodeName, -1, NULL, 0, NULL, NULL);
                            if (nBytes)
                            {
                                extHubName = (PCHAR)GlobalAlloc(GPTR, nBytes);
                                if (extHubName)
                                {
                                    nBytes = WideCharToMultiByte(CP_ACP, 0, extHubNameW->NodeName, -1, extHubName, nBytes, NULL, NULL);
                                    if (!nBytes)
                                    {
                                        GlobalFree(extHubName);
                                        extHubName = NULL;
                                    }
                                    else
                                    {
                                        ReturnValue = EnumerateHub(vId, pId, usbVersoin, extHubName, nBytes);
                                        if (ReturnValue)
                                        {
                                            GlobalFree(extHubNameW);
                                            extHubName = NULL;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        GlobalFree(extHubNameW);
                    }
                }
            }
        }
    }
    return ReturnValue;
}