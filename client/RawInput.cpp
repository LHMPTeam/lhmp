#include <stdinc.h>

namespace RawHook
{
	fPeekMessage mOriginalPeekMessage;

	// RAW Input process taken from MSDN, just google it
	void ProcessMessage(LPMSG message)
	{
		UINT dwSize;
		GetRawInputData((HRAWINPUT)message->lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

		LPBYTE lpb = new BYTE[dwSize];

		if (lpb)
		{
			if (GetRawInputData((HRAWINPUT)message->lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
			{
				RAWINPUT* raw = (RAWINPUT*)lpb;

				if (raw->header.dwType == RIM_TYPEKEYBOARD)
				{
					//OnVKKey(raw->data.keyboard.VKey, raw->data.keyboard.Message);
					if (Core::GetCore()->GetGraphics()->GetChat())
					{
						Core::GetCore()->GetGraphics()->GetChat()->ProcessKeyboard(raw->data.keyboard.VKey, raw->data.keyboard.Message);
					}
				}
				else if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					//OnMouseTick(&raw->data.mouse);
				}

				delete[] lpb;
			}
		}
	}

	BOOL WINAPI PeekMessage_Hook(
		_Out_     LPMSG lpMsg,
		_In_opt_  HWND hWnd,
		_In_      UINT wMsgFilterMin,
		_In_      UINT wMsgFilterMax,
		_In_      UINT wRemoveMsg
	)
	{
		bool bResult = mOriginalPeekMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg) == 1;

		if (bResult)
		{
			switch (lpMsg->message)
			{
			case WM_INPUT:
			{
				ProcessMessage(lpMsg);
				break;
			}
			default:
				break;
			}
		}

		return bResult;
	}

	void Hook()
	{
		mOriginalPeekMessage = reinterpret_cast <fPeekMessage> (DetourFunction(DetourFindFunction("USER32.DLL", "PeekMessageW"), reinterpret_cast < PBYTE > (PeekMessage_Hook)));
	}
};