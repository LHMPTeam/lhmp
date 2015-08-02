#ifndef _CMASTERSERVER_H
#define _CMASTERSERVER_H

#include "../sdks/UDPWrapper/UDPWrapper.h"

// defines the amount of seconds that have to elapse between reposting
// information to master server (MILISECONDS)
#define MASTERSERVER_UPDATE_INTERVAL		600000

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

	// timestamp taken when sending a request to master server
	unsigned int lastMasterPost;
};

#endif