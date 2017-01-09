#include "ServerSyncHandler.h"
#include <MessageIDs.h>
#include <BuildVersion.h>

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
	case MessageIDs::LHMP_PLAYER_ONFOOTSYNC:
	{
		OnClientFootSync(network->GetPeer(), packet);
	}
	break;
	}
}

void ServerSyncHandler::OnClientFootSync(RakNet::RakPeerInterface *peer, RakNet::Packet * packet)
{
	auto player = mClients->at(packet->guid)->GetPlayer();
	OnFootSync footSync;
	RakNet::BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));



	inStream.Read(footSync);
	printf("Sync: %f %f %f\n", footSync.Position.x, footSync.Position.y, footSync.Position.z);

	player->SetPosition(footSync.Position);
	player->SetRotation(footSync.Rotation);
	player->SetAnimationState(footSync.animationState);

	
	RakNet::BitStream outStream;

	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_SYNC_LHMP));
	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMP_PLAYER_ONFOOTSYNC));
	outStream.Write(packet->guid);
	outStream.Write(footSync);

	peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
}
