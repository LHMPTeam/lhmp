// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "d3d8.h"
#include "CDirect3DDevice8Proxy.h"

class CDirect3D8Proxy: public IDirect3D8
{
private:
	IDirect3D8* p_obj;
public:
	CDirect3D8Proxy(IDirect3D8* object) : p_obj(object)
	{
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
	{
		return p_obj->QueryInterface(riid, ppvObj);
	}

	STDMETHOD_(ULONG,AddRef)()
	{
		return p_obj->AddRef();
	}

	STDMETHOD_(ULONG,Release)()
	{
		ULONG count = p_obj->Release();
		if(0 == count)
			delete this;

		return count;
	}
	/***	IDirect3D methods***/

	STDMETHOD(RegisterSoftwareDevice)(void *pIniFunc)
	{
		return p_obj->RegisterSoftwareDevice(pIniFunc);
	}
	STDMETHOD_(UINT,GetAdapterCount)()
	{
		return p_obj->GetAdapterCount();
	}
	STDMETHOD(GetAdapterIdentifier)(UINT AdapterType, DWORD Flags, D3DADAPTER_IDENTIFIER8 *pIden)
	{
		return p_obj->GetAdapterIdentifier(AdapterType,Flags,pIden);
	}
	STDMETHOD_(UINT,GetAdapterModeCount)(UINT Adapter)
	{
		return p_obj->GetAdapterModeCount(Adapter);
	}
	STDMETHOD(EnumAdapterModes)(UINT Adapter,UINT Mode, D3DDISPLAYMODE *pMode)
	{
		return p_obj->EnumAdapterModes(Adapter,Mode,pMode);
	}
	STDMETHOD(GetAdapterDisplayMode)(UINT Adapter,D3DDISPLAYMODE *pMode)
	{
		return p_obj->GetAdapterDisplayMode(Adapter,pMode);
	}
	STDMETHOD(CheckDeviceType)(UINT Adapter,D3DDEVTYPE DT,D3DFORMAT DF,D3DFORMAT BackBufferFormat,BOOL Windowed)
	{
		return p_obj->CheckDeviceType(Adapter,DT,DF,BackBufferFormat,Windowed);
	}
	STDMETHOD(CheckDeviceFormat)(UINT Adapter,D3DDEVTYPE DT,D3DFORMAT AF,DWORD Usage,D3DRESOURCETYPE RT,D3DFORMAT CheckFormat)
	{
		return p_obj->CheckDeviceFormat(Adapter,DT,AF,Usage,RT,CheckFormat);
	}
	STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter,D3DDEVTYPE DT,D3DFORMAT SF,BOOL Windowed,D3DMULTISAMPLE_TYPE MST)
	{
		return p_obj->CheckDeviceMultiSampleType(Adapter,DT,SF,Windowed,MST); 
	}
	STDMETHOD(CheckDepthStencilMatch)(UINT Adapter,D3DDEVTYPE DT,D3DFORMAT AF,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat)
	{
		return p_obj->CheckDepthStencilMatch(Adapter,DT,AF,RenderTargetFormat,DepthStencilFormat);
	}
	STDMETHOD(GetDeviceCaps)(UINT Adapter,D3DDEVTYPE DT,D3DCAPS8 *pCaps)
	{
		return p_obj->GetDeviceCaps(Adapter,DT,pCaps); 
	}
	STDMETHOD_(HMONITOR,GetAdapterMonitor)(UINT Adapter)
	{
		return p_obj->GetAdapterMonitor(Adapter);
	}
	STDMETHOD(CreateDevice)(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice8** ppReturnedDeviceInterface)
	{
		//HWND newHWND = (HWND)*(DWORD*)(0x101C13F4);
		//SetWindowText(newHWND, "Lost-Heaven MP");
		//g_CCore->GetKeyboard()->ApplyHook();
		g_CCore->GetGraphics()->bIsFullscreen = !pPresentationParameters->Windowed;
		HRESULT result = p_obj->CreateDevice(Adapter,DeviceType,hFocusWindow,BehaviorFlags,pPresentationParameters,ppReturnedDeviceInterface);

		if(result == D3D_OK)
		{
			*ppReturnedDeviceInterface = new CDirect3DDevice8Proxy ( *ppReturnedDeviceInterface );
			g_CCore->GetGraphics()->hwnAppWindow = pPresentationParameters->hDeviceWindow;
		}
		return result;
	}
}; 