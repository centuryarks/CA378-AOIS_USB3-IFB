// ExtensionUnit.h : Declaration of the CExtensionUnit

#pragma once
#include "CAExtensionUnit_i.h"
#include <ks.h>
#include <ksproxy.h>
#include <vidcap.h>
#include <ksmedia.h>


// CNodeControl
class CNodeControl :
    public IKsNodeControl
{
public:
    STDMETHOD(put_NodeId) (DWORD dwNodeId);
    STDMETHOD(put_KsControl) (PVOID pKsControl);

    DWORD m_dwNodeId;
    CComPtr<IKsControl> m_pKsControl;
};

// CExtensionUnit
class ATL_NO_VTABLE CExtensionUnit :
    public IExtensionUnit,
    public CComObjectRootEx<CComObjectThreadModel>,
    public CComCoClass<CExtensionUnit, &CLSID_ExtensionUnit>,
    public CNodeControl
{
public:
    CExtensionUnit();
    STDMETHOD(FinalConstruct)();

BEGIN_COM_MAP(CExtensionUnit)
    COM_INTERFACE_ENTRY(IKsNodeControl)
    COM_INTERFACE_ENTRY(IExtensionUnit)
END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    DECLARE_NO_REGISTRY()
    DECLARE_ONLY_AGGREGATABLE(CExtensionUnit)

// IExtensionUnit
public:
   STDMETHOD (get_Info)(
      ULONG ulSize,
      BYTE pInfo[]);
   STDMETHOD (get_InfoSize)(
      ULONG *pulSize);
   STDMETHOD (get_PropertySize)(
      ULONG PropertyId, 
      ULONG *pulSize);
   STDMETHOD (get_Property)(
      ULONG PropertyId, 
      ULONG ulSize, 
      BYTE pValue[]);
   STDMETHOD (put_Property)(
      ULONG PropertyId, 
      ULONG ulSize, 
      BYTE pValue[]);
   STDMETHOD (get_PropertyRange)(
      ULONG PropertyId, 
      ULONG ulSize,
      BYTE pMin[], 
      BYTE pMax[], 
      BYTE pSteppingDelta[], 
      BYTE pDefault[]);
};

// GUID
#define STATIC_PROPSETID_VIDCAP_EXTENSION_UNIT \
    0x31718057, 0xa986, 0x4627, 0xb6, 0x22, 0xaf, 0x50, 0xb3, 0x53, 0x05, 0xef
DEFINE_GUIDSTRUCT("31718057-A986-4627-B622-AF50B35305EF", \
    PROPSETID_VIDCAP_EXTENSION_UNIT);

#define PROPSETID_VIDCAP_EXTENSION_UNIT \
    DEFINE_GUIDNAMED(PROPSETID_VIDCAP_EXTENSION_UNIT)

OBJECT_ENTRY_AUTO(__uuidof(ExtensionUnit), CExtensionUnit)
