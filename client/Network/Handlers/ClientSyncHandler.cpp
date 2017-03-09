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
		mPlayers->at(playerGuid)->GetInterpolator()->NewValues();
		mPlayers->at(playerGuid)->GetInterpolator()->SetInitialValue(0, mPlayers->at(playerGuid)->GetPosition().x);
		mPlayers->at(playerGuid)->GetInterpolator()->SetInitialValue(1, mPlayers->at(playerGuid)->GetPosition().y);
		mPlayers->at(playerGuid)->GetInterpolator()->SetInitialValue(2, mPlayers->at(playerGuid)->GetPosition().z);
		mPlayers->at(playerGuid)->GetInterpolator()->SetTargetValue(0, footSync.Position.x);
		mPlayers->at(playerGuid)->GetInterpolator()->SetTargetValue(1, footSync.Position.y);
		mPlayers->at(playerGuid)->GetInterpolator()->SetTargetValue(2, footSync.Position.z);
		mPlayers->at(playerGuid)->SetRotation(footSync.Rotation);
		mPlayers->at(playerGuid)->SetAnimationState(footSync.animationState);
		mPlayers->at(playerGuid)->SetIsCrouching(footSync.isCrouching);
		mPlayers->at(playerGuid)->SetIsAiming(footSync.isAiming);
	}
}
