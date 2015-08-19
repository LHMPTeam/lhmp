#ifndef _CSERVERS_H
#define _CSERVERS_H

#include "../sdks/UDPWrapper/CLHMPQuery.h"
#include <vector>
#include <mutex>

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
		this->players = 0;
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

	void ChangePlayersCount(unsigned short inPlayers)
	{
		this->players = inPlayers;
	}

	unsigned short GetPlayers()
	{
		return this->players;
	}

private:
	bool isPending;
	unsigned int ID;
	// get inet as IP
	sockaddr_in		IPAddress;
	unsigned short port;
	unsigned short players;
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

	// Locks the mutex, so array should stay consistent and not-changed until UnprotectServers
	void ProtectServers();
	// Unlock the mutex
	void UnprotectServers();


private:
	unsigned int referenceID;
	CLHMPQuery* query;
	std::vector <CServer> pool;
	// mutex = C++11 feature
	std::mutex serversControl;
	unsigned int lastCheck;
};

#endif