#include <stdinc.h>

ServerSyncHandler::ServerSyncHandler(std::map<RakNet::RakNetGUID, Client*>* Clients)
	: mClients(Clients)
{
}

ServerSyncHandler::~ServerSyncHandler()
{
}

void ServerSyncHandler::ProcessMessage(Network * network, RakNet::Packet * packet)
{
	switch (packet->data[1])
	{
	case MessageIDs::LHMPID_SYNC_ONFOOT:
	{
		OnClientFootSync(network->GetPeer(), packet);
	}
	break;
	}
}

void ServerSyncHandler::OnClientFootSync(RakNet::RakPeerInterface *peer, RakNet::Packet * packet)
{
	auto player = mClients->at(packet->guid)->GetPlayer();
	OnFootSyncStruct footSync;
	RakNet::BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));

	inStream.Read(footSync);

	player->SetPosition(footSync.Position);
	player->SetRotation(footSync.Rotation);
	player->SetAnimationState(footSync.animationState);
	player->SetIsAiming(footSync.isAiming);

	RakNet::BitStream outStream;

	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_SYNC));
	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_SYNC_ONFOOT));
	outStream.Write(packet->guid);
	outStream.Write(footSync);

	peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
}
