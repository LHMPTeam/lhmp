#include "CCore.h"
#include "CNetwork.h"
bool CNetwork::StartServer(int port)
{
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
			// if packet starts with LHMP
			if (packet->data[0] == 'L' && packet->data[1] == 'H' && packet->data[2] == 'M' && packet->data[3] == 'P')
			{
				// get packet type
				switch (packet->data[4])
				{
				case 'r':
					// r = request from server browser/etc/, demand on server list
					CCore::getInstance().getServersList()->SendAllServers(packet);
					break;
				}
			}
		}
	}
}