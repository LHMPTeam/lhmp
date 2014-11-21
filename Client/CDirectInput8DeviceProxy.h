#ifndef _CDIRECT_DEVICE_PROXY_H
#define _CDIRECT_DEVICE_PROXY_H

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
		//ATLTRACE(TEXT("MyDirectDevice::QueryInterface"));
		return p_DID->QueryInterface(riid, ppvObj);
	}

	STDMETHOD_(ULONG,AddRef)()
	{
		//ATLTRACE(TEXT("MyDirectDevice::AddRef"));
		return p_DID->AddRef();
	}

	STDMETHOD_(ULONG,Release)()
	{
		//ATLTRACE(TEXT("MyDirectDevice::Release"));
		ULONG count = p_DID->Release();
		if(0 == count)
			delete this;

		return count;
	}

	/*** IDirectInputDevice8 methods ***/
	STDMETHOD(GetCapabilities)(LPDIDEVCAPS devCaps)
	{
		//ATLTRACE(TEXT("MyDirectDevice::GetCapabilities"));
		return p_DID->GetCapabilities(devCaps);
	}

	STDMETHOD(EnumObjects)(LPDIENUMDEVICEOBJECTSCALLBACK callback, LPVOID ref, DWORD flags)	
	{
		//ATLTRACE(TEXT("MyDirectDevice::EnumObjects"));
		return p_DID->EnumObjects(callback, ref, flags);
	}

	STDMETHOD(GetProperty)(REFGUID rguid, LPDIPROPHEADER ph)
	{
		//ATLTRACE(TEXT("MyDirectDevice::GetProperty"));
		return p_DID->GetProperty(rguid, ph);
	}

	STDMETHOD(SetProperty)(REFGUID rguid, LPCDIPROPHEADER ph)
	{
		//ATLTRACE(TEXT("MyDirectDevice::SetProperty"));
		return p_DID->SetProperty(rguid, ph);

		/*
		You can detect immediate/buffered modes as such:
		
		HRESULT hr = p_DID->SetProperty(rguid, ph);

		if(SUCCEEDED(hr) && rguid == DIPROP_BUFFERSIZE)
		{
			DWORD data = *reinterpret_cast<const DWORD*>(ph + 1);
			m_is_immediate = (data == 0);
		}

		return hr;
		*/
	}

	STDMETHOD(Acquire)()
	{
		HRESULT hr = p_DID->Acquire();

		return hr;
	}

	STDMETHOD(Unacquire)()
	{
		HRESULT hr = p_DID->Unacquire();
		//ATLTRACE(TEXT("MyDirectDevice::Unacquire %d"), hr);
		return hr;
	}

	STDMETHOD(GetDeviceState)(DWORD size, LPVOID data)
	{
		/*if(p_bIsKeyboard)
		{
			DIDEVICEOBJECTDATA	didod;
			DWORD dwNumItems1 = 1;
			p_DID->GetDeviceData ( sizeof ( DIDEVICEOBJECTDATA ), &didod, &dwNumItems1, 0 );
			char buffer[255];
			sprintf(buffer,"GetDeviceState: %x %x",didod.dwOfs,didod.dwData);
			g_CCore->GetChat()->
			(buffer);
		}*/
		//g_CCore->GetChat()->AddMessage("GetDeviceState call");
		HRESULT hr = p_DID->GetDeviceState(size, data);
		if(g_CCore != NULL)
		{
			if(p_bIsKeyboard)
			{
				//g_CCore->GetChat()->ProceedKeyboard(data);
				g_CCore->GetKeyboard()->ProceedKeyboard(data);
	//			((BYTE *) data)[DIK_A] = 0x80;  // Fake press the ‘A’ key    
				((BYTE *) data)[DIK_TAB] = 0x00;  // Fake press the ‘A’ key    
				((BYTE *) data)[DIK_ESCAPE] = 0x00;  // Fake press the ‘A’ key   
			}
			/*if(g_CCore->GetChat()->IsTyping() || g_CCore->GetIngameMenu()->isActive())
			{
				DWORD   dwNumItems  = INFINITE;
				memset ( data, 0, size );
				p_DID->GetDeviceData ( sizeof ( DIDEVICEOBJECTDATA ), NULL, &dwNumItems, 0 );
			}*/
		}
		return hr;
	}

	STDMETHOD(GetDeviceData)(DWORD size, LPDIDEVICEOBJECTDATA data, LPDWORD numElements, DWORD flags)
	{
		//g_CCore->GetChat()->AddMessage("GetDeviceData call");
		//ATLTRACE(TEXT("MyDirectDevice::GetDeviceData"));
		return p_DID->GetDeviceData(size, data, numElements, flags);
	}

	STDMETHOD(SetDataFormat)(LPCDIDATAFORMAT df)
	{
		//ATLTRACE(TEXT("MyDirectDevice::SetDataFormat"));
		return p_DID->SetDataFormat(df);
	}

	STDMETHOD(SetEventNotification)(HANDLE event)
	{
		//ATLTRACE(TEXT("MyDirectDevice::SetEventNotification"));
		return p_DID->SetEventNotification(event);
	}

	STDMETHOD(SetCooperativeLevel)(HWND window, DWORD level)
	{
		//ATLTRACE(TEXT("MyDirectDevice::SetCooperativeLevel"));
		return p_DID->SetCooperativeLevel(window, level);
	}

	STDMETHOD(GetObjectInfo)(LPDIDEVICEOBJECTINSTANCE object, DWORD objId, DWORD objHow)
	{
		//ATLTRACE(TEXT("MyDirectDevice::GetObjectInfo"));
		return p_DID->GetObjectInfo(object, objId, objHow);
	}

	STDMETHOD(GetDeviceInfo)(LPDIDEVICEINSTANCE di)
	{
		//ATLTRACE(TEXT("MyDirectDevice::GetDeviceInfo"));
		return p_DID->GetDeviceInfo(di);
	}

	STDMETHOD(RunControlPanel)(HWND owner, DWORD flags)
	{
		//ATLTRACE(TEXT("MyDirectDevice::RunControlPanel"));
		return p_DID->RunControlPanel(owner, flags);
	}

	STDMETHOD(Initialize)(HINSTANCE instance, DWORD version, REFGUID rguid)
	{
		//ATLTRACE(TEXT("MyDirectDevice::Initialize"));
		return p_DID->Initialize(instance, version, rguid);
	}

	STDMETHOD(CreateEffect)(REFGUID rguid, LPCDIEFFECT effect_params, LPDIRECTINPUTEFFECT* effect, LPUNKNOWN unknown)
	{
		//ATLTRACE(TEXT("MyDirectDevice::CreateEffect"));
		return p_DID->CreateEffect(rguid, effect_params, effect, unknown);
	}

    STDMETHOD(EnumEffects)(LPDIENUMEFFECTSCALLBACK callback, LPVOID ref, DWORD type)
	{
		//ATLTRACE(TEXT("MyDirectDevice::EnumEffects"));
		return p_DID->EnumEffects(callback, ref, type);
	}

    STDMETHOD(GetEffectInfo)(LPDIEFFECTINFO effect_info, REFGUID rguid)
	{
		//ATLTRACE(TEXT("MyDirectDevice::GetEffectInfo"));
		return p_DID->GetEffectInfo(effect_info, rguid);
	}

    STDMETHOD(GetForceFeedbackState)(LPDWORD state)
	{
		//ATLTRACE(TEXT("MyDirectDevice::GetForceFeedbackState"));
		return p_DID->GetForceFeedbackState(state);
	}

    STDMETHOD(SendForceFeedbackCommand)(DWORD flags)
	{
	//	ATLTRACE(TEXT("MyDirectDevice::SendForceFeedbackCommand"));
		return p_DID->SendForceFeedbackCommand(flags);
	}

    STDMETHOD(EnumCreatedEffectObjects)(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK callback, LPVOID ref, DWORD flags)
	{
		//ATLTRACE(TEXT("MyDirectDevice::EnumCreatedEffectObjects"));
		return p_DID->EnumCreatedEffectObjects(callback, ref, flags);
	}

    STDMETHOD(Escape)(LPDIEFFESCAPE escape)
	{
		//ATLTRACE(TEXT("MyDirectDevice::Escape"));
		return p_DID->Escape(escape);
	}

    STDMETHOD(Poll)()
	{
		return p_DID->Poll();
	}

	STDMETHOD(SendDeviceData)(DWORD size, LPCDIDEVICEOBJECTDATA data, LPDWORD num_elements, DWORD flags)
	{
		//ATLTRACE(TEXT("MyDirectDevice::SendDeviceData"));
		return p_DID->SendDeviceData(size, data, num_elements, flags);
	}

	STDMETHOD(EnumEffectsInFile)(LPCSTR file_name, LPDIENUMEFFECTSINFILECALLBACK callback, LPVOID ref, DWORD flags)
	{
		//ATLTRACE(TEXT("MyDirectDevice::EnumEffectsInFile"));
		return p_DID->EnumEffectsInFile(file_name, callback, ref, flags);
	}

    STDMETHOD(WriteEffectToFile)(LPCSTR file_name, DWORD num_entries, LPDIFILEEFFECT effects, DWORD flags)
	{
		//ATLTRACE(TEXT("MyDirectDevice::WriteEffectToFile"));
		return p_DID->WriteEffectToFile(file_name, num_entries, effects, flags);
	}

    STDMETHOD(BuildActionMap)(LPDIACTIONFORMAT format, LPCSTR username, DWORD flags)
	{
		//ATLTRACE(TEXT("MyDirectDevice::BuildActionMap"));
		return p_DID->BuildActionMap(format, username, flags);
	}

    STDMETHOD(SetActionMap)(LPDIACTIONFORMAT format, LPCSTR username, DWORD flags)
	{
		//(TEXT("MyDirectDevice::SetActionMap"));
		return p_DID->SetActionMap(format, username, flags);
	}

    STDMETHOD(GetImageInfo)(LPDIDEVICEIMAGEINFOHEADER image_header)
	{
		//ATLTRACE(TEXT("MyDirectDevice::GetImageInfo"));
		return p_DID->GetImageInfo(image_header);
	}
};

#endif