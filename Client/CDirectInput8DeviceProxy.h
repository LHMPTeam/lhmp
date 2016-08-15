// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __CDIRECT_DEVICE_PROXY_H
#define __CDIRECT_DEVICE_PROXY_H

#include <dinput.h>
extern CCore	*g_CCore;
class MyDirectDevice : public IDirectInputDevice8
{
private:
	IDirectInputDevice8*	p_DID;
	bool					p_bIsKeyboard;
public:
	MyDirectDevice(IDirectInputDevice8* DID,bool isKeyboard = false) : p_DID(DID)
	{
		p_bIsKeyboard = isKeyboard;
	}
	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj)
	{
		return p_DID->QueryInterface(riid, ppvObj);
	}

	STDMETHOD_(ULONG,AddRef)()
	{
		return p_DID->AddRef();
	}

	STDMETHOD_(ULONG,Release)()
	{
		ULONG count = p_DID->Release();
		if(0 == count)
			delete this;

		return count;
	}

	/*** IDirectInputDevice8 methods ***/
	STDMETHOD(GetCapabilities)(LPDIDEVCAPS devCaps)
	{
		return p_DID->GetCapabilities(devCaps);
	}

	STDMETHOD(EnumObjects)(LPDIENUMDEVICEOBJECTSCALLBACK callback, LPVOID ref, DWORD flags)	
	{
		return p_DID->EnumObjects(callback, ref, flags);
	}

	STDMETHOD(GetProperty)(REFGUID rguid, LPDIPROPHEADER ph)
	{
		return p_DID->GetProperty(rguid, ph);
	}

	STDMETHOD(SetProperty)(REFGUID rguid, LPCDIPROPHEADER ph)
	{
		return p_DID->SetProperty(rguid, ph);
	}

	STDMETHOD(Acquire)()
	{
		return p_DID->Acquire();
	}

	STDMETHOD(Unacquire)()
	{
		HRESULT hr = p_DID->Unacquire();
		return hr;
	}

	STDMETHOD(GetDeviceState)(DWORD size, LPVOID data)
	{
		DWORD   dwNumItems = INFINITE;
		HRESULT hr = p_DID->GetDeviceState(size, data);
		//((BYTE *)data)[DIK_TAB] = 0x00;
		if(g_CCore != NULL)
		{
			// Clear the buffer so the game won't get any events.
			if ((g_CCore->GetChat()->IsTyping() || g_CCore->GetGame()->isControlLocked() || g_CCore->GetIngameMenu()->isActive() || g_CCore->GetSquirrel()->isInputBlocked()))
			{
				memset(data, 0, size);
				//p_DID->GetDeviceData(sizeof (DIDEVICEOBJECTDATA), NULL, &dwNumItems, 0);

				//return NULL;
			}
		}
		return hr;
	}

	STDMETHOD(GetDeviceData)(DWORD size, LPDIDEVICEOBJECTDATA data, LPDWORD numElements, DWORD flags)
	{
		return p_DID->GetDeviceData(size, data, numElements, flags);
	}

	STDMETHOD(SetDataFormat)(LPCDIDATAFORMAT df)
	{
		return p_DID->SetDataFormat(df);
	}

	STDMETHOD(SetEventNotification)(HANDLE event)
	{
		return p_DID->SetEventNotification(event);
	}

	STDMETHOD(SetCooperativeLevel)(HWND window, DWORD level)
	{
		return p_DID->SetCooperativeLevel(window, level);
	}

	STDMETHOD(GetObjectInfo)(LPDIDEVICEOBJECTINSTANCE object, DWORD objId, DWORD objHow)
	{
		return p_DID->GetObjectInfo(object, objId, objHow);
	}

	STDMETHOD(GetDeviceInfo)(LPDIDEVICEINSTANCE di)
	{
		return p_DID->GetDeviceInfo(di);
	}

	STDMETHOD(RunControlPanel)(HWND owner, DWORD flags)
	{
		return p_DID->RunControlPanel(owner, flags);
	}

	STDMETHOD(Initialize)(HINSTANCE instance, DWORD version, REFGUID rguid)
	{
		return p_DID->Initialize(instance, version, rguid);
	}

	STDMETHOD(CreateEffect)(REFGUID rguid, LPCDIEFFECT effect_params, LPDIRECTINPUTEFFECT* effect, LPUNKNOWN unknown)
	{
		return p_DID->CreateEffect(rguid, effect_params, effect, unknown);
	}

    STDMETHOD(EnumEffects)(LPDIENUMEFFECTSCALLBACK callback, LPVOID ref, DWORD type)
	{
		return p_DID->EnumEffects(callback, ref, type);
	}

    STDMETHOD(GetEffectInfo)(LPDIEFFECTINFO effect_info, REFGUID rguid)
	{
		return p_DID->GetEffectInfo(effect_info, rguid);
	}

    STDMETHOD(GetForceFeedbackState)(LPDWORD state)
	{
		return p_DID->GetForceFeedbackState(state);
	}

    STDMETHOD(SendForceFeedbackCommand)(DWORD flags)
	{
		return p_DID->SendForceFeedbackCommand(flags);
	}

    STDMETHOD(EnumCreatedEffectObjects)(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK callback, LPVOID ref, DWORD flags)
	{
		return p_DID->EnumCreatedEffectObjects(callback, ref, flags);
	}

    STDMETHOD(Escape)(LPDIEFFESCAPE escape)
	{
		return p_DID->Escape(escape);
	}

    STDMETHOD(Poll)()
	{
		return p_DID->Poll();
	}

	STDMETHOD(SendDeviceData)(DWORD size, LPCDIDEVICEOBJECTDATA data, LPDWORD num_elements, DWORD flags)
	{
		return p_DID->SendDeviceData(size, data, num_elements, flags);
	}

	STDMETHOD(EnumEffectsInFile)(LPCSTR file_name, LPDIENUMEFFECTSINFILECALLBACK callback, LPVOID ref, DWORD flags)
	{
		return p_DID->EnumEffectsInFile(file_name, callback, ref, flags);
	}

    STDMETHOD(WriteEffectToFile)(LPCSTR file_name, DWORD num_entries, LPDIFILEEFFECT effects, DWORD flags)
	{
		return p_DID->WriteEffectToFile(file_name, num_entries, effects, flags);
	}

    STDMETHOD(BuildActionMap)(LPDIACTIONFORMAT format, LPCSTR username, DWORD flags)
	{
		return p_DID->BuildActionMap(format, username, flags);
	}

    STDMETHOD(SetActionMap)(LPDIACTIONFORMAT format, LPCSTR username, DWORD flags)
	{
		return p_DID->SetActionMap(format, username, flags);
	}

    STDMETHOD(GetImageInfo)(LPDIDEVICEIMAGEINFOHEADER image_header)
	{
		return p_DID->GetImageInfo(image_header);
	}
};

#endif