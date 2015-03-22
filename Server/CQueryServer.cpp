#include "CCore.h"
#include "CQueryServer.h"
#include "TCPInterface.h"
#include <stdio.h>

extern CCore *g_CCore;

void threadCallback()
{
	g_CCore->GetQueryServer()->Tick();
}

bool CQueryServer::StartServer(int port)
{
	queryServer = new UDPWrapper();
	//if (tcpServer->Start(port, (unsigned int)maxConnections))
	if (queryServer->StartServer(port) == UDP_OK)
	{
		this->isRunning = true;
#ifdef _WIN32
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&threadCallback, 0, NULL, NULL);
#else
		pthread_create(&tid1, NULL, &TCP, NULL);
#endif
		return true;
	}
	return false;
}

// Receive
void CQueryServer::Tick()
{
	UDPPacket* packet;
	while (1 == 1)
	{
		packet = queryServer->Receive();
		// If there IS a packet
		if (packet)
		{
			char* text = new char[packet->messageLength + 1];
			memcpy(text, packet->data, packet->messageLength);
			text[packet->messageLength] = 0x0;
			printf("[Query] %s \n", text);

			if (packet->data[0] == 'L' && packet->data[1] == 'H' && packet->data[2] == 'M' && packet->data[3] == 'P')
			{
				switch ((char)packet->data[4])
				{
					// Overall packet
				case 'o':
					this->OverallPacket(packet);
					break;
					// Player list
				case 'p':
					this->PlayerList(packet);
					break;
					// Ping
				case 'i':
					this->PingPacket(packet);
					break;
				}
			}

		}
		else {
			Sleep(5);
		}
	}
}

/*	Packet structure
	0-5		'LHMPo'
	5-6		servername length - byte
	6-7		'server name' without \0 byte
	7-8		gamemode length - byte
	8-9		'game mode' without \0 byte
	9-11	players count - 2 bytes
	11-13	max players - 2 bytes
	13-14	website length
	14-15	'website' - string
*/
void CQueryServer::OverallPacket(UDPPacket* packet)
{
	char buff[1024];
	char* buffPointer = buff;
	int len = 0;
	sprintf(buff, "LHMPo");
	buffPointer += 5;
	len += 5;
	*(unsigned char*)buffPointer = g_CCore->GetNetworkManager()->GetServerName().length();
	buffPointer += 1;
	len += 1;
	sprintf(buffPointer, "%s", g_CCore->GetNetworkManager()->GetServerName().c_str());
	buffPointer += g_CCore->GetNetworkManager()->GetServerName().length();
	len += g_CCore->GetNetworkManager()->GetServerName().length();

	*(unsigned char*)buffPointer = g_CCore->GetNetworkManager()->GetServerMode().length();
	buffPointer += 1;
	len += 1;
	sprintf(buffPointer, "%s", g_CCore->GetNetworkManager()->GetServerMode().c_str());
	buffPointer += g_CCore->GetNetworkManager()->GetServerMode().length();
	len += g_CCore->GetNetworkManager()->GetServerMode().length();

	*(unsigned short*)buffPointer = g_CCore->GetNetworkManager()->GetPeer()->NumberOfConnections();
	buffPointer += 2; 
	len += 2;

	*(unsigned short*)buffPointer = g_CCore->GetNetworkManager()->GetPeer()->GetMaximumIncomingConnections();
	buffPointer += 2; 
	len += 2;

	*(unsigned char*)buffPointer = strlen(g_CCore->GetDefaultMap());
	buffPointer += 1;
	len += 1;
	sprintf(buffPointer, "%s", g_CCore->GetDefaultMap());
	buffPointer += strlen(g_CCore->GetDefaultMap());
	len += strlen(g_CCore->GetDefaultMap());
	
	/**(unsigned char*)buffPointer = g_CCore->GetNetworkManager()->GetServerMode().length();
	buffPointer += 1;
	sprintf(buffPointer, "%s", g_CCore->GetNetworkManager()->GetServerMode().c_str());
	buffPointer += g_CCore->GetNetworkManager()->GetServerMode().length();
	*/
	//tcpServer->Send(buff, len, receiver, false);
	queryServer->SendData(len, buff, (sockaddr*)&packet->sender);

	printf("[Query] Sending overall packet\n");
}
void CQueryServer::PlayerList(UDPPacket* packet)
{
	int playerCount = 0,messageLength = 0;
	// allocate enough space
	char* buff = new char[MAX_PLAYERS*(258)];
	sprintf(buff, "LHMPp");
	//char* message = buff+7;
	messageLength += 7;

	// loop through player list
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CPlayer* player = g_CCore->GetPlayerPool()->Return(i);
		if (player)
		{
			int nickLength = strlen(player->GetNickname());
			*(unsigned short*)(buff + messageLength) = (unsigned short) i;
			*(unsigned char*)(buff + messageLength + 2) = (unsigned char) nickLength;
			memcpy((buff + messageLength + 3), player->GetNickname(), nickLength);
			messageLength += 3 + nickLength;
			playerCount++;
		}
	}
	// now just write at the start of message the count of players we are sending info about
	*(unsigned short*)(buff + 5) = (unsigned short)playerCount;

	// Send message
	//tcpServer->Send(buff, messageLength, receiver, false);
	queryServer->SendData(messageLength, buff, (sockaddr*)&packet->sender);


}

void CQueryServer::PingPacket(UDPPacket* packet)
{
	char buff[255];
	sprintf(buff, "LHMPi");
	*(int*) (buff+5) = rand();
	//tcpServer->Send(buff, strlen(buff), receiver,false);
	queryServer->SendData(strlen(buff), buff, (sockaddr*)&packet->sender);
}