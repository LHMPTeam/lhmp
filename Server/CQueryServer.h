/**
Lost Heaven Multiplayer

Purpose: server query system

@author Romop5
@version 1.0 28/02/15
*/

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
	// Start query server
	bool StartServer(int port);
	
	// Receive
	void Tick();

	// 'o' returns basic information about server
	void OverallPacket(UDPPacket* packet);
	// 'p'
	void PlayerList(UDPPacket* packet);
	// 'i' 
	void PingPacket(UDPPacket* packet);

private:
	UDPWrapper* queryServer;
	bool isRunning;
};

#endif