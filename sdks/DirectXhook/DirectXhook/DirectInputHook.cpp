#include <stdafx.h>
#include "DirectInputHook.h"
#include <dinput.h>
void InputHook()
{
	while(GetModuleHandleA("dinput8.dll") == NULL)
	{
		Sleep(10);
	}
	HMODULE hM = GetModuleHandleA("dinput8.dll");
	pDirectInput8Create = (PBYTE)GetProcAddress(hM, "DirectInput8Create");
	orig_DirectInput8Create = (DirectInput8Create_t)DetourFunction(pDirectInput8Create, (PBYTE)MyDirectInput8Create);
}
HRESULT WINAPI MyDirectInput8CreateDevice(DWORD d1,DWORD d2,DWORD d3,DWORD d4)
{
	HRESULT hr = ((HRESULT(WINAPI*)(DWORD,DWORD,DWORD,DWORD))oldCreateDevice)(d1,d2,d3,d4);
	if(*(DWORD*)d2 == GUID_SysKeyboard)
	return hr;
}
HRESULT WINAPI MyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut,
                                                                        LPUNKNOWN punkOuter)
{
	HRESULT DI8Cresult = orig_DirectInput8Create(hinst,dwVersion, riidltf,ppvOut,punkOuter);
	DWORD dwFuncTable = (DWORD)*((DWORD*)*ppvOut);
	DWORD oldprot;
	VirtualProtect((LPVOID)dwFuncTable, 0x10, PAGE_EXECUTE_READWRITE, &oldprot);
	oldCreateDevice = (PBYTE) *((DWORD*)(dwFuncTable + 0x0C));
	*((DWORD*)(dwFuncTable + 0x0C)) = (DWORD)MyDirectInput8CreateDevice;
	return DI8Cresult;
}
