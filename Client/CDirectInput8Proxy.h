#ifndef _CDIRECT_INPUT_PROXY_H
#define _CDIRECT_INPUT_PROXY_H

#define DIRECTINPUT_VERSION 0x0800
#include "CDirectInput8DeviceProxy.h"
#include <dinput.h>
#include "CCore.h"

#pragma comment(lib,"Dxguid.lib")

extern CCore	*g_CCore;

class MyDirectInput : public IDirectInput8
{
	private:
		IDirectInput8* p_DI;
	public:
		MyDirectInput(IDirectInput8* di) : p_DI(di)
		{
		}
		~MyDirectInput ( )
		{
			//WriteDebugEvent ( SString( "~MyDirectInput %08x", this ) );
		}

		/*** IUnknown methods ***/
		HRESULT STDMETHODCALLTYPE QueryInterface ( REFIID riid,  LPVOID * ppvObj )
		{
			return p_DI->QueryInterface ( riid, ppvObj );
		}

		ULONG STDMETHODCALLTYPE AddRef ( VOID )
		{
			return p_DI->AddRef ( );
		}

		ULONG STDMETHODCALLTYPE Release ( VOID )
		{
			// Call original function
			ULONG ulRefCount = p_DI->Release ();
			if ( ulRefCount == 0 )
			{
				delete this;
			}
			return ulRefCount;
		}

		HRESULT STDMETHODCALLTYPE EnumDevices ( DWORD a, LPDIENUMDEVICESCALLBACK b, LPVOID c, DWORD d )
		{
			//char buffer[255];
			//sprintf(buffer,"EnumDevices, type: %X",a);
			//MessageBoxA(NULL,buffer,"EnumDevices",MB_OK);
			return p_DI->EnumDevices ( a, b, c, d );
		}

		HRESULT STDMETHODCALLTYPE GetDeviceStatus ( REFGUID a )
		{
			return p_DI->GetDeviceStatus ( a );
		}

		HRESULT STDMETHODCALLTYPE RunControlPanel ( HWND a, DWORD b )
		{
			return p_DI->RunControlPanel ( a, b );
		}

		HRESULT STDMETHODCALLTYPE Initialize ( HINSTANCE a, DWORD b )
		{
			return p_DI->Initialize ( a, b );
		}

		HRESULT STDMETHODCALLTYPE FindDevice ( REFGUID a, LPCSTR b, LPGUID c )
		{
			return p_DI->FindDevice ( a, b, c );
		}

		HRESULT STDMETHODCALLTYPE EnumDevicesBySemantics ( LPCSTR a, LPDIACTIONFORMAT b, LPDIENUMDEVICESBYSEMANTICSCB c, LPVOID d, DWORD e )
		{
			return p_DI->EnumDevicesBySemantics ( a, b, c, d, e );
		}

		HRESULT STDMETHODCALLTYPE ConfigureDevices ( LPDICONFIGUREDEVICESCALLBACK a, LPDICONFIGUREDEVICESPARAMS b, DWORD c, LPVOID d )
		{
			return p_DI->ConfigureDevices ( a, b, c, d );
		}

		STDMETHOD(CreateDevice)(REFGUID rguid, LPDIRECTINPUTDEVICE8 *b, LPUNKNOWN unknown)
        {
				bool isKB	= false;
				if(rguid == GUID_SysKeyboard)
				{
					isKB		= true;
				}
				//char buffer[255];
				//if(rguid == GUID_SysKeyboard)
				//	sprintf(buffer,"CreateDevice, type: sys keyboard");
				//else
				//	sprintf(buffer,"CreateDevice, type: %X",rguid);
				//MessageBoxA(NULL,buffer,"CD",MB_OK);
                //ATLTRACE(TEXT("MyDirectInput8::CreateDevice %x %x %x"), rguid.Data1, rguid.Data2, rguid.Data3);
				//bool IsKeyboard = ( (DWORD)&rguid % 2);
				//sprintf(buffer,"CreateDevice, type: %X IsKeyboard: %d",&rguid,IsKeyboard);
				//MessageBoxA(NULL,buffer,"CD",MB_OK);
                HRESULT hr = p_DI->CreateDevice(rguid, b, unknown);
                if(SUCCEEDED(hr)/* && rguid == GUID_SysKeyboard*/)
                {
                        // Return our own keyboard device that checks for injected keypresses
                        MyDirectDevice* MDD = new MyDirectDevice(*b,isKB);
						*b = MDD;
						if(isKB)
						{
							//g_CCore;
							//*g_CCore->
							//CCore *cCore = CCore::GetSingletonPtr();
							//cCore->m_K = MDD;
						//= MDD;
						}
							//CCore::getInstance()->m_K = MDD;
//							CCore::getInstance()->m_K = MDD;
							//g_Keyboard	= MDD;
                        //::CreateDevice(*device, rguid == GUID_SysKeyboard);
                }

                return hr;
        }
};

#endif