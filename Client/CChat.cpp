#include "CCore.h"
#include "CChat.h"
#define	DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "d3dx8.lib")

#include "../shared/gamestructures.h"

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h" 

extern CCore *g_CCore;


CChat::CChat()
{
	CHAT_WIDTH				=	370;
	CHAT_MAX_LINES			=	100;
	CHAT_LINES_PER_RENDER	=	12;
	CHAT_POINTER			=	0;
	IsRendering				=	false;
	bIsTyping				=	false;
	m_bBackground			=	false;
	chatTexture				=	NULL;

	shouldReRender			=	true;
	animation				=	0;
	
	this->ChatPoolStart = NULL;
	this->elementCount = 0;
}
CChat::~CChat()
{
}
void CChat::Render(IDirect3DDevice8* pInterface,LPD3DXFONT font)
{
	if (shouldReRender)
		this->RenderTexture(pInterface);
	if (chatTexture != NULL)
	{
		D3DXVECTOR2 scaling;
		scaling.x = 1.0f;
		scaling.y = 1.0f;
		D3DXVECTOR2 translation;
		translation.x = 0;
		translation.y = (float) animation;

		animation -= 1;
		if (animation < 0) animation = 0;
		
		if (g_CCore->GetGraphics()->GetSprite()->Begin() != S_OK)
			return;
		g_CCore->GetGraphics()->GetSprite()->Draw(chatTexture,NULL, &scaling, NULL, 0.0,&translation, D3DCOLOR_ARGB(255, 255, 255, 255));
		g_CCore->GetGraphics()->GetSprite()->End();
		
		return;
	}
}

void CChat::AddMessage(std::string message)
{
	//--- get num of chat messages, set reRender on

	if (message.length() > 0)
	{
		if (elementCount > (unsigned int) CHAT_LINES_PER_RENDER-1)
			animation = 20;
		//----
		CColoredText* newText = new CColoredText((char*)message.c_str());
		CChatStack* newStack = new CChatStack();
		newStack->text = newText;
		newStack->next = this->ChatPoolStart;
		this->ChatPoolStart = newStack;
		elementCount++;
	
		CColoredText* nextText = NULL;
		while (nextText = newText->SplitText(CHAT_WIDTH, true))
		{
			if (nextText->GetElementCount() == 0)
			{
				delete[] nextText;
				shouldReRender = true;
				return;
			}
			else {
				nextText->ReCalculate();
				newStack = new CChatStack();
				newStack->text = nextText;
				newStack->next = this->ChatPoolStart;
				this->ChatPoolStart = newStack;
				elementCount++;
			}
		}
		shouldReRender = true;
	}
}

void CChat::DoneMessage()
{
	if(ChatMessage != "")
	{
		if(ChatMessage.substr(0,1) == "/")
		{
			if(ChatMessage.size() > 1)
			{
				char buff[1024];
				sprintf(buff,"%s",ChatMessage.c_str());
				DoCommand(buff);
			}
		} else
		{
			RakNet::BitStream bsOut;
			char buffer[255];
			sprintf_s(buffer,"%s",ChatMessage.c_str());
			bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((RakNet::MessageID)LHMP_PLAYER_CHAT_MESSAGE);
			bsOut.Write(buffer);
			g_CCore->GetNetwork()->SendServerMessage(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED);
		}
		ChatMessage	= "";
	}
}

void CChat::DoCommand(char str[])
{
	char command[256];
	char varlist[512] = "";
	char *pch;
	pch = strchr(str,' ');
	if(pch == NULL)
	{
		memcpy(command,str+1,strlen(str));
	} else {
		memcpy(command,str+1,(pch-str));
		command[(pch-str)-1] = '\0';
		memcpy(varlist,pch+1,strlen(pch));
	}

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_COMMAND);
	bsOut.Write(command);
	bsOut.Write(varlist);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED);
	

	if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0 || strcmp(command, "q") == 0)
	{
		//g_CCore->GetNetwork()->GetPeer()->Shutdown(100,0,IMMEDIATE_PRIORITY);
		//TerminateProcess(GetCurrentProcess(), 0);
		g_CCore->ShutdownClient();
	}
	else if (strcmp(command, "setwidth") == 0)
	{
		if (strlen(varlist) > 0)
		{
			int newwidth = atoi(varlist);
			CHAT_WIDTH = Tools::Clamp(newwidth, 200, 500);
		}
	}
	else if (strcmp(command, "splittext") == 0)
	{
		g_CCore->GetGraphics()->textiqSecond = g_CCore->GetGraphics()->textiq->SplitText(200, false);
	}
	else if (strcmp(command, "setlines") == 0)
	{
		if (strlen(varlist) > 0)
		{
			int newwidth = atoi(varlist);
			if (newwidth > 5)
			{
				this->CHAT_LINES_PER_RENDER = newwidth;
			}
		}
	}

	else if (strcmp(command, "debug") == 0)
	{
		g_CCore->GetLog()->SetDebugState(!g_CCore->GetLog()->GetDebugState());
	}
	else if (strcmp(command, "killme") == 0)
	{
		g_CCore->GetGame()->KillPed(g_CCore->GetLocalPlayer()->GetBase());
	}
	else if(strcmp(command,"bg") == 0)
	{
		g_CCore->GetChat()->SetBackground(!this->IsBackgroundActive());
	}
	else if (strcmp(command, "getpos") == 0)
	{
		char buffer[255] = "";
		Vector3D pos = g_CCore->GetLocalPlayer()->GetLocalPos();
		sprintf(buffer, "PlayerPos: [X: %f] [Y: %f] [Z: %f]", pos.x,pos.y,pos.z);
		g_CCore->GetChat()->AddMessage(buffer);
	}
	else if (strcmp(command, "crash") == 0)
	{
		g_CCore->GetGame()->ChangeSkin(0, 5);	// wrong PED base => 100% crash :)
	}
	else if (strcmp(command, "fromcar") == 0)
	{
		g_CCore->GetGame()->KickPlayerFromCarFast(g_CCore->GetLocalPlayer()->GetBase());
	}
	else if (strcmp(command, "carsirene") == 0)
	{
		//g_CCore->GetGame()->KickPlayerFromCarFast(g_CCore->GetLocalPlayer()->GetBase());

		PED* local = (PED*) g_CCore->GetLocalPlayer()->GetBase();
		
		if (local)
		{
			if (local->playersCar)
			{
				DWORD frame = (DWORD) local->playersCar->object.frame;
				_asm {
					MOV ECX, frame
						MOV EAX, 0x0044DDE0
						CALL EAX;
				}
			}
		}

	}
	else if (strcmp(command, "changemap") == 0)
	{
		g_CCore->GetGame()->ChangeMap("MISE13-ZRADCE","");  //MISE20-PAULI
		//g_CCore->GetGame()->ChangeMap("MISE20-PAULI", "");
	}
	else if (strcmp(command, "changemap2") == 0)
	{
		//g_CCore->GetGame()->ChangeMap("MISE13-ZRADCE","");  MISE20-PAULI
		g_CCore->GetGame()->ChangeMap("MISE04-MOTOREST", "");
	}

	else if (strcmp(command, "getdoor") == 0)
	{
		//g_CCore->GetGame()->ChangeMap("MISE13-ZRADCE","");  MISE20-PAULI
		DWORD door = g_CCore->GetGame()->FindActor("Box66");

		char buff[255];
		sprintf(buff, "Door: %p", door);
		g_CCore->GetChat()->AddMessage(buff);
	}

	
	else if (strcmp(command, "reloadmap") == 0)
	{
		g_CCore->GetGame()->ReloadMap();
	}
	else if (strcmp(command, "kickfromcar") == 0)
	{
		g_CCore->GetGame()->KickPlayerFromCarFast(g_CCore->GetLocalPlayer()->GetBase());
	}
	else if (strcmp(command, "lang") == 0)
	{
		UINT uLayouts;
		HKL  *lpList = NULL;
		char szBuf[512]; 

			uLayouts = GetKeyboardLayoutList(0, NULL);
		lpList = (HKL*)LocalAlloc(LPTR, (uLayouts * sizeof(HKL)));
		uLayouts = GetKeyboardLayoutList(uLayouts, lpList);

		for (int i = 0; i < uLayouts; ++i)
		{
			GetLocaleInfo(MAKELCID(((UINT)lpList[i] & 0xffffffff),
				SORT_DEFAULT), LOCALE_SLANGUAGE, szBuf, 512);
			//wprintf(L"%s\n", szBuf);
			g_CCore->GetChat()->AddMessage(szBuf);
			memset(szBuf, 0, 512);
		}

		if (lpList)
			LocalFree(lpList);
	}
	else if (strcmp(command, "request") == 0)
	{
		//g_CCore->GetNetwork()->httpRequest(NULL, "lh-mp.eu/lastnews.php", Request);
	}
	else if (strcmp(command, "obor") == 0)
	{
		DWORD frame = *(DWORD*) (g_CCore->GetLocalPlayer()->GetEntity()+0x68);
		g_CCore->GetGame()->SetFrameScale(frame,2.0,2.0,2.0);
	}
	else if (strcmp(command, "turbo") == 0)
	{
		if (g_CCore->GetLocalPlayer()->IDinCar != -1)
		{
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(g_CCore->GetLocalPlayer()->IDinCar);
			if (veh != NULL)
			{
				Vector3D speed,rot = veh->GetRotation();
				speed.x = 50; speed.y = 50; speed.z = 50;
				speed.x = speed.x * rot.x; 
				speed.y = speed.y * rot.y;
				speed.z = speed.z * rot.z;
				veh->SetSpeed(speed);
			} 
		}
	} else if (strcmp(command, "chcek") == 0)
	{
		char * pEnd;
		long int vstup;
		vstup = strtol(varlist, &pEnd, 16);

		if (vstup != NULL)
		{
			int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vstup);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh)
			{
				g_CCore->GetGame()->CarUpdate(veh->GetEntity(), veh->GetPosition(), veh->GetRotation());
				g_CCore->GetChat()->AddMessage("Done");
			}
		}
			
	}
	else if (strcmp(command, "move") == 0)
	{
		char * pEnd;
		long int vstup;
		vstup = strtol(varlist, &pEnd, 16);

		if (vstup != NULL)
		{
			int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vstup);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh)
			{

				g_CCore->GetChat()->AddMessage("Done");
				g_CCore->GetGame()->CarUpdate(veh->GetEntity(), veh->GetPosition(), veh->GetRotation());
			}
		}
	}
	else if (strcmp(command, "pedframe") == 0)
	{
		PED* ped = *(PED**)((*(DWORD*)0x6F9464) + 0xE4);
		char buff[200];
		sprintf(buff, "Pos: %f %f %f Current wepID: %d", ped->object.position.x, ped->object.position.y, ped->object.position.z,ped->inventary.slot[0].weaponType);
		g_CCore->GetChat()->AddMessage(buff);

		sprintf(buff, "Health: %f Skin: %s", ped->health,ped->object.frame->frameModel);
		g_CCore->GetChat()->AddMessage(buff);

		
		sprintf(buff, "Pos: %f %f %f Gas: %f", ped->playersCar->position.x, ped->playersCar->position.y, ped->playersCar->position.z, ped->playersCar->gasState);
		g_CCore->GetChat()->AddMessage(buff);


	}
	else if (strcmp(command, "cpkar") == 0)
	{
		char * pEnd;
		long int vstup;
		vstup = strtol(varlist, &pEnd, 16);

		if (vstup != NULL)
		{
			int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vstup);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh)
			{

				g_CCore->GetChat()->AddMessage("Done");

				//------------ current CarUpdate
				/*DWORD entity = veh->GetEntity();

				//*(float*)(*(DWORD*)(entity + 0x438) + 0x40) += 10.0f;	// pos
				//*(float*)(*(DWORD*)(entity + 0x438) + 0x9C) += 0.1f;	// rot

				_asm {
					MOV EDI, entity
					MOV ECX, EDI;  EDI == actorBase
						MOV EAX, 0x00469DD0
						CALL EAx; Game.00469DD0
				}*/

				//--------------

				// takmer dokonale

				DWORD entity = veh->GetEntity();
				*(float*)(entity + 0x40C) += 0.1f;
				/*_asm
				{
				//MOV EAX, 0x0
				MOV EAX, 0x3DA9FBE8
				PUSH EAX; / Arg3 = 00000000
				PUSH EAX; | Arg2	0
				//MOV EAX, 0x3DA9FBE8
				PUSH EAX; | Arg1   	0x3DA9FBE8
				MOV ESI, entity
				ADD ESI, 0x70
				MOV ECX, ESI; | base + 0x70
				MOV EAX, 0x0052E6D0
				CALL EAX; game.0052E6D0; \game.0052E6D0
				}*/
				_asm {
				MOV ESI, entity
				PUSH 0x10; / Arg1 = 00000010
				LEA ECX, DWORD PTR DS : [ESI + 0x70]; |
				MOV EAX, 0x0052C3B0
				CALL EAX; Game.0052C3B0; \Game.0052C3B0


				}
				/*_asm {
				MOV ESI, entity
				MOV ECX, ESI
				MOV EAX, DWORD PTR DS : [ESI]
				PUSH 0x31
				CALL DWORD PTR DS : [EAX + 0x34]


				}*/
				//}
				// car_calm func
				/*_asm
				{
				PUSH 0x1
				MOV ECX, entity
				MOV EAX, 0x00470D30
				CALL EAX; Game.00470D30
				TEST AL, AL
				JE end
				PUSH 0x14; / Arg1 = 00000014
				MOV ECX, entity
				ADD ECX, 0x70; |
				MOV EAX, 0x0051A920
				CALL EAX; Game.0051A920; \Game.0051A920
				end:


				}
				*/
			}
		}
	}
	else if (strcmp(command, "fas") == 0)
	{
		char * pEnd;
		long int vstup;
		vstup = strtol(varlist, &pEnd, 16);

		if (vstup != NULL)
		{
			int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vstup);
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
			if (veh)
			{
		
				g_CCore->GetChat()->AddMessage("Done");
				Vector3D move;
				move.x = 3000.0f;
				move.y = 0.0f;
				move.z = 0.0f;
				//Vector3D position = veh->GetPosition();
				//position.x += 10.0f;
				//veh->SetPosition(position);

				DWORD entity = veh->GetEntity();
				/*_asm {
					MOV ESI,entity
						MOV ECX, DWORD PTR DS : [ESI + 0x2198]
						PUSH 0; / Arg1 = 00000000
						OR ECX, 1; |
						MOV DWORD PTR DS : [ESI + 0x2198], ECX; |
						MOV ECX, ESI; |
						MOV EAX, 0x0044C750;
					CALL EAX; Game.0044C750; \triggers car activate
				}
					
				_asm {
					mov ECX, 0x44924000
					PUSH ECX; / 44924000
					MOV EDX, 0x00656330
					PUSH EDX; | 00656330
					lea ECX, move
					lea EDX, position
					PUSH ECX; | vector of movement
					PUSH EDX; | position vector ?
					PUSH 0x3F99999A; | Arg2 = 3F99999A
					MOV EAX, entity
					MOV ESI, EAX
					PUSH EAX; | actor(vehicle)
					LEA ECX, DWORD PTR DS : [ESI + 0x70]; | ESI = again some actor(vehicle)
					MOV EAX, 0x005180A0
					CALL EAX; Game.005180A0; \Game.005180A0
					}*/
				/*_asm {
					MOV ESI, entity
					PUSH 0; / Arg1
					MOV ECX, ESI; |
					MOV EAX, 0x0044C750
					CALL EAX; Game.0044C750; \Game.0044C750 // trigger type 2
					MOV BYTE PTR DS : [ESI + 0x78], 1
				}*/



				//position.x += 10.0f;
				*(float*)(entity + 0x40C) += 10.0f;
				//*(float*)(entity + 0x410) = this->playerPos.y;
				//*(float*)(entity + 0x414) = this->playerPos.z;

				Vector3D normal;
				normal.x = normal.z = 1.0f;
				normal.y = 0.0f;

				Vector3D position;
				position.x = *(float*)(entity + 0x40C);
				position.y = *(float*)(entity + 0x410);
				position.z = *(float*)(entity + 0x414);

				_asm {
					MOV EBP, entity
					LEA ECX, DWORD PTR SS : [EBP + 0x70]; 
					lea EDX, normal
					PUSH EDX; / Arg4 normal vector ? or what
					PUSH 1; | Arg3 = 00000001
					LEA EAX, position
					PUSH EAX; | Arg2 position
					lea EDX, move
					PUSH EDX; | Arg1 strength vector
					mov eax, 0x00517B10
					CALL EAX;  Game.00517B10; \Game.00517B10


				}
			}
		}

	}

	


	/*else if (strcmp(command, "getframe") == 0)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(g_CCore->GetLocalPlayer()->IDinCar);

		if (veh != NULL)
		{
			DWORD base = veh->GetEntity();
			char buff[100];
			sprintf(buff, "carframe: 0x%p", base);
			g_CCore->GetChat()->AddMessage(buff);
		}
	}
	else if (strcmp(command, "explodeID") == 0)
	{
		int ID = atoi(varlist);
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);

		if (veh != NULL)
		{
			DWORD base = veh->GetEntity();
			_asm {
				PUSH 0x0; / Arg1	reason
					MOV ECX, base; | car
					MOV EAX, 0x00468BC0
					CALL EAX; Game.00468BC0; \Game.00468BC0
			}
		}
	}
	else if (strcmp(command, "setfocus") == 0)
	{
		int ID = atoi(varlist);
		CPed* ped = g_CCore->GetPedPool()->Return(ID);

		if (ped != NULL)
		{
			DWORD base = ped->GetEntity();
			g_CCore->GetChat()->AddMessage("Done");
			g_CCore->GetGame()->CameraSetFocusOnObject(base);
		}
	}
	else if (strcmp(command, "setdefaultfocus") == 0)
	{
		g_CCore->GetGame()->CameraSetDefaultFocus();
	}
	else if (strcmp(command, "doexplode") == 0)
	{
		Vector3D position = g_CCore->GetLocalPlayer()->GetLocalPos();
		position.x += 20.0f;
		Vector3D rot = g_CCore->GetLocalPlayer()->GetLocalRot();
		DWORD base = g_CCore->GetLocalPlayer()->GetEntity();
		_asm
		{
			XOR EAX, EAX
			PUSH EAX; / Arg8
			PUSH EAX; | Arg7
			PUSH EAX; | Arg6
			MOV EAX, 0x428D375C
			PUSH EAX; | Arg5
			LEA EAX, position; | Arg4 = position
			PUSH EAX; | Arg4
			PUSH EAX; | Arg3
			LEA EAX, rot
			PUSH EAX; | Arg2
			MOV ECX, 0x2
			PUSH ECX; | Arg1 = 00000002
			MOV ECX, base
			mov EAX, 0x00496710
			CALL EAX; Game.00496710; \Game.00496710
		}


	}
	else if (strcmp(command, "explode") == 0)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(g_CCore->GetLocalPlayer()->IDinCar);
		
		if (veh != NULL)
		{
			DWORD base = veh->GetEntity();
			_asm {
				PUSH 0x0; / Arg1	reason
				MOV ECX, base; | car
				MOV EAX, 0x00468BC0
				CALL EAX; Game.00468BC0; \Game.00468BC0
			}
		}
	}

	else if (strcmp(command, "effect") == 0)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(g_CCore->GetLocalPlayer()->IDinCar);

		//if (veh != NULL)
	//	{
			//DWORD base = veh->GetEntity();
			//Vector3D position = veh->GetPosition();
		DWORD base = g_CCore->GetLocalPlayer()->GetEntity();
		Vector3D position = g_CCore->GetLocalPlayer()->GetLocalPos();
			_asm {
				PUSH 0x0B7
				PUSH 0x1
				PUSH 0x10
				MOV ECX, DWORD PTR DS : [0x65115C]; game.006F9440
				PUSH 0xC08AB001; -WTF
				PUSH 0x43fa0000; ORIGINAL = 0x43480000
				MOV ECX, DWORD PTR DS : [ECX + 0x24]
				PUSH 0x3f800000; ORIGINAL = 0x41200000
				lea EAX, position
				PUSH EAX; positionVector
				MOV EDI, base
				PUSH EDI; carBase
				MOV EAX, 0x005E6760
				CALL EAX; game.005E6760


			}
		
	}
	else if (strcmp(command, "effect2") == 0)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(g_CCore->GetLocalPlayer()->IDinCar);

		//if (veh != NULL)
		//	{
		//DWORD base = veh->GetEntity();
		//Vector3D position = veh->GetPosition();
		DWORD base = g_CCore->GetLocalPlayer()->GetEntity();
		Vector3D position = g_CCore->GetLocalPlayer()->GetLocalPos();
		_asm {
			PUSH 0x0B7
				PUSH 0x1
				PUSH 0x1
				MOV ECX, DWORD PTR DS : [0x65115C]; game.006F9440
				PUSH 0xC08AB001; -WTF
				PUSH 0x43480000
				MOV ECX, DWORD PTR DS : [ECX + 0x24]
				PUSH 0x41200000
				lea EAX, position
				PUSH EAX; positionVector
				MOV EDI, base
				PUSH 0x0
				MOV EAX, 0x005E6760
				CALL EAX; game.005E6760


		}

	}
	else if (strcmp(command, "deleteframe") == 0)
	{
		if (strlen(varlist) > 0)
		{
			DWORD obj = NULL;
			obj = strtol(varlist, 0, 16);
			_asm {
				mov eax, obj
				push eax
				mov ecx, [eax]
				call dword ptr ds : [ecx]
			}
		}
	}
	else if (strcmp(command, "cardel") == 0)
	{
		if (strlen(varlist) > 0)
		{
			DWORD obj = NULL;
			obj = strtol(varlist, 0, 16);
			g_CCore->GetGame()->DeleteCar(obj);
		}
	}
	else if (strcmp(command, "rel") == 0)
	{
		DWORD base = g_CCore->GetLocalPlayer()->GetBase();
		_asm {
			XOR EBX, EBX
			MOV ESI, base
			LEA ECX, DWORD PTR DS : [ESI + 0x480];  Case 1 of switch 00490E36
			MOV EAX, 0x00559BA0
			CALL EAX; Game.00559BA0
			MOV EDI, EAX
			MOV EAX, DWORD PTR DS : [ESI + 0x6C4]
			CMP EAX, EBX
			MOV BYTE PTR DS : [ESI + 0x1FC], 0x1
			JE SHORT jmp01
			MOV ECX, DWORD PTR DS : [EAX]
			PUSH 0x1
			PUSH EAX
			CALL DWORD PTR DS : [ECX + 0x24]
			jmp01:
			MOV EAX, DWORD PTR DS : [ESI + 0x98]
			CMP EAX, EBX
			JE SHORT jmp02
			CMP DWORD PTR DS : [EAX + 0x10], 0x4
			JNZ SHORT jmp02
			CMP DWORD PTR DS : [ESI + 0xAC], EBP
			JNZ end
			jmp02:
			MOV ECX, 0x1388
			MOV BYTE PTR DS : [ESI + 0x1E5], 0
			MOV EAX, 0x0044DE50
			CALL EAX; Game.0044DE50
			ADD EAX, 0x1388
			MOV DWORD PTR DS : [ESI + 0x594], EBP
			MOV DWORD PTR DS : [ESI + 0x590], EAX
			MOV AL, BYTE PTR DS : [ESI + 0x1E4]
			MOV EDI, DWORD PTR DS : [EDI + 0x4C]
			TEST AL, AL
			PUSH EDI
			JE SHORT jmp03
			LEA EDX, DWORD PTR SS : [ESP + 0x8C]
			PUSH 0x0064BB74;  ASCII "gun0%i drep Reload.i3d"
			PUSH EDX
			JMP SHORT jmp04
			jmp03:
			LEA EAX, DWORD PTR SS : [ESP + 0x8C]
			PUSH 0x0064BB5C;  ASCII "gun0%i stoj Reload.i3d"
			PUSH EAX
			jmp04:
			MOV EAX, 0x00624FCF
			CALL EAX; Game.00624FCF
			ADD ESP, 0x0C
			CMP EDI, 0x8
			JNZ SHORT jmp05
			CMP WORD PTR DS : [ESI + 0x1CA], 0x0C
			JE SHORT end
			jmp05:
			PUSH EBX; / Arg4
			LEA ECX, DWORD PTR SS : [ESP + 0x18]; |
			PUSH EBX; | Arg3
			LEA EDX, DWORD PTR SS : [ESP + 0x90]; |
			PUSH ECX; | Arg2
			PUSH EDX; | Arg1
			MOV ECX, 0x006F93C8; |
			MOV DWORD PTR SS : [ESP + 0x24], EBX; |
			MOV EAX, 0x0047FA10
			CALL EAX; Game.0047FA10; \game.0047FA10
			MOV EAX, DWORD PTR SS : [ESP + 0x14]
			CMP EAX, EBX
			JE SHORT end
			CMP EDI, 0x8
			SETNE CL
			PUSH ECX
			PUSH 0x40800000
			PUSH 0x3F800000
			PUSH EAX
			LEA ECX, DWORD PTR DS : [ESI + 0xBC]
			MOV EAX, 0x005ACB70
			CALL EAX; Game.005ACB70
			CMP EDI, 0x8
			JNZ SHORT jmp06
			MOV WORD PTR DS : [ESI + 0x1CA], 0x0C
			jmp06:
			MOV EAX, DWORD PTR SS : [ESP + 0x14]
			PUSH EAX
			MOV EDX, DWORD PTR DS : [EAX]
			CALL DWORD PTR DS : [EDX]
			end:
		}
	}
	*/
}

bool CChat::IsTyping()
{
	return bIsTyping;
}

void CChat::SetTyping(bool b)
{
	bIsTyping = b;
	g_CCore->GetGame()->UpdateControls();
}


void CChat::SetBackground(bool bg)
{
	m_bBackground = bg;
}

bool CChat::IsBackgroundActive()
{
	return m_bBackground;
}

void	CChat::RenderTexture(IDirect3DDevice8* device)
{
	if (chatTexture == NULL)
	{
		D3DXCreateTexture(device, 500, 500,
			1, D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &chatTexture);
	}
	if (chatTexture == NULL)
	{
		return;
	}

	shouldReRender = false;

	IDirect3DSurface8* pSurf, *pOldTarget, *oldStencil;
	
	if (!SUCCEEDED(chatTexture->GetSurfaceLevel(0, &pSurf)))
		return;
	if (!SUCCEEDED(device->GetRenderTarget(&pOldTarget)))
		return;
	if (!SUCCEEDED(device->GetDepthStencilSurface(&oldStencil)))
		return;
	HRESULT hr;
	hr = device->SetRenderTarget(pSurf, NULL);
	if (FAILED(hr)) {
		char buffer[255];
		D3DXGetErrorString(hr, buffer, 200);
		sprintf(buffer, "SetRenderTarget %s", buffer);
		MessageBoxA(NULL, buffer, buffer, MB_OK);
	}

	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 255, 255, 255), 1.0f, 0);
	
	if (this->IsBackgroundActive() == 1)
	{
		int howMany = elementCount;
		if (elementCount > (unsigned int)this->CHAT_LINES_PER_RENDER)
			howMany = this->CHAT_LINES_PER_RENDER;
		g_CCore->GetGraphics()->Clear(10, 10, 10 + CHAT_WIDTH, 20 + (howMany * 20), D3DCOLOR_ARGB(200, 0, 0, 0));
	}
	int iRendered = 0;

	// Render chat lines
	int howMany = elementCount;
	if (elementCount > (unsigned int)this->CHAT_LINES_PER_RENDER)
		howMany = this->CHAT_LINES_PER_RENDER;
	
	CChatStack* stackPointer = this->ChatPoolStart;
	for (int i = 0; i < howMany; i++)
	{
		int line_y = (20 * (howMany-i));
		//g_CCore->GetGraphics()->DrawTextA(stack, 20, line_y, 0xffffffff, true, true);
		g_CCore->GetGraphics()->GetFont()->DrawColoredText(stackPointer->text, 20, line_y, true);
		stackPointer = stackPointer->next;
	}
	iRendered = howMany;
	// Render input with its background if it's needed
	if (IsTyping())
	{

		int base_y = 30 + (20 * iRendered);
		iRendered = 0;
		if (ChatMessage == "")
		{
			if (this->IsBackgroundActive() == true)
				g_CCore->GetGraphics()->Clear(10, base_y, 10 + CHAT_WIDTH, 30, D3DCOLOR_ARGB(200, 50, 0, 0));
				//g_CCore->GetGraphics()->FillARGB(10, base_y, 10 + CHAT_WIDTH, 30, D3DCOLOR_ARGB(200, 50, 0, 0));
			//return;
			g_CCore->GetGraphics()->GetFont()->DrawTextA(">", 21, base_y + 5, D3DCOLOR_XRGB(200, 200, 200), true);
			//g_CCore->GetGraphics()->DrawTextA(">", 21, base_y + 5, D3DCOLOR_XRGB(200, 200, 200), true, true);
		}
		else
		{
			char buff[255];
			sprintf(buff, "%s", ChatMessage.c_str());
			int index = 0;
			while (1)
			{
				int howMuchWeNeed = g_CCore->GetGraphics()->GetStrlenForWidth(CHAT_WIDTH - 10, buff + index);
				std::string	farba = g_CCore->GetGraphics()->GetLastColorInText(buff, index);
				if (howMuchWeNeed == 0) break;
				char buf[255];
				sprintf(buf, "%s%s", farba.c_str(), ChatMessage.substr(index, howMuchWeNeed).c_str());
				int line_y = base_y + (30 * iRendered);
				if (this->IsBackgroundActive() == true)
					g_CCore->GetGraphics()->Clear(10, line_y, 10 + CHAT_WIDTH, 30, D3DCOLOR_ARGB(200, 50, 0, 0));
				//g_CCore->GetGraphics()->GetFont()->DrawColoredText(buf, 21, line_y + 5, D3DCOLOR_XRGB(200, 200, 200), true);
				g_CCore->GetGraphics()->DrawTextA(buf, 21, line_y + 5, D3DCOLOR_XRGB(200, 200, 200), true, true);
				index += howMuchWeNeed;
				iRendered++;
			}
		}
	}
	
	device->SetRenderTarget(pOldTarget, oldStencil);
	pSurf->Release();
	pOldTarget->Release();
	oldStencil->Release();
	

}

void	CChat::DoRendering()
{
	if (this->IsBackgroundActive() == 1)
	{
		int howMany = elementCount;
		if (elementCount > (unsigned int)this->CHAT_LINES_PER_RENDER)
			howMany = this->CHAT_LINES_PER_RENDER;
		g_CCore->GetGraphics()->Clear(10, 10, 10 + CHAT_WIDTH, 20 + (howMany * 20), D3DCOLOR_ARGB(200, 0, 0, 0));
	}
	int iRendered = 0;

	// Render chat lines
	int howMany = elementCount;
	if (elementCount > (unsigned int)this->CHAT_LINES_PER_RENDER)
		howMany = this->CHAT_LINES_PER_RENDER;

	CChatStack* stackPointer = this->ChatPoolStart;
	for (int i = 0; i < howMany; i++)
	{
		int line_y = (20 * (howMany - i));
		g_CCore->GetGraphics()->GetFont()->DrawColoredText(stackPointer->text, 20, line_y, true);
		stackPointer = stackPointer->next;
	}
	iRendered = howMany;
	// Render input with its background if it's needed
	if (IsTyping())
	{

		int base_y = 30 + (20 * iRendered);
		iRendered = 0;
		if (ChatMessage == "")
		{
			if (this->IsBackgroundActive() == true)
				g_CCore->GetGraphics()->Clear(10, base_y, 10 + CHAT_WIDTH, 30, D3DCOLOR_ARGB(200, 50, 0, 0));
			//return;
			g_CCore->GetGraphics()->GetFont()->DrawTextA(">", 21, base_y + 5, D3DCOLOR_XRGB(200, 200, 200), true);
		}
		else
		{
			char buff[255];
			sprintf(buff, "%s", ChatMessage.c_str());
			int index = 0;
			while (1)
			{
				int howMuchWeNeed = g_CCore->GetGraphics()->GetStrlenForWidth(CHAT_WIDTH - 10, buff + index);
				std::string	farba = g_CCore->GetGraphics()->GetLastColorInText(buff, index);
				if (howMuchWeNeed == 0) break;
				char buf[255];
				sprintf(buf, "%s%s", farba.c_str(), ChatMessage.substr(index, howMuchWeNeed).c_str());
				int line_y = base_y + (30 * iRendered);
				if (this->IsBackgroundActive() == true)
					g_CCore->GetGraphics()->Clear(10, line_y, 10 + CHAT_WIDTH, 30, D3DCOLOR_ARGB(200, 50, 0, 0));

				g_CCore->GetGraphics()->DrawText(buf, 21, line_y + 5, D3DCOLOR_XRGB(200, 200, 200), true,true);
				index += howMuchWeNeed;
				iRendered++;
			}
		}
	}
}

__declspec(noinline) void	CChat::OnLostDevice()
{
	if (chatTexture != NULL)
	{
		chatTexture->Release();
		chatTexture = NULL;
	}
}

void	CChat::OnResetDevice()
{
	chatTexture = false;
	shouldReRender = true;
}


bool	CChat::shouldWeRerender()
{
	return this->shouldReRender;
}
void	CChat::SetRerenderState(bool state)
{
	this->shouldReRender = state;
}