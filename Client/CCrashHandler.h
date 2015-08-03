/**
	Lost Heaven Multiplayer

	Purpose: a class which handle crash reports

	@author Romop5, ZaKlaus
	@version 1.0 1/9/14
*/

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