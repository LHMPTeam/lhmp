#include "CGame.h"
#include "CCore.h"

#include <Windows.h>
extern CCore *g_CCore;

CGame::CGame()
{
	ShouldStop = false;
	IsStopped = false;
}
CGame::~CGame()
{
}
void CGame::Tick()
{
	//g_CCore->GetChat()->AddMessage("Tick");
	while(ShouldStop == true)
	{
		//MessageBox(NULL,"Game::Tick,","asi",MB_OK);
		IsStopped = true;
		Sleep(10);
	}
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
		float angle = -0.5f;
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

//	}
}
void CGame::ChangeSkin(char* skin)
{
	char * actualModel = (char*) (*(DWORD*)(*(DWORD*)((*(DWORD*)((*(DWORD*)0x006F9464)+0xE4))+0x68)+0x154)+0x0);
	char isExact[255];
	sprintf(isExact,"Models\\%s",skin);
	if(strcmp(isExact,actualModel) == 0)
	{
		g_CCore->GetChat()->AddMessage("Skins are same");
		return;
	}
	g_CCore->GetChat()->AddMessage(actualModel);
	g_CCore->GetChat()->AddMessage(isExact);
	char buffer[255];
	sprintf(buffer,skin);
	DWORD funcAdress	= 0x004A7700;
	//IsStopped = false;
	//ShouldStop = true;
	//while(IsStopped == false)
	//{
		//MessageBox(NULL,"este isiel,","asi",MB_OK);
//		Sleep(10);
//	}
	_asm
	{
		pushad
			mov eax, 0x006F9464
			mov eax, [eax]
			//lea eax, [006F9464h]
			lea eax, [eax+0xE4]
			mov eax, [eax]
			mov ecx,eax
			//lea eax, eax+0xE4
			//mov ecx,eax
			push 0
			lea eax,buffer
			push eax
			//mov ecx,[playerBase]
			mov ebx, 0x004A7700
			call ebx
		popad
	}
	//ShouldStop = false;
	//Sleep(100);
}


void CGame::ChangeSkinPed(char* skin, int pedID)
{
	/*
	char * actualModel = (char*)(*(DWORD*)(*(DWORD*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x68) + 0x154) + 0x0);
	char isExact[255];
	sprintf(isExact, "Models\\%s", skin);
	if (strcmp(isExact, actualModel) == 0)
	{
		g_CCore->GetChat()->AddMessage("Skins are same");
		return;
	}
	g_CCore->GetChat()->AddMessage(actualModel);
	g_CCore->GetChat()->AddMessage(isExact);
	char buffer[255];
	sprintf(buffer, skin);
	*/
	DWORD funcAdress = 0x004A7700;
	//IsStopped = false;
	//ShouldStop = true;
	//while(IsStopped == false)
	//{
	//MessageBox(NULL,"este isiel,","asi",MB_OK);
	//		Sleep(10);
	//	}
	_asm
	{
		pushad
			mov eax, 0x006560C4
			mov eax, [eax]
			//lea eax, [006F9464h]
			lea eax, [eax + pedID*0x4]
			mov eax, [eax]
			mov ecx, eax
			//lea eax, eax+0xE4
			//mov ecx,eax
			push 0
			lea eax, skin
			push eax
			//mov ecx,[playerBase]
			mov ebx, 0x004A7700
			call ebx
			popad
	}
	//ShouldStop = false;
	//Sleep(100);
}

void CGame::CreateCar()
{
	DWORD caraddr = NULL;
	DWORD addr = NULL;
	char str1[255] = "lhmp_01";
	char str2[255] = "ambulance00.i3d";
	//char str2[] = "Hoolig04.i3d";
	_asm
	{
		pushad
		MOV EAX,DWORD PTR DS:[0x6F9520]            ;  Case 185 of switch 005BB320
		PUSH 9
		PUSH EAX
		MOV ECX,DWORD PTR DS:[EAX]
		CALL DWORD PTR DS:[ECX+0x4C]
		mov addr,eax
		MOV ESI,EAX
		TEST ESI,ESI
		JE badresult_dest
		MOV EDX,DWORD PTR DS:[ESI]
		PUSH 0x0064FF98                       ;  ASCII "cheat_car"
		PUSH ESI
		CALL DWORD PTR DS:[EDX+0x28]
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
		// funkcia vytvorí 3D objekt v nami vyzvorenom I3D_Frame, objekt je ale nefyzický (nemá kolízie ani ïalšie veci)
		TEST EAX,EAX
		JNZ divnyjump_dest
		PUSH 0x21F4
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
		CALL eax
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
		PUSH EAX
		LEA EDX,DWORD PTR SS:[ESP+0x94]
		PUSH ECX
		PUSH EDX
		MOV ECX,ESI
		MOV DWORD PTR SS:[ESP+0x9C],0xc4f7a000	// coordinates x
		MOV DWORD PTR SS:[ESP+0x90],0xc09ccccd	// y
		MOV DWORD PTR SS:[ESP+0xA0],0x41bb3333	// z
		mov eax,0x005C82B0
		CALL eax
		MOV EBX,DWORD PTR DS:[ESI]
		PUSH 0
		PUSH 0
		PUSH 0
		PUSH 0x3f800000
		LEA ECX,DWORD PTR SS:[ESP+0x494]
		mov eax,0x004026C0
		CALL eax
		PUSH EAX
		PUSH ESI
		CALL DWORD PTR DS:[EBX+0xC]
		MOV EAX,DWORD PTR DS:[ESI]
		PUSH ESI
		CALL DWORD PTR DS:[EAX+0x18]
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH ESI
		mov eax,0x004253A0
		CALL eax
		MOV ECX,EAX
		mov eax,0x005C8600
		CALL eax
		MOV EDX,DWORD PTR DS:[EDI]
		PUSH ESI
		MOV ECX,EDI
		CALL DWORD PTR DS:[EDX+0x48]
		TEST AL,AL
		JE SHORT divnyjump_dest
		MOV EAX,DWORD PTR DS:[EDI]
		MOV ECX,EDI
		CALL DWORD PTR DS:[EAX+0x64]
		PUSH 1
		MOV ECX,EDI
		mov eax,0x005C8A90
		CALL eax
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH EDI
		mov eax,0x00425390
		CALL eax
		MOV ECX,EAX
		mov eax,0x005E35D0
		CALL eax
		PUSH 64                                  ; /Arg1 = 00000064
		LEA ECX,DWORD PTR DS:[EDI+0x70]            ; |
		mov eax,0x0051A920
		CALL eax                       ; \Game.0051A920
		PUSH 1
		MOV ECX,EDI
		mov eax,0x005C8740
		CALL eax
	divnyjump_dest:
		MOV ECX,DWORD PTR DS:[ESI]
		PUSH ESI
		CALL DWORD PTR DS:[ECX]
		badresult_dest:
		C:
		popad
	}
	char buff[255];
	sprintf(buff,"Adresa auta: %x %x", caraddr,addr);
	g_CCore->GetChat()->AddMessage(buff);
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
	PUSH 0x3F800000
	PUSH ESI
	CALL DWORD PTR DS:[EDX+0x68]
	MOV EAX,DWORD PTR DS:[ESI]
	PUSH 0x3F800000
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

void CGame::CreatePED()
{
	DWORD gameobject = NULL;			// gameobject
	DWORD frame = NULL;				// frame
	char str1[255] = "lhmp_01";
	char str2[255] = "Tommy.i3d";
	//char str2[] = "Hoolig04.i3d";
	_asm
	{
		pushad
		SUB ESP,124
		// core
		PUSH 0x2
		MOV BYTE PTR DS:[0x671EE8],0x1
		mov eax, 0x005FED70
		CALL eax								;	005FED70	creates a game object, rtn its address as EAX
		MOV DWORD PTR SS:[ESP+0x2C],EAX           ;	store EAX
		// this code makes a engine frame
		MOV EAX,DWORD PTR DS:[0x6F9520]			;	god know, maybe engine class instance
		PUSH 0x9
		PUSH EAX
		MOV ECX,DWORD PTR DS:[EAX]
		MOV DWORD PTR SS:[ESP+0x38],EBX
		CALL DWORD PTR DS:[ECX+0x4C]
		lea EDI, str1
		PUSH EDI								; frame name
		MOV EDI,EAX
		//PUSH Game.0064D94C                       ;  ASCII "Tommy"
		PUSH EDI
		MOV EDX,DWORD PTR DS:[EDI]
		CALL DWORD PTR DS:[EDX+0x28]			; rename our frame by str1 string
		//--------------------------------------
		/*
		MOV EAX,DWORD PTR SS:[ESP+0x14]            ;  mozno zacina hracova entita
		MOV EDX,DWORD PTR SS:[ESP+0x10]
		MOV ECX,DWORD PTR SS:[ESP+0x18]
		MOV DWORD PTR SS:[ESP+0x24],EAX
		LEA EAX,DWORD PTR SS:[ESP+0x20]
		MOV DWORD PTR SS:[ESP+0x20],EDX
		MOV EDX,DWORD PTR DS:[ESI]
		PUSH 0
		PUSH EAX
		PUSH ESI
		MOV DWORD PTR SS:[ESP+0x34],ECX
		CALL DWORD PTR DS:[EDX+C]
		MOV EDX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		MOV ECX,DWORD PTR DS:[ESI]
		PUSH 0
		MOV EAX,DWORD PTR DS:[EDX+0x10]
		MOV EDX,DWORD PTR DS:[EAX+0x210]
		PUSH EDX
		PUSH ESI
		CALL DWORD PTR DS:[ECX+0x2C]
		TEST BYTE PTR SS:[EBP+0xAC],20
		JNZ jump1
		MOV ECX,EBP
		CALL <JMP.&ls3df.?UpdateWMatrixProc@I3D_>
	jump1:
		MOV AL,BYTE PTR DS:[EDI+AC]
		TEST AL,AL
		JS SHORT Game.00560CA5
		LEA EAX,DWORD PTR SS:[EBP+40]
		LEA ECX,DWORD PTR DS:[EDI+80]
		MOV EDX,DWORD PTR DS:[EAX]
		MOV DWORD PTR DS:[ECX],EDX
		MOV EDX,DWORD PTR DS:[EAX+4]
		MOV DWORD PTR DS:[ECX+4],EDX
		MOV EAX,DWORD PTR DS:[EAX+8]
		MOV DWORD PTR DS:[ECX+8],EAX
		MOV ECX,DWORD PTR DS:[EDI+AC]
		AND ECX,FFFFFEDF
		OR ECX,40000000
		MOV DWORD PTR DS:[EDI+AC],ECX
	jump2:
		TEST BYTE PTR SS:[EBP+AC],20
		JNZ jump3
		MOV ECX,EBP
		CALL <JMP.&ls3df.?UpdateWMatrixProc@I3D_>
	jump3:
		MOV EAX,DWORD PTR DS:[EBX+4]
		MOV ECX,DWORD PTR DS:[EBX+8]
		MOV DWORD PTR SS:[ESP+14],EAX
		MOV EDX,DWORD PTR DS:[EBX]
		FLD DWORD PTR SS:[ESP+14]
		FMUL DWORD PTR SS:[ESP+14]
		MOV DWORD PTR SS:[ESP+18],ECX
		MOV DWORD PTR SS:[ESP+10],EDX
		FLD DWORD PTR SS:[ESP+18]
		FMUL DWORD PTR SS:[ESP+18]
		FADDP ST(1),ST
		FLD DWORD PTR SS:[ESP+10]
		FMUL DWORD PTR SS:[ESP+10]
		FADDP ST(1),ST
		FST DWORD PTR SS:[ESP+1C]
		FSUB DWORD PTR DS:[63B2BC]
		FABS
		FCOMP QWORD PTR DS:[63B2E0]
		FSTSW AX
		TEST AH,5
		JPO jump4
		FLD DWORD PTR SS:[ESP+1C]
		FCOMP DWORD PTR DS:[63B2F8]
		FSTSW AX
		TEST AH,5
		JPE jump5
		FLD DWORD PTR SS:[ESP+10]
		FCOMP DWORD PTR DS:[63B2A8]
		FSTSW AX
		TEST AH,44
		JPO SHORT jump6
		FLD DWORD PTR SS:[ESP+10]
		FCOMP DWORD PTR DS:[63B2A8]
		FSTSW AX
		TEST AH,5
		JPE SHORT jump7
		MOV DWORD PTR SS:[ESP+10],BF800000
		JMP jump4
		jump7:
		MOV DWORD PTR SS:[ESP+10],3F800000
		JMP jump4
		jump6:
		FLD DWORD PTR SS:[ESP+18]
		FCOMP DWORD PTR DS:[63B2A8]
		FSTSW AX
		TEST AH,44
		JPO SHORT jump9
		FLD DWORD PTR SS:[ESP+18]
		FCOMP DWORD PTR DS:[63B2A8]
		FSTSW AX
		TEST AH,5
		JPE SHORT jump10
		FLD DWORD PTR DS:[63B2FC]
		FSTP DWORD PTR SS:[ESP+18]
		JMP SHORT jump4
		jump10:
		FLD DWORD PTR DS:[63B2BC]
		FSTP DWORD PTR SS:[ESP+18]
		JMP SHORT jump4
		jump9:
		FLD DWORD PTR SS:[ESP+14]
		FCOMP DWORD PTR DS:[63B2A8]
		FSTSW AX
		TEST AH,5
		JPE SHORT jump11
		FLD DWORD PTR DS:[63B2FC]
		FSTP DWORD PTR SS:[ESP+14]
		JMP SHORT jump4
		jump11:
		FLD DWORD PTR DS:[63B2BC]
		FSTP DWORD PTR SS:[ESP+14]
		JMP SHORT jump4
		jump5:
		FLD DWORD PTR SS:[ESP+1C]
		FSQRT
		FDIVR DWORD PTR DS:[63B2BC]
		FLD DWORD PTR SS:[ESP+10]
		FMUL ST,ST(1)
		FSTP DWORD PTR SS:[ESP+10]
		FLD DWORD PTR SS:[ESP+14]
		FMUL ST,ST(1)
		FSTP DWORD PTR SS:[ESP+14]
		FLD DWORD PTR SS:[ESP+18]
		FMUL ST,ST(1)
		FSTP DWORD PTR SS:[ESP+18]
		FSTP ST
jump4:
		*/
		MOV EAX,DWORD PTR SS:[ESP+0x14]
		MOV EDX,DWORD PTR SS:[ESP+0x10]
		MOV ECX,DWORD PTR SS:[ESP+0x18]
		MOV DWORD PTR SS:[ESP+0x24],EAX
		LEA EAX,DWORD PTR SS:[ESP+0x20]
		MOV DWORD PTR SS:[ESP+0x20],EDX
		MOV EDX,DWORD PTR DS:[EDI]
		PUSH 0
		PUSH EAX
		PUSH EDI
		MOV DWORD PTR SS:[ESP+0x34],ECX
		CALL DWORD PTR DS:[EDX+0xC]
		MOV EDX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		MOV ECX,DWORD PTR DS:[EDI]
		PUSH 0
		MOV EAX,DWORD PTR DS:[EDX+0x10]
		MOV EDX,DWORD PTR DS:[EAX+0x210]
		PUSH EDX
		PUSH EDI
		CALL DWORD PTR DS:[ECX+0x2C]
		MOV EBX,DWORD PTR SS:[ESP+0x30]
		MOV EAX,DWORD PTR DS:[ESI]
		PUSH ESI
		CALL DWORD PTR DS:[EAX+0x18]
		MOV ECX,DWORD PTR DS:[EDI]
		PUSH EDI
		CALL DWORD PTR DS:[ECX+0x18]
		MOV EDX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH ESI
		MOV EAX,DWORD PTR DS:[EDX+0x10]
		PUSH EAX
		MOV ECX,DWORD PTR DS:[EAX]
		CALL DWORD PTR DS:[ECX+0x5C]
		MOV EDX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH EDI
		MOV EAX,DWORD PTR DS:[EDX+0x10]
		PUSH EAX
		MOV ECX,DWORD PTR DS:[EAX]
		CALL DWORD PTR DS:[ECX+0x5C]
		MOV EDX,DWORD PTR DS:[ESI]
		PUSH ESI
		CALL DWORD PTR DS:[EDX]
		MOV EAX,DWORD PTR DS:[EDI]
		PUSH EDI
		CALL DWORD PTR DS:[EAX]
		MOV EDX,DWORD PTR DS:[EBX]
		PUSH ESI
		MOV ECX,EBX
		CALL DWORD PTR DS:[EDX+0x48]
		MOV EAX,DWORD PTR DS:[0x671ED4]
		TEST AH,1
		JE here
		MOV DL,1
		MOV ECX,ESI
		mov eax,0x00472DE0
		CALL eax					;	Game.00472DE0
		here:

		//--------------------------------------
		PUSH 0                                   ; /Arg6 = 00000000
		PUSH 0                                   ; |Arg5 = 00000000
		PUSH 0                                   ; |Arg4 = 00000000
		PUSH 0                                   ; |Arg3 = 00000000
		PUSH 0x0064B730							 ; |Arg2 = 0064B730 ASCII "Tommy.i3d"
		PUSH EDI                                 ; |Arg1
		MOV ECX,0x06F9418						 ; |
		mov eax, 0x00448940
		CALL eax								 ;	Game.00448940                       ; \game.00448940
		// nacita graficky objekt do frame-u
		MOV ESI,DWORD PTR SS:[ESP+0x2C]
		PUSH EDI
		MOV ECX,ESI
		MOV EAX,DWORD PTR DS:[ESI]
		CALL DWORD PTR DS:[EAX+0x48]
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH 0
		PUSH EBX
		mov eax,0x005ff400
		CALL eax								;	Game.005FF400
		MOV ECX,DWORD PTR DS:[0x65115C]         ;	Game.006F9440
		PUSH 0
		PUSH ESI
		mov eax,0x005ff400
		CALL eax								;	Game.005FF400






		// core end
		ADD ESP,124
		popad
	}
	/*
	_asm
	{
		pushad
		MOV EAX,DWORD PTR DS:[0x6F9520]            ;  Case 185 of switch 005BB320
		PUSH 9
		PUSH EAX
		MOV ECX,DWORD PTR DS:[EAX]
		CALL DWORD PTR DS:[ECX+0x4C]
		mov frame,eax
		MOV ESI,EAX
		TEST ESI,ESI
		JE badresult_dest
		lea edx, str1
		PUSH edx									; frame name
		MOV EDX,DWORD PTR DS:[ESI]
		//PUSH 0x0064FF98                       ;  ASCII "cheat_car"
		PUSH ESI
		CALL DWORD PTR DS:[EDX+0x28]
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
		// funkcia vytvorí 3D objekt v nami vyzvorenom I3D_Frame, objekt je ale nefyzický (nemá kolízie ani ïalšie veci)
		TEST EAX,EAX
		JNZ divnyjump_dest
		PUSH 0x0B00								//	vytvori miesto pre herný objekt
		mov eax,0x00624934
		CALL eax
		ADD ESP,4
		mov caraddr,eax
		MOV DWORD PTR SS:[ESP+0x18],EAX
		TEST EAX,EAX
		MOV DWORD PTR SS:[ESP+0x7FC],0x0A
		JE SHORT zlyjump_dest

		// frame adresa
		mov edi, frame							// adresa ku game objectu
		mov esi, [caraddr]
		// wtf
		// PED nehotove
		
		PUSH EDI								; PED frame
		MOV ECX,ESI								// esi - je to handle pointeru na adrese herneho objektu
		MOV EAX,DWORD PTR DS:[ESI]
		CALL DWORD PTR DS:[EAX+0x48]


		// PED Code start
		MOV ECX,DWORD PTR DS:[0x65115C]				;	Game.006F9440
		PUSH 0
		mov ebx, frame
		PUSH EBX									;	ped frame
		mov eax,0x005FF400
		CALL eax									;	Game.005FF400
		MOV ECX,DWORD PTR DS:[0x65115C]				;	Game.006F9440
		PUSH 0
		mov esi, caraddr
		PUSH ESI									;	ped wtf (10BAF4C0) - to isté èo hore
		mov eax,0x005FF400
		CALL eax									;	Game.005FF400

		//mov ESP, caraddr
		/*MOV DWORD PTR SS:[ESP+0x24],0xc4f7a000	// coordinates x
		MOV DWORD PTR SS:[ESP+0x28],0xc09ccccd	// y
		MOV DWORD PTR SS:[ESP+0x2C],0x41bb3333	// z
		*/
		/*zlyjump_dest:		
		badresult_dest:
		divnyjump_dest:
		*/
		
	/*

		MOV ECX,EAX
		mov eax,0x00462AF0
		CALL eax
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
		PUSH EAX
		LEA EDX,DWORD PTR SS:[ESP+0x94]
		PUSH ECX
		PUSH EDX
		MOV ECX,ESI
		MOV DWORD PTR SS:[ESP+0x9C],0xc4f7a000	// coordinates x
		MOV DWORD PTR SS:[ESP+0x90],0xc09ccccd	// y
		MOV DWORD PTR SS:[ESP+0xA0],0x41bb3333	// z
		mov eax,0x005C82B0
		CALL eax
		MOV EBX,DWORD PTR DS:[ESI]
		PUSH 0
		PUSH 0
		PUSH 0
		PUSH 0x3f800000
		LEA ECX,DWORD PTR SS:[ESP+0x494]
		mov eax,0x004026C0
		CALL eax
		PUSH EAX
		PUSH ESI
		CALL DWORD PTR DS:[EBX+0xC]
		MOV EAX,DWORD PTR DS:[ESI]
		PUSH ESI
		CALL DWORD PTR DS:[EAX+0x18]
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH ESI
		mov eax,0x004253A0
		CALL eax
		MOV ECX,EAX
		mov eax,0x005C8600
		CALL eax
		MOV EDX,DWORD PTR DS:[EDI]
		PUSH ESI
		MOV ECX,EDI
		CALL DWORD PTR DS:[EDX+0x48]
		TEST AL,AL
		JE SHORT divnyjump_dest
		MOV EAX,DWORD PTR DS:[EDI]
		MOV ECX,EDI
		CALL DWORD PTR DS:[EAX+0x64]
		PUSH 1
		MOV ECX,EDI
		mov eax,0x005C8A90
		CALL eax
		MOV ECX,DWORD PTR DS:[0x65115C]            ;  Game.006F9440
		PUSH EDI
		mov eax,0x00425390
		CALL eax
		MOV ECX,EAX
		mov eax,0x005E35D0
		CALL eax
		PUSH 64                                  ; /Arg1 = 00000064
		LEA ECX,DWORD PTR DS:[EDI+0x70]            ; |
		mov eax,0x0051A920
		CALL eax                       ; \Game.0051A920
		PUSH 1
		MOV ECX,EDI
		mov eax,0x005C8740
		CALL eax
	divnyjump_dest:
		MOV ECX,DWORD PTR DS:[ESI]
		PUSH ESI
		CALL DWORD PTR DS:[ECX]
		badresult_dest:
		C:
		popad
		
	}*/
	char buff[255];
	sprintf(buff,"Adresa PEda: %x %x", gameobject,frame);
	g_CCore->GetChat()->AddMessage(buff);
}
