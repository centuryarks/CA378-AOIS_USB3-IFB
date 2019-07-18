#pragma once
#include <vidcap.h>
#include <ksmedia.h>
#include <DShowUtil.h>
#include "CAExtensionUnit_i.h"
#include "errcode.h"


// This class manages Extension Unit.
class ExtensionManager
{
public:
    ExtensionManager(void);
    ~ExtensionManager(void);
    IExtensionUnit* GetExtensionInterface(IBaseFilter* pBaseFilter);
    int SendExtensionUnit(unsigned char* data, int length, int unitNumber);
    int RecvExtensionUnit(unsigned char* data, int length, int unitNumber);

private:
    IExtensionUnit* m_pExtensionInterface;
};
