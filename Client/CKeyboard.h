/**
	Lost Heaven Multiplayer

	Purpose: proccess all input from keyboard

	@author Romop5
	@version 13/4/15
*/

#ifndef __C_KEYBOARD
#define __C_KEYBOARD

#include <Windows.h>

class CKeyboard
{
public:
	CKeyboard();
	// returns whether ceratin key is down
	bool				isHolding(unsigned short vk);
	// callback which proceeds key which are being held
	void				ProceedHoldingKeys();
	// Called by raw input hook -> feeds are class with fresh input
	void				ProccessMessage(LPMSG message);
private:
	void				OnKeyDown(unsigned short);
	void				OnKeyReleased(unsigned short);
	void				OnKeyHolding(unsigned short);
	// used for IsHolding
	bool				holdingKeys[256];
	void				SetHolding(unsigned short key, bool isHolded);

	// Returns NULL when conversion is invalid, otherwise a ASCII char
	char				ConvertToASCII(unsigned short VK);
};

#endif