// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __CCRASH_HANDLER
#define __CCRASH_HANDLER

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "RakSleep.h"
#include "TCPInterface.h"
#include <mutex>
using namespace RakNet;
class CCrashHandler
{
public:
	void	Prepare();

	void	SaveDumpOnDisk(char*);
	void	SendReport(char*);
	std::mutex	mtx;
private:
};
#endif