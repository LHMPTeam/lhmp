#include "CCore.h"
#include "CNetwork.h"
bool CNetwork::StartServer(int port)
{
	this->server = new UDPWrapper();
	if (this->server->StartServer(port) == UDP_OK)
	{
		return true;
	}
	return false;
}
void CNetwork::Pulse()
{
	UDPPacket* packet = this->server->Receive();
	if (packet)
	{
		// if has at least LHMP + information char
		if (packet->messageLength >= 5)
		{
			char signature[] = "LHMP";
			// if packet starts with LHMP
			//if (packet->data[0] == 'L' && packet->data[1] == 'H' && packet->data[2] == 'M' && packet->data[3] == 'P')
			if (*(int*)signature == *(int*) packet->data)
			{
				// get packet type
				switch (packet->data[4])
				{
				// server browser/any device/ request
				case 'r':
					printf("Request from browser \n");
					// r = request from server browser/etc/, demand on server list
					//CCore::getInstance().getServers()->SendAllServers(packet);
					this->SendServers(packet->sender);
					break;
				case 's':
					printf("Request from browser(master info) \n");
					// r = request for master info (servers/players count)
					this->SendMasterInfo(packet->sender);
					break;

				// new client
				case 'c':
					printf("New server \n");
					// c = register a new server
					// if packet has desired length (e.g. LHMPc00)
					if (packet->messageLength >= 7)
					{
						// extract data from packet
						unsigned short port = *(unsigned short*)(packet->data + 5);
						CCore::getInstance().getServers()->AddServer(packet->sender,port);
						// 1 - IN_PROGRESS message
						const char message[] = "LHMP1";

						this->server->SendData(5, message, (sockaddr*)&packet->sender);
						
					}
					break;
				}
			}
		}
	}
}

UDPWrapper* CNetwork::GetServerHandle()
{
	return this->server;
}

// Called when server has been successfully added onto public server list
void	CNetwork::OnSuccess(sockaddr_in client)
{
	// 2 - OK message
	const char message[] = "LHMP2";
	this->server->SendData(5, message, (sockaddr*) &client);
}

void	CNetwork::OnFail(sockaddr_in client)
{
	// 3 - FAIL message
	const char message[] = "LHMP3";
	this->server->SendData(5, message, (sockaddr*)&client);
}


void	CNetwork::SendServers(sockaddr_in receiver)
{
	std::vector <CServer>* pool = CCore::getInstance().getServers()->GetPool();
	unsigned int size = pool->size();
	unsigned char* data = new unsigned char[(6 * size)+7];
	memcpy(data, "LHMPr", 5);
	*(short*)(data + 5) = size;
	unsigned int pointer = 0;
	for (std::vector<CServer>::iterator it = pool->begin(); it != pool->end(); ++it)
	{
		//printf("[S][%d] IP: %s Port: %d",)
		*(in_addr*) (7+data+(pointer * 6)) = it->GetHost().sin_addr;
		*(unsigned short*)(7+data + (pointer * 6)+4) = (unsigned short) it->GetPort();
		pointer++;
	}

	unsigned packetSize = (size * 6) + 7;


	this->server->SendData(packetSize, data, (sockaddr*)&receiver);

	delete[] data;
}


void	CNetwork::SendMasterInfo(sockaddr_in receiver)
{
	unsigned int players = 0;
	std::vector <CServer>* pool = CCore::getInstance().getServers()->GetPool();
	unsigned short servers = pool->size();

	// get current count of players
	for (std::vector<CServer>::iterator it = pool->begin(); it != pool->end(); ++it)
	{
		players += it->GetPlayers();
	}

	// LHMPs(5) + uint (2) + uint(4) => 11 bytes long packet
	unsigned char* data = new unsigned char[11];
	memcpy(data, "LHMPs", 5);
	*(unsigned short *)(data + 5) = servers;
	*(unsigned int *)(data + 7) = players;


	this->server->SendData(11, data, (sockaddr*)&receiver);

	delete[] data;
}