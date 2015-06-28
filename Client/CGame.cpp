#include "CGame.h"
#include "CCore.h"


#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../shared/gamestructures.h"
extern CCore *g_CCore;

CGame::CGame()
{
	ShouldStop = false;
	IsStopped = false;
	MusicState = false;
	sprintf(mapName, "freeride");
	loadingStatus = 0.0f;
	bLockControls = false;
	pickupsAngle = 0.0f;
	ShouldKill = false;

}
CGame::~CGame()
{
}
void CGame::UpdateCars()
{
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(i);
		if (veh != NULL)
		{
			veh->Interpolate();
		}
	}
}

void CGame::ToggleVehicleRoof(DWORD vehbase, BYTE state)
{
	__asm
	{
		MOV EAX, vehbase
		MOV ECX, DWORD PTR DS : [EAX + 0x68]
		MOV DL, state
		MOV EAX, 0x00472DE0
		CALL EAX
	}
}

void CGame::ToggleVehicleEngine(DWORD vehicle, BYTE state)
{
	if (state == 1)
	{
		__asm
		{
			MOV EAX, vehicle
				ADD EAX, 0x70
				MOV    BYTE PTR DS : [EAX + 0xCA8], 1
				MOV BYTE PTR DS : [EAX + 0x6B4], 1
		}
	}
	else if (state == 0)
	{
		__asm
		{
			MOV EAX, vehicle
				ADD EAX, 0x70
				MOV    BYTE PTR DS : [EAX + 0xCA8], 0
				MOV BYTE PTR DS : [EAX + 0x6B4], 0
				MOV BYTE PTR DS : [EAX + 0x66C], 0
		}
	}
}

void CGame::ToggleCityMusic(byte state)
{
	g_CCore->GetGame()->SetMusicState(state);

	__asm
	{
		XOR EAX,EAX
		MOV AL, state
		PUSH EAX;  Case 139 of switch 005BB320
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		MOV EAX, 0x00425390

		CALL EAX
		MOV ECX, EAX
		MOV EAX, 0x005E9DF0
		CALL EAX
	}
}

void CGame::UpdatePeds()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		//int* myarray = new int[1000];
		CPed* ped = g_CCore->GetPedPool()->Return(i);
		if (ped != NULL)
		{
			ped->Interpolate();
			ped->gCheckWeapons();
		}
	}
}

void CGame::Tick()
{
	//g_CCore->GetGame()->SetTrafficVisible(false);
	Vector3D cam;
	float rot1,rot2,rot3;
	int randomNum;
	if(g_CCore->GetNetwork()->IsConnected() == false)
	{
		DWORD actual = timeGetTime();
		if(gameStart == 0)
			gameStart = actual;

		if((actual-gameStart) > 20000 || (actual-gameStart) < 100)
		{
			if((actual-gameStart) > 100)
				gameStart = actual-10000;
			randomNum = rand() % 5;
			switch(randomNum)
			{
			case 0:
				cam.x = -1788.927612f;
				cam.y = -4.542368f;
				cam.z = -9.105090f;
				rot1 = 0.982404f, rot2 = 0.520000f, rot3 = 1.017291f;
			break;
			case 1:
				cam.x = -946.888489f;
				cam.y =  7.617611f;
				cam.z = 123.585570f;
				rot1 = 1.345240f, rot2 = 0.440000f, rot3 = -0.436266f;
			break;
			case 2:
				cam.x = -1118.757568f;
				cam.y = 21.857584f;
				cam.z = 466.080688f;
				rot1 = 1.0f, rot2 = -0.680000f, rot3 = 0.927845f;
			break;
			case 3:
				cam.x = -3517.833252f;
				cam.y = 17.697630f;
				cam.z = -606.585388f;
				rot1 = -0.022980f, rot2 = -0.440000f, rot3 = -1.414027f;
			break;
			case 4:
				cam.x = 912.252319f;
				cam.y = 41.057575f;
				cam.z = -903.820923f;
				rot1 = 0.530041f, rot2 = -0.120000f, rot3 = 1.311128f;
			break;
			}
			g_CCore->GetGame()->SetCameraPos(cam,rot1,rot2,rot3,0);
		}
	/*		}
		}*/
	}
	else {
		this->PickupsTick();
	}
	
	g_CCore->GetEngineStack()->DoMessage();

	if (g_CCore->m_bIsRespawning == false)
	{
		if (this->GetLocalPED() != NULL)
		{
			if (strcmp(this->mapName, this->GetActualMapName()) != 0)
			{
				Tools::SetString((DWORD)globalMap, this->mapName);
				g_CCore->GetGame()->ChangeMap((char*)globalMap, "");
			}
		}
	}

	g_CCore->GetKeyboard()->ProceedHoldingKeys();
}
void CGame::StopAndWaitForGame()
{
	IsStopped = false;
	ShouldStop = true;
	while(IsStopped == false)
	{
		Sleep(10);
	}
}
void CGame::ContinueGame()
{
	ShouldStop = false;
}
void CGame::Camera_lock(DWORD address)
{
	_asm
	{
		sub esp, 0x400
		LEA ECX,DWORD PTR SS:[ESP+0x388]
		PUSH ECX                                 ; /Arg1
		MOV EAX, address
		MOV ECX,EAX                              ; |cameraFrame
		MOV EAX, 0x00425220
		CALL EAX							     ; \Game.00425220	// this fce gets frame position and saves it at ESP+0x388
		PUSH EAX
		MOV ECX, address						 ;  cameraFrame
		MOV EAX, 0x00425200
		CALL EAX								 ;  Game.00425200	// return frame's rotation vector address
		MOV ECX,DWORD PTR DS:[0x65115C]          ;  Game.006F9440
		PUSH EAX
		MOV EAX, 0x00425390
		CALL EAX								 ;  Game.00425390
		MOV ECX,EAX
		MOV EAX, 0x00425510
		CALL EAX								 ;  Game.00425510
		MOV ECX,EAX
		MOV EAX, 0x005DB000
		CALL EAX			                     ;  entire camera change - Game.005DB000   
		MOV ECX,DWORD PTR DS:[0x65115C]          ;  Game.006F9440
		MOV EAX, 0x00425390
		CALL EAX								 ;  Game.00425390
		MOV ECX,EAX
		MOV EAX, 0x005C8B50
		CALL EAX								 ;  Game.005C8B50
		CMP EAX,0x6
		JNB SHORT end
		MOV ECX,DWORD PTR DS:[0x65115C]          ;  Game.006F9440
		MOV EAX, 0x00425390
		CALL EAX								 ; Game.00425390
		MOV ECX,EAX
		MOV EAX, 0x005F5C60
		CALL EAX								 ; Game.005F5C60
		end:
		add esp, 0x400
	}

	/*if(address == NULL)	// we want to restore old camera behind player body
	{
		_asm
		{
			pushad
			MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440; Case 77 of switch 005BB320
			PUSH 0
			mov eax,0x00425390
			CALL eax								 ;	Game.00425390
			MOV ECX,EAX
			mov eax,0x00425510
			CALL eax								 ;	Game.00425510
			MOV ECX,EAX
			mov eax,0x005DB5D0
			CALL eax								 ;	Game.005DB5D0
			MOV ECX,DWORD PTR DS:[65115C]            ;  Game.006F9440
			mov eax,0x00425390
			CALL eax								 ;	Game.00425390
			MOV ECX,EAX
			mov eax,0x005F19C0
			CALL eax								 ;	Game.005F19C0
			popad
		}
	} else
	{
	*/
	/*	float angle = -0.5f;
		_asm
		{
			push angle
			lea eax,address
			push eax
			mov eax, 0x2F9464
			lea eax,[eax]
			lea ecx,[eax+0x4C]
			//MOV ECX,EAX
			mov eax,0x005DB000
			CALL eax									;Game.005DB000                       ;  entire camera change
			MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
			mov eax, 0x00425390
			CALL eax									;Game.00425390
			MOV ECX,EAX
			mov eax, 0x005C8B50
			CALL eax									;Game.005C8B50
			CMP EAX,6
			jnb next									; JNB SHORT Game.005BF92F
			MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
			mov eax,0x00425390
			CALL eax									;Game.00425390
			MOV ECX,EAX
			mov eax,0x005F5C60
			CALL eax									;Game.005F5C60
			jmp end
			next:
			MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
			mov eax, 0x00425390
			CALL eax									;Game.00425390
			MOV ECX,EAX
			mov eax, 0x005F19C0
			CALL eax									;Game.005F19C0
			end:
		}
		*/
//	}
}
void CGame::ChangeSkin(DWORD PED,int skinId)
{
	char buff[500];
	sprintf(buff, "CGame::ChangeSkin %d %d", PED, skinId);
	g_CCore->GetLog()->AddLog(buff);

	skinId = Tools::Clamp(skinId, 0, (int) (sizeof(SKINS) / 200));
	char * actualModel = (char*) (*(DWORD*)(*(DWORD*)(PED+0x68)+0x154)+0x0);
	char isExact[255];
	sprintf(isExact,"Models\\%s.i3d",SKINS[skinId]);
	if(strcmp(isExact,actualModel) == 0)
	{
		//g_CCore->GetChat()->AddMessage("Skins are same");
		return;
	}
	char buffer[255];
	sprintf(buffer,"%s.i3d",SKINS[skinId]);
	DWORD funcAdress	= 0x004A7700;
	_asm
	{
		pushad
			mov ecx,PED
			push 0
			lea eax,buffer
			push eax
			mov ebx, 0x004A7700
			call ebx
		popad
	}
}

DWORD CGame::CreateCar(int skin, Vector3D position, Vector3D rotation)
{
	//CreateCar(skin);
	DWORD entity = g_CCore->GetGame()->CreateCar(skin);
	if (entity)
	{
		// add car to radar pool
		_asm {
			PUSH -1
			PUSH ESI; car
			MOV ECX, 0x00658330
			MOV EAX,0x0054C630
			CALL EAX
		}
		g_CCore->GetGame()->CarUpdate(entity, position , rotation);
		return entity;
	}
	return NULL;
}

DWORD CGame::CreateCar(int skin)
{
	skin = Tools::Clamp(skin, 0, (int)(sizeof(CAR_SKINS)/200));
	DWORD caraddr = NULL;
	DWORD addr = NULL;
	char str1[255] = "lhmp_01";
	char str2[255]; // = "arrow00.i3d";
	sprintf(str2, "%s.i3d", CAR_SKINS[skin]);
	_asm
	{
		pushad
		MOV EAX,DWORD PTR DS:[0x6F9520]            ;  Case 185 of switch 005BB320
		PUSH 9
		PUSH EAX
		MOV ECX,DWORD PTR DS:[EAX]
		CALL DWORD PTR DS:[ECX+0x4C]				// CreateFrame
		mov addr,eax
		MOV ESI,EAX									// esi stores frame
		TEST ESI,ESI
		JE badresult_dest
		MOV EDX,DWORD PTR DS:[ESI]
		PUSH 0x0064FF98                       ;  ASCII "cheat_car"
		PUSH ESI
		CALL DWORD PTR DS:[EDX+0x28]				// frame->SetUnicateName
		PUSH 0                                   ; /Arg6 = 00000000
		PUSH 0                                   ; |Arg5 = 00000000
		PUSH 0                                   ; |Arg4 = 00000000
		PUSH 0                                   ; |Arg3 = 00000000
		lea eax, str2
		PUSH EAX								 ; |Arg2 = 0064FF84 ASCII "thunderbird00.i3d"
		PUSH ESI                                 ; |Arg1
		MOV ECX,0x006F9418						 ; |
		mov eax,0x00448940
		CALL eax								; \Game.00448940
		// Load graphic model into frame
		// funkcia vytvorí 3D objekt v nami vyzvorenom I3D_Frame, objekt je ale nefyzický (nemá kolízie ani ïalšie veci)
		TEST EAX,EAX
		JNZ divnyjump_dest
		PUSH 0x21F4								// creates object (It allocates memory in Heap for object)
		mov eax,0x00624934
		CALL eax
		ADD ESP,4
		mov caraddr,eax
		MOV DWORD PTR SS:[ESP+0x18],EAX
		TEST EAX,EAX
		MOV DWORD PTR SS:[ESP+0x7FC],0x0A
		JE SHORT zlyjump_dest
		MOV ECX,EAX
		mov eax,0x00462AF0
		CALL eax								// fills CAR game object (with defaults - you will find 0x4 there, so it is ONLY for cars)
		MOV EDI,EAX
		JMP SHORT dobryjump_dest
	zlyjump_dest:
		XOR EDI,EDI
	dobryjump_dest:
		TEST EDI,EDI
		MOV DWORD PTR SS:[ESP+0x7FC],-1
		JE divnyjump_dest
		LEA EAX,DWORD PTR SS:[ESP+0x94]
		LEA ECX,DWORD PTR SS:[ESP+0x84]
		PUSH EAX								// coord z
		LEA EDX,DWORD PTR SS:[ESP+0x94]
		PUSH ECX								// coord y
		PUSH EDX								// now it stores pointer to vector - coord x
		MOV ECX,ESI								// object (frame)
		MOV DWORD PTR SS:[ESP+0x9C],0xc4f7a000	// coordinates x
		MOV DWORD PTR SS:[ESP+0x90],0xc09ccccd	// y
		MOV DWORD PTR SS:[ESP+0xA0],0x41bb3333	// z
		mov eax,0x005C82B0
		CALL eax								// Change object's position (actually change frame's position)
		MOV EBX,DWORD PTR DS:[ESI]				// get frame's vtable
		PUSH 0
		PUSH 0
		PUSH 0
		PUSH 0x3f800000
		LEA ECX,DWORD PTR SS:[ESP+0x494]
		mov eax,0x004026C0
		CALL eax								// makes a 3D vector
		PUSH EAX								// 3d vector
		PUSH ESI								// frame 
		CALL DWORD PTR DS:[EBX+0xC]				// setframesize ?
		MOV EAX,DWORD PTR DS:[ESI]
		PUSH ESI								// frame
		CALL DWORD PTR DS:[EAX+0x18]
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH ESI								// frame
		mov eax,0x004253A0						// get some pointer from ECX, maybe some global class ?
		CALL eax
		MOV ECX,EAX
		mov eax,0x005C8600
		CALL eax
		MOV EDX,DWORD PTR DS:[EDI]				// edi should be game object
		PUSH ESI								// ESI = frame
		MOV ECX,EDI								// game object
		CALL DWORD PTR DS:[EDX+0x48]
		TEST AL,AL
		JE SHORT divnyjump_dest					// if FAILED
		MOV EAX,DWORD PTR DS:[EDI]
		MOV ECX,EDI
		CALL DWORD PTR DS:[EAX+0x64]
		PUSH 1
		MOV ECX,EDI
		mov eax,0x005C8A90						// set byte 0x214C1 to 1
		CALL eax
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH EDI
		mov eax,0x00425390
		CALL eax								// again, get some global class
		MOV ECX,EAX								// and save it as ECX
		mov eax,0x005E35D0
		CALL eax
		PUSH 64                                  ; /Arg1 = 00000064
		LEA ECX,DWORD PTR DS:[EDI+0x70]            ; |
		mov eax,0x0051A920
		CALL eax                       ; \Game.0051A920		// this func calls getTime, strange
		PUSH 1
		MOV ECX,EDI			
		mov eax,0x005C8740						// +0x65 offset = set byte to 1
		CALL eax								// set something in object on, god know what it is, well, this has just one call overall, strange
	divnyjump_dest:
		// the following code destroys frame
		MOV ECX,DWORD PTR DS:[ESI]
		PUSH ESI
		CALL DWORD PTR DS:[ECX]
		badresult_dest:
		//C:
		popad
	}
	char buff[255];
	sprintf(buff,"Car address: %x %x", caraddr,addr);
	g_CCore->GetLog()->AddLog(buff);
	return caraddr;
}

void CGame::PlayGameSound(char soundname[])
{
	//char test[255] = "system\\m_pravda.wav";
_asm
{
	MOV EAX,DWORD PTR DS:[0x6F9520]            ;  Case 186 of switch 005BB320
	PUSH 0x4
	PUSH EAX
	MOV EDX,DWORD PTR DS:[EAX]
	CALL DWORD PTR DS:[EDX+0x4C]
	MOV ESI,EAX
	TEST ESI,ESI
	JE end
	MOV EAX,DWORD PTR DS:[ESI]
	PUSH 0
	PUSH 0
	PUSH 0
	//LEA EAX, soundname
	PUSH soundname
	;PUSH 0x0064FF70                       ;  ASCII "system\m_pravda.wav"
	MOV EAX,DWORD PTR DS:[ESI]
	PUSH ESI
	CALL DWORD PTR DS:[EAX+0x50]
	TEST EAX,EAX
	JNZ SHORT badend
	MOV ECX,DWORD PTR DS:[ESI]
	PUSH 0x3
	PUSH ESI
	CALL DWORD PTR DS:[ECX+0x58]
	MOV EDX,DWORD PTR DS:[ESI]
	PUSH 0x3f000000										//0x3F800000
	PUSH ESI
	CALL DWORD PTR DS:[EDX+0x68]
	MOV EAX,DWORD PTR DS:[ESI]
	PUSH 0x3f000000										//0x3F800000
	PUSH ESI
	CALL DWORD PTR DS:[EAX+0x64]
	MOV ECX,DWORD PTR DS:[ESI]
	PUSH 0x0
	PUSH ESI
	CALL DWORD PTR DS:[ECX+0x6C]
	MOV EDX,DWORD PTR DS:[ESI]
	PUSH 0x1
	PUSH ESI
	CALL DWORD PTR DS:[EDX+0x24]
	MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
	PUSH ESI
	MOV EAX,0x00425390
	CALL EAX
	MOV ECX,EAX                              ; |
	MOV EAX,0x005E8840                       ; \game.005E8840
	CALL EAX
	JMP end
	MOV EAX,DWORD PTR DS:[ESI]
	PUSH ESI
	CALL DWORD PTR DS:[EAX]
	JMP end

	badend:
	MOV EAX,DWORD PTR DS:[ESI]
	PUSH ESI
	CALL DWORD PTR DS:[EAX]
	end:
	}
}

DWORD CGame::CreatePED()
{
	//g_CCore->GetChat()->AddMessage("#e3e3e3CreatePED is called now");
	DWORD pedaddr, frameaddr;
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	//DWORD TommyEngineObj = (DWORD) ped->object.frame;
	DWORD TommyEngineObj = ((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x68);
	char objectname[255] = "lhmp";
	char PEDmodel[255] = "Tommy.i3d";
	_asm
	{
		sub esp, 2000
			MOV EAX, DWORD PTR DS : [0x6F9520]
			PUSH 9
			PUSH EAX
			MOV ECX, DWORD PTR DS : [EAX]
			CALL DWORD PTR DS : [ECX + 0x4C]
			mov frameaddr, EAX
			MOV ESI, TommyEngineObj
			MOV EBX, [ESI]
			//	MOV ECX,EBX                              ;  EBX = engine object of actor, which we want to duplicate
			MOV ESI, EAX
			//CALL Game.005C85D0                       ;  this function returns object's skin in EAX
			//005C2897  |. 8BF8           MOV EDI,EAX
			/*005C2899  |. 6A 5C          PUSH 5C
			005C289B  |. 57             PUSH EDI
			005C289C  |. E8 CF2A0600    CALL Game.00625370
			005C28A1  |. 83C4 08        ADD ESP,8
			005C28A4  |. 85C0           TEST EAX,EAX
			005C28A6  |. 74 03          JE SHORT Game.005C28AB*/
			LEA EDI, PEDmodel;  EDI = skin without "Models\"
			PUSH 0; / Arg6 = 00000000
			PUSH 0; | Arg5 = 00000000
			PUSH 0; | Arg4 = 00000000
			PUSH 0; | Arg3 = 00000000
			PUSH EDI; | Arg2
			PUSH ESI; | our object's engine address
			MOV ECX, 0x006F9418; |
			MOV EAX, 0x00448940
			CALL EAX; \game.00448940
			/*
			005C28BF  |. 85C0           TEST EAX,EAX
			005C28C1  |. 0F85 593B0000  JNZ Game.005C6420
			005C28C7  |. 8BCB           MOV ECX,EBX
			005C28C9  |. E8 525C0000    CALL Game.005C8520
			005C28CE  |. 894424 10      MOV DWORD PTR SS:[ESP+10],EAX
			005C28D2  |. 8BF8           MOV EDI,EAX
			005C28D4  |. 83C9 FF        OR ECX,FFFFFFFF
			005C28D7  |. 33C0           XOR EAX,EAX
			005C28D9  |. F2:AE          REPNE SCAS BYTE PTR ES:[EDI]
			005C28DB  |. F7D1           NOT ECX
			005C28DD  |. 41             INC ECX
			005C28DE  |. 51             PUSH ECX
			005C28DF  |. E8 50200600    CALL Game.00624934                       ;  create string (dynamic memory) for object's name
			005C28E4  |. 8BD0           MOV EDX,EAX
			005C28E6  |. 8B4424 14      MOV EAX,DWORD PTR SS:[ESP+14]
			005C28EA  |. 8BFA           MOV EDI,EDX
			005C28EC  |. 83C4 04        ADD ESP,4
			005C28EF  |. 2BF8           SUB EDI,EAX
			005C28F1  |> 8A08           /MOV CL,BYTE PTR DS:[EAX]
			005C28F3  |. 880C07         |MOV BYTE PTR DS:[EDI+EAX],CL
			005C28F6  |. 40             |INC EAX
			005C28F7  |. 84C9           |TEST CL,CL
			005C28F9  |.^75 F6          \JNZ SHORT Game.005C28F1
			005C28FB  |. 8BFA           MOV EDI,EDX
			005C28FD  |. 83C9 FF        OR ECX,FFFFFFFF
			005C2900  |. 33C0           XOR EAX,EAX*/
			; LEA EDX, objectname
			; PUSH EDX
			
			//005C2903  |. F2:AE          REPNE SCAS BYTE PTR ES:[EDI]
			
			// TEST ---------------------------------------------------------------------------
			//
			/*PUSH 0x0064FF98
			MOV AX, WORD PTR DS : [0x64FFA4]
			PUSH ESI
			MOV WORD PTR DS : [EDI - 1], AX
			MOV ECX, DWORD PTR DS : [ESI]
			CALL DWORD PTR DS : [ECX + 0x28];  set duplicated object's name
			*/

			// TEST ---------------------------------------------------------------------------
			MOV ECX, EBX
			MOV EAX, 0x00528B70
			CALL EAX;  do something with engine object(offset 120)
			MOV EAX, DWORD PTR DS : [EAX]
			MOV EDX, DWORD PTR DS : [ESI];  ESI = duplicated engine object
			PUSH 0
			PUSH EAX
			PUSH ESI
			CALL DWORD PTR DS : [EDX + 0x2C]
			MOV ECX, EBX
			MOV EAX, 0x004251F0
			CALL EAX; Game.004251F0
			PUSH EAX
			MOV ECX, ESI
			MOV EAX, 0x0045F000
			CALL EAX;  potrebujeme zreversovat
			MOV ECX, EBX
			MOV EAX, 0x005C8370
			CALL EAX; Game.005C8370
			PUSH EAX
			MOV ECX, ESI
			MOV EAX, 0x0045F040
			CALL EAX
			MOV ECX, DWORD PTR DS : [ESI]
			PUSH ESI
			CALL DWORD PTR DS : [ECX + 0x18]
			/*
			005C294B  |. 8B5424 14      MOV EDX,DWORD PTR SS:[ESP+14]
			005C294F  |. 8B42 08        MOV EAX,DWORD PTR DS:[EDX+8]
			005C2952  |. 85C0           TEST EAX,EAX
			005C2954  |. 74 07          JE SHORT Game.005C295D
			005C2956  |. B8 01000000    MOV EAX,1
			005C295B  |. EB 11          JMP SHORT Game.005C296E
			005C295D  |> 8B4424 14      MOV EAX,DWORD PTR SS:[ESP+14]
			005C2961  |. 8B55 58        MOV EDX,DWORD PTR SS:[EBP+58]
			005C2964  |. 8B08           MOV ECX,DWORD PTR DS:[EAX]
			005C2966  |. 8B0C8A         MOV ECX,DWORD PTR DS:[EDX+ECX*4]
			005C2969  |. E8 322AE6FF    CALL Game.004253A0                       ;  ECX = engine object wich we are duplicating*/
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, 0x1B								// 27 = PED
			PUSH EAX;  EAX = 2 (if object is Tommy)
			MOV EAX, 0x005FED70
			CALL EAX;  (005FED70) hm..herny objekt to robi az tuna, divne
			MOV EDI, EAX
			MOV pedaddr, EAX
			PUSH ESI;  ESI = duplicate engine obj
			TEST EDI, EDI
			//005C297F  |. 74 43          JE SHORT Game.005C29C4 check if game object was created
			MOV EAX, DWORD PTR DS : [EDI]
			MOV ECX, EDI
			CALL DWORD PTR DS : [EAX + 0x48]
			MOV DWORD PTR SS : [EDI + 0x28], 0x0
			MOV BYTE PTR SS : [EDI + 0x5FC], 0x4				// we want neutral AI - dont react on attack
			MOV DWORD PTR SS : [EDI + 0x628], 0x3f800000	// shooting
			//005C2988  |. 84C0           TEST AL,AL
			MOV ECX, EDI;  EDI = normal object(duplicate)
			//005C298C  |. 74 2C          JE SHORT Game.005C29BA
			PUSH 1
			MOV EAX, 0x005C8730
			CALL EAX;	Game.005C8730
			MOV ECX, DWORD PTR DS : [0x65115C];	Game.006F9440
			PUSH EDI;	normal obj(duplicate)
			MOV EAX, 0x00425390
			CALL EAX;	Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x005E3220
			CALL EAX;	Game.005E3220		// changes PED pos
			/*005C29A8  |. 8B4C24 14      MOV ECX,DWORD PTR SS:[ESP+14]
			005C29AC  |. 8B45 58        MOV EAX,DWORD PTR SS:[EBP+58]
			005C29AF  |. 8B51 04        MOV EDX,DWORD PTR DS:[ECX+4]
			005C29B2  |. 893C90         MOV DWORD PTR DS:[EAX+EDX*4],EDI         ;  probably store duplicate's address as script variable
			005C29B5  |. E9 204D0000    JMP Game.005C76DA
			*/
			add esp, 2000
	}
	
	char buff[255];
	sprintf(buff, "PED address: %x %x", pedaddr, frameaddr);
	g_CCore->GetLog()->AddLog(buff);
	return pedaddr;
}

void CGame::PlayAnim(DWORD PED,int animId)
{
	if(animId == -1)
		return;
	
	char anim[255];
	sprintf(anim,"%s.i3d",ANIMS[animId]);
	_asm {
		PUSH 0					// bohvie co, mozno kontrolna premenna
		PUSH 0					// 0
		lea ESI,anim	
		PUSH ESI				// string  ASCII "Nuda01.i3d"
		MOV ECX,PED				// player object
		mov eax,0x004A63F0
		CALL EAX				//Game.004A63F0
	}
}
void CGame::PlayAnimString(DWORD PED, char* animId)
{
	char anim[255];
	sprintf(anim, "%s.i3d", animId);
	_asm {
		PUSH 0					// bohvie co, mozno kontrolna premenna
			PUSH 0					// 0
			lea ESI, anim
			PUSH ESI				// string  ASCII "Nuda01.i3d"
			MOV ECX, PED				// player object
			mov eax, 0x004A63F0
			CALL EAX				//Game.004A63F0
	}
}
// this function tells game which object it shouldn't spawn (we skip metro, tram and some others)
bool CGame::GameSpawnManager()
{
	// bool: return if it should skip loading of it or not
	char* frameName;
	_asm
	{
		MOV eax,DWORD PTR SS:[ESP+0x64]			// 0x58+0x38 + povodny offset		// povodne 0x90
		ADD EAX,0x8						// now it stores C string (frame name)
		mov frameName,eax				// and store it into frameName variable
	}
	//g_CCore->GetChat()->AddMessage(frameName);	// spravy funguje ked je ESP+0x90, blby VS
	// now we need to check if frameName contains a key word (salina, barel, whatever)

	if (strstr(frameName, "salina") != 0 || strstr(frameName, "metro") != 0)  //metro
	{
			return 1;
	}

	// if it doesnt contain key word, we will continue in spawning proccess
	return 0;
}

void CGame::PreRespawn()
{
	if (g_CCore->GetLocalPlayer()->IDinCar != -1)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(g_CCore->GetLocalPlayer()->IDinCar);
		if (veh != NULL)
		{
			veh->PlayerExit(g_CCore->GetLocalPlayer()->GetOurID());
		}
	}
	g_CCore->GetLocalPlayer()->SetIsOnFoot(true);
	g_CCore->GetLocalPlayer()->IDinCar = -1;
	g_CCore->GetLog()->AddLog("CCGame::PreRespawn");
	DWORD PED = NULL, frame = NULL;
	for(int i = 0; i < MAX_PLAYERS;i++)
	{
		//if (i == g_CCore->GetLocalPlayer()->GetOurID()) continue;
		CPed* ped = g_CCore->GetPedPool()->Return(i);
		if(ped != NULL)
		{
			if (ped->GetEntity() != NULL)
			{
				g_CCore->GetGame()->ChangeSkin(ped->GetEntity(), 293);
				g_CCore->GetGame()->DeletePed(ped->GetEntity());
				ped->SetEntity(NULL);
			}
			ped->SetEntity(NULL);
		}
	}
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(i);
		if (veh != NULL)
		{
			if (veh->GetEntity())
			{
				//g_CCore->GetGame()->DeleteCar(veh->GetEntity());
				veh->SetEntity(NULL);
			}
		}
	}

	// Delete all localplayer weapons
	if (g_CCore->GetGame()->GetLocalPED() != NULL)
	{
		g_CCore->GetGame()->DeleteAllWeapons((DWORD)g_CCore->GetGame()->GetLocalPED());
	}
}
void CGame::Respawn()
{
	g_CCore->m_bIsRespawning = true;
}

// this function is called by hook - in game thread
void CGame::AfterRespawn()
{
	g_CCore->GetGame()->SetTrafficVisible(false);
	//g_CCore->GetGame()->UpdateControls();		--- NO LONGER NEEDED
	g_CCore->GetGame()->PoliceManager();

	//g_CCore->GetChat()->AddMessage("Respawned !");
	g_CCore->GetGame()->DisableBridges();
	for(int i = 0; i < MAX_PLAYERS;i++)
	{
		if (i == g_CCore->GetLocalPlayer()->GetOurID()) continue;
		CPed* ped = g_CCore->GetPedPool()->Return(i);
		if(ped != NULL)
		{		
			char buff[250];
			DWORD handle = g_CCore->GetGame()->CreatePED();
			sprintf(buff, "AfterRespawn CreatePED %d %x", i, handle);
			g_CCore->GetLog()->AddLog(buff);
			ped->SetEntity(handle);
			if (handle != NULL)
			{
				g_CCore->GetGame()->ChangeSkin(ped->GetEntity(), ped->GetSkin());
				/*for(int e = 0; e < 8; e++)
				{
					SWeapon* pw = ped->GetWeapon(e);
					if(pw->wepID == NULL) continue;
					g_CCore->GetGame()->AddWeapon(ped->GetEntity(), pw->wepID, pw->wepLoaded, pw->wepHidden, 0);
					g_CCore->GetLog()->AddLog("AfterRespawn AddWeapon");
				}
				g_CCore->GetGame()->SwitchWeapon(ped->GetEntity(), ped->GetCurrentWeapon());
				*/
			}
		}
	}

	for (int e = 0; e < MAX_VEHICLES; e++)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(e);
		if (veh != NULL)
		{
			if (veh->IsActive())
			{
				DWORD base = g_CCore->GetGame()->CreateCar(veh->GetSkin(), veh->GetPosition(), veh->GetRotation());
				veh->SetEntity(base);
				for (int i = 0; i < 4; i++)
				{
					if (veh->GetSeat(i) != -1)
					{
						CPed* ped = g_CCore->GetPedPool()->Return(veh->GetSeat(i));
						if (ped != NULL)
						{
							if (ped->GetEntity() != NULL)
							{
								g_CCore->GetGame()->GivePlayerToCarFast(ped->GetEntity(), e, i);
							}
							else
							{
								// shit
								g_CCore->GetLog()->AddLog("ES[CreateCar] - no PED entity");
							}
							ped->InCar = e;
						}
					}
				}
				veh->SetDamage(veh->GetDamage());
				veh->SetShotDamage(veh->GetShotDamage());
				veh->ToggleRoof(veh->GetRoofState());
				veh->SetSirenState(veh->GetSirenState());
				veh->ToggleEngine(veh->GetEngineState());

			}
		}
	}
	
	g_CCore->GetGame()->ToggleCityMusic(g_CCore->GetGame()->GetMusicState());
	g_CCore->m_bIsRespawning = false;
	RakNet::BitStream bsOut;

	// tell all about respawn
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_RESPAWN);
	bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
	g_CCore->GetNetwork()->SendServerMessage(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED);

	g_CCore->GetEngineStack()->AddMessage(ES_CHANGESKIN, g_CCore->GetLocalPlayer()->GetOurID());


	/*// Test
	DWORD poolStart = *(DWORD*)((*(DWORD*)0x0065115C) + 0x38);
	DWORD poolEnd = *(DWORD*)((*(DWORD*)0x0065115C) + 0x3C);
	while (poolStart != poolEnd)
	{
		OBJECT* obj = (OBJECT*)*(DWORD*)poolStart;
		if (obj->objectType == 0x1B) {
			g_CCore->GetChat()->AddMessage("Deleting PED which is in pool");
			// swap it
			poolStart = poolEnd;
			poolEnd--;
			*(DWORD*)((*(DWORD*)0x0065115C) + 0x3C) -= 4;
			g_CCore->GetGame()->DeletePed((DWORD)obj);
		}
		poolStart += 4;
	}
	*/

	g_CCore->GetSquirrel()->onSpawn();
}

void CGame::ThrowAwayWeapon()
{
	DWORD testPed;
	_asm mov testPed,esi
	if((g_CCore->GetLocalPlayer()->GetBase()+0x4A0) == testPed)
	{
		DWORD wepId;
		_asm
		{
			XOR EAX,EAX
			MOV AX,WORD PTR DS:[ESI]
			MOV wepId,eax
		}
		char buff[255];
		sprintf(buff,"CGame::ThrowAway: %i",wepId);
		//g_CCore->GetChat()->AddMessage(buff);
		g_CCore->GetLog()->AddLog(buff);
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_PLAYER_DELETEWEAPON);
		bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
		bsOut.Write(wepId);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED);
	}
}
void CGame::TakeWeapon()
{
	/*DWORD workingbase;
	_asm
	{
		mov eax, edi
		mov EAX, DWORD PTR SS:[ESP+0x28C]
		mov workingbase,EAX
	}
	if(g_CCore->GetLocalPlayer()->GetBase() == workingbase)
	{
		*/
		DWORD wepId,wepLoaded,wepHidden;
		_asm
		{
			//mov eax,edi
			MOV EDI, DWORD PTR SS:[ESP+0x25C]  // 14C
			MOV EAX,DWORD PTR SS:[EDI]
			MOV wepId, EAX
			MOV EAX,DWORD PTR SS:[EDI+0x4]
			MOV wepLoaded, EAX
			MOV EAX,DWORD PTR SS:[EDI+0x8]
			MOV wepHidden, EAX
		}
		char buff[255];
		sprintf(buff,"CGame::TakeWep: %i %i %i",wepId,wepLoaded,wepHidden);
		//g_CCore->GetChat()->AddMessage(buff);
		g_CCore->GetLog()->AddLog(buff);
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_PLAYER_ADDWEAPON);
		bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
		bsOut.Write(wepId);
		bsOut.Write(wepLoaded);
		bsOut.Write(wepHidden);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED);
	//}
}
void CGame::ChangeWeapon()
{
	DWORD testPed;
	_asm mov testPed,esi
	if((g_CCore->GetLocalPlayer()->GetBase()+0x4A0) == testPed)
	{
		DWORD wepId;
		_asm
		{
			MOV EAX,DWORD PTR SS:[ESI]
			MOV wepId, EAX
		}
		char buff[255];
		sprintf(buff,"CGame::SwitchWep: %i",wepId);
		//g_CCore->GetChat()->AddMessage(buff);
		g_CCore->GetLog()->AddLog(buff);
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SWITCHWEAPON);
		bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
		bsOut.Write(wepId);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED);
	}
}

// TODO - clean comment
/*
void CGame::OnShoot()
{
	DWORD testPed;
	float x,y,z;
	_asm mov testPed,esi
	if((g_CCore->GetLocalPlayer()->GetBase()) == testPed)
	{
		_asm
		{
			//MOV EDI, DWORD PTR SS : [0x0018F800]
			MOV EDI, DWORD PTR SS:[ESP+0x28C]
			cmp EDI, 0x01
			jnz good
			//MOV EDI, DWORD PTR SS : [0x0018F804]
			MOV EDI, DWORD PTR SS:[ESP+0x290]
			//add EDI, 0x4		// 4 byte fix for Zak's PC
			
			//MOV EDI, [EAX]
			//MOV EDI, 0x0018F838
			//MOV EDI, DWORD PTR SS:[ESP+0x1C4]
			good:
			MOV EAX, DWORD PTR SS:[EDI]
			MOV x, EAX			
			MOV EAX, DWORD PTR SS:[EDI+0x4]
			MOV y, EAX			
			MOV EAX, DWORD PTR SS:[EDI+0x8]
			MOV z, EAX
		}
		char buff[255];
		sprintf(buff,"On Shot: %f %f %f",x,y,z);
		//g_CCore->GetChat()->AddMessage(buff);
		g_CCore->GetLog()->AddLog(buff);

		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SHOOT);
		bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
		bsOut.Write(x);
		bsOut.Write(y);
		bsOut.Write(z);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED);
	}
}
*/

// TODO - clean this comment
/*void CGame::OnCarJack()
{
	int carId = 0,seatId = 0;
	_asm
	{
		MOV EDI, DWORD PTR SS : [ESP + 0x258]
		MOV carId, EDI
		MOV EDI, DWORD PTR SS : [ESP + 0x248]
		MOV seatId, EDI
	}
	carId = g_CCore->GetVehiclePool()->GetVehicleIdByBase(carId);
	char buff[255];
	sprintf(buff, "Car jack ID: %i Seat: %i", carId,seatId);
	g_CCore->GetLog()->AddLog(buff);

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_JACK);
	bsOut.Write(carId);
	bsOut.Write(seatId);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(carId);
	if (veh != NULL)
	{
		if (veh->GetSeat(seatId) != -1)
		{
			CPed* ped = g_CCore->GetPedPool()->Return(veh->GetSeat(seatId));
			ped->InCar = -1;
			veh->PlayerExit(veh->GetSeat(seatId));
			ped->SetIsOnFoot(true);
		}
	}
}
*/

void CGame::CarJack(DWORD PED, DWORD car, int seat)
{
	_asm
	{
		pushad
		sub ESP, 0x2000
		MOV EDI, seat
		MOV ECX, car
		//MOV EBP, PED
		PUSH EDI; seat
		PUSH ECX; carBase
		MOV ECX, PED; actorBase(the jacker - ten kto ho ide vyhodit)
		MOV EAX, 0x004A82E0
		CALL EAX; game.004A82E0
		add ESP, 0x2000
		popad
	}
}
void CGame::OnDeath()
{
	DWORD killerId, killerBase;
	_asm
	{
		mov eax, DWORD PTR SS : [ESP+0x254]
		mov killerBase,eax
	}
	killerId = g_CCore->GetPedPool()->GetPedIdByBase(killerBase);
	if (killerId != -1)
	{
		char buff[255];
		sprintf(buff, "CGame::OnDeath - Killed by 0x%i", killerId);
		g_CCore->GetLog()->AddLog(buff);

		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_PLAYER_DEATH);
		bsOut.Write(killerId);
		bsOut.Write((unsigned char) 0xFF);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
	}
}

void CGame::OnSuicide()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_DEATH);
	bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
	bsOut.Write((unsigned char)0xFF);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
void CGame::Shoot(DWORD PED,Vector3D pos)
{
	g_CCore->GetLog()->AddLog("CGame:Shoot");
	//g_CCore->GetChat()->AddMessage("CGame::Shoot");
	//MessageBox(NULL,"a","b",MB_OK);
	float x = pos.x,y = pos.y, z = pos.z;
	_asm {
		sub esp,0x100
		mov esi, PED
		mov eax, x
		mov dword ptr ss:[esp],eax				; 0xC4F56B92
		mov eax, y
		mov dword ptr ss:[esp+0x4],eax			; 0xBFB6B7D5
		mov eax, z
		mov dword ptr ss:[esp+0x8],eax			; 0x41F6F110
		PUSH esp                                ;  odkaz na vector (kam mieri)
		PUSH 1
		MOV ECX,ESI
		//MOV DWORD PTR SS:[ESP+40],EAX
		mov eax,0x004A3AE0
		CALL eax								//Game.004A3AE0
		// zastavenie - vystreli len raz
		PUSH 0                                
		PUSH 0
		MOV ECX,ESI
		//MOV DWORD PTR SS:[ESP+40],EAX
		mov eax,0x004A3AE0
		CALL eax								//Game.004A3AE0
		add esp,0x100
	}	
}
void CGame::ThrowGranade(DWORD PED,Vector3D way)
{
	_PED* character = (_PED*)PED;
	g_CCore->GetLog()->AddLog("CGame:ThrowGranade");
	char wtf[500];
	sprintf(wtf, "Wep: %d", character->inventary.slot[0].weaponType);
	g_CCore->GetLog()->AddLog(wtf);

	_asm {
		sub esp, 0x100

		MOV EAX, way.x
		MOV DWORD PTR DS : [ESP + 0x34], EAX;
		MOV EAX, way.y
		MOV DWORD PTR DS : [ESP + 0x38], EAX
		MOV EAX, way.z
		MOV DWORD PTR DS : [ESP + 0x3C], EAX
		LEA EDX, DWORD PTR DS:[ESP+0x34]
		MOV ECX, PED; PED
		PUSH EDX
		MOV EAX, 0x004A4490
		CALL EAX
		add esp, 0x100
	}
}
void CGame::AddWeapon(DWORD PED, DWORD wepCat, DWORD wepID,DWORD wepLoaded,DWORD wepHidden)
{
	//DWORD obj = NULL;
	//sscanf(varlist,"%x",obj);
	//obj = strtol (varlist,0,16);
	_asm {
		sub esp,0x100
		mov eax,[wepCat]						;// e.g. 0x9
		mov DWORD PTR DS:[esp],eax				
		mov eax,wepID							;// e.g. 0x7
		mov DWORD PTR DS:[esp+0x4],eax
		mov eax,wepLoaded						;// e.g. 0x0
		mov DWORD PTR DS:[esp+0x8],eax
		mov eax,wepHidden						;// e.g. 0x0
		mov DWORD PTR DS:[esp+0xC],eax
		mov EAX,ESP
		mov EDI, PED
		lea esi, [edi+0x480]
		mov EDX, 0x15
		mov EBX, 0x10
		PUSH 0
		PUSH EAX					;	// EDX, obj. novej zbrane
		MOV ECX,ESI                              	;  	player invetary start 0x480
													;	EDX 0x15, EBX 0x10
		mov eax, 0x0055B2D0
		call eax										;	CALL Game.0055B2D0
		MOV ECX,EDI                              	;  	EDI = player base
		mov eax,0x0049F180
		call eax									;	CALL Game.0049F180
		add esp,0x100
	}
}

void CGame::SwitchWeapon(DWORD PED,DWORD wepId = 0)
{
	_asm {
	sub esp, 0x1024
	LEA EAX,DWORD PTR SS:[ESP+0x54]          ;  struct v stacku
	MOV EDI,PED								 ;  EDI = pedBase
	PUSH EAX                                 ;  --- zaciatok samotneho kodu
	LEA ECX,DWORD PTR SS:[ESP+0x134]		 ; struct 2, ta prava
	MOV EAX,0x00443B00
	CALL EAX				                 ;  EDI = pedBase  - Game.00443B00 
	MOV EDX,wepId							 ;  EDX = ID zbrane  e.g. 0 = ruky
	LEA ECX,DWORD PTR SS:[ESP+0x130]         ;  zas adresa na struct 2, ten pravy
	PUSH ECX                                 ;  struct adresa
	PUSH EDX                                 ;  ID zbrane
	LEA ECX,DWORD PTR DS:[EDI+0x480]         ;  EDI = PEDbase, +480 = nejake frame(mozno zbran)
	MOV DWORD PTR SS:[ESP+0x804],0x1           ;  co to moze byt neviem
	MOV EAX,0x0055A1F0						 ;
	CALL EAX								 ;  Game.0055A1F0
	MOV ECX,EDI                              ;  pedBase
	MOV EAX,0x0049F180
	CALL EAX								 ;  Game.0049F180
	LEA ECX,DWORD PTR SS:[ESP+0x130]         ;  struct
	MOV EAX,0x00606550
	CALL EAX								 ;  Game.00606550
	TEST EAX,EAX
	JE SHORT zerocase
	LEA EAX,DWORD PTR SS:[ESP+0x130]		 ;  asi struct 2
	MOV ECX,EDI
	PUSH EAX                                 ; /Arg1
	MOV EAX,0x0049FFF0
	CALL EAX								 ; \game.0049FFF0
	zerocase:
	MOV ECX,EDI                              ;  pedBase
	MOV EAX,0x004253A0
	CALL EAX								 ;  Game.004253A0	;  meni aj ESI
	CMP EAX,0x1B								 ; isPED
	JNZ SHORT localplayer
	MOV ECX,DWORD PTR DS:[ESI+0x4]           ;  pedov kod
	PUSH ECX                                 ; /Arg1
	MOV ECX,EDI                              ; |
	MOV EAX,0x00432C50
	CALL EAX								 ; \game.00432C50
	localplayer:
	LEA ECX,DWORD PTR SS:[ESP+0x130]           ;  struct, pedov kod potialto
	MOV DWORD PTR SS:[ESP+0x7FC],-1
	MOV EAX,0x00566200
	CALL EAX								; Game.00566200
	add esp, 0x1024
	}
}

void CGame::PlayerEnteredVehicle()
{
	DWORD vehicle,seatID;
	_asm
	{
		// seatID = 0018F884 - 0018F618
		mov EAX, DWORD PTR DS : [ESP + 0x26C]
		mov seatID, EAX;
		// vehicle id 0018F870
		mov EAX, DWORD PTR DS : [ESP + 0x254]
		mov vehicle, EAX;
	}
	char buff[255];

	int vehID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vehicle);
	g_CCore->GetVehiclePool()->Return(vehID)->PlayerEnter(g_CCore->GetLocalPlayer()->GetOurID(),seatID);
	g_CCore->GetLocalPlayer()->SetIsOnFoot(0);
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_ENTERED_VEHICLE);
	bsOut.Write(vehID);
	bsOut.Write(seatID);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);

	sprintf(buff, "CGame::PlayerEnteredVehicle %x, seat id: %d", vehicle, seatID);
	g_CCore->GetLog()->AddLog(buff);
}
void CGame::PlayerExitVehicle()
{
	DWORD vehicle;
	_asm
	{
		// vehicle id 0018F870
		mov EAX, DWORD PTR DS : [ESP + 0x25C]
			mov vehicle, EAX;
	}
	char buff[255];

	int vehID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(vehicle);
	g_CCore->GetVehiclePool()->Return(vehID)->PlayerExit(g_CCore->GetLocalPlayer()->GetOurID());
	g_CCore->GetLocalPlayer()->SetIsOnFoot(1);
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_EXIT_VEHICLE);
	bsOut.Write(vehID);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);

	sprintf(buff, "CGame::PlayerExitVehicle %x", vehicle);
	g_CCore->GetLog()->AddLog(buff);
}

void CGame::PlayerExitVehicleFinish()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_EXIT_VEHICLE_FINISH);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
	g_CCore->GetLog()->AddLog("CGame::PlayerExitVehicleFinish");
}

void CGame::DeleteWeapon(DWORD PED,DWORD weaponID)
{
	_asm
	{
		sub esp,0x1024
		MOV EDI,PED								 ;  pedBase
		LEA ECX,DWORD PTR SS:[ESP+0x1E0]         ;  struct
		MOV EAX,0x00498740
		CALL EAX								 ;  ClearStruct   - Game.00498740 
		MOV EAX,weaponID							 ;  wepId (maybe)
		LEA ECX,DWORD PTR SS:[ESP+0x1E0]         ;  struct pointer
		PUSH 0                                   ; /Arg2 = 00000000
		PUSH ECX                                 ; |Arg1
		LEA ECX,DWORD PTR DS:[EDI+0x480]         ; |pedBase+480
		MOV WORD PTR SS:[ESP+0x1E8],AX           ; |copy wepID
		MOV EAX,0x0055B600
		CALL EAX								 ; \Game.0055B600
		MOV ECX,EDI                              ;  EDI = pedBase
		MOV EAX,0x0049F180
		CALL EAX								 ;  Game.0049F180
		add esp,0x1024
	}
}


void CGame::DeleteAllWeapons(DWORD ped)
{
	if (ped != NULL)
	{
		PED* player = (PED*)ped;
		for (int i = 0; i < 8; i++)
		{
			if (player->inventary.slot[i].weaponType != 0)
			{
				g_CCore->GetGame()->DeleteWeapon(ped, player->inventary.slot[i].weaponType);
			}
		}
	}
}

_PED*	CGame::GetLocalPED()
{
	if ((*(DWORD*)0x006F9464) != NULL)
	{
		return *(_PED**)(*(DWORD*)(0x006F9464) + 0xE4);
	}
	else {
		return NULL;
	}
}

void CGame::KillPed(DWORD PED)
{
	g_CCore->GetGame()->ShouldKill = true;
	/*_asm
	{
		MOV ECX,PED								;  pedBase
		mov EAX, DWORD PTR DS : [ECX+0x5D]
		cmp EAX,0x0
		je end
		PUSH ECX
		MOV ECX,DWORD PTR DS:[0x65115C]          ;  Game.006F9440
		MOV EAX, 0x00425390
		CALL EAX								 ; Game.00425390
		MOV ECX,EAX                              ; |
		MOV EAX,0x005F01E0
		CALL EAX			                     ; \Game.005F01E0	
		end:
	}*/
	_asm {
		sub ESP, 0x500
			MOV ESI, PED
			MOV DWORD PTR DS : [ESI + 0x644], 0x3F800000
			MOV EDX, DWORD PTR DS : [ESI]
			PUSH 0
			PUSH 5
			PUSH 0
			LEA EAX, DWORD PTR SS : [ESP + 0x100]
			PUSH 0x41200000
			PUSH EAX
			LEA ECX, DWORD PTR SS : [ESP + 0xB0]
			LEA EAX, DWORD PTR SS : [ESP + 0x1A8]
			PUSH ECX
			PUSH EAX
			PUSH 6
			MOV ECX, ESI
			CALL DWORD PTR DS : [EDX + 0x7C];  Game.004CBC10
			add ESP, 0x500
	}
	g_CCore->GetGame()->ShouldKill = false;

}


void CGame::FixAfterDeath(DWORD ped)
{
	DWORD locPed = ped;
	_asm {
		mov ECX, locPed
		mov eax, 0x004ABE40
		call EAX
	}
}

void CGame::KillPedEx(DWORD ped, DWORD reason, DWORD part)
{
	DWORD locPed = ped;
	DWORD locReason = reason;
	DWORD locPart = part;
	/*char buff[500];
	sprintf(buff, "%x; %x; %x", ped, reason, part);
	g_CCore->GetLog()->AddLog(buff);
	*/

	g_CCore->GetGame()->ShouldKill = true;

	_asm {
		sub ESP, 0xC	// create null vector
			mov DWORD PTR DS : [ESP], 0x0
			mov DWORD PTR DS : [ESP + 0x4], 0x0
			mov DWORD PTR DS : [ESP + 0x8], 0x0
			MOV ESI, locPed
			MOV DWORD PTR DS : [ESI + 0x644], 0x3F800000
			MOV EDX, DWORD PTR DS : [ESI]
			PUSH 0
			PUSH locPart
			PUSH 0
			LEA EAX, DWORD PTR SS : [ESP + 0xC]
			PUSH 0x41200000
			PUSH EAX
			LEA ECX, DWORD PTR SS : [ESP + 0x14]
			LEA EAX, DWORD PTR SS : [ESP + 0x14]
			PUSH ECX
			PUSH EAX
			PUSH locReason
			MOV ECX, ESI
			CALL DWORD PTR DS : [EDX + 0x7C];  Game.004CBC10
			add ESP, 0xC
	}
	g_CCore->GetGame()->ShouldKill = false;
}

void CGame::SetOn(DWORD PED,bool hide)
{
	_asm
	{
		sub esp,0x200
		//MOV ECX, PED
		//MOV EAX,DWORD PTR SS:[ECX+0x68]
		//MOV ESI,EAX         ;  ESI = player's frame
		MOV ESI, PED
	//005BBABD  |. 8B57 04        MOV EDX,DWORD PTR DS:[EDI+4]
		MOV ECX,ESI
	//005BBAC2  |. 85D2           TEST EDX,EDX
	//005BBAC4  |. 0F95C3         SETNE BL
		MOV BL,0x0
		MOV BYTE PTR SS:[ESP+0x10],BL
		MOV EAX,0x005C82A0
		CALL EAX				;Game.005C82A0
		CMP DWORD PTR DS:[EAX],0x4
		JNZ SHORT nextcase
		MOV ECX,DWORD PTR SS:[ESP+0x10]
		MOV EAX,DWORD PTR DS:[ESI]
		PUSH 1
		PUSH ECX
		PUSH ESI
		CALL DWORD PTR DS:[EAX+0x88]
		JMP end
		nextcase:
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH ESI                                 ;  ESI = player's frame (+68offset of pedBase)
		MOV EAX, 0x00425390
		CALL EAX								 ; Game.00425390
		MOV ECX,EAX                              ; |
		MOV EAX, 0x005EBF80 
		CALL EAX								 ; \game.005EBF80
		MOV EAX,DWORD PTR SS:[ESP+0x10]
		MOV EDX,DWORD PTR DS:[ESI]
		PUSH EAX                                 ;  napr. C502A200
		PUSH ESI                                 ;  ped's frame
		CALL DWORD PTR DS:[EDX+0x24]
		MOV ECX,ESI                              ;  frame
		MOV EAX, 0x00556EA0
		CALL EAX								 ;  Game.00556EA0
		TEST BL,BL
		MOV EDI,EAX
		JNZ SHORT nextplease
		MOV ECX,DWORD PTR DS:[0x65115C]          ;  Game.006F9440
		PUSH ESI
		MOV EAX, 0x00425390
		CALL EAX								 ;  Game.00425390
		MOV ECX,EAX
		MOV EAX,0x005ED9F0
		CALL EAX								 ;  Game.005ED9F0
		TEST EDI,EDI
		JE end
		nextplease:


		MOV ECX,PED								 ;  ECX = pedBase
		PUSH 0                                   ;  hide
		MOV EAX,0x00425410
		CALL EAX								 ;  Game.00425410
		MOV ECX,DWORD PTR DS:[0x65115C]          ;  Game.006F9440
		PUSH EAX
		MOV EAX,0x00425390
		CALL EAX								 ;  Game.00425390
		MOV ECX,EAX
		MOV EAX,0x005E9350
		CALL EAX								 ;  Game.005E9350
		end:
		add esp,0x200
	}
}

void CGame::SetFramePos(DWORD frame,float f1,float f2,float f3)
{     
	_asm
	{
	sub esp, 0x500
	PUSH f3								 ;  pos.z    
	PUSH f2								 ;  pos.y
	PUSH f1								 ;  pos.x
	LEA ECX,DWORD PTR SS:[ESP+0x270]
	MOV EAX, 0x004026C0 
	CALL EAX							 ;  FillStruct
	LEA ECX,DWORD PTR SS:[ESP+0x264]     ;  struct
	PUSH ECX
	MOV ECX, frame						 ;  frame
	MOV EAX, 0x0045F000
	CALL EAX
	MOV EAX, frame						 ;  EAX = frame
	PUSH EAX
	MOV ECX,DWORD PTR DS:[EAX]
	CALL DWORD PTR DS:[ECX+0x18]
	MOV ECX,frame						 ;  frame
	MOV EAX, 0x00528B70
	CALL EAX
	CMP DWORD PTR DS:[EAX],0
	JE SHORT badcase
	MOV ECX, frame						 ;  frame
	MOV EAX, 0x00528B70
	CALL EAX
	MOV ECX,DWORD PTR DS:[EAX]
	MOV EAX, 0x005C82A0
	CALL EAX
	CMP DWORD PTR DS:[EAX],5
	JNZ end
    badcase:
	MOV EDX, frame
	MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
	PUSH EDX
	MOV EAX, 0x004253A0
	CALL EAX
	MOV ECX,EAX
	MOV EAX, 0x005C8600
	CALL EAX
	end:
	add esp, 0x500
	}
}

void CGame::DeletePed(DWORD PED)
{
	if (PED == NULL)
		return;

	// Delete weapons
	_PED* deletingPed = (_PED*)PED;
	if (PED != NULL)
	{
		// TODO - test
		/*for (int i = 0; i < 8; i++)
		{
			if (deletingPed->inventary.slot[i].weaponType != 0)
			{
				g_CCore->GetGame()->DeleteWeapon(PED, deletingPed->inventary.slot[i].weaponType);
			}
		}*/

		g_CCore->GetGame()->DeleteAllWeapons(PED);

		/*// testing - crashed, however, it surely works ingame - fucking ped spawning
		_asm {
			PUSH PED;  object to find(car / ped)
				MOV EAX, DWORD PTR DS : [0x006F9464]	// static pool pointer
				MOV ECX, EAX
				MOV EAX, 0x005E3920
				CALL EAX
		}
		return;*/

		DWORD ped_frame = (DWORD)deletingPed->object.frame;
		if (ped_frame)
		{
			g_CCore->GetChat()->AddDebugMessage("Deleting ped #00d717%p#e3e3e3 [#f31d2f%p#e3e3e3]", PED, ped_frame);

			/*// TESTING STUFF
			_asm {
				MOV ECX, PED
				PUSH 0
				MOV EAX, DWORD PTR DS : [ECX];  Game.0063B488
				CALL DWORD PTR DS : [EAX + 0x94]
			}
			return;
			*/


			// END OF TEST
			_asm{
				/*PUSH PED
				MOV ECX,DWORD PTR DS:[0x65115C]				;  Game.006F9440
				mov eax,0x00425390
				CALL eax									; Game.00425390
				MOV ECX,EAX									; |
				mov eax,0x005E3400
				CALL eax									; \Game.005E3400
				*/

				/*	// destroy player structure
					PUSH PED
					MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
					mov eax, 0x00425390
					CALL eax; Game.00425390
					MOV ECX, EAX; |
					mov eax, 0x005E3400
					CALL eax; \Game.005E3400

					// now frame
					mov eax, ped_frame
					push eax
					mov ecx, [eax]
					call dword ptr ds : [ecx]*/

				// testing old code
				MOV ECX, DWORD PTR DS : [0x65115C]; Game.006F9440
					PUSH PED
					MOV EAX, 0x00425390
					CALL EAX; Game.00425390
					MOV ECX, EAX
					MOV EAX, 0x00425560
					CALL EAX; Game.00425560
					MOV ECX, EAX
					MOV EAX, 0x005CFB60
					CALL EAX; Game.005CFB60
					PUSH PED
					MOV ECX, DWORD PTR DS : [0x65115C]; Game.006F9440
					MOV EAX, 0x00425390
					CALL EAX; Game.00425390
					MOV ECX, EAX
					MOV EAX, 0x005E3400
					CALL EAX; Game.005E3400

					mov eax, ped_frame
					push eax
					mov ecx, [eax]
					call dword ptr ds : [ecx]
			}
		}
		//g_CCore->GetGame()->SetFramePos(ped_frame,0,0,0);
	}
}
void CGame::DeleteCar(DWORD PED)
{
	_PED *localis =(_PED*) g_CCore->GetLocalPlayer()->GetEntity();
	if (localis->playersCar == (_VEHICLE*)PED || localis->carLeavingOrEntering == (_VEHICLE*)PED)
	{
		g_CCore->GetGame()->KickPlayerFromCarFast((DWORD)localis);
	}

	// Testing part
	_asm {
		PUSH PED;  object to find(car / ped)
		MOV EAX, DWORD PTR DS : [0x006F9464]	// static pool pointer
		MOV ECX, EAX
		MOV EAX, 0x005E3920
		CALL EAX 
	}
	/*_asm{
		MOV ECX, DWORD PTR DS : [0x65115C]; Game.006F9440
			PUSH PED
			MOV EAX, 0x00425390
			CALL EAX; Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x00425560
			CALL EAX; Game.00425560
			MOV ECX, EAX
			MOV EAX, 0x005CFB60
			CALL EAX; Game.005CFB60
			PUSH PED
			MOV ECX, DWORD PTR DS : [0x65115C]; Game.006F9440
			MOV EAX, 0x00425390
			CALL EAX; Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x005E3400
			CALL EAX; Game.005E3400
	}*/
}

DWORD CGame::FindFrame(char* frame)
{
	DWORD res;
	_asm
	{
		mov ebx,frame
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		MOV EAX,0x004253A0
		CALL EAX								   ;  Game.004253A0
		MOV ECX,DWORD PTR DS:[EAX]
		PUSH 0x4FFFF
		PUSH EBX
		PUSH EAX
		CALL DWORD PTR DS:[ECX+0x58]
		MOV res,EAX
	}
	return res;
}

DWORD CGame::FindActor(char* frame)
{
	DWORD res = NULL;
	_asm
	{
		//mov edi, frame
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		MOV EAX, 0x005C8640
		CALL EAX; Game.005C8640
		MOV EBX, EAX;  getFrame
		MOV ECX, EBX
		MOV EAX, 0x00474220
		CALL EAX; Game.00474220
		MOV EDI, EAX
		TEST EDI, EDI
		JE SHORT actor_jmp1
		actor_jmp2:
		DEC EDI
		MOV ECX, EBX
		PUSH EDI
		MOV EAX, 0x005A17B0
		CALL EAX; Game.005A17B0
		MOV ECX, DWORD PTR DS : [EAX]
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410
		TEST EAX, EAX
		JE SHORT actor_jmp3
		PUSH EDI
		MOV ECX, EBX
		MOV EAX, 0x005A17B0
		CALL EAX; Game.005A17B0
		MOV ESI, frame
		MOV ECX, DWORD PTR DS : [EAX]
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410
		MOV ECX, EAX
		MOV EAX, 0x005C8520
		CALL EAX; Game.005C8520
		actor_jmp6:
		MOV DL, BYTE PTR DS : [EAX]
		MOV CL, DL
		CMP DL, BYTE PTR DS : [ESI]
		JNZ SHORT actor_jmp4
		TEST CL, CL
		JE SHORT actor_jmp5
		MOV DL, BYTE PTR DS : [EAX + 0x1]
		MOV CL, DL
		CMP DL, BYTE PTR DS : [ESI + 0x1]
		JNZ SHORT actor_jmp4
		ADD EAX, 0x2
		ADD ESI, 0x2
		TEST CL, CL
		JNZ SHORT actor_jmp6
		actor_jmp5:
		XOR EAX, EAX
		JMP SHORT actor_jmp7
		actor_jmp4:
		SBB EAX, EAX
		SBB EAX, -1
		actor_jmp7:
		TEST EAX, EAX
		JE actor_jmp8
		actor_jmp3:
		TEST EDI, EDI
		JNZ SHORT actor_jmp2
		actor_jmp1:
		XOR EDI, EDI
		actor_jmp14a:
		MOV ECX, DWORD PTR DS : [EDI * 0x4 + 0x6F9570]
		TEST ECX, ECX
		JE SHORT actor_jmp9
		MOV ESI, frame
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410
		MOV ECX, EAX
		MOV EAX, 0x005C8520
		CALL EAX; Game.005C8520
		actor_jmp12:
		MOV DL, BYTE PTR DS : [EAX]
		MOV BL, BYTE PTR DS : [ESI]
		MOV CL, DL
		CMP DL, BL
		JNZ SHORT actor_jmp10
		TEST CL, CL
		JE SHORT actor_jmp11
		MOV DL, BYTE PTR DS : [EAX + 0x1]
		MOV BL, BYTE PTR DS : [ESI + 0x1]
		MOV CL, DL
		CMP DL, BL
		JNZ SHORT actor_jmp10
		ADD EAX, 0x2
		ADD ESI, 0x2
		TEST CL, CL
		JNZ SHORT actor_jmp12
		actor_jmp11:
		XOR EAX, EAX
		JMP SHORT actor_jmp13
		actor_jmp10:
		SBB EAX, EAX
		SBB EAX, -1
		actor_jmp13:
		TEST EAX, EAX
		JE SHORT actor_jmp14
		actor_jmp9:
		INC EDI
		CMP EDI, 0x2
		JL SHORT actor_jmp14a
		JMP SHORT actor_jmp15a
		actor_jmp14:
		MOV EAX, DWORD PTR SS : [ESP + 0x14]
		MOV EDX, DWORD PTR SS : [EBP + 0x58]
		MOV ECX, DWORD PTR DS : [EAX]
		MOV EAX, DWORD PTR DS : [EDI * 0x4 + 0x0c6F9570]
		MOV DWORD PTR DS : [EDX + ECX * 0x4], EAX
		actor_jmp15a:
		MOV EAX, 0x005C8750
		CALL EAX; Game.005C8750
		MOV EBX, EAX
		MOV ECX, EBX
		MOV EAX, 0x00474220
		CALL EAX; Game.00474220
		MOV EDI, EAX
		TEST EDI, EDI
		JE badjmp
		actor_jmp20:
		DEC EDI
		MOV ECX, EBX
		PUSH EDI
		MOV EAX, 0x005A17B0
		CALL EAX; Game.005A17B0
		CMP DWORD PTR DS : [EAX], 0x0
		JE SHORT actor_jmp15
		PUSH EDI
		MOV ECX, EBX
		MOV EAX, 0x005A17B0
		CALL EAX; Game.005A17B0
		MOV ECX, DWORD PTR DS : [EAX]
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410
		TEST EAX, EAX
		JE SHORT actor_jmp15
		PUSH EDI
		MOV ECX, EBX
		MOV EAX, 0x005A17B0
		CALL EAX; Game.005A17B0
		MOV ESI, frame
		MOV ECX, DWORD PTR DS : [EAX]
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410
		MOV ECX, EAX
		MOV EAX, 0x005C8520
		CALL EAX; Game.005C8520
		actor_jmp18:
		MOV DL, BYTE PTR DS : [EAX]
		MOV CL, DL
		CMP DL, BYTE PTR DS : [ESI]
		JNZ SHORT actor_jmp16
		TEST CL, CL
		JE SHORT actor_jmp17
		MOV DL, BYTE PTR DS : [EAX + 0x1]
		MOV CL, DL
		CMP DL, BYTE PTR DS : [ESI + 0x1]
		JNZ SHORT actor_jmp16
		ADD EAX, 0x2
		ADD ESI, 0x2
		TEST CL, CL
		JNZ SHORT actor_jmp18
		actor_jmp17:
		XOR EAX, EAX
		JMP SHORT actor_jmp19
		actor_jmp16:
		SBB EAX, EAX
		SBB EAX, -1
		actor_jmp19:
		TEST EAX, EAX
		JE SHORT actor_jmp8
		actor_jmp15:
		TEST EDI, EDI
		JNZ SHORT actor_jmp20
		JMP badjmp
		actor_jmp8:
		PUSH EDI
		MOV ECX, EBX
		MOV EAX, 0x005A17B0
		CALL EAX; Game.005A17B0
		MOV EAX, DWORD PTR DS : [EAX];  finaly actor
		MOV res, EAX
		jmp end
		// badjump = NO RESULT
		badjmp:
		MOV res,0x0
		end:
		
	}
	return res;
}

char* CGame::GetFrameName(FRAME* frm)
{
	return (char*)(*(DWORD*)((DWORD)frm + 0x100) + 0x0);
}

void CGame::SetCameraPos(Vector3D pos,float r1,float r2,float r3,float r4)
{
	_asm
	{
		sub esp, 0x400
		LEA ECX, DWORD PTR SS : [ESP + 0x388]
		mov EAX, r1
		MOV DWORD PTR SS : [ECX], EAX
		mov EAX, r2
		MOV DWORD PTR SS : [ECX + 0x4], EAX
		mov EAX, r3
		MOV DWORD PTR SS : [ECX + 0x8], EAX
		mov EAX, r4
		MOV DWORD PTR SS : [ECX + 0xC], EAX
		PUSH ECX
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		lea EAX, pos
		PUSH EAX
		MOV EAX, 0x00425390
		CALL EAX;  Game.00425390
		MOV ECX, EAX
		MOV EAX, 0x00425510
		CALL EAX;  Game.00425510
		MOV ECX, EAX
		MOV EAX, 0x005DB000
		CALL EAX;  entire camera change - Game.005DB000
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		MOV EAX, 0x00425390
		CALL EAX;  Game.00425390
		MOV ECX, EAX
		MOV EAX, 0x005C8B50
		CALL EAX;  Game.005C8B50
		CMP EAX, 0x6
		JNB SHORT end
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		MOV EAX, 0x00425390
		CALL EAX; Game.00425390
		MOV ECX, EAX
		MOV EAX, 0x005F5C60
		CALL EAX; Game.005F5C60
		end:
		add esp, 0x400
	}
	
}


Vector3D CGame::GetCameraPos()
{
	return *(Vector3D*)(0x101C0C10);
}

void CGame::CameraUnlock()
{
	_asm {
	MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440; Case 77 of switch 005BB320
	PUSH 0
	MOV EAX, 0x00425390
	CALL EAX								   ;  Game.00425390
	MOV ECX,EAX
	MOV EAX, 0x00425510
	CALL EAX								   ;  Game.00425510
	MOV ECX,EAX
	MOV EAX, 0x005DB5D0
	CALL EAX								   ;  Game.005DB5D0
	MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
	MOV EAX, 0x00425390						   
	CALL EAX								   ;  Game.00425390
	MOV ECX,EAX
	MOV EAX, 0x005F19C0
	CALL EAX								   ;  Game.005F19C0
	}
}

void CGame::CameraSetFocusOnObject(DWORD base)
{
	_asm
	{
		MOV EDX, DWORD PTR DS : [0x65115C]; game.006F9440
			MOV ECX, DWORD PTR DS : [EDX + 0x24]
			ADD ECX, 0x4C
			MOV EAX, base
			MOV DWORD PTR DS : [ECX + 0xC], EAX
	}
	//CALL game.005D4AA0
}

void CGame::CameraSetDefaultFocus()
{
	_asm
	{
		MOV EDX, DWORD PTR DS : [0x65115C]; game.006F9440
			MOV ECX, DWORD PTR DS : [EDX + 0x24]
			ADD ECX, 0x4C
			MOV EAX, 0x0
			MOV DWORD PTR DS : [ECX + 0xC], EAX
	}
	//CALL game.005D4AA0
}




void CGame::SetCarPosition(DWORD carBase,Vector3D position)
{
	if (carBase == NULL)
	{
		g_CCore->GetLog()->AddLog("SetCarPosition - car doesnt exist");
		return;
	}
	DWORD car_frame = *(DWORD*)(carBase + 0x68);
	if (car_frame == NULL)
	{
		g_CCore->GetLog()->AddLog("SetCarPosition - carframe doesnt exist");
		return;
	}
	float x = position.x, y = position.y, z = position.z;
	_asm
	{
		sub ESP, 0x1000
		MOV ECX, ESP
		MOV EAX, x
		MOV DWORD PTR DS : [ECX], EAX
		MOV EAX, y
		MOV DWORD PTR DS : [ECX + 4], EAX
		MOV EAX, z
		MOV DWORD PTR DS : [ECX + 8], EAX
		MOV ECX, carBase
		MOV EAX, 0x005C86F0
		CALL EAX; Game.005C86F0;  SetActorPosition

		SUB ESP, 0x0C
		MOV EAX, x
		MOV DWORD PTR DS : [ESP], EAX
		MOV EAX, y
		MOV DWORD PTR DS : [ESP + 0x4], EAX
		MOV EAX, z
		MOV DWORD PTR DS : [ESP + 0x8], EAX
		MOV EAX, ESP
		PUSH EAX;  position vector(for SetFramePositionFromVector)
		MOV ECX, car_frame
		MOV EAX, 0x0045F000
		CALL EAX; Game.0045F000;  SetFramePositionFromVector(vect as ESP + 4)
		MOV EDI, car_frame
		LEA ECX, DWORD PTR SS : [ESP + 0x3E8];  vector
		MOV EBX, DWORD PTR DS : [EDI];  frame vtable
		PUSH 0
		PUSH ECX;  vector
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x005C8710
		CALL EAX; Game.005C8710;  GetActorRotationVector(EAX == vect)
		PUSH EAX
		MOV EDI, car_frame
		PUSH EDI;  Actor's frame
		CALL DWORD PTR DS : [EBX + 0xC];  IDK, but important(checks obj rotation)
		MOV EAX, car_frame
		MOV ECX, DWORD PTR DS : [EAX]
		PUSH EAX;  frame
		CALL DWORD PTR DS : [ECX + 0x18];  important, too
		MOV EAX, car_frame
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		PUSH EAX;  frame
		MOV EAX, 0x004253A0
		CALL EAX; Game.004253A0;  return something(maybe pointer)
		MOV ECX, EAX
		MOV EAX, 0x005C8600
		CALL EAX;  Game.005C8600;  Maybe update frame
		PUSH 0; / Arg1 = 00000000
		MOV EDI, carBase; | actorBase
		LEA ECX, DWORD PTR DS : [EDI + 0x70]; |
		MOV EAX, 0x0050EE60
		CALL EAX; Game.0050EE60; \important, does something with actor obj
		MOV ECX, EDI;  EDI == actorBase
		MOV EAX, 0x00469DD0
		CALL EAX; Game.00469DD0

		MOV EAX, car_frame
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		MOV EAX, 0x004253A0
		CALL EAX; Game.004253A0
		MOV EDI, EAX
		PUSH 0
		PUSH 0x3F000000
		PUSH 0
		MOV EBX, DWORD PTR DS : [EDI]
		LEA ECX, DWORD PTR SS : [ESP + 0x364];  vector
		MOV EAX, 0x004026C0
		CALL EAX;  Game.004026C0;  CopyPushedDataToVector
		PUSH EAX
		LEA EAX, DWORD PTR SS : [ESP + 0x404]
		PUSH EAX
		MOV EAX, car_frame
		MOV ECX, EAX
		MOV EAX, 0x00425200
		CALL EAX; Game.00425200
		MOV ECX, EAX
		MOV EAX, 0x00424FD0
		CALL EAX; Game.00424FD0;  GetPositionVector
		PUSH EAX
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		PUSH EAX
		PUSH EDI
		CALL DWORD PTR DS : [EBX + 0xBC]
		add ESP, 0x1000
	}
}



void CGame::SetCarRotation(DWORD carBase, Vector3D rotation)
{
	if (carBase == NULL)
	{
		g_CCore->GetLog()->AddLog("SetCarRotation - car doesnt exist");
		return;
	}
	DWORD car_frame = *(DWORD*)(carBase + 0x68);
	if (car_frame == NULL)
	{
		g_CCore->GetLog()->AddLog("SetCarRotation - carframe doesnt exist");
		return;
	}
	float x = rotation.x, y = rotation.y, z = rotation.z;
	_asm
	{
		sub ESP, 0x1000
			SUB ESP, 0x0C
			MOV ECX, ESP
			MOV EAX, x
			MOV DWORD PTR DS : [ECX], EAX
			MOV EAX, y
			MOV DWORD PTR DS : [ECX + 4], EAX
			MOV EAX, z
			MOV DWORD PTR DS : [ECX + 8], EAX

			MOV ECX, carBase;  actorBase
			MOV EAX, 0x004253F0
			CALL EAX;  Game.004253F0;  SetActorRotation

			SUB ESP, 0x0C
			MOV EDI, car_frame
			LEA ECX, DWORD PTR SS : [ESP + 0x3E8];  vector
			MOV EBX, DWORD PTR DS : [EDI];  frame vtable
			PUSH 0
			PUSH ECX;  vector
			MOV ECX, carBase;  actorBase
			MOV EAX, 0x005C8710
			CALL EAX; Game.005C8710;  GetActorRotationVector(EAX == vect)
			PUSH EAX
			MOV EDI, car_frame
			PUSH EDI;  Actor's frame
			CALL DWORD PTR DS : [EBX + 0xC];  IDK, but important(checks obj rotation)
			MOV EAX, car_frame
			MOV ECX, DWORD PTR DS : [EAX]
			PUSH EAX;  frame
			CALL DWORD PTR DS : [ECX + 0x18];  important, too
			MOV EAX, car_frame
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			PUSH EAX;  frame
			MOV EAX, 0x004253A0
			CALL EAX; Game.004253A0;  return something(maybe pointer)
			MOV ECX, EAX
			MOV EAX, 0x005C8600
			CALL EAX;  Game.005C8600;  Maybe update frame
			PUSH 0; / Arg1 = 00000000
			MOV EDI, carBase; | actorBase
			LEA ECX, DWORD PTR DS : [EDI + 0x70]; |
			MOV EAX, 0x0050EE60
			CALL EAX; Game.0050EE60; \important, does something with actor obj
			MOV ECX, EDI;  EDI == actorBase
			MOV EAX, 0x00469DD0
			CALL EAX; Game.00469DD0
			MOV EAX, car_frame
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, 0x004253A0
			CALL EAX; Game.004253A0
			MOV EDI, EAX
			PUSH 0
			PUSH 0x3F000000
			PUSH 0
			MOV EBX, DWORD PTR DS : [EDI]
			LEA ECX, DWORD PTR SS : [ESP + 0x364];  vector
			MOV EAX, 0x004026C0
			CALL EAX;  Game.004026C0;  CopyPushedDataToVector
			PUSH EAX
			LEA EAX, DWORD PTR SS : [ESP + 0x404]
			PUSH EAX
			MOV EAX, car_frame
			MOV ECX, EAX
			MOV EAX, 0x00425200
			CALL EAX; Game.00425200
			MOV ECX, EAX
			MOV EAX, 0x00424FD0
			CALL EAX; Game.00424FD0;  GetPositionVector
			PUSH EAX
			MOV ECX, carBase;  actorBase
			MOV EAX, 0x00425410
			CALL EAX; Game.00425410;  GetFrame
			PUSH EAX
			PUSH EDI
			CALL DWORD PTR DS : [EBX + 0xBC]
			add ESP, 0x1000
	}
}

void CGame::CarUpdate(DWORD carBase, Vector3D position, Vector3D rotation)
{
	VEHICLE* veh = (VEHICLE*)carBase;
	if (veh)
	{
		veh->position = position;
		veh->rotation = rotation;
		_asm
		{
			//MOV EAX, 0x0
			MOV EAX, 0x3DA9FBE8
				PUSH EAX; / Arg3 = 00000000
				PUSH EAX; | Arg2	0
				//MOV EAX, 0x3DA9FBE8
				PUSH EAX; | Arg1   	0x3DA9FBE8
				MOV ESI, carBase
				ADD ESI, 0x70
				MOV ECX, ESI; | base + 0x70
				MOV EAX, 0x0052E6D0
				CALL EAX; game.0052E6D0; \game.0052E6D0
		}
		_asm {
			MOV ESI, carBase
				MOV ECX, ESI
				MOV EAX, DWORD PTR DS : [ESI]
				PUSH 0x31
				CALL DWORD PTR DS : [EAX + 0x34]


		}
	}
}

/*void CGame::CarUpdate(DWORD carBase, Vector3D position, Vector3D rotation)
{
	//*(float*)(*(DWORD*)(entity + 0x438) + 0x40) += 10.0f;	// pos
	//*(float*)(*(DWORD*)(entity + 0x438) + 0x9C) += 0.1f;	// rot

	//0050F245   8D46 50          LEA EAX, DWORD PTR DS : [ESI + 50]; 50



	*(Vector3D*)(*(DWORD*)(carBase + 0x438) + 0x40) = position;
	*(Vector3D*)(*(DWORD*)(carBase + 0x438) + 0x50) = rotation;

	_asm {
		MOV EDI, carBase
			MOV ECX, EDI;  EDI == actorBase
			MOV EAX, 0x00469DD0
			CALL EAx; Game.00469DD0
	}
}*/

/*void CGame::CarUpdate(DWORD carBase, Vector3D position, Vector3D rotation)
{
	if (carBase == NULL)
	{
		g_CCore->GetLog()->AddLog("UpdateCar - car doesnt exist");
		return;
	}
	DWORD car_frame = *(DWORD*)(carBase + 0x68);
	if (car_frame == NULL)
	{
		g_CCore->GetLog()->AddLog("UpdateCar - carframe doesnt exist");
		return;
	}
	float x = position.x, y = position.y, z = position.z;
	float rotx = rotation.x, roty = rotation.y, rotz = rotation.z;
	_asm
	{		
		sub ESP, 0x1000
		SUB ESP, 0x0C;  allocate stack for new vector
		MOV ECX, ESP
		MOV EDX, x
		MOV DWORD PTR DS : [ECX], EDX;  EDX = posX
		MOV EDX, y
		MOV DWORD PTR DS : [ECX + 0x4], EDX
		MOV EDX, z
		MOV DWORD PTR DS : [ECX + 0x8], EDX
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x005C86F0
		CALL EAX; Game.005C86F0;  SetActorPosition
		SUB ESP, 0x0C
		MOV ECX, ESP
		MOV EDX, rotx
		MOV DWORD PTR DS : [ECX], EDX
		MOV EDX, roty
		MOV DWORD PTR DS : [ECX + 0x4], EDX
		MOV EDX, rotz
		MOV DWORD PTR DS : [ECX + 0x8], EDX
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x004253F0
		CALL EAX; Game.004253F0;  SetActorRotation
		LEA EAX, DWORD PTR SS : [ESP + 0x340];  new vector
		MOV ECX, carBase;  actorBase
		PUSH EAX
		MOV EAX, 0x004253D0
		CALL EAX; Game.004253D0;  GetActorPositionVector
		PUSH EAX;  position vector(for SetFramePositionFromVector)
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		MOV ECX, EAX
		MOV EAX, 0x0045F000
		CALL EAX; Game.0045F000;  SetFramePositionFromVector(vect as ESP + 4)
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		MOV EDI, EAX
		LEA ECX, DWORD PTR SS : [ESP + 0x3E8];  vector
		MOV EBX, DWORD PTR DS : [EDI];  frame vtable
		PUSH 0
		PUSH ECX;  vector
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x005C8710
		CALL EAX; Game.005C8710;  GetActorRotationVector(EAX == vect)
		PUSH EAX
		PUSH EDI;  Actor's frame
		CALL DWORD PTR DS : [EBX + 0xC];  IDK, but important(checks obj rotation)
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		MOV ECX, DWORD PTR DS : [EAX]
		PUSH EAX;  frame
		CALL DWORD PTR DS : [ECX + 0x18];  important, too
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		TEST EAX, EAX
		JE SHORT badjump
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		PUSH EAX;  frame
		MOV EAX, 0x004253A0
		CALL EAX; Game.004253A0;  return something(maybe pointer)
		MOV ECX, EAX
		MOV EAX, 0x005C8600
		CALL EAX; Game.005C8600;  Maybe update frame
		badjump:
		PUSH 0; / Arg1 = 00000000
		MOV EDI, carBase; | actorBase
		LEA ECX, DWORD PTR DS : [EDI + 0x70]; |
		MOV EAX, 0x0050EE60
		CALL EAX; Game.0050EE60; \important, does something with actor obj
		MOV ECX, EDI;  EDI == actorBase
		MOV EAX, 0x00469DD0
		CALL EAx; Game.00469DD0				// tu sa meni pozicia, na to to vola este jednu sub funkciu
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		TEST EAX, EAX
		JE dieEnd
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		MOV EAX, 0x004253A0
		CALL EAX; Game.004253A0
		MOV EDI, EAX
		PUSH 0x0
		PUSH 0x3F000000
		PUSH 0x0
		MOV EBX, DWORD PTR DS : [EDI]
		LEA ECX, DWORD PTR SS : [ESP + 0x364];  vector
		MOV EAX, 0x004026C0
		CALL EAX; Game.004026C0;  CopyPushedDataToVector
		PUSH EAX
		LEA EAX, DWORD PTR SS : [ESP + 0x404]
		PUSH EAX
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		MOV ECX, EAX
		MOV EAX, 0x00425200
		CALL EAX; Game.00425200
		MOV ECX, EAX
		MOV EAX, 0x00424FD0
		CALL EAX; Game.00424FD0;  GetPositionVector
		PUSH EAX
		MOV ECX, carBase;  actorBase
		MOV EAX, 0x00425410
		CALL EAX; Game.00425410;  GetFrame
		PUSH EAX
		PUSH EDI
		CALL DWORD PTR DS : [EBX + 0xBC]
		dieEnd:


			add ESP, 0x1000
	}
}
*/
void CGame::GivePlayerToCarFast(DWORD ped, int vehId, int seatId)
{
	if (ped == NULL)
		return;
	DWORD car = g_CCore->GetVehiclePool()->Return(vehId)->GetEntity();
	if (car != NULL && seatId != -1)
	{
		_asm
		{
			MOV EAX, seatId
			PUSH EAX; / Arg2 = 00000000
			MOV EAX, car
			PUSH EAX; | Arg1
			MOV ESI, ped
			MOV ECX, ESI; |
			MOV EAX, 0x0049E580
			CALL EAX;  Game.0049E580; \Game.0049E580
		}
	}
}

void CGame::GivePlayerToCar(DWORD ped, int vehId, int seatId)
{
	char buff[255];
	sprintf(buff, "%x %d %d", ped, vehId, seatId);
	g_CCore->GetLog()->AddLog(buff);
	DWORD car = g_CCore->GetVehiclePool()->Return(vehId)->GetEntity();
/*	if (car == NULL)
		g_CCore->GetLog()->AddLog("GivePTC - car doesnt exist");
		return;*/
	_asm
	{
		MOV ESI, car
		//MOV EBP, obj
		MOV EAX, seatId
		PUSH 0; / Arg4 = 00000000
		PUSH EAX; | Arg3
		PUSH 1; | Arg2 = 00000001
		PUSH ESI; | Arg1 = 0E830248	// auto
		//MOV ECX, EBP; |	// ped
		MOV ECX, ped
		MOV EAX, 0x004A26D0
		CALL EAX;  Game.004A26D0; \Game.004A26D0
	}
}

void CGame::CarRepair(DWORD vehicle)
{
	__asm
	{
		MOV ECX, vehicle
			MOV EAX, 0x04253A0
			CALL EAX
			CMP EAX, 4
			JNZ skip_function
			PUSH 1
			MOV ECX, vehicle
			MOV EAX, 0x00465760
			CALL EAX
		skip_function :
	}
}

void CGame::CarLock(DWORD vehicle, BYTE locked)
{
	DWORD dwLocked = locked;
	__asm
	{
		MOV ECX, vehicle
			PUSH dwLocked
			MOV EAX, 0x00470A60
			CALL EAX
	}
}


void CGame::SetPlayerPosition(DWORD ped, Vector3D position)
{
	OBJECT* pedObj = (OBJECT*) ped;
	if (ped == NULL)
		return;
	if (pedObj->frame == NULL)
		return;

	DWORD pFrame = (DWORD)pedObj->frame;
	Vector3D* objPos = &pedObj->position;
	// set obj. position
	pedObj->position = position;
	Vector3D strangeVect;
	strangeVect.x = 0.0f;
	strangeVect.y = 1.0f;
	strangeVect.z = 0.0f;

	// used in code to store some temporary info
	Vector3D result;
	
	_asm {
		PUSH objPos
			MOV ECX, pFrame
			MOV EAX, 0x0045F000
			CALL EAX; Game.0045F000;  SetFramePosition ?
			MOV EAX, pFrame
			MOV ECX, DWORD PTR DS : [EAX]
			PUSH EAX
			CALL DWORD PTR DS : [ECX + 0x18]; get frame vector
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, pFrame
			PUSH EAX
			MOV EAX, 0x004253A0
			CALL EAX; Game.004253A0;  GetSomeObject
			MOV ECX, EAX
			MOV EAX, 0x005C8600
			CALL EAX; Game.005C8600;  UpdateObject ?


			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, 0x004253A0
			CALL EAX; Game.004253A0
			MOV EDI, EAX;  EDI = kind of object(class) ?
			MOV EBX, DWORD PTR DS : [EDI]
			// EAX contains vector {0,1.0,0}
			LEA EAX, strangeVect
			PUSH EAX; (vector2)
			LEA EAX, result
			PUSH EAX; (vector1)
			MOV ECX, pFrame
			MOV EAX, 0x00425200
			CALL EAX; Game.00425200;  returns frame's vector
			MOV ECX, EAX
			MOV EAX, 0x00424FD0
			CALL EAX; Game.00424FD0;  it does something with those 2 vectors vector1 = (ECX vector + vector2) // crashes here
			PUSH EAX; 		// push finalised vector
			MOV EAX, pFrame
			PUSH EAX;  Frame
			PUSH EDI;  that strange object(class or what)
			CALL DWORD PTR DS : [EBX + 0xBC] // ebx class or what
	}
}


void CGame::KickPlayerFromCar(DWORD ped, int vehId)
{
	char buff[255];
	sprintf(buff, "%x %d", ped, vehId);
	g_CCore->GetLog()->AddLog(buff);
	DWORD car = g_CCore->GetVehiclePool()->Return(vehId)->GetEntity();
	//if (car == NULL)
	//	g_CCore->GetLog()->AddLog("KickPFC - car doesnt exist");
	//return;
	_asm
	{
		mov edi, ped
		mov ecx, car
		mov EBX, 0x0
		PUSH EBX; / Arg4 = 00000000
		PUSH EBX; | Arg3
		PUSH 2; | Arg2 = 00000002
		PUSH ECX; | Arg1	auto
		MOV ECX, EDI; |
		MOV EAX, 0x004A26D0
		CALL EAX;  Game.004A26D0; \Game.004A26D0
	}
}

void CGame::KickPlayerFromCarFast(DWORD ped)
{
	char buff[255];
	sprintf(buff, "%x", ped);
	DWORD car = NULL;
	_asm{
		mov ECX, ped
		mov eax, DWORD PTR DS:[ECX+0x98]
		mov car, eax
	}
	if (car != NULL)
	{
		_asm
		{
			MOV ECX, ped
			MOV EAX, 0x004A1770
			CALL EAX
		}
	}
	else
	{
		g_CCore->GetLog()->AddLog("[Err] Player is not in car");
	}
}

void CGame::HideWeapon()
{
	char buff[255];
	sprintf(buff, "HideWep");
	g_CCore->GetLog()->AddLog(buff);
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SWITCHWEAPON);
	bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
	bsOut.Write((int)0);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}


void CGame::OnCollision()
{
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	DWORD car = (DWORD) ped->playersCar;
	float damage = ped->playersCar->engineDamage;

	int vehID		= g_CCore->GetVehiclePool()->GetVehicleIdByBase(car);
	CVehicle* veh	= g_CCore->GetVehiclePool()->Return(vehID);
	if (veh != NULL)
	{
		veh->SetDamage(damage);
		// send server message about it
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_DAMAGE);
		bsOut.Write(vehID);
		bsOut.Write(damage);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED);
		char buff[250];
		sprintf(buff,"OnCollision %d %f",vehID,damage);
		g_CCore->GetLog()->AddLog(buff);
	}
}

void CGame::OnCarShot()
{
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	DWORD car = (DWORD)ped->playersCar;

	byte damage = *(byte*)(car + 0x2094);

	int vehID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(car);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehID);
	if (veh != NULL)
	{
		veh->SetShotDamage(damage);
		// send server message about it
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_SHOTDAMAGE);
		bsOut.Write(vehID);
		bsOut.Write(damage);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED);
		char buff[250];
		sprintf(buff, "OnCarShot %d %d", vehID, damage);
		g_CCore->GetLog()->AddLog(buff);
	}
}


void CGame::DisableBridges()
{
	for (int i = 0; i < sizeof(FORBIDDEN_BRIDGES) / sizeof(FORBIDDEN_BRIDGES[0]); i++)
	{
		DWORD bridge = g_CCore->GetGame()->FindActor((char*) FORBIDDEN_BRIDGES[i]);
		if (bridge != NULL)
		{
			__asm
			{
				PUSH 1
				MOV ECX, bridge
				MOV EAX, 0x00462450
				CALL EAX
			}
		}
	}

}


byte CGame::GetMusicState()
{
	return this->MusicState;
}
void CGame::SetMusicState(byte state)
{
	this->MusicState = state;
}

void CGame::ChangeMap(char map[], char* start)
{
	char mapBuff[512];
	memset(mapBuff, 0x0, 512);
	sprintf(mapBuff, map);

	sprintf(mapBuff + 0x40, "none");

	g_CCore->GetChat()->AddDebugMessage("Changing map to %s..", mapBuff);
	g_CCore->GetGame()->PreRespawn();
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(i);
		if (veh != NULL)
		{
			if (veh->GetEntity())
			{
				g_CCore->GetGame()->DeleteCar(veh->GetEntity());
				veh->SetEntity(NULL);
			}
		}
	}
	_asm
	{
		sub ESP, 0x1000
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			PUSH 1
			MOV EAX, 0x00425390
			CALL EAX; Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x005C8C70
			CALL EAX; Game.005C8C70
			MOV ECX, ESI
			MOV EDX, 0x006F9258
			LEA ESI, mapBuff
			//mov ESI, map
			MOV EAX, ESI;  VAR - mission name
			SUB EDX, ESI; wtf ?
		// --- following code copies mission name into 0x6F9258
		jmp1:
		MOV CL, BYTE PTR DS : [EAX];  copy mission string into global(? ) variable
		MOV BYTE PTR DS : [EDX + EAX], CL
		INC EAX
		TEST CL, CL
		JNZ SHORT jmp1
		//---
		LEA EAX, DWORD PTR DS : [ESI + 0x40];  mission frame
		MOV EDX, 0x006F9234
		MOV BYTE PTR DS : [0x6F9254], 0x1		// set something on - maybe map change = true ?
		SUB EDX, EAX
		
		//--- copy string, this is the problem that we are not having anything at EAX address, just random bytes
		//--- TODO fix it
		jmp2:
		MOV CL, BYTE PTR DS : [EAX]
			MOV BYTE PTR DS : [EDX + EAX], CL
			INC EAX
			TEST CL, CL
			JNZ SHORT jmp2
			//---
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, 0x005C8680
			CALL EAX; Game.005C8680
			MOV EDI, 0x006F9234
			OR ECX, 0xFFFFFFFF
			XOR EAX, EAX
			MOV EDX, DWORD PTR DS : [ESI + 0x80]		// again string ?
			REPNE SCAS BYTE PTR ES : [EDI]
			NOT ECX
			DEC ECX
			MOV DWORD PTR DS : [0x6F9578], EDX			// cmp string length of ESI+x040
			// if string length = NULL
			// TEST STUFF - JE end
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, 0x00425390
			CALL EAX; Game.00425390		// return a class handle ?
			MOV ECX, EAX
			MOV EAX, 0x00425530
			CALL EAX; Game.00425530 // ECX+x0E4
			MOV ESI, EAX
			TEST ESI, ESI		// if localplayer exists
			JE SHORT jmp3
			MOV ECX, ESI;  //ESI = localPlayer
			mov EAX, 0x00425730
			CALL EAX; Game.00425730;  IsLocalPlayerInCar
		TEST EAX, EAX
		// JE to JMP	---
		JMP SHORT jmp4
		MOV ECX, ESI;  CarCase:
		MOV EAX, 0x00425730
		CALL EAX; Game.00425730;  GetPlayerCar
		MOV DWORD PTR DS : [0x6F9570], EAX;  save car
		MOV DWORD PTR DS : [0x6F9574], ESI;  save localPlayer
		JMP SHORT jmp3
	jmp4:
		//no car case
		MOV DWORD PTR DS : [0x6F9570], ESI;  noCarCase:
		MOV DWORD PTR DS : [0x6F9574], 0;  don't save a car
		jmp3:
		MOV ESI, 0x006F9570
		// a loop which, while ESI >= 0x0006F9578
		jmp6 :
			 MOV EAX, DWORD PTR DS : [ESI];  some object
			 TEST EAX, EAX
			 JE SHORT jmp5
			 MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			 PUSH EAX	// push local player
			 MOV EAX, 0x005FF5E0
			 CALL EAX; Game.005FF5E0
		jmp5:
		ADD ESI, 0x4
			CMP ESI, 0x006F9578		// compare if localplayer is type of
			JL SHORT jmp6
			MOV EAX, DWORD PTR DS : [0x6F9570]	// returns local player ?
			TEST EAX, EAX
			JE end
			PUSH 0x3F800000
			PUSH 0
			PUSH 0
			// WTF is ESP +0x478 ? might be dangerous, in stack history
			// TODO explore it, replace with our vector
			LEA ECX, DWORD PTR SS : [ESP + 0x478]
			MOV EAX, 0x004026C0
			CALL EAX; Game.004026C0;  make a 3D vector structure in stack(returns  pointer)
			MOV ECX, DWORD PTR DS : [EAX]
			PUSH 0
			MOV DWORD PTR DS : [0x6F9210], ECX
			MOV EDX, DWORD PTR DS : [EAX + 0x4]
			MOV DWORD PTR DS : [0x6F9214], EDX
			MOV EAX, DWORD PTR DS : [EAX + 0x8]
			PUSH 0x3F800000
			PUSH 0
			LEA ECX, DWORD PTR SS : [ESP + 0x31C]
			MOV DWORD PTR DS : [0x6F9218], EAX
			MOV EAX, 0x004026C0
			CALL EAX; Game.004026C0;  again some structure
			MOV ECX, DWORD PTR DS : [EAX]
			MOV DWORD PTR DS : [0x6F921C], ECX
			MOV EDX, DWORD PTR DS : [EAX + 0x4]
			MOV ECX, DWORD PTR DS : [0x6F9570];  getSomeCar
			MOV DWORD PTR DS : [0x6F9220], EDX
			MOV EAX, DWORD PTR DS : [EAX + 0x8]
			MOV DWORD PTR DS : [0x6F9224], EAX
			MOV EAX, 0x00425410
			CALL EAX; Game.00425410;  GetFrame
			MOV ECX, EAX
			mov eax, 0x004F46C0
			CALL EAX; Game.004F46C0
			MOV ECX, 0x10
			MOV ESI, EAX
			LEA EDI, DWORD PTR SS : [ESP + 0x1A0]
			PUSH 0x2
			REP MOVS DWORD PTR ES : [EDI], DWORD PTR DS : [ESI]
			PUSH 0x3
			LEA ECX, DWORD PTR SS : [ESP + 0x1A8]
			mov eax, 0x005A14F0
			CALL EAX; Game.005A14F0
			PUSH 0x1
			PUSH 0x3
			LEA ECX, DWORD PTR SS : [ESP + 0x1A8]
			MOV DWORD PTR DS : [EAX], 0
			MOV EAX, 0x005A14F0
			CALL EAX; Game.005A14F0
			PUSH 0x0
			PUSH 0x3
			LEA ECX, DWORD PTR SS : [ESP + 0x1A8]
			MOV DWORD PTR DS : [EAX], 0
			mov eax, 0x005A14F0
			CALL EAX; Game.005A14F0
			LEA ECX, DWORD PTR SS : [ESP + 0x1A0]
			MOV DWORD PTR DS : [EAX], 0
			PUSH ECX;
		mov EAX, 0x006F9210
			PUSH EAX; Game.006F9210
			mov EAX, 0x00624310
			CALL EAX; <JMP.&ls3df. ? ? XS_vector@@QAGAAU0@AB>
				LEA EDX, DWORD PTR SS : [ESP + 0x1A0]		// mozno toto
				PUSH EDX
				PUSH 0x006F921C
				mov EAX, 0x00624310
				CALL EAX; <JMP.&ls3df. ? ? XS_vector@@QAGAAU0@AB>
				//MOV ECX, DWORD PTR SS : [EBP + 0x44]
				//MOV EAX, 0x00425410
				//CALL EAX; Game.00425410
				//MOV ECX, EAX					// bad EAX - wtf
				//MOV EAX, 0x004F46C0
			//	CALL EAX; Game.004F46C0
				//MOV ECX, 0x10
				//MOV ESI, EAX
				LEA EDI, DWORD PTR SS : [ESP + 0x1A0]
				PUSH 0x2
				//REP MOVS DWORD PTR ES : [EDI], DWORD PTR DS : [ESI]
				PUSH 0x3
				LEA ECX, DWORD PTR SS : [ESP + 0x1A8]
				MOV EAX, 0x005A14F0
				CALL EAX; Game.005A14F0
				PUSH 0x1
				PUSH 0x3
				LEA ECX, DWORD PTR SS : [ESP + 0x1A8]
				MOV DWORD PTR DS : [EAX], 0
				MOV EAX, 0x005A14F0
				CALL EAX; Game.005A14F0
				PUSH 0x0
				PUSH 0x3
				LEA ECX, DWORD PTR SS : [ESP + 0x1A8]
				MOV DWORD PTR DS : [EAX], 0
				MOV EAX, 0x005A14F0
				CALL EAX; Game.005A14F0
				MOV DWORD PTR DS : [EAX], 0
				LEA EAX, DWORD PTR SS : [ESP + 0x1A0]
				PUSH EAX; / Arg1
				MOV EAX, 0x005C8030
				CALL EAX; Game.005C8030; \Game.005C8030
				LEA ECX, DWORD PTR SS : [ESP + 0x1A0]
				PUSH ECX
				PUSH 0x006F9210
				mov EAX, 0x00624310
				CALL EAX; <JMP.&ls3df. ? ? XS_vector@@QAGAAU0@AB>
				LEA EDX, DWORD PTR SS : [ESP + 0x1A0]
				PUSH EDX
				PUSH 0x006F921C
				mov EAX, 0x00624310
				CALL EAX; <JMP.&ls3df. ? ? XS_vector@@QAGAAU0@AB>
				//MOV ECX, DWORD PTR SS : [EBP + 0x44]
				//MOV EAX, 0x00425410
				//CALL EAX; Game.00425410
				//MOV ECX, EAX
				//MOV EAX, 0x004F46C0
				//CALL EAX; Game.004F46C0	// omg zasa
				//MOV ESI, EAX
				MOV ECX, 0x10
				LEA EDI, DWORD PTR SS : [ESP + 0x1A0]
				LEA EAX, DWORD PTR SS : [ESP + 0x1A0]
				//REP MOVS DWORD PTR ES : [EDI], DWORD PTR DS : [ESI]
				//PUSH EAX; / Arg1
				//MOV EAX, 0x005C8030
				//CALL EAX; Game.005C8030; \Game.005C8030
				MOV ECX, DWORD PTR DS : [0x6F9570]
				MOV EAX, 0x00425410
				CALL EAX; Game.00425410
				MOV ECX, EAX
				MOV EAX, 0x00425200
				CALL EAX; Game.00425200
		MOV ECX, DWORD PTR DS : [EAX]
		MOV DWORD PTR DS : [0x6F9228], ECX
		MOV EDX, DWORD PTR DS : [EAX + 0x4]
		LEA ECX, DWORD PTR SS : [ESP + 0x1A0]
		MOV DWORD PTR DS : [0x6F922C], EDX
		MOV EAX, DWORD PTR DS : [EAX + 0x8]
		PUSH ECX
		PUSH 0x006F9228
		MOV DWORD PTR DS : [0x6F9230], EAX
		mov EAX, 0x00624310
		CALL EAX; <JMP.&ls3df. ? ? XS_vector@@QAGAAU0@AB>
	end:

			add ESP, 0x1000
	}
}

void CGame::ReloadMap()
{
	_asm
	{
		MOV ECX, DWORD PTR DS : [0x65115C];  reload:
		PUSH 0x3E8
		MOV EAX, 0x00425390
		CALL EAX; game.00425390
		MOV ECX, EAX
		MOV EAX, 0x005F22E0
		CALL EAX; game.005F22E0
		MOV ECX, DWORD PTR DS : [0x65115C];  game.006F9440
		PUSH 0
		MOV EAX, 0x00425390
		CALL EAX; game.00425390
		MOV ECX, EAX; |
		MOV EAX, 0x005F1B10
		CALL EAX; game.005F1B10; \game.005F1B10
		MOV ECX, DWORD PTR DS : [0x65115C];  game.006F9440
		MOV ESI, EAX
		PUSH ESI
		MOV EAX, 0x00425390
		CALL EAX; game.00425390
		MOV ECX, EAX
		MOV EAX, 0x005F2140
		CALL EAX; game.005F2140
		MOV EDI, EAX
		TEST EDI, EDI
		JE end
		MOV ECX, DWORD PTR DS : [0x65115C];  game.006F9440
		PUSH ESI
		PUSH 0x0064FFB8;  ASCII "sounds\music\13 - Game Over.mp3"
		MOV EAX, 0x00425390
		CALL EAX; game.00425390
		MOV ECX, EAX
		MOV EAX, 0x005F1D90
		CALL EAX; game.005F1D90
		MOV ECX, DWORD PTR DS : [EDI]
		PUSH EDI
		CALL DWORD PTR DS : [ECX + 0x14]
		MOV ECX, DWORD PTR DS : [0x65115C];  game.006F9440
		PUSH 0x3E8
		PUSH 0x3F800000
		PUSH 0
		PUSH ESI
		MOV EAX, 0x00425390
		CALL EAX; game.00425390
		MOV ECX, EAX; |
		MOV EAX, 0x005F2390
		CALL EAX; game.005F2390; \game.005F2390
		end:
	}
}
void CGame::TrafficSetup()
{
	_asm
	{
		MOV ECX, 0x00671FB0;  Case 17F of switch 005BB320
		MOV EAX, 0x00560950
		CALL EAX; Game.00560950
	}
}

void CGame::Set8SlotInv(DWORD ped)
{
	_asm
	{
		MOV ECX, ped
		ADD ECX, 0x480
		MOV EAX, 0x0055B940
		CALL EAX
	}
}


void CGame::SetDoorState(char* door, bool state)
{
	DWORD peat = FindActor(door);
	DWORD ped = g_CCore->GetLocalPlayer()->GetEntity();
	if (peat != NULL && ped != NULL)
	{
		_asm
		{
			XOR EAX, EAX
				MOV AL, state
				MOV EDX, EAX
				PUSH 0
				PUSH 0
				PUSH 0
				PUSH peat
				MOV ECX, ped
				MOV EAX, 0x004A26D0
				CALL EAX
		}
	}
}

void CGame::SetDoorStateFacing(char* door, bool shouldClose,bool facing)
{
	DWORD doorBase = FindActor(door);
	// if close(1), then action = 3
	// if open(0), then action = 2
	byte action = (shouldClose == 1 ? 3 : 2);
	if (doorBase)
	{
		_asm {
			PUSH 0
			MOV ECX, doorBase;
			MOV ESI, ECX
			XOR EAX, EAX
			MOV AL, action
			MOV DWORD PTR DS : [ESI + 0x70], EAX;				set what we want to achieve(2 - open,3 - close doors)
			XOR EAX, EAX
			MOV AL,facing
			MOV BYTE PTR DS : [ESI + 0x11E], AL;				set facing
			MOV EAX, 0x0044C750
			CALL EAX
		}
	}
}

char*	CGame::GetActualMapName()
{
	return (char*)(*(DWORD*)0x006F94A8);
}


void CGame::PoliceManager()
{
	_asm {
		MOV EDX, 0x000EF420
		XOR ECX, ECX
		PUSH ECX;  ECX == 0
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		PUSH EDX;  EDX = 0x000EF420
		PUSH 1
		MOV EAX, 0x00425390
		CALL EAX; Game.00425390
		MOV ECX, EAX
		MOV EAX, 0x00425560
		CALL EAX; Game.00425560
		MOV ECX, EAX; |
		MOV EAX, 0x005CC9E0
		CALL EAX; Game.005CC9E0; \Game.005CC9E0
	}
}

DWORD CGame::CreateParticle(DWORD frame, int particle_id)
{
	DWORD particle_addr = NULL;

	__asm
	{
		sub esp, 0x500
			MOV EAX, frame
			PUSH EAX;  push frame
			PUSH 0
			PUSH 0x3F800000
			PUSH 0
			PUSH 0
			LEA ECX, DWORD PTR SS : [ESP + 0x384];   fill vector pos
			MOV EAX, 0x004026C0
			CALL EAX
			PUSH EAX
			PUSH 0
			PUSH 0
			PUSH 0
			LEA ECX, DWORD PTR SS : [ESP + 0x430]
			MOV EAX, 0x004026C0;  fill vector rot
			CALL EAX
			MOV EDX, particle_id
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			PUSH EAX
			PUSH EDX;  push particle id
			MOV EAX, 0x005C8650
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005A40B0
			CALL EAX
			MOV particle_addr, EAX
			MOV ECX, 0x00656368
			MOV EAX, 0x005467C0
			CALL EAX
			TEST AL, AL
			JE skip
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			PUSH EDI
			MOV EAX, 0x00425390
			CALL EAX
			MOV ECX, EAX; |
			MOV EAX, 0x005F3E60; \Game.005F3E60
			CALL EAX
		skip :

		add esp, 0x500


	}

	return particle_addr;
}

DWORD CGame::CreateEmptyFrame()
{
	DWORD frame = NULL;
	_asm
	{
		pushad
			MOV EAX, DWORD PTR DS : [0x6F9520];  Case 185 of switch 005BB320
			PUSH 9
			PUSH EAX
			MOV ECX, DWORD PTR DS : [EAX]
			CALL DWORD PTR DS : [ECX + 0x4C]
			mov frame, eax
			popad
	}
	return frame;
}

void CGame::SetFrameModel(DWORD frame, char* modelname)
{
	char texture[255] = "";
	sprintf_s(texture, "%s", modelname);

	__asm
	{
		MOV EAX, frame
			MOV ESI, EAX
			TEST ESI, ESI
			JE badresult_dest
			MOV EDX, DWORD PTR DS : [ESI]
			PUSH 0x0064FF98
			PUSH ESI
			CALL DWORD PTR DS : [EDX + 0x28]
			PUSH 0
			PUSH 0
			PUSH 0
			PUSH 0
			lea eax, texture
			PUSH EAX
			PUSH ESI
			MOV ECX, 0x006F9418
			mov eax, 0x00448940
			CALL eax
		badresult_dest :
	}
}

void CGame::RemoveParticle(DWORD particle)
{
	__asm
	{
		MOV EAX, particle
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			PUSH EAX
			MOV EAX, 0x005C8650
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005A4C60
			CALL EAX
	}
}



void CGame::CameraSetSwing(byte enable, float rotation)
{
	float rot = rotation / 100;

	__asm
	{
		PUSH 0
			MOV ECX, DWORD PTR DS : [0x65115C]
			MOV EDX, rot
			PUSH EDX
			MOV AL, enable
			PUSH EAX
			MOV EAX, 0x00425390
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x00425510
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005D4B20
			CALL EAX
	}
}

void CGame::CameraSetFov(float fov)
{
	//70 normal val for res 4:3
	//80 normal val for res 16:10
	//85 normal val for res 16:9

	__asm
	{
		MOV ECX, DWORD PTR DS : [0x65115C]
			MOV EAX, 0x004253A0
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005C85E0
			CALL EAX
			FLD DWORD PTR DS : [fov]
			FMUL DWORD PTR DS : [0x63CFA8]
			MOV EAX, DWORD PTR DS : [EAX]
			PUSH ECX
			MOV EDX, DWORD PTR DS : [EAX]
			FSTP DWORD PTR SS : [ESP]
			PUSH EAX
			CALL DWORD PTR DS : [EDX + 0x50]
	}
}

float CGame::CameraGetFov()
{
	float fov = 0;

	__asm
	{
		MOV ECX, DWORD PTR DS : [0x65115C]
			MOV EAX, 0x004253A0
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005C85E0
			CALL EAX
			MOV ECX, DWORD PTR DS : [EAX]
			MOV EAX, 0x005C85A0
			CALL EAX
			FLD DWORD PTR DS : [EAX]
			FMUL DWORD PTR DS : [0x63CFAC]
			FST DWORD PTR DS : [fov]
	}

	return floor(fov);
}

// --- GAME 

void CGame::TimerOn(int remain, int pos_sec, int pos_min, int pos_hour)
{
	__asm
	{
		PUSH pos_sec
		PUSH pos_min
		PUSH pos_hour
		MOV ECX, 0x658330
		MOV EAX, 0x005495F0
		CALL EAX
		MOV ECX, 0x658330
		PUSH remain
		MOV EAX, 0x00549630
		CALL EAX
		PUSH 8
		MOV ECX, 0x658330
		MOV EAX, 0x00424E20
		CALL EAX
		MOV EAX, 0x004B4520
		CALL EAX
	}
}

void CGame::TimerOff()
{
	__asm
	{
		PUSH 8
			MOV ECX, 0x00658330
			MOV EAX, 0x005C7DB0
			CALL EAX
	}
}

void CGame::TimerSetInterval(float interval)
{
	__asm
	{
		FLD DWORD PTR DS : [interval]
			MOV EAX, 0x006249F0
			CALL EAX
			PUSH EAX
			MOV ECX, 0x00658330
			MOV EAX, 0x00549630
			CALL EAX
	}
}

float CGame::TimerGetInterval()
{
	int interval = 0;

	__asm
	{
		sub ESP, 0x100
			PUSH 8
			MOV ECX, 0x00658330
			MOV EAX, 0x005C7DD0
			CALL EAX
			TEST AL, AL
			JE end
			LEA ECX, DWORD PTR SS : [ESP + 0x64]
			PUSH ECX
			MOV ECX, 0x00658330
			MOV EAX, 0x00549710
			CALL EAX
			MOV EDX, DWORD PTR SS : [ESP + 0x64]
			MOV interval, EDX
		end :
		add ESP, 0x100
	}

	return (float)interval;
}


void CGame::WeatherSetParam(char* str, int value)
{
	bool usefloated = false;
	int parameter = NULL;

	//ON 0
	//MAX_CNT 10
	//SPEED 5 floated
	//LEN 6 floated
	//WIDTH 7 floated
	//MAX_DIST 8 floated
	//MAX_HEIGHT 9 flaoted
	//MODE 14 

	if (strcmp(str, "ON") == 0)
	{
		parameter = 0;
	}
	else if (strcmp(str, "MAX_CNT") == 0)
	{
		parameter = 10;
	}
	else if (strcmp(str, "SPEED") == 0)
	{
		parameter = 5;
		usefloated = true;
	}
	else if (strcmp(str, "LEN") == 0)
	{
		parameter = 6;
		usefloated = true;
	}
	else if (strcmp(str, "WIDTH") == 0)
	{
		parameter = 7;
		usefloated = true;
	}
	else if (strcmp(str, "MAX_DIST") == 0)
	{
		parameter = 8;
		usefloated = true;
	}
	else if (strcmp(str, "MAX_HEIGHT") == 0)
	{
		parameter = 9;
		usefloated = true;
	}
	else if (strcmp(str, "MODE") == 0)
	{
		parameter = 14;
	}
	else return;

	if (usefloated)
	{
		float value_ex = (float) value;
		__asm
		{
			MOV ECX, DWORD PTR DS : [0x65115C]
				MOV EAX, 0x004253A0
				CALL EAX
				MOV EDX, DWORD PTR DS : [EAX]
				PUSH value_ex;
			PUSH parameter
				PUSH EAX
				CALL DWORD PTR DS : [EDX + 0xE8]
		}
	}
	else
	{
		_asm
		{
			MOV ECX, DWORD PTR DS : [0x65115C]
				MOV EAX, 0x004253A0
				CALL EAX
				MOV EDX, DWORD PTR DS : [EAX]
				PUSH value;
			PUSH parameter
				PUSH EAX
				CALL DWORD PTR DS : [EDX + 0xE8]
		}
	}
}

void CGame::SetObjective(char* text)
{
	DWORD adresa = NULL;

	__asm
	{
		PUSH 3472
			MOV ECX, 0x67A57C
			MOV EAX, 0x0057D850
			CALL EAX
			LEA EDX, [EAX]
			MOV adresa, EDX
	}

	sprintf((char*)adresa, text);

	__asm
	{
		PUSH 3472
			MOV ECX, 0x00671FB0;
		MOV EAX, 0x0055D760
			CALL EAX
	}
}

void CGame::ClearObjective()
{
	__asm
	{
		MOV ECX, 0x00671FB0
			MOV EAX, 0x0055D730
			CALL EAX
	}
}

void CGame::ConsoleAddText(char* text, DWORD color)
{
	DWORD adresa = NULL;

	__asm
	{
		PUSH 3472
			MOV ECX, 0x67A57C
			MOV EAX, 0x0057D850
			CALL EAX
			LEA EDX, [EAX]
			MOV adresa, EDX
	}

	sprintf((char*)adresa, text);

	__asm
	{
		PUSH color
			MOV ECX, 0x00658330
			PUSH 3472
			MOV EAX, 0x0054BF10
			CALL EAX
	}
}

void CGame::ExplodeCar(DWORD base)
{
	_asm {
		PUSH 0x0; / Arg1	reason
			MOV ECX, base; | car
			MOV EAX, 0x00468BC0
			CALL EAX; Game.00468BC0; \Game.00468BC0
	}
}

bool CGame::isControlLocked()
{
	return this->bLockControls;
}

void CGame::SetLockControls(bool b)
{
	this->bLockControls = b;
	//this->UpdateControls();
}
// NO LONGER USED, input is blocked in Main.cpp / PeekMessageHook
/*
void CGame::UpdateControls()
{
	DWORD pedbase = g_CCore->GetLocalPlayer()->GetEntity();
	if (pedbase)
	{
		*(byte*)(pedbase + 0xADA) = (g_CCore->GetChat()->IsTyping() || this->isControlLocked() || g_CCore->GetIngameMenu()->isActive());
	}
}*/

void CGame::SetTrafficVisible(bool baf)
{
	DWORD value = (DWORD)baf;
	_asm {
		mov EDX, value
		MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
		TEST EDX, EDX
		SETNE DL
		PUSH EDX
		MOV EAX, 0x00425390
		CALL EAX; Game.00425390
		MOV ECX, EAX
		MOV EAX, 0x005E3DC0
		CALL EAX; Game.005E3DC0
	}
}

DWORD CGame::PlaySoundAtFrame(DWORD frame, char* filename, float volume, float radius)
{
	DWORD sound_id = NULL;

	__asm {
		sub ESP, 0x500
			MOV ESI, filename
			MOV EDI, frame
			FLD DWORD PTR DS : [ESI + 0x50]
			FCOMP DWORD PTR DS : [0x63B2A8]
			PUSH 0
			PUSH 0
			PUSH 0
			FSTSW AX
			LEA ECX, DWORD PTR SS : [ESP + 0x430]
			MOV EAX, 0x004026C0
			CALL EAX
			PUSH 0
			PUSH 0x3A83126F
			PUSH 0
			MOV EDX, volume
			MOV ECX, radius
			FLD DWORD PTR DS : [radius]
			FMUL DWORD PTR DS : [0x63B2C4]
			PUSH EDX
			PUSH ECX
			MOV ECX, DWORD PTR DS : [EAX]
			SUB ESP, 0x10
			MOV EDX, ESP
			FSTP DWORD PTR SS : [ESP + 0xC]
			MOV DWORD PTR DS : [EDX], ECX
			MOV ECX, DWORD PTR DS : [EAX + 0x4]
			MOV EAX, DWORD PTR DS : [EAX + 0x8]
			PUSH 1
			MOV DWORD PTR DS : [EDX + 0x4], ECX
			MOV ECX, DWORD PTR DS : [0x65115C]
			PUSH ESI
			PUSH EDI
			MOV DWORD PTR DS : [EDX + 0x8], EAX
			MOV EAX, 0x00425390
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005E8600;
		CALL EAX
			MOV sound_id, EAX
			add ESP, 0x500
	}
}

void CGame::StopSound(DWORD soundid)
{
	__asm
	{
		FLD DWORD PTR DS : [soundid]
			PUSH 0x3A83126F
			PUSH 0
			MOV EAX, 0x06249F0
			CALL EAX
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			PUSH EAX
			MOV EAX, 0x00425390
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005E8790
			CALL EAX
	}
}

void CGame::LockCarDoor(DWORD car, int seat, bool status)
{
	if (car != NULL)
	{
		DWORD logic = (status == 1);
		_asm
		{
				MOV EDI, seat
				MOV EBX, car
				LEA ECX, DWORD PTR DS : [EBX + 0x70]
				MOV EAX, 0x00425460
				CALL EAX; Game.00425460
				CMP EAX, EDI
				JLE end
				TEST EDI, EDI
				JL end
				MOV EDX, logic
				MOV ECX, EBX
				TEST EDX, EDX
				SETNE DL
				PUSH EDX
				PUSH EDI
				MOV EAX, 0x00470C50
				CALL EAX; Game.00470C50
			end:

		}
	}
}

void CGame::SetFrameRot(DWORD frame, float w, float x, float y, float z)
{
	Vector4D rot;
	rot.x = w;
	rot.y = x;
	rot.z = y;
	rot.w = z;

	__asm {
		MOV EAX, frame
			LEA EDX, rot;  toto by mohol byt vektor
			MOV ECX, EAX;  EAX = frame
			PUSH EDX;  vector
			MOV EAX, 0x0045F040
			CALL EAX

			MOV EAX, frame
			PUSH EAX
			MOV EDX, DWORD PTR DS : [EAX]
			CALL DWORD PTR DS : [EDX + 0x18]
	}

}

void CGame::SetFrameScale(DWORD frame, float x, float y, float z)
{
	Vector3D scale;
	scale.x = x;
	scale.y = y;
	scale.z = z;
	__asm {

		MOV EAX, frame
			LEA ECX, scale
			PUSH ECX
			MOV ECX, EAX
			MOV EAX, 0x005C8300
			CALL EAX
			MOV EAX, frame
			PUSH EAX
			MOV ECX, DWORD PTR DS : [EAX]
			CALL DWORD PTR DS : [ECX + 0x18]
	}
}

DWORD CGame::CreateFrame(char* str)
{
	DWORD addr = NULL;
	char model[255] = "";
	sprintf_s(model, "%s", str);

	_asm
	{
		pushad
			MOV EAX, DWORD PTR DS : [0x6F9520]
			PUSH 9
			PUSH EAX
			MOV ECX, DWORD PTR DS : [EAX]
			CALL DWORD PTR DS : [ECX + 0x4C]
			MOV addr, EAX; Save Frame Addres into DWORD var
			MOV ESI, EAX
			TEST ESI, ESI
			JE badresult_dest
			MOV EDX, DWORD PTR DS : [ESI]
			PUSH 0x0064FF98
			PUSH ESI
			CALL DWORD PTR DS : [EDX + 0x28]
			PUSH 0;
		PUSH 0;
		PUSH 0;
		PUSH 0;
		LEA EAX, model
			PUSH EAX
			PUSH ESI
			MOV ECX, 0x006F9418
			mov EAX, 0x00448940
			CALL EAX
		badresult_dest :
		popad
	}
	return addr;
}

void CGame::PickupsTick()
{
	//DWORD tickCount = (timeGetTime()/30)%360;
	pickupsAngle += 0.1f;
	float rw = cos(pickupsAngle / (180.0f / 3.14f));
	float ry = sin(pickupsAngle / (180.0f / 3.14f));
	
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		CPickup* pickup = g_CCore->GetPickupPool()->Return(i);
		if (pickup)
		{
			if (pickup->GetEntity())
			{
				SetFrameRot(pickup->GetEntity(),rw, 0.0, ry,0.0);
				
				//sprintf(buff, "%f %f", rw, ry);
				//g_CCore->GetChat()->AddMessage(buff);
			}

		}
	}
}

byte CGame::IsTabMapEnabled()
{
	return *(BYTE*)0x00661A14;
}

int CGame::GetGameVersion()
{
	// 180 - 385 - 1.00
	if (*(DWORD*)0x005F99FE == 0x180)
		return 384;
	// 18B - 395 - 1.02
	if (*(DWORD*)0x005BEC2E == 0x18B)
		return 395;
	// if we haven't detected any version
	return 0;
}


Vector3D	CGame::GetPEDNeckPosition(PED* ped)
{
	Vector3D pos;
	// if ped exists
	if (ped)
	{
		// then sum up PED's base position (world position) with neck position (model relative position)
		// Neck position is related from model origin -> doesn't represent world coords and must be sum up 
		Vector3D neckPos = *(Vector3D*)((DWORD) (ped->frm_neck) + 0x40);
		/*Vector3D pedPos = ped->object.position;
		pos.x = neckPos.x + pedPos.x;
		pos.y = neckPos.y + pedPos.y;
		pos.z = neckPos.z + pedPos.z;
		return pos;
		*/
		return neckPos;
	}
	// if PED doesn't exist, returns empty Vector3D
	return Vector3D();
}