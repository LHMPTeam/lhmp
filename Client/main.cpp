#include "CCore.h"
#include "directx8hook.h"
#include "CDirectInput8Proxy.h"
#include "CDirect3D8Proxy.h"
#include "CHooks.h"
#include <Psapi.h>
#include <Windows.h>
#pragma comment( lib, "psapi.lib" )

CCore		*g_CCore = NULL;

typedef HRESULT (WINAPI *DirectInput8Create_t)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf,
                 LPVOID * ppvOut, LPUNKNOWN punkOuter);

HRESULT WINAPI MyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut,
                                                                        LPUNKNOWN punkOuter);

typedef BOOL (WINAPI *PeekMessageW_hook) (
__out LPMSG lpMsg,
__in_opt HWND hWnd,
__in UINT wMsgFilterMin,
__in UINT wMsgFilterMax,
__in UINT wRemoveMsg);


PeekMessageW_hook orig_PeekMessage;
DirectInput8Create_t orig_DirectInput8Create;


void MainThread(void);

int count = 0;
int GetProcessCount()
{
	PERFORMANCE_INFORMATION info;
	info.cb = sizeof(info);
	GetPerformanceInfo(&info, sizeof(info));
	return info.ProcessCount;
}

void TestKB()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_F1))
		{
			g_CCore->testStop = false;
		} else if (GetAsyncKeyState(VK_F2))
		{
			g_CCore->testStop = true;
		}
		else if (GetAsyncKeyState(VK_F3))
		{
			/*char buff[200];
			PSIZE_T readen;
			ReadProcessMemory(NULL, (LPCVOID)g_CCore->GetLocalPlayer()->GetBase(), buff, 190, readen);*/
		}
		Sleep(100);
	}
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
			SetHooks();
			WaitTillD3D8IsLoaded();	// DxHook
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainThread, 0, 0, 0);	// hlavne vlakno
			//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&TestKB, 0, 0, 0);	// testovacia klavesnica
			
			break;
	case DLL_PROCESS_DETACH:
			if(g_CCore != NULL)
			{
				g_CCore->SetRunning(false);
				g_CCore = NULL;
			}
			break;
	}
	return TRUE;
}

void MainThread(void)
{
	CCore CCore;
	// loads our hooks, make code patchs
	CCore.Run();
	g_CCore = &CCore;
	while(g_CCore->IsLoaded() == false)		// we must wait till freeride (freeitaly) loads
	{
		Sleep(100);
	}
	while (g_CCore->testStop == false)		// we must wait till freeride (freeitaly) loads
	{
		Sleep(100);
	}
	CCore.SetRunning(true);
	CCore.Start();
	while(CCore.IsRunning())
	{
		//if(CCore.m_bIsRespawning == false)
			CCore.Pulse();
		RakSleep(30);

		
		/*int c2 = GetProcessCount();
		if (c2 != count)
		{
			if (c2 > count)
			{
				//g_CCore->GetChat()->AddMessage("Process count's changed");
				//CheckProccesses();
			}
			count = c2;
		}*/
	}
}

BOOL WINAPI hookPeekMessageW(
	__out LPMSG lpMsg,
	__in_opt HWND hWnd,
	__in UINT wMsgFilterMin,
	__in UINT wMsgFilterMax,
	__in UINT wRemoveMsg)
{
	bool result = orig_PeekMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg) == 1;
	if (result)
	{
		if (lpMsg->message == WM_INPUT)
		{
			g_CCore->GetKeyboard()->ProccessMessage(lpMsg);
		}
	}
	return result;
}

void WaitTillD3D8IsLoaded()
{
	while(GetModuleHandleA("d3d8.dll") == NULL)
	{
		Sleep(10);
	}
	HMODULE hM = GetModuleHandleA("d3d8.dll");
	pDirect3DCreate8 = (PBYTE)GetProcAddress(hM, "Direct3DCreate8");

	hM = GetModuleHandleA("user32.dll");
	PBYTE peekMessageAdd = (PBYTE)GetProcAddress(hM, "PeekMessageW");
	orig_PeekMessage = (PeekMessageW_hook)DetourFunction(peekMessageAdd, (PBYTE)hookPeekMessageW);
	HookAPI();
}

void HookAPI()
{
	orig_DirectInput8Create = reinterpret_cast < DirectInput8Create_t > ( DetourFunction ( DetourFindFunction ( "DINPUT8.DLL", "DirectInput8Create" ),reinterpret_cast < PBYTE > ( MyDirectInput8Create ) ));
	orig_Direct3DCreate8 = (Direct3DCreate8_t)DetourFunction(pDirect3DCreate8, (PBYTE)hook_Direct3DCreate8);
}

IDirect3D8* __stdcall hook_Direct3DCreate8(UINT sdkVers)
{
	IDirect3D8* pD3d8 = orig_Direct3DCreate8(sdkVers); // real one
	CDirect3D8Proxy* CDP = new CDirect3D8Proxy(pD3d8);
	return CDP;
}

HRESULT WINAPI MyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut,
                                                                        LPUNKNOWN punkOuter)
{
	HRESULT DI8Cresult = orig_DirectInput8Create(hinst,dwVersion, riidltf,ppvOut,punkOuter);
	IDirectInput8* pDirectInput8 = static_cast < IDirectInput8 * > ( *ppvOut );

    // Give the caller a proxy interface.
    MyDirectInput* pProxyDirectInput8 = new MyDirectInput ( pDirectInput8 );
    pProxyDirectInput8->AddRef();
	//g_pDirectInput8 = pProxyDirectInput8;
    *ppvOut = pProxyDirectInput8;
	g_CCore->GetGraphics()->SetDirect3D((IDirect3D8*)pProxyDirectInput8);
	return DI8Cresult;
}
