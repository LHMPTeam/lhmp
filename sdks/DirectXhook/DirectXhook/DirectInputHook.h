#include <stdafx.h>

// Function pointer types.
typedef HRESULT (WINAPI *DirectInput8Create_t)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf,
                 LPVOID * ppvOut, LPUNKNOWN punkOuter);
typedef HRESULT (WINAPI *DirectInput8CreateDevice_t)(REFGUID rguid, LPDIRECTINPUTDEVICE * lplpDirectInputDevice, LPUNKNOWN pUnkOuter);
// Function prototypes.
HRESULT WINAPI MyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut,
                                                                        LPUNKNOWN punkOuter);
DirectInput8Create_t orig_DirectInput8Create;
DirectInput8CreateDevice_t orig_DI8CreateDevice;
PBYTE pDirectInput8Create;
PBYTE oldCreateDevice;


void InputHook()