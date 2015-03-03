/**
Lost Heaven Multiplayer

Purpose: server query system

@author Romop5
@version 1.0 28/02/15
*/

#ifndef _CQUERYSERVER
#define _CQUERYSERVER
#include "TCPInterface.h"

//----------	Query types ---------//
/*
	- 'o'		overall packet		returns server name, gamemode, website url, players count, max players
	- 'p'		player list			returns player list
	- 'i'		ping packet			returns random 4 bytes
*/




class CQueryServer 
{
public:
	// Start TCP server
	bool StartServer(int port, int maxConnections);
	
	// Receive
	void Tick();

	// 'o' returns basic information about server
	void OverallPacket(RakNet::SystemAddress receiver);
	// 'p'
	void PlayerList(RakNet::SystemAddress receiver);
	// 'i' 
	void PingPacket(RakNet::SystemAddress receiver);

private:
	RakNet::TCPInterface* tcpServer;
	bool isRunning;
};

#endif