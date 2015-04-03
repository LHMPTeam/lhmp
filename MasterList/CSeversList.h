// this class hols all LHMP servers
#ifndef _CSERVERSLIST_H
#define _CSERVERSLIST_H

// count of milliseconds between serverlist checks (check for offline)
#define CHECKDELAY	30000

#include "../sdks/UDPWrapper/clhmpquery.h"
#include <vector>

class CServer
{
public:
	int		GetID() { return this->ID; }
	char*	GetIP() { return this->IPaddres; }
	int		GetPort() { return this->port; }
private:
	int ID;
	char IPaddres[20];
	int port;
};
class CServersList
{
public:
	// Start
	void Start();
	// check if enough time elapsed, then call TestAllServers
	void Pulse();
	// Called once a time
	// This function should test whether all servers are responsing and remove the offline ones
	void TestAllServers();

	// Sends all servers+port to guy, who is responding
	void SendAllServers(UDPPacket* packet);

	// Add new server into list
	void AddServerIntoList(char* IP, int port);

	// intern function
	void HandleCallback(int reason, int ID);

private:
	void DeleteServerWithID(int ID);
	CLHMPQuery* query;
	int currentID;
	std::vector <CServer> list;
	unsigned int lastCheck;
};

#endif