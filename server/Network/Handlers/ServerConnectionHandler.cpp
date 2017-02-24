#include <stdinc.h>

ServerConnectionHandler::ServerConnectionHandler(std::map<RakNet::RakNetGUID, Client*>* Clients)
	: mClients(Clients)
{
}

ServerConnectionHandler::~ServerConnectionHandler()
{

}

void ServerConnectionHandler::ProcessMessage(Network* network, RakNet::Packet* packet)
{
	switch(packet->data[1])
	{
		case MessageIDs::LHMPID_CONNECTION_INIT:
		{
			OnClientInit(network->GetPeer(), packet);
		}
		break;
	}
}

void ServerConnectionHandler::OnClientInit(RakNet::RakPeerInterface *peer, RakNet::Packet* packet) const
{
	RakNet::BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientVersion = -1;
	inStream.Read((int)clientVersion);

	RakNet::BitStream outStream;

	if (clientVersion != BUILD_VERSION)
	{
		Core::GetCore()->Log("[SERVER] Player connection refused (Wrong Version)");

		outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION));
		outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION_REFUSED));
		//outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::REFUSED_CLIENT_VERSION));
		outStream.Write((int)BUILD_VERSION);

		peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		return;
	}

	RakNet::RakString nickName;
	inStream.Read(nickName);

	inStream.Reset();

	RakNet::BitStream utist;
	utist.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
	utist.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_CREATE));
	utist.Write(packet->guid);
	utist.Write("Tommy.i3d");
	peer->Send(&utist, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);

	Client* client = new Client(nickName.C_String(), packet->systemAddress);

	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION));
	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION_ACCEPTED));
	outStream.Write(client->GetPlayer()->GetModel().c_str());
	outStream.Write(mClients->size());
	
	for (auto client : *mClients) {

		outStream.Write(client.first);
		outStream.Write(client.second->GetPlayer()->GetModel().c_str());
	}

	Core::GetCore()->Log("Player <%s> connected ID: %s", nickName.C_String(), packet->guid.ToString());

	mClients->insert(std::make_pair(packet->guid, client));

	//outStream.Write(client->GetPlayer()->GetModel().c_str());
	peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}