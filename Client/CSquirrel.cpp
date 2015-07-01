#include "CCore.h"
#include "CSquirrel.h"

extern CCore* g_CCore;

#ifdef _MSC_VER
#pragma comment (lib ,"squirrel.lib")
#pragma comment (lib ,"sqstdlib.lib")
#endif


CSquirrel::CSquirrel()
{
	this->p_VM = NULL;

	// NULL all pointers
	for (int i = 0; i < 100; i++)
		this->p_scriptPool[i] = NULL;
	this->blockInput = false;
	this->hideChat = false;
}

// default script printf function - goes to log
void printfunc(HSQUIRRELVM v, const SQChar *s, ...)
{
	char endbuff[500] = "MP  ";
	va_list args;
	va_start(args, s);
	vsprintf(endbuff+4, s, args);
	va_end(args);
	//g_CCore->GetLog()->AddLog(endbuff);
	g_CCore->GetChat()->AddMessage(endbuff);
}

static void squirrel_stack_trace(HSQUIRRELVM sqvm)
{
	SQStackInfos stack_info;
	SQInteger stack_depth = 1;

	printfunc(sqvm, "Stack Dump:\n");
	while (SQ_SUCCEEDED(sq_stackinfos(sqvm, stack_depth, &stack_info))) {
		const SQChar *func_name = (stack_info.funcname) ?
			stack_info.funcname : "unknown_function";
		const SQChar *source_file = (stack_info.source) ?
			stack_info.source : "unknown_source_file";
		printfunc(sqvm, "[%d]: function [%s()] %s line [%d]\n",
			stack_depth, func_name, source_file, stack_info.line);
		stack_depth++;
	}
}

// NEW
static SQInteger squirrel_runtime_error(HSQUIRRELVM sqvm)
{
	const SQChar* error_message = NULL;
	if (sq_gettop(sqvm) >= 1) {
		if (SQ_SUCCEEDED(sq_getstring(sqvm, 2, &error_message))) {
			char buff[500];
			sprintf(buff, "sqvm: %s.\n", error_message);
			g_CCore->GetChat()->AddMessage(buff);
		}
		squirrel_stack_trace(sqvm);
	}
	return 0;
}

// NEW!
static void squirrel_compile_error(HSQUIRRELVM sqvm, const SQChar* description,
	const SQChar* file, SQInteger line, SQInteger column)
{
	char endbuff[500];
	sprintf(endbuff, "sqvm: '%s' (Ln:%d Col:%d) : %s.\n", file, line, column, description);
	g_CCore->GetChat()->AddMessage(endbuff);
}

static void squirrel_print_function(HSQUIRRELVM sqvm, const SQChar *format, ...)
{
	char endbuff[500];
	va_list args;
	va_start(args, format);
	//vfprintf(stdout, format, args);
	//printfunc(sqvm, format, args);
	//g_CCore->GetChat()->AddMessage("PRINTF");
	
	vsprintf(endbuff, format, args);
	g_CCore->GetChat()->AddMessage(endbuff);
	va_end(args);
}

// NEW!
static void squirrel_error_function(HSQUIRRELVM sqvm, const SQChar *format, ...)
{
	char endbuff[500];
	va_list args;
	va_start(args, format);
	vsprintf(endbuff, format, args);
	g_CCore->GetChat()->AddMessage(endbuff);
	va_end(args);
	
}

void CSquirrel::LoadClientScript(char* scriptname)
{
	g_CCore->GetChat()->AddMessage(scriptname);
	// Need to get the correct filename as all client files 
	// from server are stored with their MD5 checksum as name
	char* realName = g_CCore->GetFileSystem()->GetFileAliasFromName(scriptname);

	char szScriptPath[512];
	sprintf(szScriptPath, "lhmp/files/%s", realName);

	char buff[300];
	sprintf(buff, "Script: %s", szScriptPath);
	g_CCore->GetChat()->AddMessage(buff);

	g_CCore->GetChat()->AddMessage("Adding script 1...");
	HSQUIRRELVM sqvm = sq_open(1024);
	SQVM * pVM = sqvm;

	sq_setprintfunc(sqvm, squirrel_print_function, /* NEW */ squirrel_error_function /* NEW */);
	sq_setcompilererrorhandler(sqvm, squirrel_compile_error); // NEW!
	sq_newclosure(sqvm, squirrel_runtime_error, 0); // NEW!
	sq_seterrorhandler(sqvm); // NEW!

	//const SQChar *program = "::print(\"Hello World, I'm back!\\n\");::print(\"Fuck this\");";
	const SQChar *program = "function onRender(){ drawText(\"This text is rendered with Squirrel :)\", 300, 300, COLOR_RED);}";
	//const SQChar *program = "function onRender(){ ::print(\"This text is rendered with Squirrel :)\");}";
	/*if (SQ_FAILED(sq_compilebuffer(sqvm, program,
		sizeof(SQChar)* strlen(program),
		"program", SQTrue))) {
		return;
	}*/
	

	sq_pushroottable(sqvm);

	/* ------------------ Additional libs */
	
	sqstd_register_stringlib(sqvm);
	sqstd_register_mathlib(sqvm);
	sqstd_register_systemlib(sqvm);
	sqstd_register_iolib(sqvm);
	
	/* ------------------ Register our natives */
	this->PrepareMachine(sqvm);

	/*if (SQ_FAILED(sq_call(sqvm, 1, SQFalse, SQTrue))) {

		g_CCore->GetChat()->AddMessage("Compile failed");
		return;
	}*/
	if (SQ_FAILED(sqstd_dofile(pVM, szScriptPath, SQFalse, SQTrue))) {
		// script compilation failed
		g_CCore->GetChat()->AddMessage("Compile failed");
		return;
	}

	g_CCore->GetChat()->AddMessage("Add script");
	// add script into pool
	//this->p_scriptPool.push_back(CScript(pVM));

	for (int i = 0; i < 100; i++)
	{
		if (this->p_scriptPool[i] == NULL)
		{
			g_CCore->GetChat()->AddMessage("#00adefSlot founded !");
			this->p_scriptPool[i] = new CScript(pVM, scriptname);
			break;
		}
	}
	g_CCore->GetChat()->AddMessage("#ff0000Script loaded !");

	this->onSpawn();

	/*char buff[100];
	sprintf(buff, "Origi %p", pVM);
	g_CCore->GetLog()->AddLog(buff);
	//sq_close(sqvm);
	this->p_VM = sqvm;
	//------------------
	/*int iTop = sq_gettop(this->p_VM);
	sq_pushroottable(this->p_VM);

	// Push the function name onto the stack
	g_CCore->GetLog()->AddLog("rendering test...");
	sq_pushstring(this->p_VM, "onRender", -1);
	// Get the closure for the function
	if (SQ_SUCCEEDED(sq_get(this->p_VM, -2))) {
		// Push the root table onto the stack
		sq_pushroottable(this->p_VM);
		sq_call(this->p_VM, 1, true, true);
	}
	sq_settop(this->p_VM, iTop);*/

	return;
}

/*---
void CSquirrel::LoadClientScript(char* scriptname)
{
	g_CCore->GetChat()->AddMessage(scriptname);
	// Need to get the correct filename as all client files 
	// from server are stored with their MD5 checksum as name
	char* realName = g_CCore->GetFileSystem()->GetFileAliasFromName(scriptname);
	
	char szScriptPath[512];
	sprintf(szScriptPath, "lhmp/files/%s", realName);

	g_CCore->GetChat()->AddMessage(szScriptPath);

	// make sure the script exists
	FILE * fp = fopen(szScriptPath, "rb");
	if (!fp) {
		g_CCore->GetChat()->AddMessage("[Err] Failed to run script: SCRIPT_NOT_FOUND");
		return;
	}
	fclose(fp);
	// create the squirrel VM with an initial stack size of 1024 bytes
	HSQUIRRELVM hVM=  sq_open(1024);

	// get the script vm pointer
	SQVM * pVM = hVM;

	// register the default error handlers
	//sqstd_seterrorhandlers(pVM);

	// set the print and error functions
	//sq_setprintfunc(pVM, printfunc, printfunc);

	// push the root table onto the stack
	//sq_pushroottable(pVM);

	/* ------------------ Additional libs */
	/*
	sqstd_register_stringlib(pVM);
	sqstd_register_mathlib(pVM);
	sqstd_register_systemlib(pVM);
	*/
	/* ------------------ Register our natives */
	//this->PrepareMachine(pVM);
	
	/*// load and compile the script
	if (SQ_FAILED(sqstd_dofile(pVM, szScriptPath, SQFalse, SQTrue))) {
		// script compilation failed
		g_CCore->GetChat()->AddMessage("[Err] Failed to run script: SQUIRREL_LOADING_FAILED");
		return;
	}
	// pop the root table from the stack
	sq_pop(pVM, 1);

	printfunc(pVM, "Virtual-Machine: %x", pVM);

	// add script into pool
	this->p_scriptPool.push_back(CScript(pVM));

	printfunc(pVM, "Virtual-Machine: %x", this->p_scriptPool.rbegin()->GetVirtualMachine());
	*/
	//--------------------------------------
/*---
	sq_setprintfunc(pVM, printfunc, printfunc);

	const SQChar *program = "print(\"Hi, suckerr\");function onRender(){drawText(\"This text is rendered with Squirrel :)\", 300, 300, COLOR_RED);}\");";

	if (SQ_FAILED(sq_compilebuffer(pVM, program,
		sizeof(SQChar)* strlen(program),
		"program", SQTrue))) {
		return;
	}

	sq_pushroottable(pVM);
	if (SQ_FAILED(sq_call(pVM, 1, SQFalse, SQTrue))) {
		return;
	}


	//--------------------------------------

	// test
	int iTop = sq_gettop(pVM);
	sq_pushroottable(pVM);

	// Push the function name onto the stack
	sq_pushstring(pVM, "onRender", -1);
	// Get the closure for the function
	if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
		// Push the root table onto the stack
		sq_pushroottable(pVM);
		sq_call(pVM, 1, true, true);
	}
	sq_settop(pVM, iTop);
	
	//g_CCore->GetSquirrel()->onRender();

	// script loaded successfully
	return;
}
*/

// Delete all scripts & restore default values
void CSquirrel::DeleteScripts()
{
	for (int i = 0; i < 100; i++)
	{
		// check if script exists
		if (this->p_scriptPool[i] != NULL)
		{
			// delete the instance of script
			delete this->p_scriptPool[i];
			// set pointer to NULL
			this->p_scriptPool[i] = NULL;
		}
	}

	// important
	this->blockInput = false;
	this->hideChat = false;
}


void CSquirrel::BlockInput(bool param)
{
	this->blockInput = param;
}

bool CSquirrel::isInputBlocked()
{
	return this->blockInput;
}


void CSquirrel::HideChat(bool status)
{
	this->hideChat = status;
}
bool CSquirrel::isChatHidden()
{
	return this->hideChat;
}

void CSquirrel::onRender()
{
	//g_CCore->GetChat()->AddMessage("renderSTART");
	/*unsigned int size = this->p_scriptPool.size();
	for (int i = 0; i < size; i++) {
			SQVM * pVM = this->p_scriptPool[i].GetVirtualMachine();
			if (pVM == NULL)
				continue;
			int iTop = sq_gettop(pVM);
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			g_CCore->GetChat()->AddMessage("rendering...");
			sq_pushstring(pVM, "onRender", -1);
			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);
				sq_call(pVM, 1, true, true);
			}
			sq_settop(pVM, iTop);


			char buff[100];
			sprintf(buff, "NEXT: %p", pVM);
			g_CCore->GetLog()->AddLog(buff);
		}*/
		
	/*if (this->p_VM != NULL)
	{
		int iTop = sq_gettop(this->p_VM);
		sq_pushroottable(this->p_VM);

		// Push the function name onto the stack
		g_CCore->GetChat()->AddMessage("rendering...");
		sq_pushstring(this->p_VM, "onRender", -1);
		// Get the closure for the function
		if (SQ_SUCCEEDED(sq_get(this->p_VM, -2))) {
			// Push the root table onto the stack
			sq_pushroottable(this->p_VM);
			sq_call(this->p_VM, 1, true, true);
		}
		sq_settop(this->p_VM, iTop);
	}*/


	for (int i = 0; i < 100; i++) {
		//SQVM * pVM = this->p_scriptPool[i].GetVirtualMachine();
		if (this->p_scriptPool[i] != NULL)
		{
			SQVM * pVM = this->p_scriptPool[i]->GetVirtualMachine();
			if (pVM != NULL)
			{
				int iTop = sq_gettop(pVM);
				sq_pushroottable(pVM);

				// Push the function name onto the stack
				sq_pushstring(pVM, "onRender", -1);
				// Get the closure for the function
				if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
					// Push the root table onto the stack
					sq_pushroottable(pVM);
					sq_call(pVM, 1, true, true);
				}
				sq_settop(pVM, iTop);


				//char buff[100];
				//sprintf(buff, "NEXT: %p", pVM);
				//g_CCore->GetLog()->AddLog(buff);
			}
		}
	}
}


void CSquirrel::onKeyDown(unsigned int key)
{
	for (int i = 0; i < 100; i++) {
		if (this->p_scriptPool[i] != NULL)
		{
			SQVM * pVM = this->p_scriptPool[i]->GetVirtualMachine();
			if (pVM != NULL)
			{
				int iTop = sq_gettop(pVM);
				sq_pushroottable(pVM);

				// Push the function name onto the stack
				sq_pushstring(pVM, "onKeyDown", -1);
				// Get the closure for the function
				if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
					// Push the root table onto the stack
					sq_pushroottable(pVM);
					sq_pushinteger(pVM, key);
					sq_call(pVM, 2, true, true);
				}
				sq_settop(pVM, iTop);
			}
		}
	}
}
void CSquirrel::onSpawn()
{
	for (int i = 0; i < 100; i++) {
		if (this->p_scriptPool[i] != NULL)
		{
			SQVM * pVM = this->p_scriptPool[i]->GetVirtualMachine();
			if (pVM != NULL)
			{
				int iTop = sq_gettop(pVM);
				sq_pushroottable(pVM);

				// Push the function name onto the stack
				sq_pushstring(pVM, "onSpawn", -1);
				// Get the closure for the function
				if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
					// Push the root table onto the stack
					sq_pushroottable(pVM);
					sq_call(pVM, 1, true, true);
				}
				sq_settop(pVM, iTop);
			}
		}
	}
}

void CSquirrel::callClientFunc(char* scriptname, char* scriptfunc, BitStream* message)
{
	char buff[500];
	sprintf(buff, "callClientFunc[%s][%s]", scriptname, scriptfunc);
	g_CCore->GetChat()->AddMessage(buff);

	CScript* pointer = NULL;
	for (int i = 0; i < 100; i++)
	{
		if (this->p_scriptPool[i] != NULL)
		{
			if (strcmp(this->p_scriptPool[i]->GetName(), scriptname) == 0)
			{
				pointer = this->p_scriptPool[i];
				break;
			}
		}
	}

	if (pointer)
	{
		// now get type of param and serialize it
		SQInteger iPar;
		SQFloat fPar;
		char parString[500];
		SQBool	bPar;

		SQObjectType type;
		message->Read(type);

		int len;
		
		int iTop = sq_gettop(pointer->GetVirtualMachine());
		sq_pushroottable(pointer->GetVirtualMachine());

		// Push the function name onto the stack
		sq_pushstring(pointer->GetVirtualMachine(), scriptfunc, -1);
		// Get the closure for the function
		if (SQ_SUCCEEDED(sq_get(pointer->GetVirtualMachine(), -2))) {
			// Push the root table onto the stack
			sq_pushroottable(pointer->GetVirtualMachine());

			switch (type)
			{
			case OT_INTEGER:
				message->Read(iPar);
				sq_pushinteger(pointer->GetVirtualMachine(), iPar);
				break;
			case OT_FLOAT:
				message->Read(fPar);
				sq_pushfloat(pointer->GetVirtualMachine(), fPar);
				break;
			case OT_STRING:
				message->Read(parString);
				len = strlen(parString);
				sq_pushstring(pointer->GetVirtualMachine(), parString, len);
				break;
			case OT_BOOL:
				message->Read(bPar);
				sq_pushbool(pointer->GetVirtualMachine(), bPar);
				break;
			case OT_NULL:
				// if null, no extra data are sent
				sq_pushnull(pointer->GetVirtualMachine());
			default:
			case -1:
				// in case bad type occurs, do nothing
				break;
			}

			sq_call(pointer->GetVirtualMachine(), 2, true, true);
		}
		sq_settop(pointer->GetVirtualMachine(), iTop);
	}
	else {

	}
}

/* ----------------------- Utils -----------------------------------*/

void RegisterFunction(HSQUIRRELVM pVM, char * szFunc, SQFUNCTION func, int params, const char * szTemplate)
{
	sq_pushroottable(pVM);

	sq_pushstring(pVM, szFunc, -1);
	sq_newclosure(pVM, func, 0);
	if (params != -1)
	{
		sq_setparamscheck(pVM, params, szTemplate);
	}
	sq_createslot(pVM, -3);
	sq_pop(pVM, 1); //pops the root table
}

void CSquirrel::RegisterVariable(HSQUIRRELVM pVM, const char * szVarName, bool bValue)
{
	sq_pushstring(pVM, szVarName, -1);
	sq_pushbool(pVM, bValue);
	sq_createslot(pVM, -3);
}

void CSquirrel::RegisterVariable(HSQUIRRELVM pVM, const char * szVarName, int iValue)
{
	sq_pushstring(pVM, szVarName, -1);
	sq_pushinteger(pVM, iValue);
	sq_createslot(pVM, -3);
}

void CSquirrel::RegisterVariable(HSQUIRRELVM pVM, const char * szVarName, float fValue)
{
	sq_pushstring(pVM, szVarName, -1);
	sq_pushfloat(pVM, fValue);
	sq_createslot(pVM, -3);
}

void CSquirrel::RegisterVariable(HSQUIRRELVM pVM, const char * szVarName, const char * szValue)
{
	sq_pushstring(pVM, szVarName, -1);
	sq_pushstring(pVM, szValue, -1);
	sq_createslot(pVM, -3);
}

/*------------------------- Natives ------------------------------- */

// drawText - text, x, y, color
SQInteger sq_drawText(SQVM *vm)
{
	SQInteger	coord_x, coord_y, color;
	const SQChar* text;
	sq_getstring(vm, -4, &text);
	sq_getinteger(vm, -3, &coord_x);
	sq_getinteger(vm, -2, &coord_y);
	sq_getinteger(vm, -1, &color);

	g_CCore->GetGraphics()->GetFont()->DrawTextA((char*)text, coord_x, coord_y, color, true);
	return 1;
}

// fillBox(startX,StartY,width,height,color)
SQInteger sq_fillBox(SQVM *vm)
{
	SQInteger	coord_x, coord_y, width, height, color;
	sq_getinteger(vm, -5, &coord_x);
	sq_getinteger(vm, -4, &coord_y);
	sq_getinteger(vm, -3, &width);
	sq_getinteger(vm, -2, &height);
	sq_getinteger(vm, -1, &color);

	g_CCore->GetGraphics()->FillARGB(coord_x, coord_y, width, height, color);
	return 1;
}

SQInteger sq_getScreenSize(SQVM *vm)
{
	Vector2D screen = g_CCore->GetGraphics()->GetResolution();
	sq_newarray(vm, 0);
	sq_pushinteger(vm, screen.x);
	sq_arrayappend(vm, -2);
	sq_pushinteger(vm, screen.y);
	sq_arrayappend(vm, -2);
	sq_push(vm, -1);

	return 1;
}

SQInteger sq_blockInput(SQVM *vm)
{
	SQBool should;
	sq_getbool(vm, -1, &should);
	g_CCore->GetSquirrel()->BlockInput((should == 1));
	return 1;
}

SQInteger sq_isInputBlocked(SQVM *vm)
{
	SQBool status = g_CCore->GetSquirrel()->isInputBlocked();
	sq_pushbool(vm, status);
	return 1;
}

SQInteger sq_hideChat(SQVM *vm)
{
	SQBool should;
	sq_getbool(vm, -1, &should);
	g_CCore->GetSquirrel()->HideChat((should == 1));
	return 1;
}

SQInteger sq_isChatHidden(SQVM *vm)
{
	SQBool status = g_CCore->GetSquirrel()->isChatHidden();
	sq_pushbool(vm, status);
	return 1;
}

// set camera to look at @lookAt position from @cameraposition
// cameraLookAtFrom(camerapos,lookA)
SQInteger sq_cameraLookAtFrom(SQVM *vm)
{
	Vector3D cameraPos, lookAtPoint;

	sq_getfloat(vm, -1, &lookAtPoint.z);
	sq_getfloat(vm, -2, &lookAtPoint.y);
	sq_getfloat(vm, -3, &lookAtPoint.x);

	sq_getfloat(vm, -4, &cameraPos.z);
	sq_getfloat(vm, -5, &cameraPos.y);
	sq_getfloat(vm, -6, &cameraPos.x);

	// simple way how to calculate LookAt rotation (substraction of positions
	// and dividing by the length of the vector in order to get UNIT circle (radius = 1)

	Vector3D rotation;
	rotation.x = lookAtPoint.x - cameraPos.x;
	rotation.y = lookAtPoint.y - cameraPos.y;
	rotation.z = lookAtPoint.z - cameraPos.z;

	int ratio = (int) sqrtf(rotation.x*rotation.x + rotation.y*rotation.y + rotation.z*rotation.z);

	rotation.x /= ratio;
	rotation.y /= ratio;
	rotation.z /= ratio;

	g_CCore->GetGame()->SetCameraPos(cameraPos,rotation.x,rotation.y,rotation.z, 0.0f);
	return 1;
}

SQInteger sq_restoreCamera(SQVM *vm)
{
	g_CCore->GetGame()->CameraUnlock();
	return 1;
}

// set localplayer position
SQInteger sq_setPosition(SQVM *vm)
{
	Vector3D position;
	sq_getfloat(vm, -1, &position.z);
	sq_getfloat(vm, -2, &position.y);
	sq_getfloat(vm, -3, &position.x);

	// if is on-foot
	if (g_CCore->GetLocalPlayer()->IsOnFoot())
	{
		g_CCore->GetLocalPlayer()->SetLocalPos(position);
	}
	else {
		// TODO
	}
	return 1;
}

SQInteger sq_setRotation(SQVM *vm)
{
	Vector3D rotation;
	sq_getfloat(vm, -1, &rotation.z);
	sq_getfloat(vm, -2, &rotation.y);
	sq_getfloat(vm, -3, &rotation.x);

	// if is on-foot
	if (g_CCore->GetLocalPlayer()->IsOnFoot())
	{
		g_CCore->GetLocalPlayer()->SetLocalRot(rotation);
	}
	else {
		// TODO
	}
	return 1;
}

SQInteger sq_changeSkin(SQVM *vm)
{
	SQInteger skin;
	sq_getinteger(vm, -1, &skin);

	g_CCore->GetLocalPlayer()->SetSkin(skin);
	g_CCore->GetEngineStack()->AddMessage(ES_CHANGESKIN, g_CCore->GetLocalPlayer()->GetOurID());
	return 1;
}

SQInteger sq_callServerFunc(SQVM *vm)
{
	const SQChar* script_name, *func_name;

	sq_getstring(vm, -3, &script_name);
	sq_getstring(vm, -2, &func_name);

	SQObjectType type_of_param = sq_gettype(vm, -1);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_SCRIPT_CALLFUNC);
	bsOut.Write(script_name);
	bsOut.Write(func_name);

	// now get type of param and serialize it
	SQInteger iPar;
	SQFloat fPar;
	const SQChar* stringPar;
	SQBool	bPar;

	bsOut.Write(type_of_param);
	switch (type_of_param)
	{
	case OT_INTEGER:
		sq_getinteger(vm, -1, &iPar);
		bsOut.Write(iPar);
		break;
	case OT_FLOAT:
		sq_getfloat(vm, -1, &fPar);
		bsOut.Write(fPar);
		break;
	case OT_STRING:
		sq_getstring(vm, -1, &stringPar);
		bsOut.Write(stringPar);
		break;
	case OT_BOOL:
		sq_getbool(vm, -1, &bPar);
		bsOut.Write(bPar);
		break;
	case OT_NULL:
		// if null, no extra data are sent
	default:
	case -1:
		// in case bad type occurs, do nothing
		break;
	}
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE);
	return 1;
}

SQInteger sq_createTexture(SQVM *vm)
{
	const SQChar* textureName;
	sq_getstring(vm, -1, &textureName);

	char* realName = g_CCore->GetFileSystem()->GetFileAliasFromName((char*)textureName);

	char szScriptPath[512];
	sprintf(szScriptPath, "lhmp/files/%s", realName);

	CSQImage* image = g_CCore->GetSquirrelImages()->createTexture((char*)szScriptPath);
	/*if (image != NULL)
	{
		// on success, returns the handle to image(texture)
		sq_pushuserpointer(vm, image);
	}
	else {
		sq_pushbool(vm, false);
	}*/
	// on success, it returns handle, otherwise NULL
	sq_pushuserpointer(vm, image);
	return 1;
}

SQInteger sq_getTextureSize(SQVM *vm)
{
	CSQImage* image;
	sq_getuserpointer(vm, -1, (SQUserPointer*)&image);

	D3DSURFACE_DESC descript;
	image->GetTexture()->GetLevelDesc(0, &descript);
	
	sq_newarray(vm, 0);
	sq_pushinteger(vm, descript.Width);
	sq_arrayappend(vm, -2);
	sq_pushinteger(vm, descript.Height);
	sq_arrayappend(vm, -2);
	sq_push(vm, -1);

	return 1;
}

SQInteger sq_drawTexture(SQVM* vm)
{
	CSQImage*		image;
	D3DXVECTOR2		position;
	D3DXVECTOR2		scaling;

	sq_getuserpointer(vm, -5, (SQUserPointer*) &image);
	if (image != NULL)
	{
		sq_getfloat(vm, -4, &position.x);
		sq_getfloat(vm, -3, &position.y);
		sq_getfloat(vm, -2, &scaling.x);
		sq_getfloat(vm, -1, &scaling.y);

		g_CCore->GetGraphics()->GetSprite()->Draw(image->GetTexture(), NULL,&scaling,NULL,0.0f, &position, 0xFFFFFFFF);
	}
	else {
		g_CCore->GetChat()->AddMessage("[Err] drawImage - image doesn't exists");
	}
	return 1;
}

SQInteger sq_getPosition(SQVM *vm) {
	Vector3D pos = g_CCore->GetLocalPlayer()->GetLocalPos();
	sq_newarray(vm, 0);
	sq_pushfloat(vm, pos.x);
	sq_arrayappend(vm, -2);
	sq_pushfloat(vm, pos.y);
	sq_arrayappend(vm, -2);
	sq_pushfloat(vm, pos.z);
	sq_arrayappend(vm, -2);
	sq_push(vm, -1);

	return 1;
}

SQInteger sq_getRotation(SQVM *vm) {
	Vector3D pos = g_CCore->GetLocalPlayer()->GetLocalRot();
	sq_newarray(vm, 0);
	sq_pushfloat(vm, pos.x);
	sq_arrayappend(vm, -2);
	sq_pushfloat(vm, pos.y);
	sq_arrayappend(vm, -2);
	sq_pushfloat(vm, pos.z);
	sq_arrayappend(vm, -2);
	sq_push(vm, -1);

	return 1;
}

SQInteger sq_getDistanceBetween3DPoints(SQVM *vm) {
	Vector3D pointA, pointB;
	sq_getfloat(vm, -6, &pointA.x);
	sq_getfloat(vm, -5, &pointA.y);
	sq_getfloat(vm, -4, &pointA.z);
	sq_getfloat(vm, -3, &pointB.x);
	sq_getfloat(vm, -2, &pointB.y);
	sq_getfloat(vm, -1, &pointB.z);

	float c = (pointA.x - pointB.x)*(pointA.x - pointB.x) + (pointA.y - pointB.x)*(pointA.y - pointB.y)
		+ (pointA.z - pointB.z)*(pointA.z - pointB.z);

	c = sqrt(c);

	if (c != c) c = 0;

	sq_pushfloat(vm, c);
	return 1;
}

SQInteger sq_getDistanceBetween2DPoints(SQVM *vm) {
	float pointAx, pointAy, pointBx, pointBy;
	sq_getfloat(vm, -6, &pointAx);
	sq_getfloat(vm, -5, &pointAy);
	sq_getfloat(vm, -3, &pointBx);
	sq_getfloat(vm, -2, &pointBy);

	float c = (pointAx - pointBx)*(pointAx - pointBx) + (pointAy - pointBx)*(pointAy - pointBy);

	c = sqrt(c);

	if (c != c) c = 0;

	sq_pushfloat(vm, c);
	return 1;
}


SQInteger sq_drawTextureEx(SQVM* vm)
{
	CSQImage*		image;
	D3DXVECTOR2		position;
	D3DXVECTOR2		scaling;
	float			rotation;
	RECT			sourceRect;


	sq_getuserpointer(vm, -10, (SQUserPointer*)&image);
	if (image != NULL)
	{
		sq_getfloat(vm, -9, &position.x);
		sq_getfloat(vm, -8, &position.y);
		sq_getfloat(vm, -7, &scaling.x);
		sq_getfloat(vm, -6, &scaling.y);

		sq_getinteger(vm, -5, (int*)&sourceRect.left);
		sq_getinteger(vm, -4, (int*)&sourceRect.top);
		sq_getinteger(vm, -3, (int*)&sourceRect.right);
		sq_getinteger(vm, -2, (int*)&sourceRect.bottom);

		sq_getfloat(vm, -1, &rotation);

		g_CCore->GetGraphics()->GetSprite()->Draw(image->GetTexture(), &sourceRect, &scaling, NULL, rotation, &position, 0xFFFFFFFF);
	}
	else {
		g_CCore->GetChat()->AddMessage("[Err] drawImageEx - image doesn't exists");
	}
	return 1;
}

SQInteger sq_createFont(SQVM *vm)
{
	const SQChar* name_font;
	int size;
	sq_getstring(vm, -2, &name_font);
	sq_getinteger(vm, -1, &size);

	CSQFont* font = g_CCore->GetSquirrelFonts()->createFont((char*)name_font,size);
	
	char buff[500];
	sprintf(buff, "createFont: %p", font);
	g_CCore->GetChat()->AddMessage(buff);
	sq_pushuserpointer(vm, font);
	return 1;
}

SQInteger sq_drawFontText(SQVM *vm)
{
	const SQChar* text;
	int x, y;
	CSQFont* font;
	int color;
	sq_getstring(vm, -5, &text);
	sq_getinteger(vm, -4, &x);
	sq_getinteger(vm, -3, &y);
	sq_getinteger(vm, -2, &color);
	sq_getuserpointer(vm, -1,(SQUserPointer*) &font);

	if (font)
	{
		font->GetFont()->DrawTextA((char*)text, x, y, color);
	}
	return 1;
}

SQInteger sq_getFontWidthOfText(SQVM *vm)
{
	const SQChar* text;
	CSQFont* font;
	sq_getstring(vm, -2, &text);
	sq_getuserpointer(vm, -1, (SQUserPointer*)&font);

	if (font)
	{
		if (font->GetFont())
		{
			SIZE size = font->GetFont()->GetFontWidth((char*)text);
			sq_newarray(vm, 0);
			sq_pushinteger(vm, size.cx);
			sq_arrayappend(vm, -2);
			sq_pushinteger(vm, size.cy);
			sq_arrayappend(vm, -2);
			sq_push(vm, -1);
			return 1;
		}
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_ColorRGB(SQVM *vm)
{
	int r, g, b;
	sq_getinteger(vm, -3, &r);
	sq_getinteger(vm, -2, &g);
	sq_getinteger(vm, -1, &b);

	Tools::Clamp(r, 0, 256);
	Tools::Clamp(g, 0, 256);
	Tools::Clamp(b, 0, 256);

	DWORD color = 0xFF000000;

	// push color fragmets into one 4-byte value (using bitwise OR)
	DWORD helpVar = (r << 16);
	color |= helpVar;
	helpVar = (g << 8);
	color |= helpVar;
	helpVar = (b);
	color |= helpVar;
	// return color
	sq_pushinteger(vm, color);
	return 1;
}
SQInteger sq_ColorARGB(SQVM *vm)
{
	int a,r, g, b;

	sq_getinteger(vm, -4, &r);
	sq_getinteger(vm, -3, &r);
	sq_getinteger(vm, -2, &g);
	sq_getinteger(vm, -1, &b);


	Tools::Clamp(a, 0, 255);
	Tools::Clamp(r, 0, 255);
	Tools::Clamp(g, 0, 255);
	Tools::Clamp(b, 0, 255);

	DWORD color = 0x0;

	// push color fragmets into one 4-byte value (using bitwise OR)
	DWORD helpVar = (a << 24);
	color |= helpVar;
	helpVar = (r << 16);
	color |= helpVar;
	helpVar = (g << 8);
	color |= helpVar;
	helpVar = (b);
	color |= helpVar;
	// return color
	sq_pushinteger(vm, color);
	return 1;
}

SQInteger sq_playAnimation(SQVM *vm) 
{
	SQInteger anim;
	sq_getinteger(vm, -1, &anim);

	g_CCore->GetLocalPlayer()->ourAnim = anim;
	g_CCore->GetEngineStack()->AddMessage(ES_PLAYANIM, anim);
	return 1;
}


//Sound Functions
SQInteger sq_Play3DSound(SQVM* vm)
{

	Vector3D position;
	const SQChar* soundname;
	SQFloat distance, volume;

	sq_getfloat(vm, -1, &position.z);
	sq_getfloat(vm, -2, &position.y);
	sq_getfloat(vm, -3, &position.x);
	sq_getfloat(vm, -4, &volume);
	sq_getfloat(vm, -5, &distance);
	sq_getstring(vm, -6, &soundname);

	DWORD createdFrame = g_CCore->GetGame()->CreateEmptyFrame();
	if (createdFrame != NULL)
	{
		g_CCore->GetGame()->SetFramePos(createdFrame, position.x, position.y, position.z);
		DWORD soundID = g_CCore->GetGame()->PlaySoundAtFrame(createdFrame, (char*)soundname, volume, distance);
		sq_pushinteger(vm, (int)soundID);
		return 1;

	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_Remove3DSound(SQVM* vm)
{
	SQInteger soundid;
	sq_getinteger(vm, -1, &soundid);

	g_CCore->GetGame()->StopSound((DWORD)soundid);
	sq_pushnull(vm);
	return 1;
}

//Particles Functions
SQInteger sq_CreateParticle(SQVM* vm)
{

	Vector3D position;
	SQInteger particleid;


	sq_getfloat(vm, -1, &position.z);
	sq_getfloat(vm, -2, &position.y);
	sq_getfloat(vm, -3, &position.x);
	sq_getinteger(vm, -4, &particleid);

	DWORD createdFrame = g_CCore->GetGame()->CreateEmptyFrame();
	if (createdFrame != NULL)
	{
		g_CCore->GetGame()->SetFramePos(createdFrame, position.x, position.y, position.z);
		DWORD particleID = g_CCore->GetGame()->CreateParticle(createdFrame, particleid);
		sq_pushinteger(vm, (int)particleID);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_RemoveParticle(SQVM* vm)
{
	SQInteger particleid;
	sq_getinteger(vm, -1, &particleid);

	g_CCore->GetGame()->RemoveParticle((DWORD)particleid);
	sq_pushnull(vm);
	return 1;
}


//Money Functions
SQInteger sq_GetMoney(SQVM* vm)
{
	int playerMoney = g_CCore->GetLocalPlayer()->GetMoney();
	sq_pushinteger(vm, playerMoney);
	return 1;
}

SQInteger sq_SetMoney(SQVM* vm)
{
	SQInteger money;
	sq_getinteger(vm, -1, &money);
	g_CCore->GetLocalPlayer()->SetMoney(money);
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_EnableMoney(SQVM* vm)
{
	SQInteger enable;
	sq_getinteger(vm, -1, &enable);
	g_CCore->GetLocalPlayer()->EnableMoney(enable);
	sq_pushnull(vm);
	return 1;
}

/*------------------------- /Natives ------------------------------- */
// Register all native functions and constants
void CSquirrel::PrepareMachine(SQVM* pVM)
{
	// TODO - put here all natives - functions, consts

	// consts

	RegisterVariable(pVM, "COLOR_WHITE", (int)0xFFFFFFFF);
	RegisterVariable(pVM, "COLOR_RED", (int)0xFFFF0000);
	RegisterVariable(pVM, "COLOR_GREEN", (int)0xFF00FF00);
	RegisterVariable(pVM, "COLOR_BLUE", (int)0xFF0000FF);
	RegisterVariable(pVM, "COLOR_BLACK", (int)0xFF000000);
	RegisterVariable(pVM, "COLOR_GRAY", (int)0xFF555555);


	// Virtual codes - for key strokes

	RegisterVariable(pVM, "VK_BACK", (int)0x08);
	RegisterVariable(pVM, "VK_TAB", (int)0x09);
	RegisterVariable(pVM, "VK_RETURN", (int)0x0D);
	RegisterVariable(pVM, "VK_SPACE", (int)0x20);
	RegisterVariable(pVM, "VK_LEFT", (int)0x25);
	RegisterVariable(pVM, "VK_RIGHT", (int)0x27);
	RegisterVariable(pVM, "VK_UP", (int)0x26);
	RegisterVariable(pVM, "VK_DOWN", (int)0x28);
	RegisterVariable(pVM, "VK_NUM0", (int)0x30);
	RegisterVariable(pVM, "VK_NUM1", (int)0x31);
	RegisterVariable(pVM, "VK_NUM2", (int)0x32);
	RegisterVariable(pVM, "VK_NUM3", (int)0x33);
	RegisterVariable(pVM, "VK_NUM4", (int)0x34);
	RegisterVariable(pVM, "VK_NUM5", (int)0x35);
	RegisterVariable(pVM, "VK_NUM6", (int)0x36);
	RegisterVariable(pVM, "VK_NUM7", (int)0x37);
	RegisterVariable(pVM, "VK_NUM8", (int)0x38);
	RegisterVariable(pVM, "VK_NUM9", (int)0x39);
	RegisterVariable(pVM, "VK_A", (int)0x41);
	RegisterVariable(pVM, "VK_B", (int)0x42);
	RegisterVariable(pVM, "VK_C", (int)0x43);
	RegisterVariable(pVM, "VK_D", (int)0x44);
	RegisterVariable(pVM, "VK_E", (int)0x45);
	RegisterVariable(pVM, "VK_F", (int)0x46);
	RegisterVariable(pVM, "VK_G", (int)0x47);
	RegisterVariable(pVM, "VK_H", (int)0x48);
	RegisterVariable(pVM, "VK_I", (int)0x49);
	RegisterVariable(pVM, "VK_J", (int)0x4A);
	RegisterVariable(pVM, "VK_K", (int)0x4B);
	RegisterVariable(pVM, "VK_L", (int)0x4C);
	RegisterVariable(pVM, "VK_M", (int)0x4D);
	RegisterVariable(pVM, "VK_N", (int)0x4E);
	RegisterVariable(pVM, "VK_O", (int)0x4F);
	RegisterVariable(pVM, "VK_P", (int)0x50);
	RegisterVariable(pVM, "VK_Q", (int)0x51);
	RegisterVariable(pVM, "VK_R", (int)0x52);
	RegisterVariable(pVM, "VK_S", (int)0x53);
	RegisterVariable(pVM, "VK_T", (int)0x54);
	RegisterVariable(pVM, "VK_U", (int)0x55);
	RegisterVariable(pVM, "VK_V", (int)0x56);
	RegisterVariable(pVM, "VK_W", (int)0x57);
	RegisterVariable(pVM, "VK_X", (int)0x58);
	RegisterVariable(pVM, "VK_Y", (int)0x59);
	RegisterVariable(pVM, "VK_Z", (int)0x5A);

	// functions


	// Returns size of screen in 2D vector (x,y)
	RegisterFunction(pVM, "getScreenSize", (SQFUNCTION)sq_getScreenSize, 1, ".");

	// Draws @text at @pos.x @pos.y with @color (hex)
	RegisterFunction(pVM, "drawText", (SQFUNCTION)sq_drawText, 5, ".snnn");

	// Draws colorbox at @pos.x @pos.y (@width, @height) with @color (hex)
	RegisterFunction(pVM, "fillBox", (SQFUNCTION)sq_fillBox, 6, ".nnnnn");

	// Block or unblock user in-game input
	RegisterFunction(pVM, "blockInput", (SQFUNCTION)sq_blockInput, 2, ".b");

	// returns TRUE when input is blocked
	RegisterFunction(pVM, "isInputBlocked", (SQFUNCTION)sq_isInputBlocked, 1, ".");

	// Block or unblock user in-game input
	RegisterFunction(pVM, "hideChat", (SQFUNCTION)sq_hideChat, 2, ".b");

	// returns TRUE when input is blocked
	RegisterFunction(pVM, "isChatHidden", (SQFUNCTION)sq_isChatHidden, 1, ".");

	// set camera at position to look at another position
	RegisterFunction(pVM, "cameraLookAtFrom", (SQFUNCTION)sq_cameraLookAtFrom, 7, ".ffffff");

	// restore original camera position
	RegisterFunction(pVM, "restoreCamera", (SQFUNCTION)sq_restoreCamera, 1, ".");

	// set localplayer position
	// Params: x, y, z
	RegisterFunction(pVM, "setPosition", (SQFUNCTION)sq_setPosition, 4, ".fff");

	// set localplayer rotation
	// Params: x, y, z
	RegisterFunction(pVM, "setRotation", (SQFUNCTION)sq_setRotation, 4, ".fff");

	// Get localplayer position
	RegisterFunction(pVM, "getPosition", (SQFUNCTION)sq_getPosition, 1, ".");

	// Get localplayer rotation
	RegisterFunction(pVM, "getRotation", (SQFUNCTION)sq_getRotation, 1, ".");

	// Play anim
	RegisterFunction(pVM, "playAnimation", (SQFUNCTION)sq_playAnimation, 1, ".n");

	// set localplayer skin
	// Params: skinID
	RegisterFunction(pVM, "changeSkin", (SQFUNCTION)sq_changeSkin, 2, ".n");

	// Call function located at server-side in a script
	// Params: scriptname, functionname, param(int, float, bool, string)
	RegisterFunction(pVM, "callServerFunc", (SQFUNCTION)sq_callServerFunc, 4, ".ss.");

	// Creates a texture(image)
	// Returns pointer or FALSE (on fail)
	RegisterFunction(pVM, "createTexture", (SQFUNCTION)sq_createTexture, 2, ".s");

	// Returns the size of texture (width/height) as an array
	RegisterFunction(pVM, "getTextureSize", (SQFUNCTION)sq_getTextureSize, 2, ".p");

	// Draws image(created in 'createTexture') at X and Y and scaling width and height
	RegisterFunction(pVM, "drawTexture", (SQFUNCTION)sq_drawTexture, 6, ".pnnnn");

	// Draws image(created in 'createTexture') at X and Y, scaling width and height, source rect(x1,y1,x2,y2),rotation(0-360f)
	RegisterFunction(pVM, "drawTextureEx", (SQFUNCTION)sq_drawTexture, 11, ".pnnnnnnnnn");

	// Creates a new font 
	RegisterFunction(pVM, "createFont", (SQFUNCTION)sq_createFont, 3, ".sn");

	// draw @test at @x/y with @color and @font
	RegisterFunction(pVM, "drawFontText", (SQFUNCTION)sq_drawFontText, 6, ".snnnp");

	// returns width and height of text if it's rendered with current font
	RegisterFunction(pVM, "getTextWidth", (SQFUNCTION)sq_getFontWidthOfText, 3, ".sp");

	// Returns RGB color as DWORD (useful for all Color params, drawtext/fillbox)
	RegisterFunction(pVM, "ColorRGB", (SQFUNCTION)sq_ColorRGB, 4, ".nnn");

	// Returns RGB color as DWORD (useful for all Color params, drawtext/fillbox)
	RegisterFunction(pVM, "ColorARGB", (SQFUNCTION)sq_ColorARGB, 4, ".nnnn");

	//Create 3D sound at specific coordinates	
	RegisterFunction(pVM, "create3DSound", (SQFUNCTION)sq_Play3DSound, 7, ".sfffff");

	//Remove 3D sound
	RegisterFunction(pVM, "remove3DSound", (SQFUNCTION)sq_Remove3DSound, 2, ".n");

	//Create animated particle at specific coordinates	
	RegisterFunction(pVM, "createParticle", (SQFUNCTION)sq_CreateParticle, 5, ".nfff");

	//Remove particle
	RegisterFunction(pVM, "removeParticle", (SQFUNCTION)sq_RemoveParticle, 2, ".n");

	//Money Functions
	RegisterFunction(pVM, "enableMoney", (SQFUNCTION)sq_EnableMoney, 2, ".n");

	RegisterFunction(pVM, "setMoney", (SQFUNCTION)sq_SetMoney, 2, ".n");

	RegisterFunction(pVM, "getMoney", (SQFUNCTION)sq_GetMoney, 1, ".");

	//Tools Functions
	RegisterFunction(pVM, "getDistanceBetween3DPoints", (SQFUNCTION)sq_getDistanceBetween3DPoints, 7, ".ffffff");

	RegisterFunction(pVM, "getDistanceBetween2DPoints", (SQFUNCTION)sq_getDistanceBetween2DPoints, 5, ".ffff");

}
