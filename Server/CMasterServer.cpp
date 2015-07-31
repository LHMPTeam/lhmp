#include "CCore.h"
#include "CMasterServer.h"

extern CCore* g_CCore;


CMasterList::CMasterList()
{
	this->client = NULL;
	this->isPending = false;
	
}

void CMasterList::AddServerToMaster()
{
	if (client == NULL)
		client = new UDPWrapper;
	client->StartClient("master.lh-mp.eu", 50000);

	char message[] = "LHMPcPP";

	// PP is replaced by our server port
	*(unsigned short*)(message + 5) = (unsigned short)g_CCore->GetNetworkManager()->GetPort();

	// send request to master
	client->SendData(7, message);

	this->timestampStart = GetTickCount();
	this->isPending = true;
}

// Tick - handles master-server communication
void CMasterList::Pulse()
{
	if (isPending)
	{
		char signature[] = "LHMP";
		UDPPacket* pack = this->client->Receive();
		if (pack)
		{
			if (pack->messageLength >= 5)
			{
				// compare if packet starts with LHMP chars
				if (*(int*)pack->data == *(int*)signature)
				{
					switch (pack->data[4])
					{
					case '1':
						// ok, server is online, now let's wait for task response
						g_CCore->GetLog()->AddNormalLog("[Master] Connected to master!");
						this->timestampStart = GetTickCount();
						break;
					case '2':
						// server has already been registered
						this->HandleMasterResponse(MASTERLIST_OK); 
						this->isPending = false;
						return;

						break;
					case '3':
						// fail
						this->HandleMasterResponse(MASTERLIST_FAILED);
						this->isPending = false;
						return;
					}
				}
			}
			// dellocates dynamical memory in order to prevent any memory leak
			this->client->DellocatePacket(pack);
		}

		unsigned int time = GetTickCount();
		if ((time - this->timestampStart) > 1000)
		{
			this->HandleMasterResponse(MASTERLIST_CONNECTIONFAILED);
			this->isPending = false;
		}
	}
}

// handles the results of pending request
void CMasterList::HandleMasterResponse(int reason)
{
	switch (reason)
	{
	case MASTERLIST_CONNECTIONFAILED:
		g_CCore->GetLog()->AddNormalLog("[Error] Master server is down !"); 
		g_CCore->GetLog()->AddNormalLog("        This server won't be visible in server list !");
		break;
	case MASTERLIST_FAILED:
		g_CCore->GetLog()->AddNormalLog("[Error] Failed to request master server !");
		g_CCore->GetLog()->AddNormalLog("        This server won't be visible in server list !");
		break;
	case MASTERLIST_OK:
		g_CCore->GetLog()->AddNormalLog("[Master] Server's been successfully added to the masterlist !");
		break;
	}
}