#include "stdafx.h"
#include "chat.h"
CChat::CChat()
{
	chatMessage = "";
}
void CChat::DoneMessage()
{
	IsTyping = false;
	chatMessage = "";
}

void CChat::AddChar(char C)
{
}