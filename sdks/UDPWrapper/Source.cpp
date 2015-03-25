#include <iostream>
#include <stdio.h>
#include "clhmpquery.h"

using namespace std;

void callback(unsigned int serverID, void* data, unsigned char reasonID)
{
	printf("callback %d\n", reasonID);

	if (reasonID == QUERY_OVERALL)
	{
		OverallPacket* packet = (OverallPacket*)data;
		printf("server:'%s' '%u' '%s'", packet->hostname, packet->ping,packet->mapname);
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
		printf("Connection failed on task: %d\n", reasonID);
	}
	
}

int main()
{
	CLHMPQuery* system = CLHMPQuery::getInstance(); 
	system->Prepare((void*)callback);
	/*while (1 == 1)
	{
		for (int i = 0; i < 100; i++)
			system->queryInfo("127.0.0.1", 27016, 5);
		//Sleep(100);
	}*/

	char address[] = "78.141.75.10";
	system->queryInfo(address, 27016, 5);
	system->queryPlayerlist("78.141.75.10", 27016, 5);

	cin.get();
	cin.get();
	return 0;
}