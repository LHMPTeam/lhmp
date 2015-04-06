#include "CCore.h"
#include "CKeyboard.h"

#define	DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

extern CCore* g_CCore;

CKeyboard::CKeyboard()
{
	lastBACKSPACE = NULL;
	ignore = false;
}
void CKeyboard::ProceedKeyboard(void *data)
{
	this->SetStates((((BYTE*)data)[DIK_LSHIFT] != 0 || ((BYTE*)data)[DIK_RSHIFT] != 0), ((BYTE*)data)[DIK_CAPSLOCK] != 0);
	DWORD time = timeGetTime();
	for (unsigned int i = 0; i < 256; i++)
	{
		if (((BYTE*)data)[i] == keys[i] && ((BYTE*)data)[i] != 0)	// holding
		{
			if (g_CCore->GetGraphics()->renderMap)
			{
				if (i == DIK_NUMPADPLUS)
					g_CCore->GetGraphics()->mapScale *= 1.1f;
				if (i == DIK_NUMPADMINUS)
					g_CCore->GetGraphics()->mapScale *= 0.9f;
				g_CCore->GetGraphics()->mapScale = Tools::Clamp(g_CCore->GetGraphics()->mapScale, 0.5f, 2.0f);
			}
			if (i == DIK_TAB)
			{
				g_CCore->GetGraphics()->renderMap = 1;
			}
			if (i == DIK_F1)
			{
				g_CCore->testStop = true;
			}
			if (i == DIK_F2)
			{
				g_CCore->testStop = false;
			}
			else if (i == DIK_F5)
			{
				g_CCore->GetGraphics()->renderScoreboard = 1;
			}
			else if (i == DIK_F6)
			{
				g_CCore->GetGraphics()->renderNetStat = 1;
			}
			else if (i == DIK_F9)
			{
				g_CCore->GetGraphics()->TakeScreenshot();
			}
			
		}
		if (((BYTE*)data)[i] != keys[i] && ((BYTE*)data)[i] != 0)	// pressed
		{
			this->OnKeyDown(i);
		}
	}
	memcpy(keys, data, 256);		// save actual status for next proceed
}


void	CKeyboard::OnKeyDown(unsigned char i)
{
	if (g_CCore->GetChat()->IsTyping())
	{
		g_CCore->GetChat()->SetRerenderState(true);
		
		if (i == DIK_UPARROW)
		{
			g_CCore->GetChat()->ChatMessage = g_CCore->GetChat()->LastInput;
		}

		else if (i == DIK_NUMPADENTER || i == 28)	// if ENTER
		{
			if (g_CCore->GetChat()->ChatMessage != "")
				g_CCore->GetChat()->LastInput = g_CCore->GetChat()->ChatMessage;
			g_CCore->GetChat()->DoneMessage();
			g_CCore->GetChat()->SetTyping(false);
		}
		else if (i == DIK_SPACE)
		{
			g_CCore->GetChat()->ChatMessage += ' ';
		}
		else if (i == DIK_ESCAPE)
		{
			g_CCore->GetChat()->SetTyping(false);
			g_CCore->GetChat()->ChatMessage = "";
		}
	}
	else	// no typing
	{
		if (i == DIK_ESCAPE)
		{
			g_CCore->GetIngameMenu()->setActive(!g_CCore->GetIngameMenu()->isActive());
		}
		else if (i == DIK_UPARROW)
		{
			g_CCore->GetIngameMenu()->OnArrowUp();
		}
		else if (i == DIK_DOWNARROW)
		{
			g_CCore->GetIngameMenu()->OnArrowDown();
		}
		else if (i == DIK_NUMPADENTER || i == 28)
		{
			g_CCore->GetIngameMenu()->OnPressEnter();
		}
		int resultkey = this->GetScriptKey(i);
		if (resultkey != -1)
		{
			// send server message
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((RakNet::MessageID)LHMP_SCRIPT_ON_KEY_PRESSED);
			bsOut.Write(resultkey);
			g_CCore->GetNetwork()->SendServerMessage(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED);

		}
	}
}

int		CKeyboard::GetScriptKey(unsigned char input)
{
	int key = 0;
	switch (input)
	{
	case DIK_0:	key = KEY_NUM0;
		break;
	case DIK_1:	key = KEY_NUM1;
		break; 
	case DIK_2:	key = KEY_NUM2;
		break;
	case DIK_3:	key = KEY_NUM3;
		break;
	case DIK_4:	key = KEY_NUM4;
		break;
	case DIK_5:	key = KEY_NUM5;
		break;
	case DIK_6:	key = KEY_NUM6;
		break;
	case DIK_7:	key = KEY_NUM7;
		break;
	case DIK_8:	key = KEY_NUM8;
		break;
	case DIK_9:	key = KEY_NUM9;
		break; 
	case DIK_A:	key = KEY_A;
		break;
	case DIK_B:	key = KEY_B;
		break;
	case DIK_C:	key = KEY_C;
		break;
	case DIK_D:	key = KEY_D;
		break;
	case DIK_E:	key = KEY_E;
		break;
	case DIK_F:	key = KEY_F;
		break;
	case DIK_G:	key = KEY_G;
		break;
	case DIK_H:	key = KEY_H;
		break;
	case DIK_I:	key = KEY_I;
		break;
	case DIK_J:	key = KEY_J;
		break;
	case DIK_K:	key = KEY_K;
		break;
	case DIK_L:	key = KEY_L;
		break;
	case DIK_M:	key = KEY_M;
		break;
	case DIK_N:	key = KEY_N;
		break;
	case DIK_O:	key = KEY_O;
		break;
	case DIK_P:	key = KEY_P;
		break;
	case DIK_R:	key = KEY_R;
		break;
	case DIK_Q:	key = KEY_Q;
		break;
	case DIK_S:	key = KEY_S;
		break;
	case DIK_T:	key = KEY_T;
		break;
	case DIK_U:	key = KEY_U;
		break;
	case DIK_V:	key = KEY_V;
		break;
	case DIK_W:	key = KEY_W;
		break;
	case DIK_X:	key = KEY_X;
		break;
	case DIK_Z:	key = KEY_Z;
		break;
	case DIK_UPARROW:	key = KEY_ARROWUP;
		break;
	case DIK_DOWNARROW:	key = KEY_ARROWDOWN;
		break;
	case DIK_LEFTARROW:	key = KEY_ARROWLEFT;
		break;
	case DIK_RIGHTARROW: key = KEY_ARROWRIGHT;
		break;
	default:
		return -1;
	}
	return key;
}
void	CKeyboard::OnKeyReleased(unsigned char key)
{

}
void	CKeyboard::OnKeyHolding(unsigned char key)
{
}

void	CKeyboard::SetStates(bool shift, bool caps)
{
	this->shiftPressed = shift;
	this->capslockPressed = caps;
}
bool	CKeyboard::isCapsLockPressed()
{
	return this->capslockPressed;
}
bool	CKeyboard::isShiftPressed()
{
	return this->shiftPressed;
}

void	CKeyboard::OnASCIIKeyDown(char key)
{
	this->ignore = !ignore;
	if (ignore == false)
	{
		if (g_CCore->GetChat()->IsTyping())
		{
			g_CCore->GetChat()->SetRerenderState(true);
			if (g_CCore->GetChat()->ChatMessage.length() < MAX_CHAT_MESSAGE_LENGTH - 10)
			{
				g_CCore->GetChat()->ChatMessage += key;
			}
		}
		else
		{
			if (key == 't' || key == 'T')
			{
				if (!g_CCore->GetIngameMenu()->isActive())
				{
					g_CCore->GetChat()->SetTyping(true);
					g_CCore->GetChat()->SetRerenderState(true);
				}
			}
		}
	}
}


void	CKeyboard::ProccessMessage(LPMSG lpMsg)
{
	UINT dwSize;

	GetRawInputData((HRAWINPUT)lpMsg->lParam, RID_INPUT, NULL, &dwSize,
		sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		return;
	}

	if (GetRawInputData((HRAWINPUT)lpMsg->lParam, RID_INPUT, lpb, &dwSize,
		sizeof(RAWINPUTHEADER)) != dwSize)
		g_CCore->GetChat()->AddMessage("raw input error");

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		if (!(raw->data.keyboard.Flags & RI_KEY_BREAK))
		{
			switch (raw->data.keyboard.VKey)
			{
				case VK_BACK:
					if (g_CCore->GetChat()->ChatMessage.size() > 0)
						g_CCore->GetChat()->ChatMessage.resize(g_CCore->GetChat()->ChatMessage.size() - 1);
					g_CCore->GetChat()->SetRerenderState(true);
					break;

				case VK_F11:
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
					break;
				case VK_F12:
				{
					g_CCore->GetGraphics()->m_bUserShowNameTags = !g_CCore->GetGraphics()->m_bUserShowNameTags;
				}
					break;
				case VK_RETURN:
				case VK_SHIFT:
				case VK_RSHIFT:
				case VK_LSHIFT:
				case VK_TAB:
				case VK_SPACE:
					break;

				default:
					char key = this->ConvertToASCII(raw->data.keyboard.VKey);
					if (key != NULL)
					{
						if (g_CCore->GetChat()->IsTyping())
						{
							g_CCore->GetChat()->SetRerenderState(true);
							if (g_CCore->GetChat()->ChatMessage.length() < MAX_CHAT_MESSAGE_LENGTH - 10)
							{
								g_CCore->GetChat()->ChatMessage += key;
							}
						}
						else
						{
							if (key == 't' || key == 'T')
							{
								if (!g_CCore->GetIngameMenu()->isActive())
								{
									g_CCore->GetChat()->SetTyping(true);
									g_CCore->GetChat()->SetRerenderState(true);
								}
							}
						}
					}
				

			}
		}

	}

	delete[] lpb;
}


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