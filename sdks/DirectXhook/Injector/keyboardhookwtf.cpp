#include "stdafx.h"
#include "keyboardhook.h"
LRESULT __stdcall CALLBACK Keyboardhook::HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        // the action is valid: HC_ACTION.
        if (wParam == WM_KEYDOWN)
        {
            // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
            Keyboardhook::kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
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

void Keyboardhook::SetHook()
{
	HOOKPROC ourProc = (HOOKPROC)GetProcAddress(NULL, "HookCallback"); 
    if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, ourProc, NULL, 0)))
    {
        MessageBoxA(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
    }
}

void Keyboardhook::ReleaseHook()
{
    UnhookWindowsHookEx(_hook);
}
