#include "ServerConnectionHandler.h"
#include <MessageIDs.h>
#include <BuildVersion.h>

ServerConnectionHandler::ServerConnectionHandler(std::map<RakNet::RakNetGUID, Client*>* Clients)
	: mClients(Clients)
{
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
			OnClientDisconnected(network->GetPeer(), packet);
		}
		break;
	}
}

void ServerConnectionHandler::OnClientConnected(RakNet::RakPeerInterface *peer, RakNet::Packet* packet) const
{
	RakNet::BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientVersion = -1;
	inStream.Read((int)clientVersion);

	RakNet::BitStream outStream;

	if (clientVersion != BUILD_VERSION)
	{
		Core::GetCore()->Log("asksal");

		outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_CONNECTION_REFUSED_LHMP));
		outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::REFUSED_CLIENT_VERSION));
		outStream.Write((int)BUILD_VERSION);

		peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		return;
	}

	RakNet::RakString nickName;
	inStream.Read(nickName);

	inStream.Reset();

	RakNet::BitStream utist;
	utist.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_CREATE_PLAYER));
	utist.Write(packet->guid);
	utist.Write("Tommy.i3d");
	peer->Send(&utist, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);

	Client* client = new Client(nickName.C_String(), packet->systemAddress);

	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_CONNECTION_ACCEPTED_LHMP));
	outStream.Write(client->GetPlayer()->GetModel().c_str());
	outStream.Write(mClients->size());
	
	for (auto client : *mClients) {

		outStream.Write(client.first);
		outStream.Write(client.second->GetPlayer()->GetModel().c_str());
	}

	mClients->insert(std::make_pair(packet->guid, client));

	//outStream.Write(client->GetPlayer()->GetModel().c_str());
	peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

void ServerConnectionHandler::OnClientDisconnected(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{
	delete mClients->at(packet->guid);
	mClients->erase(packet->guid);

	// TODO(zaklaus): Tell the gamemode client has been disconnected!
}
