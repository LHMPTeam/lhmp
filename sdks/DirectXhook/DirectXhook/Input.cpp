#include "stdafx.h"
/*//#include "input.h"
//#include <chat.h>
//extern CChat* cChat;
CChat *cChat = NULL; 
void KBProc()
{
	if(cChat->IsTyping == true)
	{
		if(GetAsyncKeyState(VK_RETURN))
		{
			cChat->DoneMessage();
		} else
		{
			for(int i = 0; i < 9; i++)
				if(GetAsyncKeyState(0x30+i))		// key 0
					sprintf(*cChat->chatMessage,"%s%s",*cChat->chatMessage,"0"+i);
		}
	} else 
	{
		if(GetAsyncKeyState(0x54))
			MessageBoxA(NULL,"zaciname","hura",MB_OK);
			cChat->IsTyping = true;
	}
}
*/