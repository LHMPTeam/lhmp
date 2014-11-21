#ifndef CCRASH_HANDLER
#define CCRASH_HANDLER

//RakNet
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "RakSleep.h"
#include "TCPInterface.h"

using namespace RakNet;
class CCrashHandler
{
public:
	void	Prepare();
	//LONG	HandleIt(_EXCEPTION_POINTERS * ExceptionInfo);

	void	SaveDumpOnDisk(char*);
	void	SendReport(char*);
};
#endif