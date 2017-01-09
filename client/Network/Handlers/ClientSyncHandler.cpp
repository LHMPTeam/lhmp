#include <stdinc.h>

ClientSyncHandler::ClientSyncHandler(std::map<RakNet::RakNetGUID, Player*>* players)
	: mPlayers(players)
{
}

ClientSyncHandler::~ClientSyncHandler()
{
}

void ClientSyncHandler::ProcessMessage(Network * network, RakNet::Packet * packet)
{
	switch (packet->data[1])
	{
	case MessageIDs::LHMP_PLAYER_ONFOOTSYNC:
	{
		OnClientFootSync(network, packet);
	}
	break;
	}
}

void ClientSyncHandler::OnClientFootSync(Network * network, RakNet::Packet * packet)
{
	if (network->IsReadySync() == false) return;
	//printf("Senced!\n");
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	//printf("Ignoring bytes\n");
	RakNet::RakNetGUID playerGuid;
	bitStream.Read(playerGuid);
	//printf("Readed guid\n");
	OnFootSync footSync;
	bitStream.Read(footSync);
	//printf("Footsync \n");
	if (mPlayers->at(playerGuid) != nullptr)
	{
		//printf("lalala\n");
		printf("Position: %f %f %f\n", footSync.Position.x, footSync.Position.y, footSync.Position.z);
		mPlayers->at(playerGuid)->SetPosition(footSync.Position);
		mPlayers->at(playerGuid)->SetRotation(footSync.Rotation);
		mPlayers->at(playerGuid)->SetAnimationState(footSync.animationState);
	}
}
