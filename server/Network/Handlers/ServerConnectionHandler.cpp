#include "ServerConnectionHandler.h"
#include <MessageIDs.h>
#include <BuildVersion.h>

ServerConnectionHandler::ServerConnectionHandler(std::map<RakNet::RakNetGUID, Client>* Clients)
{
	mClients = Clients;
}

ServerConnectionHandler::~ServerConnectionHandler()
{

}

void ServerConnectionHandler::ProcessMessage(Network* network, RakNet::Packet* packet)
{
	switch(packet->data[0])
	{
		case MessageIDs::ID_CONNECTION_INIT_LHMP:
		{
			OnClientConnected(network->GetPeer(), packet);
		}
		break;

		case ID_DISCONNECTION_NOTIFICATION:
		case ID_CONNECTION_LOST:
		{
			OnClientConnected(network->GetPeer(), packet);
		}
		break;
	}
}

void ServerConnectionHandler::OnClientConnected(RakNet::RakPeerInterface *peer, RakNet::Packet* packet) const
{
	RakNet::BitStream inStream;

	int clientVersion = -1;
	inStream.Read(clientVersion);

	RakNet::BitStream outStream;

	if (clientVersion != BUILD_VERSION)
	{
		outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_CONNECTION_REFUSED_LHMP));
		outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::REFUSED_CLIENT_VERSION));
		outStream.Write(BUILD_VERSION);

		peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		return;
	}

	std::string nickName;
	inStream.Read(nickName);

	Client client(nickName, packet->systemAddress);

	mClients->insert(std::make_pair(packet->guid, client));

	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_CONNECTION_ACCEPTED_LHMP));
	peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

void ServerConnectionHandler::OnClientDisconnected(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{
	mClients->erase(packet->guid);

	// TODO(zaklaus): Tell the gamemode client has been disconnected!
}
