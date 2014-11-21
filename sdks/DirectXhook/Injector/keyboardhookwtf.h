#include "stdafx.h"
#include <Windows.h>
class Keyboardhook
{
public:	HHOOK				_hook;
			KBDLLHOOKSTRUCT		kbdStruct;
public:		LRESULT _stdcall	HookCallback(int nCode, WPARAM wParam, LPARAM lParam);
			void				SetHook();
			void				ReleaseHook();

};
