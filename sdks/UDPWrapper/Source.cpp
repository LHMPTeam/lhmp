#include <iostream>
#include <stdio.h>
#include "clhmpquery.h"

using namespace std;

void callback(unsigned int serverID, void* data, unsigned char reasonID)
{
	//printf("callback %d\n", reasonID);

	if (reasonID == QUERY_OVERALL)
	{
		OverallPacket* packet = (OverallPacket*)data;
		printf("server:'%s' '%u' '%s' '%s' pass?%d", packet->hostname, packet->ping, packet->mapname, packet->website,packet->hasPassword);
	}
	else if (reasonID == QUERY_PLAYERLIST){
		PlayerPacket* packet = (PlayerPacket*)data;
		printf("Players count: %d \n", packet->playerCount);
		for (int i = 0; i < packet->playerCount; i++)
		{
			printf("ID[%d] ... %s\n", packet->playersPool[i].ID, packet->playersPool[i].nickname);
		}

	}
	else if (reasonID == QUERY_CONNECTION_FAILED) {
		printf("Connection failed on task: %d ID[%u]\n", reasonID,serverID);
	}
	else if (reasonID == QUERY_MASTER_FAILED) {
		printf("Connection to master FAILED \n");
	}
	else if (reasonID == QUERY_MASTER_SUCCESS) {
		printf("Servers TY OK: \n");
		MasterResponse* packet = (MasterResponse*)data;
		for (int i = 0; i < packet->count; i++)
			printf("Server[%d] IP: %s:%u \n", i,packet->servers[i].IP, packet->servers[i].port);
	}
	
}

int main()
{
	CLHMPQuery* system = CLHMPQuery::getInstance();
	system->Prepare((void*)callback);

	char address[] = "78.141.75.10";
	system->queryInfo(address, 27016, 5);
	system->queryPlayerlist("78.141.75.10", 27016, 5);

	system->queryMasterlist("lh-mp.eu");
	//system->queryMasterlist("178.62.194.147");

	cin.get();
	cin.get();
	return 0;
}