#include "stdafx.h"
class CChat
{
public:
	CChat();
	std::string chatMessage;
	void DoneMessage();
	void AddChar(char);
	bool IsTyping; 
	bool IsVisible;
};