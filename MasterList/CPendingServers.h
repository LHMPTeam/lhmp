#ifndef _CPENDINGSERVERS_H
#define _CPENDINGSERVERS_H

// servers which are about to add to server list, but awaiting to confirm they public accessibility
//#include "CCore.h"
#include "../sdks/UDPWrapper/clhmpquery.h"

struct PendingServer
{
	unsigned int ID;
	sockaddr_in client;
	PendingServer(unsigned int inID,sockaddr_in inclient)
	{
		ID = inID;
		memcpy(&client, &inclient, sizeof(sockaddr));
	}
};

class CPendingServers
{
public:
	// Prepare it
	void Start();

	// Add new pending client into query queue
	void AddNew(UDPPacket* packet);

	// delete
	void DeleteServerWithID(unsigned int ID);

	// called if sever's query has successfully responded
	void OnSuccess(unsigned int ID);
	// called if not
	void OnFail(unsigned int ID);



private:
	unsigned int referenceID;
	CLHMPQuery* query;
	std::vector <PendingServer> pendingClients;
};

#endif