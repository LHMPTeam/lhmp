#include "CKeyboard.h"
#include "CCore.h"

#define	DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
//#include "Windows.h"

extern CCore* g_CCore;

CKeyboard::CKeyboard()
{
	//firstBACKSPACE = NULL;
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
			if (g_CCore->GetChat()->IsTyping())
			{
				
				if (i == DIK_BACKSPACE)
				{
					if (lastBACKSPACE + 100 < time)
					{

						if (g_CCore->GetChat()->ChatMessage.size() > 0)
							g_CCore->GetChat()->ChatMessage.resize(g_CCore->GetChat()->ChatMessage.size() - 1);
						lastBACKSPACE = time;
						g_CCore->GetChat()->shouldReRender = true;
					}
				}
			}
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
			else if (i == DIK_F5)
			{
				g_CCore->GetGraphics()->renderScoreboard = 1;
			}
			else if (i == DIK_F6)
			{
				g_CCore->GetGraphics()->renderNetStat = 1;
			}
			else if (i == DIK_F7)
			{
				g_CCore->GetGraphics()->bShowHud = false;
				g_CCore->GetGraphics()->ToggleInGameHud(false);
			}
			else if (i == DIK_F8)
			{
				g_CCore->GetGraphics()->bShowHud = true;
				g_CCore->GetGraphics()->ToggleInGameHud(true);
			}
			else if (i == DIK_F9)
			{
				g_CCore->GetGraphics()->TakeScreenshot();
			}
			else if (i == DIK_F10)
			{
				g_CCore->GetChat()->camSpeed *= 0.9f;
				//g_CCore->GetGraphics()->ToggleInGameHud(true);
			}
			else if (i == DIK_F11)
			{
				g_CCore->GetChat()->camSpeed *= 1.1f;
				//g_CCore->GetGraphics()->TakeScreenshot();
			}
			if (g_CCore->GetChat()->isCamOn)
			{
				float rate = 1;
				rate *= g_CCore->GetChat()->camSpeed;
				if (((BYTE*)data)[DIK_LSHIFT] != 0 || ((BYTE*)data)[DIK_RSHIFT] != 0)
					rate = 10;
				if (i == DIK_NUMPAD8 || i == DIK_W)
				{
					g_CCore->GetChat()->camPos.x += (float) 0.16*rate*g_CCore->GetChat()->camR1;
					g_CCore->GetChat()->camPos.z += (float) 0.16*rate*g_CCore->GetChat()->camR3;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
				if (i == DIK_NUMPAD2 || i == DIK_S)
				{
					//camPos.x -= 0.16*rate;
					g_CCore->GetChat()->camPos.x -= (float) 0.16*rate*g_CCore->GetChat()->camR1;
					g_CCore->GetChat()->camPos.z -= (float) 0.16*rate*g_CCore->GetChat()->camR3;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
				if (i == DIK_NUMPAD4 || i == DIK_A)
				{
					//camPos.z += 0.16*rate;
					g_CCore->GetChat()->camPos.x += (float) 0.16*rate*(-1)*g_CCore->GetChat()->camR3;
					g_CCore->GetChat()->camPos.z += (float) 0.16*rate*g_CCore->GetChat()->camR1;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
				if (i == DIK_NUMPAD6 || i == DIK_D)
				{
					//camPos.z -= 0.16*rate;
					g_CCore->GetChat()->camPos.x -= (float) 0.16*rate*(-1)*g_CCore->GetChat()->camR3;
					g_CCore->GetChat()->camPos.z -= (float) 0.16*rate*g_CCore->GetChat()->camR1;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
				if (i == DIK_NUMPADPLUS)
				{
					g_CCore->GetChat()->camPos.y += (float) 0.16*rate;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
				if (i == DIK_NUMPADMINUS)
				{
					g_CCore->GetChat()->camPos.y -= (float) 0.16*rate;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2,g_CCore->GetChat()->camR3);
				}
				/*if(i == DIK_NUMPAD7)
				{
				camR1 += 0.16;
				if(camR1 > 1.0f) camR1 = 1.0f;
				g_CCore->GetGame()->SetCameraPos(camPos,camR1,camR2,camR3);
				}
				if(i == DIK_NUMPAD9)
				{
				camR1 -= 0.16;
				if(camR1 < -1.0f) camR1 = -1.0f;
				g_CCore->GetGame()->SetCameraPos(camPos,camR1,camR2,camR3);
				}*/
				if (i == DIK_NUMPAD7)
				{
					g_CCore->GetChat()->camR2 += 0.16f;
					if (g_CCore->GetChat()->camR2 > 1.0f) g_CCore->GetChat()->camR2 = 1.0f;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
				if (i == DIK_NUMPAD9)
				{
					g_CCore->GetChat()->camR2 -= 0.16f;
					if (g_CCore->GetChat()->camR2 < -1.0f) g_CCore->GetChat()->camR2 = -1.0f;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
				if (i == DIK_NUMPAD1)
				{
					g_CCore->GetChat()->camDegree++;
					if (g_CCore->GetChat()->camDegree > 180) g_CCore->GetChat()->camDegree = -180;
					Vector3D angle = Tools::ComputeOffsetDegrees(g_CCore->GetChat()->camDegree);
					g_CCore->GetChat()->camR1 = angle.x;
					g_CCore->GetChat()->camR3 = angle.z;

					//camR3 += 0.16;
					//if(camR3 > 1.0f) camR3 = 1.0f;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
				if (i == DIK_NUMPAD3)
				{
					/*camR3 -= 0.16;
					if(camR3 < -1.0f) camR3 = -1.0f;
					*/
					g_CCore->GetChat()->camDegree--;
					if (g_CCore->GetChat()->camDegree < -180)g_CCore->GetChat()->camDegree = 180;
					Vector3D angle = Tools::ComputeOffsetDegrees(g_CCore->GetChat()->camDegree);
					g_CCore->GetChat()->camR1 = angle.x;
					g_CCore->GetChat()->camR3 = angle.z;
					g_CCore->GetGame()->SetCameraPos(g_CCore->GetChat()->camPos, g_CCore->GetChat()->camR1,
						g_CCore->GetChat()->camR2, g_CCore->GetChat()->camR3);
				}
			}
		}
		if (((BYTE*)data)[i] != keys[i] && ((BYTE*)data)[i] != 0)	// pressed
		{
			this->OnKeyDown(i);
			/*if (i == DIK_ESCAPE)
			{

				//TerminateProcess(GetCurrentProcess(), 0);
			}*/

		}
	}
	memcpy(keys, data, 256);		// save actual status for next proceed
}


void	CKeyboard::OnKeyDown(unsigned char i)
{
	if (g_CCore->GetChat()->IsTyping())
	{
		g_CCore->GetChat()->shouldReRender = true;
		// key IDs for debug
		//char buff[20];
		//sprintf(buff,"%d",i);
		//AddMessage(buff);
		/*char pismeno = 0x00;
		if (i == DIK_0 && this->isShiftPressed())
			pismeno = ')';
		else if (i == DIK_0 || i == DIK_NUMPAD0)
			pismeno = '0';
		else if (i == DIK_1 || i == DIK_NUMPAD1)
			pismeno = '1';
		else if (i == DIK_2 || i == DIK_NUMPAD2)
			pismeno = '2';
		else if (i == DIK_3 && this->isShiftPressed())
			pismeno = '#';
		else if (i == DIK_3 || i == DIK_NUMPAD3)
			pismeno = '3';
		else if (i == DIK_4 || i == DIK_NUMPAD4)
			pismeno = '4';
		else if (i == DIK_5 || i == DIK_NUMPAD5)
			pismeno = '5';
		else if (i == DIK_6 || i == DIK_NUMPAD6)
			pismeno = '6';
		else if (i == DIK_7 || i == DIK_NUMPAD7)
			pismeno = '7';
		else if (i == DIK_8 || i == DIK_NUMPAD8)
			pismeno = '8';
		else if (i == DIK_9 && this->isShiftPressed())
			pismeno = '(';
		else if (i == DIK_9 || i == DIK_NUMPAD9)
			pismeno = '9';
		else if (i == DIK_A)
			pismeno = 'a';
		else if (i == DIK_B)
			pismeno = 'b';
		else if (i == DIK_C)
			pismeno = 'c';
		else if (i == DIK_D)
			pismeno = 'd';
		else if (i == DIK_E)
			pismeno = 'e';
		else if (i == DIK_F)
			pismeno = 'f';
		else if (i == DIK_G)
			pismeno = 'g';
		else if (i == DIK_H)
			pismeno = 'h';
		else if (i == DIK_I)
			pismeno = 'i';
		else if (i == DIK_J)
			pismeno = 'j';
		else if (i == DIK_K)
			pismeno = 'k';
		else if (i == DIK_L)
			pismeno = 'l';
		else if (i == DIK_M)
			pismeno = 'm';
		else if (i == DIK_N)
			pismeno = 'n';
		else if (i == DIK_O)
			pismeno = 'o';
		else if (i == DIK_P)
			pismeno = 'p';
		else if (i == DIK_Q)
			pismeno = 'q';
		else if (i == DIK_R)
			pismeno = 'r';
		else if (i == DIK_S)
			pismeno = 's';
		else if (i == DIK_T)
			pismeno = 't';
		else if (i == DIK_U)
			pismeno = 'u';
		else if (i == DIK_V)
			pismeno = 'v';
		else if (i == DIK_W)
			pismeno = 'w';
		else if (i == DIK_X)
			pismeno = 'x';
		else if (i == DIK_Y)
			pismeno = 'y';
		else if (i == DIK_Z)
			pismeno = 'z';
		else if (i == DIK_PERIOD && this->isShiftPressed())
			pismeno = '>';
		else if (i == DIK_PERIOD)
			pismeno = '.';
		else if (i == DIK_COMMA && this->isShiftPressed())
			pismeno = '<';
		else if (i == DIK_COMMA)
			pismeno = ',';
		else if (i == DIK_DIVIDE)
			pismeno = '/';
		else if (i == DIK_MULTIPLY)
			pismeno = '*';
		else if (i == DIK_SUBTRACT)
			pismeno = '-';
		else if (i == DIK_ADD)
			pismeno = '+';
		else if (i == DIK_OEM_102 && this->isShiftPressed())
			pismeno = '|';
		else if (i == DIK_OEM_102)
			pismeno = '\\';
		else if (i == DIK_SEMICOLON && this->isShiftPressed())
			pismeno = ':';
		else if (i == DIK_SEMICOLON)
			pismeno = ';';
		else if (i == DIK_APOSTROPHE && this->isShiftPressed())
			pismeno = '"';
		else if (i == DIK_APOSTROPHE)
			pismeno = '\'';
		else if (i == DIK_EQUALS && this->isShiftPressed())
			pismeno = '+';
		else if (i == DIK_EQUALS)
			pismeno = '=';
		else if (i == DIK_SLASH && this->isShiftPressed())
			pismeno = '?';
		else if (i == DIK_SLASH)
			pismeno = '/';
		else if (i == DIK_LBRACKET && this->isShiftPressed())
			pismeno = '{';
		else if (i == DIK_LBRACKET)
			pismeno = '[';
		else if (i == DIK_RBRACKET && this->isShiftPressed())
			pismeno = '}';
		else if (i == DIK_RBRACKET)
			pismeno = ']';*/
		if (i == DIK_UPARROW)
		{
			g_CCore->GetChat()->ChatMessage = g_CCore->GetChat()->LastInput;
		}

		//if (this->isShiftPressed() || this->isCapsLockPressed())		// makes text uppercase if SHIFT is pressed
		//	pismeno = toupper(pismeno);

		/*if (pismeno != 0x00)
		{
			if (g_CCore->GetChat()->ChatMessage.length() < MAX_CHAT_MESSAGE_LENGTH - 10)
				g_CCore->GetChat()->ChatMessage += pismeno;
		}*/

		/*if (i == DIK_BACKSPACE)
		{
		if (g_CCore->GetChat()->ChatMessage.size() > 0)
		g_CCore->GetChat()->ChatMessage.resize(g_CCore->GetChat()->ChatMessage.size() - 1);
		}*/
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
	}
	else	// no typing
	{
		/*if (i == DIK_T)
		{
			g_CCore->GetChat()->shouldReRender = true;
			g_CCore->GetChat()->SetTyping(true);
		}*/
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

LRESULT CALLBACK ProcessMessage(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	//MessageBox(NULL, "wtf", "wtf", MB_OK);
	//g_CCore->GetChat()->AddMessage("ProccessMessage");
	/*switch (uMsg)
	{
	case WM_CHAR:
		char buff[50];
		sprintf(buff,"Char: %c",wParam);
		g_CCore->GetChat()->AddMessage(buff);
		return 0;
	}
	*/
	/*char buff[255];
	sprintf(buff, "WM message ID: %u", uMsg);
	g_CCore->GetLog()->AddLog(buff);
	*/
	//return CallWindowProc((WNDPROC)g_CCore->GetKeyboard()->oldProc, hwnd, uMsg, wParam, lParam);
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

HHOOK _hook;
KBDLLHOOKSTRUCT kbdStruct;
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	char buff[255];
	sprintf(buff, "WM message ID: %d", nCode);
	g_CCore->GetLog()->AddLog(buff);

	/*//g_CCore->GetChat()->AddMessage("HookCallback");
	if (nCode >= 0)
	{
		// the action is valid: HC_ACTION.
		if (wParam == WM_KEYDOWN)
		{
			// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
			kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
			// a key (non-system) is pressed.
			if (kbdStruct.vkCode == VK_F1)
			{
				// F1 is pressed!
				MessageBox(NULL, "F1 is pressed!", "key pressed", MB_ICONINFORMATION);
			}
		}
	}*/

	// call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);

	g_CCore->GetChat()->AddMessage("keyboard");
	// If key is being pressed
	//if (wParam == WM_KEYDOWN) {
	/*	switch (p->vkCode) {
			// Invisible keys
		case VK_CAPITAL:	out << "<CAPLOCK>";		break;
		case VK_SHIFT:		out << "<SHIFT>";		break;
		case VK_LCONTROL:	out << "<LCTRL>";		break;
		case VK_RCONTROL:	out << "<RCTRL>";		break;
		case VK_INSERT:		out << "<INSERT>";		break;
		case VK_END:		out << "<END>";			break;
		case VK_PRINT:		out << "<PRINT>";		break;
		case VK_DELETE:		out << "<DEL>";			break;
		case VK_BACK:		out << "<BK>";			break;
		case VK_LEFT:		out << "<LEFT>";		break;
		case VK_RIGHT:		out << "<RIGHT>";		break;
		case VK_UP:			out << "<UP>";			break;
		case VK_DOWN:		out << "<DOWN>";		break;
			// Visible keys
		default:
			out << char(tolower(p->vkCode));
		}
	}*/
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void	CKeyboard::ApplyHook()
{
	/*if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
	{
		MessageBox(NULL, "Failed to install hook!", "Error", MB_ICONERROR);
	}*/
	/*WNDPROC OldWndProc = (WNDPROC)SetWindowLongPtr(g_CCore->GetGraphics()->hwnAppWindow,
		GWLP_WNDPROC, (LONG_PTR)ProcessMessage);
	oldProc = OldWndProc;
	if (OldWndProc == NULL)
	{*/
/*		char buff[50];
		sprintf(buff, "#ff0000Err: 0x % x",GetLastError());
		g_CCore->GetChat()->AddMessage(buff);
*/		//MessageBox(NULL, "sex", "sex", MB_OK);
	//}
	//SetFocus(g_CCore->GetGraphics()->hwnAppWindow);

	//SetWindowLongW(g_CCore->GetGraphics()->hwnAppWindow, GWL_WNDPROC, GetWindowLong(g_CCore->GetGraphics()->hwnAppWindow, GWL_WNDPROC));
	//MessageBox(NULL, "wtf", "wtf", MB_OK);
	//SetWindowText(g_CCore->GetGraphics()->hwnAppWindow, "Vesmir je cesta");
	/*HHOOK keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,
		keyboardHookProc,
		GetModuleHandle(NULL),
		0);*/
}


void	CKeyboard::OnASCIIKeyDown(char key)
{
	this->ignore = !ignore;
	if (ignore == false)
	{
		/*char buff[20];
		sprintf(buff, "key: %c", key);
		g_CCore->GetChat()->AddMessage(buff);*/
		if (g_CCore->GetChat()->IsTyping())
		{
			//g_CCore->GetChat()->AddMessage("we are typing");
			g_CCore->GetChat()->shouldReRender = true;
			if (g_CCore->GetChat()->ChatMessage.length() < MAX_CHAT_MESSAGE_LENGTH - 10)
			{
				g_CCore->GetChat()->ChatMessage += key;
			}
		}
		else
		{
			if (key == 't' || key == 'T')
			{
				g_CCore->GetChat()->SetTyping(true);
				g_CCore->GetChat()->shouldReRender = true;
			}
		}
	}
}

