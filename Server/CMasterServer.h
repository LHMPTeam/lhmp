// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef _CMASTERSERVER_H
#define _CMASTERSERVER_H

#include "../sdks/UDPWrapper/UDPWrapper.h"

// defines the amount of miliseconds that have to elapse between reposting
// information to master server (MILISECONDS)
#define MASTERSERVER_UPDATE_INTERVAL		600000

// Response status
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
	~CMasterList();
	// setup new master list request
	void AddServerToMaster();

	// Tick - handles master-server communication
	void Pulse();

	// Sets our visibility, if false, we wont communicate with master
	void SetVisibility(bool);
private:
	UDPWrapper* client;
	bool isPending;

	// start of current request
	unsigned int timestampStart;

	// timestamp taken when sending a request to master server
	unsigned int lastMasterPost;

	// should we be announced in the master query 
	bool isVisible;

	// handles the results of pending request
	void HandleMasterResponse(int reason);
};

#endif