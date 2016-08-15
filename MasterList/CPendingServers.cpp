// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
//#include "CPendingServers.h"

void callbackPending(unsigned int serverID, void* data, unsigned char reasonID)
{
	if (reasonID == QUERY_OVERALL)
	{
		CCore::getInstance().getPendingServers()->OnSuccess(serverID);
	}
	else {
		CCore::getInstance().getPendingServers()->OnFail(serverID);
	}
}

void CPendingServers::Start()
{
	this->referenceID = 0;
	this->query = new CLHMPQuery();
	this->query->Prepare(&callbackPending);
}
// Add new pending client into query queue
void CPendingServers::AddNew(UDPPacket* packet)
{
	PendingServer server = PendingServer(this->referenceID++, packet->sender);
	this->pendingClients.push_back(server);

	// request info from it
	this->query->queryInfo(inet_ntoa(server.client.sin_addr),server.client.sin_port);
}

// delete
void CPendingServers::DeleteServerWithID(unsigned int ID)
{

}

// called if sever's query has successfully responded
void CPendingServers::OnSuccess(unsigned int ID)
{
	unsigned int size = this->pendingClients.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (this->pendingClients[i].ID == ID)
		{
			CCore::getInstance().getNetwork()->OnSuccess(this->pendingClients[i].client);
		}
	}
	this->DeleteServerWithID(ID);
}
// called if not
void CPendingServers::OnFail(unsigned int ID)
{
	unsigned int size = this->pendingClients.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (this->pendingClients[i].ID == ID)
		{
			CCore::getInstance().getNetwork()->OnFail(this->pendingClients[i].client);
		}
	}
	this->DeleteServerWithID(ID);
}