#ifndef __RAWINPUT__
#define __RAWINPUT__

namespace RawHook
{
	//Typedef
	typedef BOOL(WINAPI* fPeekMessage)(
		_Out_     LPMSG lpMsg,
		_In_opt_  HWND hWnd,
		_In_      UINT wMsgFilterMin,
		_In_      UINT wMsgFilterMax,
		_In_      UINT wRemoveMsg
		);

	//Funciton defs
	BOOL WINAPI PeekMessage_Hook(
		_Out_     LPMSG lpMsg,
		_In_opt_  HWND hWnd,
		_In_      UINT wMsgFilterMin,
		_In_      UINT wMsgFilterMax,
		_In_      UINT wRemoveMsg
	);

	void Hook();

	// RAW Input process taken from MSDN, just google it
	void ProcessMessage(LPMSG message);
};

#endif // !__RAWINPUT__
