#include "StdAfx.h"
#include "ExtensionManager.h"


ExtensionManager::ExtensionManager(void)
{
    HRESULT hr = S_OK;
    CHAR    videoDevName[20][MAX_PATH];
    UINT    uIndex = 0;
    ULONG   cFetched;
    size_t  returnValue;
    ICreateDevEnum  *pCreateDevEnum = NULL;
    IEnumMoniker    *pEm            = NULL;
    IMoniker        *pM             = NULL;
    IBaseFilter     *pVCap          = NULL;
    m_pExtensionInterface           = NULL;

    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // Enumerate all video capture devices
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (FAILED(hr))
    {
        perror("Error Creating Device Enumerator\r\n");
        return;
    }

    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (FAILED(hr))
    {
        perror("Sorry, you have no video capture hardware.\r\n");
        perror("Video capture will not function properly.\r\n");
        return;
    }

    pEm->Reset();

    // Get all video devices
    while (hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
    {
        IPropertyBag *pBag = NULL;

        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if (SUCCEEDED(hr))
        {
            VARIANT var;
            var.vt = VT_BSTR;

            // Get the device names
            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if (SUCCEEDED(hr))
            {
                wcstombs_s(&returnValue, videoDevName[uIndex], MAX_PATH, var.bstrVal, MAX_PATH);
                SysFreeString(var.bstrVal);
            }
            pBag->Release();

            // Store the App note firmware (Whose name is *CX3*) device index
            if (strstr(videoDevName[uIndex], "USBCA") != NULL)
            {
                hr = pM->BindToObject(0, 0, IID_IBaseFilter, (void**)&pVCap);
            }
        }

        pM->Release();
        uIndex++;
    }

    pEm->Release();

    if (pVCap == NULL)
    {
        return;
    }

    m_pExtensionInterface = GetExtensionInterface(pVCap);
}

ExtensionManager::~ExtensionManager(void)
{
    if (m_pExtensionInterface)
    {
        m_pExtensionInterface->Release();
    }

    CoUninitialize();
}

IExtensionUnit* ExtensionManager::GetExtensionInterface(IBaseFilter* pBaseFilter)
{
    IKsTopologyInfo *pKsTopologyInfo = NULL;
    IExtensionUnit  *pExtensionUnit  = NULL;

    do
    {
        HRESULT hr;
        int i;

        // pUnkOuter: pointer to the capture filter
        if (pBaseFilter == NULL)
        {
            perror("No capture filter opened!\r\n");
            break;
        }

        hr = pBaseFilter->QueryInterface( __uuidof(IKsTopologyInfo), (void **) &pKsTopologyInfo);

        if ((FAILED(hr)) || pKsTopologyInfo == NULL)
        {
            perror("Unable to get IKsTopologyInfo\r\n");
            break;
        }

        // Find extension node with GUID
        DWORD nodeCount = 0;
        DWORD extensionNode = (DWORD)-1;
        hr = pKsTopologyInfo->get_NumNodes(&nodeCount);
        for (i = 0; i < (int)nodeCount; i++)
        {
            GUID type;
            WCHAR buf[128];
            DWORD len = 0;
            pKsTopologyInfo->get_NodeName(i, buf, 128, &len);
            buf[len] = 0;
            pKsTopologyInfo->get_NodeType(i, &type);
            if (type == KSNODETYPE_DEV_SPECIFIC)
            {
                extensionNode = i;
                break;
            }
        }

        if (extensionNode < 0)
        {
            break;
        }

        // Create node instance
        hr = pKsTopologyInfo->CreateNodeInstance(
                 extensionNode,
                 __uuidof(IExtensionUnit),
                 (void **) &pExtensionUnit);
        if (FAILED(hr))
        {
            LPOLESTR lpolestr;
            StringFromCLSID(__uuidof(IExtensionUnit), &lpolestr);
            printf("Unable to create extension node instance : %x\r\n", hr);
            wprintf_s(L"GUID = %s\r\n", lpolestr);
            CoTaskMemFree(lpolestr);
            break;
        }

    }
    while (0);

    if (pKsTopologyInfo)
    {
        pKsTopologyInfo->Release();
    }

    return pExtensionUnit;
};

// Send data into Extension Unit
int ExtensionManager::SendExtensionUnit(unsigned char* data, int length, int unitNumber)
{
    IExtensionUnit* pExtensionUnit = m_pExtensionInterface;
    if (pExtensionUnit)
    {
        HRESULT hr;
        int sz = 0;

        hr = pExtensionUnit->get_PropertySize(unitNumber, (ULONG*)&sz);

        if (sz > 0 || sz == (int)length)
        {
            BYTE* pData = (BYTE*) malloc(length);
            for (int i = 0; i < length; i++)
            {
                pData[i] = data[i];
            }
            hr = pExtensionUnit->put_Property(unitNumber, sz, (BYTE*)pData);

            free(pData);

            if (hr != 0) return hr;
        }
        else if (sz == -1)
        {
            perror("Put Property Failed.");
            return CA378_ERR_EXT_UNIT_FAIL;
        }
        else if (sz == 0)
        {
            return CA378_ERR_EXT_UNIT_TIMEOUT;
        }
        else
        {
            // different size
            return CA378_ERR_EXT_UNIT_BAD_SIZE;
        }

        return 0;
    }
    else
    {
        return CA378_ERR_EXT_UNIT_NOT_FOUND;
    }
};

// Recv data into Extension Unit
int ExtensionManager::RecvExtensionUnit(unsigned char* data, int length, int unitNumber)
{
    IExtensionUnit* pExtensionUnit = m_pExtensionInterface;
    if (pExtensionUnit)
    {
        HRESULT hr;
        int sz;
        hr = pExtensionUnit->get_PropertySize(unitNumber, (ULONG*)&sz);
        if (length <= sz)
        {
            hr = pExtensionUnit->get_Property(unitNumber, sz, (BYTE*)data);
        }
        else if (sz == -1)
        {
            perror("Put Property Failed.\r\n");
            return CA378_ERR_EXT_UNIT_FAIL;
        }
        else if (sz == 0)
        {
            return CA378_ERR_EXT_UNIT_TIMEOUT;
        }
        else
        {
            // different size
            return CA378_ERR_EXT_UNIT_BAD_SIZE;
        }

        return 0;
    }
    else
    {
        return CA378_ERR_EXT_UNIT_NOT_FOUND;
    }
};
