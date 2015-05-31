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
			this->p_scriptPool[i] = new CScript(pVM);
			break;
		}
	}
	g_CCore->GetChat()->AddMessage("#ff0000Script loaded !");

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

// Delete all scripts
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
	SQInteger	coord_x, coord_y, width,height,color;
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

void RegisterFunction(HSQUIRRELVM pVM, char * szFunc, SQFUNCTION func, int params, const char * szTemplate);

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

	// functions


	// Returns size of screen in 2D vector (x,y)
	RegisterFunction(pVM, "getScreenSize", (SQFUNCTION)sq_getScreenSize, 1, ".");

	// Draws @text at @pos.x @pos.y with @color (hex)
	RegisterFunction(pVM, "drawText", (SQFUNCTION)sq_drawText, 5, ".snnn");

	// Draws colorbox at @pos.x @pos.y (@width, @height) with @color (hex)
	RegisterFunction(pVM, "fillBox", (SQFUNCTION)sq_fillBox, 6, ".nnnnn");
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
