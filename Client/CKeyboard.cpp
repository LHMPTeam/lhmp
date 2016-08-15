// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include "CKeyboard.h"

//#define	DIRECTINPUT_VERSION 0x0800
//#include <dinput.h>

extern CCore* g_CCore;

CKeyboard::CKeyboard()
{
	// set all keys to released state
	memset(this->holdingKeys, 0x0, 256);
}

bool	CKeyboard::isHolding(unsigned short vk)
{
	return this->holdingKeys[vk];
}

// called in in-game loop
// this function proceeds all holding keys
void	CKeyboard::ProceedHoldingKeys()
{
	if (this->isHolding(VK_TAB))
	{
		g_CCore->GetGraphics()->renderMap = 1;
	}
	if (this->isHolding(VK_F1))
	{
		g_CCore->testStop = true;
	}
	if (this->isHolding(VK_F2))
	{
		g_CCore->testStop = false;
	}
	if (this->isHolding(VK_F5))
	{
		g_CCore->GetGraphics()->renderScoreboard = 1;
	}
	if (this->isHolding(VK_F6))
	{
		g_CCore->GetGraphics()->renderNetStat = 1;
	}
	if (this->isHolding(VK_F9))
	{
		g_CCore->GetGraphics()->TakeScreenshot();
	}

	if (g_CCore->GetGraphics()->renderMap)
	{
		if (this->isHolding(VK_OEM_PLUS) || this->isHolding(VK_ADD))
			g_CCore->GetGraphics()->mapScale *= 1.1f;
		if (this->isHolding(VK_OEM_MINUS) || this->isHolding(VK_SUBTRACT))
			g_CCore->GetGraphics()->mapScale *= 0.9f;
		g_CCore->GetGraphics()->mapScale = Tools::Clamp(g_CCore->GetGraphics()->mapScale, 0.5f, 2.0f);
	}
}

// Called when certain key is pressed
void	CKeyboard::OnKeyDown(unsigned short VK_code)
{
	if (g_CCore->GetChat()->IsTyping())
	{
		g_CCore->GetChat()->SetRerenderState(true);
		
		if (VK_code == VK_UP)
		{
			g_CCore->GetChat()->ChatMessage = g_CCore->GetChat()->GetPreviousLast();
		}
		else if (VK_code == VK_DOWN)
		{
			g_CCore->GetChat()->ChatMessage = g_CCore->GetChat()->GetNextLast();
		}

		else if (VK_code == VK_RETURN)	// if ENTER
		{
			if (g_CCore->GetChat()->ChatMessage != "")
			{
				g_CCore->GetChat()->AddNewLastMessage(g_CCore->GetChat()->ChatMessage);
			}
			g_CCore->GetChat()->DoneMessage();
			g_CCore->GetChat()->SetTyping(false);
		}
		else if (VK_code == VK_SPACE)
		{
			g_CCore->GetChat()->ChatMessage += ' ';
		}
		else if (VK_code == VK_BACK)
		{
			if (g_CCore->GetChat()->ChatMessage.size() > 0)
				g_CCore->GetChat()->ChatMessage.resize(g_CCore->GetChat()->ChatMessage.size() - 1);
			g_CCore->GetChat()->SetRerenderState(true);
		}
		else if (VK_code == VK_ESCAPE)
		{
			g_CCore->GetChat()->SetTyping(false);
			g_CCore->GetChat()->ChatMessage = "";
		}
		else if (VK_code == 0x56)
		{
			if (this->isHolding(VK_CONTROL))
			{
				// CTR-V case
				// Credits: stackoverflow 
				// Try opening the clipboard
				if (!OpenClipboard(nullptr))
					return;

					// Get handle of clipboard object for ANSI text
					HANDLE hData = GetClipboardData(CF_TEXT);
				if (hData == nullptr)
					return;

					// Lock the handle to get the actual text pointer
					char * pszText = static_cast<char*>(GlobalLock(hData));
				if (pszText == nullptr)
					return;

					// Save text in a string class instance
					std::string text(pszText);

				// Release the lock
				GlobalUnlock(hData);

				// Release the clipboard
				CloseClipboard();

				// add text to chat
				g_CCore->GetChat()->ChatMessage += text;
				if (g_CCore->GetChat()->ChatMessage.length() > MAX_CHAT_MESSAGE_LENGTH - 10)
				{
					g_CCore->GetChat()->ChatMessage.resize(MAX_CHAT_MESSAGE_LENGTH - 10);
				}
			}
		}
	}
	else	// no typing
	{
		if (VK_code == VK_ESCAPE)
		{
			g_CCore->GetIngameMenu()->setActive(!g_CCore->GetIngameMenu()->isActive());
		}
		else if (VK_code == VK_UP)
		{
			g_CCore->GetIngameMenu()->OnArrowUp();
		}
		else if (VK_code == VK_DOWN)
		{
			g_CCore->GetIngameMenu()->OnArrowDown();
		}
		else if (VK_code == VK_RETURN)
		{
			g_CCore->GetIngameMenu()->OnPressEnter();
		}

		if (VK_code == 0x54) // 0x54 - T key
		{
			if (!g_CCore->GetIngameMenu()->isActive())
			{
				g_CCore->GetChat()->SetTyping(true);
				g_CCore->GetChat()->SetRerenderState(true);
			}
		}

		if (VK_code == VK_F11)
		{
			if (g_CCore->GetGraphics()->bShowHud)
			{
				g_CCore->GetGraphics()->bShowHud = false;
				g_CCore->GetGraphics()->ToggleInGameHud(false);
			}
			else {
				g_CCore->GetGraphics()->bShowHud = true;
				g_CCore->GetGraphics()->ToggleInGameHud(true);
			}
		}
		if (VK_code == VK_F12)
		{
					   g_CCore->GetGraphics()->m_bUserShowNameTags = !g_CCore->GetGraphics()->m_bUserShowNameTags;
		}

		// send server the VK code
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((RakNet::MessageID)LHMP_SCRIPT_ON_KEY_PRESSED);
			bsOut.Write(VK_code);
			g_CCore->GetNetwork()->SendServerMessage(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED);

		// call client-side callback
			g_CCore->GetEngineStack()->AddMessage(ES_SCRIPT_ONKEYDOWN,VK_code);
	}
}

// Called when certain key is released
void	CKeyboard::OnKeyReleased(unsigned short code)
{

}


// gets input from raw input
void	CKeyboard::ProccessMessage(LPMSG lpMsg)
{
	UINT dwSize;

	GetRawInputData((HRAWINPUT)lpMsg->lParam, RID_INPUT, NULL, &dwSize,
		sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		g_CCore->GetChat()->AddMessage("raw input error LOL");
		return;
	}

	if (GetRawInputData((HRAWINPUT)lpMsg->lParam, RID_INPUT, lpb, &dwSize,
		sizeof(RAWINPUTHEADER)) != dwSize)
		g_CCore->GetChat()->AddMessage("raw input error");

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		//g_CCore->GetChat()->AddMessage("KeyPressed");

		//g_CCore->GetChat()->AddMessage("key pressed");
		// if key is up
		if (!(raw->data.keyboard.Flags & RI_KEY_MAKE))
		{
			this->SetHolding(raw->data.keyboard.VKey, false);
			this->OnKeyReleased(raw->data.keyboard.VKey);
		} 
		// if key is down (pressed)
		if (!(raw->data.keyboard.Flags & RI_KEY_BREAK))
		{
			this->SetHolding(raw->data.keyboard.VKey, true);

			switch (raw->data.keyboard.VKey)
			{
				case VK_TAB:
				case VK_MENU:
					break;
				default:
					char key = this->ConvertToASCII(raw->data.keyboard.VKey);
					if (key != NULL)
					{
						if (g_CCore->GetChat()->IsTyping())
						{
							// if it's printable ASCII code
							if (key >= '!' && key <= '~')
							{
								if (g_CCore->GetChat()->ChatMessage.length() < MAX_CHAT_MESSAGE_LENGTH - 10)
								{
									g_CCore->GetChat()->ChatMessage += key;
								}
								g_CCore->GetChat()->SetRerenderState(true);
							}
						}
					}
			}

			this->OnKeyDown(raw->data.keyboard.VKey);
		}

	}

	delete[] lpb;
}

// Utils, converts virtual-key codes to ASCII
char	CKeyboard::ConvertToASCII(unsigned short VK)
{
	byte kbs[256];
	GetKeyboardState(kbs);
	char buff[4];
	int rre = ToAsciiEx(VK, MapVirtualKey(VK, MAPVK_VK_TO_VSC), kbs, (LPWORD)&buff, 0, GetKeyboardLayout(0));
	if (rre == 1)
	{
			return buff[0];
	}
	return NULL;
}

// private - set isHolding state
void CKeyboard::SetHolding(unsigned short key, bool isHolded)
{
	this->holdingKeys[key] = isHolded;
}