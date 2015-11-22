/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose query system providing information about server instance to anybody\
		 via UDP and internet
@author Romop5
******************************************************************************/

#ifndef _CQUERYSERVER
#define _CQUERYSERVER

#include "../sdks/UDPWrapper/UDPWrapper.h"

//----------	Query types ---------//
/*
	- 'o'		overall packet		returns server name, gamemode, website url, players count, max players
	- 'p'		player list			returns player list
	- 'i'		ping packet			returns random 4 bytes
*/




class CQueryServer 
{
public:
	CQueryServer();
	// Start query server
	bool StartServer(int port);
	
	// Receive
	void Tick();

private:
	UDPWrapper* queryServer;
	bool isRunning;

	// 'o' returns basic information about server
	void OverallPacket(UDPPacket* packet);
	// 'p'
	void PlayerList(UDPPacket* packet);
	// 'i' 
	void PingPacket(UDPPacket* packet);
};

#endif