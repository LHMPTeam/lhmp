/**
    Lost Heaven Multiplayer
    Source.cpp
	Purpose: demontration of query system capabilities
	@author Romop5
*/

#include <iostream>
#include <stdio.h>
#include "CLHMPQuery.h"

using namespace std;

// Callback is a function which is called when new event takes places
// e.g. when server sends data
// @serverID is the ID you have specified in queryInfo() or queryPlayers()
// @data is data sent by server, cast them as below
// @reasonID specifies what has happened (whether we were successful)
void callback(unsigned int serverID, void* data, unsigned char reasonID)
{
	// ---------- LHMP Server responses -----------------//
	if (reasonID == QUERY_OVERALL)
	{
		OverallPacket* packet = (OverallPacket*)data;
		// print out data we have received
		printf("-----------------------\n");
		printf("Server \t\t- %s\n", packet->hostname);
		printf("GM \t\t- %s\n", packet->gamemode);
		printf("Players \t- %u/%u\n", packet->players,packet->maxPlayer);
		printf("Website \t- %s\n", packet->website);
		printf("-----------------------\n");
	}
	else if (reasonID == QUERY_PLAYERLIST){
		PlayerPacket* packet = (PlayerPacket*)data;
		printf("Players count: %d \n", packet->playerCount);
		for (int i = 0; i < packet->playerCount; i++)
		{
			printf("\t ID[%d] ... %s\n", packet->playersPool[i].ID, packet->playersPool[i].nickname);
		}
	}
	else if (reasonID == QUERY_CONNECTION_FAILED) {
		printf("Connecting LHMP server has failed on task: %d ID[%u]\n", reasonID,serverID);
	}

	//----------- Master server responses --------------//
	else if (reasonID == QUERY_MASTER_FAILED) {
		printf("Connection to master FAILED \n");
	}
	else if (reasonID == QUERY_MASTER_SUCCESS) {
		printf("\nMaster server has sent us data: \n");
		MasterResponse* packet = (MasterResponse*)data;
		printf("Count of servers %d: \n",packet->count);
		for (int i = 0; i < packet->count; i++)
		{
			printf("\tServer[%d] IP: %s:%u \n", i, packet->servers[i].IP, packet->servers[i].port);
			
			// query each server for more data
			// NOTE that port has to be increased by one
			CLHMPQuery::getInstance()->queryInfo(packet->servers[i].IP, packet->servers[i].port + 1);
		}
	}
	
}

int main()
{
	// CLHMPQuery is a singletone, it's possible to call it from anywhere just using getInstance
	CLHMPQuery* system = CLHMPQuery::getInstance();

	// but you have to specify a callback at first before doing anyting else
	system->Prepare((void*)callback);

	// query master server for IPs
	system->queryMasterlist("master.lh-mp.eu");
	// note that whole CLHMPQuery is non-blocking, that means you have to wait until callback function is called

	cin.get();
	cin.get();
	return 0;
}