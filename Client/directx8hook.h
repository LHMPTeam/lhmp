#ifndef DIRECTX8HOOK_H
#define	DIRECTX8HOOK_H

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d8.h>
#include <detours.h>

typedef IDirect3D8* (__stdcall *Direct3DCreate8_t)(UINT SDKVersion);
typedef HRESULT		(APIENTRY *EndScene_t)(IDirect3DDevice8*);
typedef HRESULT		(APIENTRY *CreateDevice_t)(IDirect3D8*,UINT,D3DDEVTYPE,HWND,DWORD,D3DPRESENT_PARAMETERS*,IDirect3DDevice8**);

IDirect3D8* __stdcall hook_Direct3DCreate8(UINT sdkVers);
PBYTE				pDirect3DCreate8;
Direct3DCreate8_t	orig_Direct3DCreate8;
EndScene_t			orig_EndScene;
//EndScene_t			orig_EndScene;
CreateDevice_t		orig_CreateDevice;
IDirect3DDevice8*	g_D3DDevice				= NULL;
HWND				g_hwndWindow			= NULL;

void				HookAPI();
void				WaitTillD3D8IsLoaded();
void				HookFunction(DWORD*,void*, void*, int);
DWORD*				GetVtableAddress(void*);
void				HookOtherFunctions();
void				OurD3DInit();
#endif