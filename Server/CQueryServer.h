// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

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