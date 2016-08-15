// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include "CMasterServer.h"

extern CCore* g_CCore;


CMasterList::CMasterList()
{
	this->client = NULL;
	this->isPending = false;
	this->lastMasterPost = 0;
	this->isVisible = false;
	this->timestampStart = NULL;
}

CMasterList::~CMasterList()
{
	if (client)
	{
		client->CleanUP();
		delete client;
	}
}

void CMasterList::AddServerToMaster()
{
	// we want to be announced in any case, so change to "visible" mode
	this->isVisible = true;
	// create a new instance of UDPWrapper if needed
	if (client == NULL)
		client = new UDPWrapper;
	// set connection details to master server 
	client->StartClient("master.lh-mp.eu", 50000);

	// master server message consists of LHMPcPP where PP is port casted to unsigned short,
	// thus 2 bytes long
	char message[] = "LHMPcPP";

	// PP is replaced by our server port
	*(unsigned short*)(message + 5) = (unsigned short)g_CCore->GetNetworkManager()->GetPort();

	// send request to master
	client->SendData(7, message);

	this->timestampStart = RakNet::GetTimeMS();
	// turn on callback checking
	this->isPending = true;
}

// Tick - handles master-server communication
void CMasterList::Pulse()
{
	// stop the function if we are hidden for the master
	if (this->isVisible == false)
		return;
	unsigned int time = RakNet::GetTimeMS();

	// if we are awaiting communication from the master
	if (isPending)
	{
		UDPPacket* pack = this->client->Receive();
		// if there is a packet
		if (pack)
		{
			if (pack->messageLength >= 5)
			{
				char signature[] = "LHMP";
				// compare if packet starts with LHMP chars
				if (*(int*)pack->data == *(int*)signature)
				{
					switch (pack->data[4])
					{
					case '1':
						// ok, server is online, now let's wait for task response
						g_CCore->GetLog()->AddNormalLog("[Master] Connected to master!");
						this->timestampStart = RakNet::GetTimeMS();
						break;
					case '2':
						// server has already been registered
						this->HandleMasterResponse(MASTERLIST_OK); 
						this->isPending = false;
						break;
					case '3':
						// fail
						this->HandleMasterResponse(MASTERLIST_FAILED);
						this->isPending = false;
						break;
					}
				}
			}
			// dellocates dynamical memory in order to prevent any memory leak
			this->client->DellocatePacket(pack);
			return;
		}

		
		if (time > this->timestampStart && (time - this->timestampStart) > 5000)
		{
			this->HandleMasterResponse(MASTERLIST_CONNECTIONFAILED);
			this->isPending = false;
		}
	}
	else {
		if (time > (this->lastMasterPost + MASTERSERVER_UPDATE_INTERVAL))
		{
			// trigger a new request to server
			this->AddServerToMaster();
		}
	}
}

// handles the results of pending request
void CMasterList::HandleMasterResponse(int reason)
{
	switch (reason)
	{
	case MASTERLIST_CONNECTIONFAILED:
		g_CCore->GetLog()->AddNormalLog("[Error] Master server is down ! Retrying in %u minute(s)",MASTERSERVER_UPDATE_INTERVAL/1000); 
		g_CCore->GetLog()->AddNormalLog("        This server won't be visible in server list !");
		break;
	case MASTERLIST_FAILED:
		g_CCore->GetLog()->AddNormalLog("[Error] Masterlist cound't connect this server from the internet ");
		g_CCore->GetLog()->AddNormalLog("        This server won't be visible in server list !");
		g_CCore->GetLog()->AddNormalLog("        Make sure you have public IP & ports are open and forwarded !");
		break;
	case MASTERLIST_OK:
		g_CCore->GetLog()->AddNormalLog("[Master] Server's been successfully added to the masterlist !");
		break;
	}

	this->lastMasterPost = RakNet::GetTimeMS();
}

void CMasterList::SetVisibility(bool vis)
{
	this->isVisible = vis;
}