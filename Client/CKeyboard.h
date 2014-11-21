#ifndef C_KEYBOARD
#define C_KEYBOARD

//#include "Windows.h"
/*#ifndef CALLBACK
#define CALLBACK _stdcall
#endif
*/


class CKeyboard
{
private:
	bool				shiftPressed;
	bool				capslockPressed;
	bool				ignore;
public:
	CKeyboard();
	unsigned char		keys[256];
	void				ProceedKeyboard(void *);
	void				OnKeyDown(unsigned char);	
	void				OnKeyReleased(unsigned char);
	void				OnKeyHolding(unsigned char);

	void				ApplyHook();
	void*				oldProc;
	//static  LRESULT CALLBACK ProcessMessage(HWND, UINT, WPARAM, LPARAM);
	unsigned int		lastBACKSPACE;

	int					GetScriptKey(unsigned char);

	void				SetStates(bool shift, bool caps);
	bool				isCapsLockPressed();
	bool				isShiftPressed();
	void				OnASCIIKeyDown(char);
};

#endif