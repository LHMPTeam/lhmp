#include <stdinc.h>

ServerRakNetHandler::ServerRakNetHandler(std::map<RakNet::RakNetGUID, Client*>* Clients)
	: mClients(Clients)
{
}

ServerRakNetHandler::~ServerRakNetHandler()
{

}

void ServerRakNetHandler::ProcessMessage(Network* network, RakNet::Packet* packet)
{
	switch (packet->data[0])
	{
	case ID_DISCONNECTION_NOTIFICATION:
	case ID_CONNECTION_LOST:
	{
		OnClientDisconnected(network->GetPeer(), packet);
	}
	break;
	}
}

void ServerRakNetHandler::OnClientConnected(RakNet::RakPeerInterface *peer, RakNet::Packet* packet) const
{
}

void ServerRakNetHandler::OnClientDisconnected(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{
	Core::GetCore()->LogW(L"Player <%s> disconnected ID: %s", mClients->at(packet->guid)->GetNickName().c_str(), packet->guid.ToString());

	delete mClients->at(packet->guid);
	mClients->erase(packet->guid);

	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION));
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION_DISCONNECTED));
	bitStream.Write(packet->guid);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);

	// TODO(zaklaus): Tell the gamemode client has been disconnected!
}
