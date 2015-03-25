#ifndef _CPENDINGSERVERS_H
#define _CPENDINGSERVERS_H

// servers which are about to add to server list, but awaiting to confirm they public accessibility
#include "CCore.h"

struct PendingServer
{
	char IP[20];
	int port;
	//addrinfo client;
};

class CPendingServers
{
public:
	// Prepare it
	void Start();

	// Add new pending client into query queue
	void AddNew(UDPPacket* packet);


private:
	std::vector <PendingServer> prendingClients;
};

#endif