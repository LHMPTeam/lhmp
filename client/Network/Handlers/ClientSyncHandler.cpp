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
	case MessageIDs::LHMPID_SYNC_ONFOOT:
	{
		OnClientFootSync(network, packet);
	}
	break;
	}
}

void ClientSyncHandler::OnClientFootSync(Network * network, RakNet::Packet * packet)
{
	if (!mPlayers->size()) return;

	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

	RakNet::RakNetGUID playerGuid;
	bitStream.Read(playerGuid);

	OnFootSyncStruct footSync;
	bitStream.Read(footSync);

	if (mPlayers->at(playerGuid) != nullptr)
	{
		mPlayers->at(playerGuid)->SetPosition(footSync.Position);
		mPlayers->at(playerGuid)->SetRotation(footSync.Rotation);
		mPlayers->at(playerGuid)->SetAnimationState(footSync.animationState);
		mPlayers->at(playerGuid)->SetIsCrouching(footSync.isCrouching);
	}
}
