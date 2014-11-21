// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "chat.h"

typedef HMODULE (WINAPI *LoadLibrary_t)(LPCSTR);
LoadLibrary_t orig_LoadLibrary; // holds address of original non-detoured function
// globals
// Our hook function
// The original to call
typedef IDirect3D8* (__stdcall *Direct3DCreate8_t)(UINT SDKVersion);
Direct3DCreate8_t orig_Direct3DCreate8;
IDirect3D8* __stdcall hook_Direct3DCreate8(UINT sdkVers);
typedef HRESULT (APIENTRY *EndScene_t)(IDirect3DDevice8*);
HRESULT APIENTRY hook_EndScene(IDirect3DDevice8* pInterface);
extern EndScene_t orig_EndScene;
IDirect3DDevice8* d3ddev = NULL;

D3DCOLOR fontRed = D3DCOLOR_ARGB(255, 255, 0, 0);

static RECT textbox;
ID3DXFont		*g_font=NULL;
HRESULT hr;

HHOOK _hook;
KBDLLHOOKSTRUCT kbdStruct;
CChat* cChat = NULL;
// hooks.cpp

LPD3DXSPRITE sprite=NULL;
LPDIRECT3DTEXTURE8 pTexture;
HWND g_Gamewindow = NULL;
//Endscene
EndScene_t orig_EndScene;

HRESULT APIENTRY hook_EndScene(IDirect3DDevice8* pInterface)
{
	__asm pushad
	if(cChat != NULL)
	{
		RECT testChat;
		SetRect(&testChat,0,100,500, 200);
		const char *cstr = cChat->chatMessage.c_str();
		g_font ->DrawTextA(cstr,strlen(cstr),&testChat,DT_LEFT | DT_TOP,D3DCOLOR_ARGB(255,255,255,255));
	}
	//static RECT textbox;
    //SetRect(&textbox,0,0,500, 50);
	//ID3DXFont		*g_font=NULL;
	char DrawBuff[256];
/*	time_t rawtime;
	time (&rawtime);
	sprintf (DrawBuff,"Time: %s", ctime (&rawtime));
	g_font ->DrawTextA(DrawBuff,strlen(DrawBuff),&textbox,DT_LEFT | DT_TOP,D3DCOLOR_ARGB(255,255,255,255));
*/
	D3DDISPLAYMODE D3Ddm;
	//pInterface->GetDisplayMode(&D3Ddm);
	d3ddev->ShowCursor(1);
	d3ddev->GetDisplayMode(&D3Ddm);

	RECT rect;
	GetClientRect(g_Gamewindow,&rect);

	DWORD	status1,status2,status3;
	pInterface->GetRenderState(D3DRS_ALPHABLENDENABLE,&status1);
	pInterface->GetRenderState(D3DRS_SRCBLEND, &status2);
	pInterface->GetRenderState(D3DRS_DESTBLEND, &status3);
	sprintf (DrawBuff,"RenderState ALPHA: %d %X %X ",status1,status2,status3);
	//sprintf (DrawBuff,"Width: %u Height: %u",rect.right ,rect.bottom);
	//sprintf (DrawBuff,"Width: %u Height: %u", D3Ddm.Width,D3Ddm.Height);
	g_font ->DrawTextA(DrawBuff,strlen(DrawBuff),&textbox,DT_LEFT | DT_TOP,D3DCOLOR_ARGB(255,255,255,255));
	//MessageBoxA(NULL,"ENDSCENE FAKCI","HURA",MB_OK);

	D3DXVECTOR2 pos;

	pos.x=0.0f;
	pos.y=0.0f;

	D3DXVECTOR2 scaling;
	scaling.x=2.0f;
	scaling.y=2.0f;

	//pInterface->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	//pInterface->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	//pInterface->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	sprite->Begin();
	sprite->Draw(pTexture,NULL,&scaling,NULL,0.0,NULL,D3DCOLOR_ARGB(255,255,255,255));
	sprite->End();
	
	__asm popad

	return orig_EndScene(pInterface);
}

// Holds address that we get in our LoadLibrary hook (used for detour)
PBYTE pDirect3DCreate8;

// CreateDevice
typedef HRESULT (APIENTRY *CreateDevice_t)(IDirect3D8*,UINT,D3DDEVTYPE,HWND,DWORD,D3DPRESENT_PARAMETERS*,IDirect3DDevice8**);
CreateDevice_t orig_CreateDevice;
HRESULT APIENTRY hook_CreateDevice(IDirect3D8* pInterface, UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice8** ppReturnedDeviceInterface);


HRESULT APIENTRY hook_CreateDevice(IDirect3D8* pInterface, UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice8** ppReturnedDeviceInterface)
{
	HRESULT ret = orig_CreateDevice(pInterface, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	// Registers MUST be preserved when doing your own stuff!!
	__asm pushad

	// get a pointer to the created device 
	d3ddev = *ppReturnedDeviceInterface;

	// lets log it (format in hex mode to make it easier to work with)
	char buf[50] = {0};
	sprintf_s(buf, sizeof(buf), "pD3ddev: %X", d3ddev);
	g_Gamewindow = pPresentationParameters->hDeviceWindow;
	//MessageBoxA(NULL,buf,"LOL",MB_OK);

//	std::ofstream of;
	//of.open("C:\\d3d_log.txt", std::ios::app); // append mode
	//of << buf;
	//of.close();

	__asm popad

	return ret;
}

void HookFunction(DWORD* pVtable, void* pHookProc, void* pOldProc, int iIndex)
{
	// Enable writing to the vtable at address we aquired
	DWORD lpflOldProtect;
	VirtualProtect((void*)&pVtable[iIndex], sizeof(DWORD), PAGE_READWRITE, &lpflOldProtect);

	// Store old address
	if (pOldProc) {
		*(DWORD*)pOldProc = pVtable[iIndex];
	}

	// Overwrite original address
	pVtable[iIndex] = (DWORD)pHookProc;

	// Restore protection
	VirtualProtect(pVtable, sizeof(DWORD), lpflOldProtect, &lpflOldProtect);
}

DWORD* GetVtableAddress(void* pObject)
{
	// The first 4 bytes of the object is a pointer to the vtable:
	return (DWORD*)*((DWORD*)pObject);
}

IDirect3D8* __stdcall hook_Direct3DCreate8(UINT sdkVers)
{
	IDirect3D8* pD3d8 = orig_Direct3DCreate8(sdkVers); // real one

	// Use a vtable hook on CreateDevice to get the device pointer later
	DWORD* pVtable = GetVtableAddress(pD3d8);
	HookFunction(pVtable, (void*)&hook_CreateDevice, (void*)&orig_CreateDevice, 15);
	//HookFunction(pVtable, (void*)&hook_EndScene, (void*)&orig_EndScene, 35);

	return pD3d8;
}

// Our hooked LoadLibrary
void HookAPI()
{
	// simple detour
	orig_Direct3DCreate8 = (Direct3DCreate8_t)DetourFunction(pDirect3DCreate8, (PBYTE)hook_Direct3DCreate8);
}
HMODULE WINAPI LoadLibrary_Hook ( LPCSTR lpFileName ) 
{
	static int hooked = 0;
//	std::cout << lpFileName << "\n";
	HMODULE hM = orig_LoadLibrary( lpFileName );
	if ( strcmp( lpFileName, "d3d8.dll" ) == 0) 
	{
		//hooked++;

		//if (hooked == 3) {
			// get address of function to hook
			MessageBoxA(NULL,"´d3d8.dll found","wtf",MB_OK);
			pDirect3DCreate8 = (PBYTE)GetProcAddress(hM, "Direct3DCreate8");
			HookAPI();
		//}
	}

	return hM;
}

void WaitForD3D()
{
	while(GetModuleHandleA("d3d8.dll") == NULL)
	{
		Sleep(10);
	}
	HMODULE hM = GetModuleHandleA("d3d8.dll");
	pDirect3DCreate8 = (PBYTE)GetProcAddress(hM, "Direct3DCreate8");
	HookAPI();
}

void WaitForInterfacePointer()
{
	while(d3ddev == NULL)
	{
		Sleep(10);
	}
	DWORD* pVtable = GetVtableAddress(d3ddev);
	HookFunction(pVtable, (void*)&hook_EndScene, (void*)&orig_EndScene, 35);
	SetRect(&textbox,0,0,500, 50);
	hr	=	D3DXCreateFont(d3ddev,CreateFont(18,0,0,0,0,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,0,TEXT("Arial")),&g_font);


	if (SUCCEEDED(D3DXCreateSprite(d3ddev,&sprite)))
	{
		//D3DXCreateTextureFromFileA( d3ddev, "test.bmp", &pTexture);
		//D3DXCreateTextureFromFileExA( d3ddev, "test.bmp", &pTexture);
		D3DXCreateTextureFromFileExA( d3ddev, "test.bmp",D3DX_DEFAULT, D3DX_DEFAULT, 
           1, 0, D3DFMT_A8R8G8B8, 
           D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
           D3DX_FILTER_LINEAR, 0x10000000, 
           NULL, NULL, &pTexture);

	}

}
LRESULT CALLBACK HookCallbackNew(int nCode, WPARAM wParam, LPARAM lParam)
{
	//MessageBox(NULL,TEXT("Keyboard hook worked"),TEXT("Hura"),MB_OK);
	 if (nCode >= 0)
    {
        // the action is valid: HC_ACTION.
        if (wParam == WM_KEYDOWN)
        {
            // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
            kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
            // a key (non-system) is pressed.
            if (kbdStruct.vkCode == VK_F1)
            {
                // F1 is pressed!
                MessageBoxA(NULL, "F1 is pressed!", "key pressed", MB_ICONINFORMATION);
            }
        }
    }

    // call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

LRESULT WINAPI CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
 
	MessageBox(NULL,TEXT("Keyboard hook worked"),TEXT("Hura"),MB_OK);
    if( code < 0 ) 
        return CallNextHookEx(_hook, code, wParam, lParam );
 
    //////////
    // Chops away the WM_KEYUP msg's
    // that are also sent to filter function
    ///////////
    if( (DWORD) lParam & 0x40000000 )
    {
        // Handle vkCodes in wParam here        
    }
    return CallNextHookEx(_hook, code, wParam, lParam);
}

void Hook(HMODULE hModule)
{
	//(HINSTANCE) hModule
	SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hModule, 0);
	MSG msg;
    while(!GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void KBProc()
{
	while(1==1)
	{
		//MessageBoxA(NULL,"zaciname","hura",MB_OK);
		if(cChat->IsTyping == true)
		{
			if(GetAsyncKeyState(VK_RETURN))
			{
				cChat->DoneMessage();
			} else
			{
				/*if(GetAsyncKeyState(0x41))
				{
					char Pismeno = '0';
					cChat->chatMessage = cChat->chatMessage + Pismeno;
					Sleep(200);
				}*/
					//*cChat->chatMessage = "A";
					//sprintf(*cChat->chatMessage,"%s%s",*cChat->chatMessage,"A");
				//cChat->chatMessage.resize(cChat->chatMessage.size()-1);
				if(GetAsyncKeyState(VK_BACK))
					if(cChat->chatMessage.size() > 0)
						cChat->chatMessage.resize(cChat->chatMessage.size()-1);
				for(int i = 0; i < 10; i++)
				{
					if(GetAsyncKeyState(0x30+i))		// key 0
					{
						char Pismeno = '0'+i;
						cChat->chatMessage = cChat->chatMessage + Pismeno;
					}
					break;
				}
				for(int i = 0; i < 25; i++)
				{
					if(GetAsyncKeyState(0x41+i))		// key 0
					{
						char Pismeno;
						if(GetAsyncKeyState(VK_SHIFT))
						{
							Pismeno = 'A'+i;
						}
						else
						{
							Pismeno = 'a'+i;
						}
						cChat->chatMessage = cChat->chatMessage + Pismeno;
						break;
					}
				}
				Sleep(100);
				
			}
		} else 
		{
			if(GetAsyncKeyState(VK_TAB))
			{
				//MessageBoxA(NULL,"zaciname","hura",MB_OK);
				cChat->IsTyping = true;
			}
		}
		Sleep(10);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			DisableThreadLibraryCalls(hModule);
			// Apply the hook
			//AllocConsole();
			//AttachConsole( GetCurrentProcessId() ) ;
			//freopen( "CON", "w", stdout ) ;
			//std::cout << "working";
			//MessageBoxA(NULL,"lol","wtf",MB_OK);
			//orig_LoadLibrary = (LoadLibrary_t)DetourFunction((LPBYTE) LoadLibraryA, (LPBYTE) LoadLibrary_Hook );
			WaitForD3D();
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)WaitForInterfacePointer,0,0,0);
			//CreateThread(0,0,(LPTHREAD_START_ROUTINE)Hook,hModule,0,0);
			cChat = new CChat;
			cChat->IsTyping = false;
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)KBProc,hModule,0,0);
		}
	}
	return TRUE;
}
