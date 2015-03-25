#ifndef _CNETWORK_H
#define _CNETWORK_H

// handles all connections & data receiving
#include "../sdks/UDPWrapper/UDPWrapper.h"
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
private:
	UDPWrapper* server;
};

#endif