#include "CCore.h"
#include "CQueryServer.h"
#include "TCPInterface.h"
#include <stdio.h>
#include "../shared/version.h"

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
			//printf("[Query] %s \n", text);
			char signature[] = "LHMP";
			//if (packet->data[0] == 'L' && packet->data[1] == 'H' && packet->data[2] == 'M' && packet->data[3] == 'P')
			if (*(int*)signature == *(int*) packet->data)
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
	// --------- Add protocol version - 1 byte
	*(unsigned char*)buffPointer = (unsigned char)LHMP_QUERY_PROTOCOL;
	buffPointer++;
	len++;
	// --------- Add hasPassword - 1 byte
	// TODO - implement passwords
	*(unsigned char*)buffPointer = (unsigned char)0;
	buffPointer++;
	len++;	
	// --------- Add player's count - 2 bytes
	*(unsigned short*)buffPointer = (unsigned short)g_CCore->GetNetworkManager()->GetPlayerCount();
	buffPointer+= 2;
	len += 2;
	// --------- Add max player's count - 2 bytes
	*(unsigned short*)buffPointer = (unsigned short)g_CCore->GetNetworkManager()->GetMaxPlayerCount();
	buffPointer += 2;
	len += 2;

	//---------------------------------- STRINGS ----------------------------------------------------------//

	// --------- Add server name (length) - 1 byte
	*(unsigned char*)buffPointer = (unsigned char)g_CCore->GetNetworkManager()->GetServerName().length();
	buffPointer ++;
	len++;
	// --------- Add server name - N bytes
	memcpy(buffPointer, g_CCore->GetNetworkManager()->GetServerName().c_str(), g_CCore->GetNetworkManager()->GetServerName().length());
	buffPointer += g_CCore->GetNetworkManager()->GetServerName().length();
	len += g_CCore->GetNetworkManager()->GetServerName().length();

	// --------- Add gamemode (length) - 1 byte
	*(unsigned char*)buffPointer = (unsigned char)g_CCore->GetNetworkManager()->GetServerMode().length();
	buffPointer++;
	len++;
	// --------- Add gamemode - N bytes
	memcpy(buffPointer, g_CCore->GetNetworkManager()->GetServerMode().c_str(), g_CCore->GetNetworkManager()->GetServerMode().length());
	buffPointer += g_CCore->GetNetworkManager()->GetServerMode().length();
	len += g_CCore->GetNetworkManager()->GetServerMode().length();

	// --------- Add website (length) - 1 byte
	*(unsigned char*)buffPointer = (unsigned char)strlen(g_CCore->GetNetworkManager()->GetWebsite());
	buffPointer++;
	len++;
	// --------- Add website - N bytes
	memcpy(buffPointer, g_CCore->GetNetworkManager()->GetWebsite(), strlen(g_CCore->GetNetworkManager()->GetWebsite()));
	buffPointer += strlen(g_CCore->GetNetworkManager()->GetWebsite());
	len += strlen(g_CCore->GetNetworkManager()->GetWebsite());

	// --------- Add default map (length) - 1 byte
	*(unsigned char*)buffPointer = (unsigned char)strlen(g_CCore->GetDefaultMap());
	buffPointer++;
	len++;
	// --------- Add default map - N bytes
	memcpy(buffPointer, g_CCore->GetDefaultMap(), strlen(g_CCore->GetDefaultMap()));
	buffPointer += strlen(g_CCore->GetDefaultMap());
	len += strlen(g_CCore->GetDefaultMap());

	// Now just send data
	queryServer->SendData(len, buff, (sockaddr*)&packet->sender);

	/*for (int i = 0; i < len; i++)
	{
		printf("%x%c ", buff[i], buff[i]);
	}
	printf("\n");
	*/
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


	printf("[Query] Sending playerlist packet\n");
}

void CQueryServer::PingPacket(UDPPacket* packet)
{
	char buff[255];
	sprintf(buff, "LHMPi");
	*(int*) (buff+5) = rand();
	//tcpServer->Send(buff, strlen(buff), receiver,false);
	queryServer->SendData(strlen(buff), buff, (sockaddr*)&packet->sender);
	printf("[Query] Sending pingpacket\n");
}