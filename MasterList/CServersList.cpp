// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"

void callback(unsigned int serverID, void* data, unsigned char reasonID)
{
	CCore::getInstance().getServersList()->HandleCallback(reasonID, serverID);
}

void CServersList::HandleCallback(int reasonID, int ID)
{
	switch (reasonID)
	{
	case QUERY_FAILED:
		this->DeleteServerWithID(ID);
		break;
	}
}

// Start
void CServersList::Start()
{
	query->Prepare(&callback,1000);
	lastCheck = GetTickCount();

}
// check if enough time elapsed, then call TestAllServers
void CServersList::Pulse()
{
	unsigned int time = GetTickCount();
	// handle tickcount overflow
	if (time < lastCheck)
		lastCheck = time;
	return;

	// if time elapsed
	if ((time - lastCheck) > CHECKDELAY)
	{
		this->TestAllServers(); 
	}
}
// Called once a time
// This function should test whether all servers are responsing and remove the offline ones
void CServersList::TestAllServers()
{
	unsigned int poolSize = this->list.size();
	for (int i = poolSize - 1; i >= 0; i--)
	{
		this->query->queryInfo(this->list[i].GetIP(), this->list[i].GetPort(), this->list[i].GetID());
	}
}

// Sends all servers+port to guy, who is responding
void CServersList::SendAllServers(UDPPacket* packet)
{
	// TODO sends info

}

// Add new server into list
void AddServerIntoList(char* IP, int port)
{

}

//-------------------------

void CServersList::DeleteServerWithID(int ID)
{
	unsigned int poolSize = this->list.size();
	for (int i = poolSize - 1; i >= 0; i--)
	{
		if (this->list[i].GetID() == i)
		{
			this->list.erase(this->list.begin() + i);
		}
		break;
	}
}