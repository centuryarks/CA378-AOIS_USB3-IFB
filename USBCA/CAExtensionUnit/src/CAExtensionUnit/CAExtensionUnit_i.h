

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CAExtensionUnit.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __CAExtensionUnit_i_h__
#define __CAExtensionUnit_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IExtensionUnit_FWD_DEFINED__
#define __IExtensionUnit_FWD_DEFINED__
typedef interface IExtensionUnit IExtensionUnit;

#endif 	/* __IExtensionUnit_FWD_DEFINED__ */


#ifndef __ExtensionUnit_FWD_DEFINED__
#define __ExtensionUnit_FWD_DEFINED__

#ifdef __cplusplus
typedef class ExtensionUnit ExtensionUnit;
#else
typedef struct ExtensionUnit ExtensionUnit;
#endif /* __cplusplus */

#endif 	/* __ExtensionUnit_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IExtensionUnit_INTERFACE_DEFINED__
#define __IExtensionUnit_INTERFACE_DEFINED__

/* interface IExtensionUnit */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IExtensionUnit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A817E7A2-43FA-11D0-9E44-00AA00B6770A")
    IExtensionUnit : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE get_InfoSize( 
            /* [out] */ ULONG *pulSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Info( 
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ BYTE pInfo[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_PropertySize( 
            /* [in] */ ULONG PropertyId,
            /* [out] */ ULONG *pulSize) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_Property( 
            /* [in] */ ULONG PropertyId,
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ BYTE pValue[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE put_Property( 
            /* [in] */ ULONG PropertyId,
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ BYTE pValue[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE get_PropertyRange( 
            /* [in] */ ULONG PropertyId,
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ BYTE pMin[  ],
            /* [size_is][out][in] */ BYTE pMax[  ],
            /* [size_is][out][in] */ BYTE pSteppingDelta[  ],
            /* [size_is][out][in] */ BYTE pDefault[  ]) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IExtensionUnitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IExtensionUnit * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IExtensionUnit * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IExtensionUnit * This);
        
        HRESULT ( STDMETHODCALLTYPE *get_InfoSize )( 
            IExtensionUnit * This,
            /* [out] */ ULONG *pulSize);
        
        HRESULT ( STDMETHODCALLTYPE *get_Info )( 
            IExtensionUnit * This,
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ BYTE pInfo[  ]);
        
        HRESULT ( STDMETHODCALLTYPE *get_PropertySize )( 
            IExtensionUnit * This,
            /* [in] */ ULONG PropertyId,
            /* [out] */ ULONG *pulSize);
        
        HRESULT ( STDMETHODCALLTYPE *get_Property )( 
            IExtensionUnit * This,
            /* [in] */ ULONG PropertyId,
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ BYTE pValue[  ]);
        
        HRESULT ( STDMETHODCALLTYPE *put_Property )( 
            IExtensionUnit * This,
            /* [in] */ ULONG PropertyId,
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ BYTE pValue[  ]);
        
        HRESULT ( STDMETHODCALLTYPE *get_PropertyRange )( 
            IExtensionUnit * This,
            /* [in] */ ULONG PropertyId,
            /* [in] */ ULONG ulSize,
            /* [size_is][out][in] */ BYTE pMin[  ],
            /* [size_is][out][in] */ BYTE pMax[  ],
            /* [size_is][out][in] */ BYTE pSteppingDelta[  ],
            /* [size_is][out][in] */ BYTE pDefault[  ]);
        
        END_INTERFACE
    } IExtensionUnitVtbl;

    interface IExtensionUnit
    {
        CONST_VTBL struct IExtensionUnitVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IExtensionUnit_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IExtensionUnit_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IExtensionUnit_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IExtensionUnit_get_InfoSize(This,pulSize)	\
    ( (This)->lpVtbl -> get_InfoSize(This,pulSize) ) 

#define IExtensionUnit_get_Info(This,ulSize,pInfo)	\
    ( (This)->lpVtbl -> get_Info(This,ulSize,pInfo) ) 

#define IExtensionUnit_get_PropertySize(This,PropertyId,pulSize)	\
    ( (This)->lpVtbl -> get_PropertySize(This,PropertyId,pulSize) ) 

#define IExtensionUnit_get_Property(This,PropertyId,ulSize,pValue)	\
    ( (This)->lpVtbl -> get_Property(This,PropertyId,ulSize,pValue) ) 

#define IExtensionUnit_put_Property(This,PropertyId,ulSize,pValue)	\
    ( (This)->lpVtbl -> put_Property(This,PropertyId,ulSize,pValue) ) 

#define IExtensionUnit_get_PropertyRange(This,PropertyId,ulSize,pMin,pMax,pSteppingDelta,pDefault)	\
    ( (This)->lpVtbl -> get_PropertyRange(This,PropertyId,ulSize,pMin,pMax,pSteppingDelta,pDefault) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IExtensionUnit_INTERFACE_DEFINED__ */



#ifndef __CAExtensionUnitLib_LIBRARY_DEFINED__
#define __CAExtensionUnitLib_LIBRARY_DEFINED__

/* library CAExtensionUnitLib */
/* [custom][helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_CAExtensionUnitLib;

EXTERN_C const CLSID CLSID_ExtensionUnit;

#ifdef __cplusplus

class DECLSPEC_UUID("BB558C0E-C636-4BC3-992C-B717D498D579")
ExtensionUnit;
#endif
#endif /* __CAExtensionUnitLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


