#ifndef _CMASTERSERVER_H
#define _CMASTERSERVER_H

#include "../sdks/UDPWrapper/UDPWrapper.h"

enum MasterListResponse
{
	MASTERLIST_CONNECTIONFAILED,
	MASTERLIST_FAILED,
	MASTERLIST_OK
};


class CMasterList
{
public:
	CMasterList();

	// setup new master list request
	void AddServerToMaster();

	// handles the results of pending request
	void HandleMasterResponse(int reason);

	// Tick - handles master-server communication
	void Pulse();
private:
	UDPWrapper* client;
	bool isPending;

	// start of current request
	unsigned int timestampStart;
};

#endif