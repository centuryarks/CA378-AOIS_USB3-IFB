// dllmain.h : Declaration of module class.

class CCAExtensionUnitModule : public CAtlDllModuleT< CCAExtensionUnitModule >
{
public :
    DECLARE_LIBID(LIBID_CAExtensionUnitLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CAEXTENSIONUNIT, "{50BE0117-41C1-480C-AA0B-DE4E82A0CDEC}")
};

extern class CCAExtensionUnitModule _AtlModule;
