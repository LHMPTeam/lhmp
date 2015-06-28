#include "CScript.h"
#include "squirrelheads.h"

CScript::CScript(const char *szScriptName)
{
	// get the script path
	char szScriptPath[512];
	sprintf(szScriptPath, "%s", szScriptName);

	// make sure the script exists
	FILE * fp = fopen(szScriptPath, "rb");

	if (!fp) {
		// script does not exist
		return;
	}

	fclose(fp);

	// set the script name
	strcpy(m_szScriptName, szScriptName);

	// create the squirrel VM with an initial stack size of 1024 bytes
	m_pVM = sq_open(1024);

	// get the script vm pointer
	SQVM * pVM = m_pVM;

	// register the default error handlers
	sqstd_seterrorhandlers(pVM);

	// set the print and error functions
	sq_setprintfunc(pVM, printfunc, printfunc);

	// push the root table onto the stack
	sq_pushroottable(pVM);

	// register the string library
	sqstd_register_stringlib(pVM);

	// register the math library
	sqstd_register_mathlib(pVM);

	sqstd_register_systemlib(pVM);

	sqstd_register_iolib(pVM);

	//sqstd_register_stringlib(pVM);

	// register the blob library
	/*sqstd_register_bloblib(pVM);

	// register the input/out library
	sqstd_register_iolib(pVM);
	*/
	// register the math library
	//sqstd_register_mathlib(pVM);
	/*
	// register the string library
	sqstd_register_stringlib(pVM);

	// register the system library
	sqstd_register_systemlib(pVM);

	// register the vcmp functions
	sq_register_natives(pVM);
	*/
	Init(pVM);	// register native functions
	// load and compile the script
	if (SQ_FAILED(sqstd_dofile(pVM, szScriptPath, SQFalse, SQTrue))) {
		// script compilation failed
		return;
	}

	// pop the root table from the stack
	sq_pop(pVM, 1);

	// call the plugins
	//pPlugins->OnScriptLoad(pVM);

	// script loaded successfully


	// save script name without path
	int len = strlen(szScriptName);
	// i - character counter
	int i;
	for (i = len - 1; i >= 0; i--)
	{
		if (szScriptName[i] == '/' || szScriptName[i] == '\\')
			break;
	}
	strcpy(this->m_szScriptName, szScriptName+i+1);
	return;
}

CScript::~CScript()
{
	// kill all timers
	//pNetowkManager->GetTimerPool()->HandleScriptUnload(m_pVM);

	// call the plugins
	//pPlugins->OnScriptUnload(m_pVM);

	// close the script vm
	sq_close(m_pVM);

	// reset the script vm pointer
	m_pVM = NULL;
}