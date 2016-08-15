// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef _CNETWORK_H
#define _CNETWORK_H

// handles all connections & data receiving
#include "../sdks/UDPWrapper/clhmpquery.h"
class CNetwork
{
public:
	// starts UDP server
	// returns TRUE on success
	bool StartServer(int port);

	// handle all communication via Receive
	void Pulse();

	// Get server handle (to send data from other classes)
	UDPWrapper* GetServerHandle();

	void	OnSuccess(sockaddr_in);
	void	OnFail(sockaddr_in);

	void	SendServers(sockaddr_in);

	void	SendMasterInfo(sockaddr_in);
private:
	UDPWrapper* server;
};

#endif