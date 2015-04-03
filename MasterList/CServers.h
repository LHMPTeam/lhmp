#ifndef _CSERVERS_H
#define _CSERVERS_H

#include "../sdks/UDPWrapper/clhmpquery.h"
#include <vector>

// Stores a server, provides functions to handle it
class CServer
{
public:
	CServer(unsigned inID,bool pendingOrStatic, sockaddr_in inAddr, unsigned short inPort)
	{
		this->ID = inID;
		this->isPending = pendingOrStatic;
		memcpy(&this->IPAddress, &inAddr, sizeof(sockaddr_in));
		this->port = inPort;
	}
	// returns IP address
	char* GetIP()
	{
		return inet_ntoa(this->IPAddress.sin_addr);
	}
	unsigned int GetID()
	{
		return this->ID;
	}

	int GetPort()
	{
		return (int) this->port;
	}

	bool IsPending()
	{
		return this->isPending;
	}

	sockaddr_in	GetHost()
	{
		return this->IPAddress;
	}

	void ChangeToPersistent()
	{
		this->isPending = false;
	}

private:
	bool isPending;
	unsigned int ID;
	// get inet as IP
	sockaddr_in		IPAddress;
	unsigned short port;
};

class CServers
{
public:

	// start LHMP query interface and get ready
	void Prepare();

	void AddServer(sockaddr_in addr, unsigned short port);

	std::vector<CServer>::iterator GetServerWithID(unsigned int ID);

	void HandleCallback(unsigned int ID, void* data, unsigned char reason);

	std::vector <CServer>* GetPool();

	void Pulse();


private:
	unsigned int referenceID;
	CLHMPQuery* query;
	std::vector <CServer> pool;

	unsigned int lastCheck;
};

#endif