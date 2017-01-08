#include "ClientConnectionHandler.h"
#include <MessageIDs.h>
#include <BuildVersion.h>

#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "..\Network.h"

ClientConnectionHandler::ClientConnectionHandler()
{
}


ClientConnectionHandler::~ClientConnectionHandler()
{
}

void ClientConnectionHandler::ProcessMessage(Network* network, RakNet::Packet* packet)
{
	switch(packet->data[0])
	{
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			OnConnectionAccepted(network, packet);
		}
		break;
		
		case MessageIDs::ID_CONNECTION_REFUSED_LHMP:
		{
			// TODO(zaklaus): Handle this case.
		}
		break;
		
		case MessageIDs::MessageIDs::ID_CONNECTION_ACCEPTED_LHMP:
		{
			// TODO(zaklaus): We've got player data!
		}
		break;
	}
}

void ClientConnectionHandler::OnConnectionAccepted(Network *network, RakNet::Packet* packet) const
{
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_CONNECTION_INIT_LHMP));
	bitStream.Write(BUILD_VERSION);
	bitStream.Write(network->GetUserName());
	network->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}
