#include "CCore.h"
#include "CQueryServer.h"
#include "TCPInterface.h"
#include <stdio.h>

extern CCore *g_CCore;

bool CQueryServer::StartServer(int port, int maxConnections)
{
	tcpServer = new RakNet::TCPInterface();
	if (tcpServer->Start(port, (unsigned int)maxConnections))
	{
		this->isRunning = true;
		return true;
	}
	return false;
}

// Receive
void CQueryServer::Tick()
{
	if (tcpServer->ReceiveHasPackets())
	{
		RakNet::Packet* packet = tcpServer->Receive();
		char* text = new char[packet->length + 1];
		memcpy(text, packet->data, packet->length);
		text[packet->length] = 0x0;
		printf("[Query] %s \n", text);
		
		if (packet->data[0] == 'L' && packet->data[1] == 'H' && packet->data[2] == 'M' && packet->data[3] == 'P')
		{
			switch ((char)packet->data[4])
			{
				// Overall packet
			case 'o':
				this->OverallPacket(packet->systemAddress);
				break;
				// Player list
			case 'p':
				this->PlayerList(packet->systemAddress);
				break;
				// Ping
			case 'i':
				this->PingPacket(packet->systemAddress);
				break;
			}

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
void CQueryServer::OverallPacket(RakNet::SystemAddress receiver)
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
	
	/**(unsigned char*)buffPointer = g_CCore->GetNetworkManager()->GetServerMode().length();
	buffPointer += 1;
	sprintf(buffPointer, "%s", g_CCore->GetNetworkManager()->GetServerMode().c_str());
	buffPointer += g_CCore->GetNetworkManager()->GetServerMode().length();
	*/
	tcpServer->Send(buff, len, receiver, false);


	printf("[Query] Sending overall packet\n");
}
void CQueryServer::PlayerList(RakNet::SystemAddress receiver)
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
	tcpServer->Send(buff, messageLength, receiver, false);



}

void CQueryServer::PingPacket(RakNet::SystemAddress receiver)
{
	char buff[255];
	sprintf(buff, "LHMPi");
	*(int*) (buff+5) = rand();
	tcpServer->Send(buff, strlen(buff), receiver,false);
}