/**
	Lost Heaven Multiplayer

	Purpose: proccess all input from keyboard

	@author Romop5
	@version 1.0 1/9/14
	@todo clean code, ged rid of DirectInput
*/

#ifndef __C_KEYBOARD
#define __C_KEYBOARD

#include <Windows.h>

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
	unsigned int		lastBACKSPACE;

	int					GetScriptKey(unsigned char);

	void				SetStates(bool shift, bool caps);
	bool				isCapsLockPressed();
	bool				isShiftPressed();
	void				OnASCIIKeyDown(char);

	// Raw input
	
	void				ProccessMessage(LPMSG message);
	// Returns NULL when conversion is invalid, otherwise a ASCII char
	char				ConvertToASCII(unsigned short VK);
};

#endif