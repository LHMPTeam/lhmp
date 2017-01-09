#include <stdinc.h>

ClientConnectionHandler::ClientConnectionHandler(std::map<RakNet::RakNetGUID, Player*>* players)
	: mPlayers(players)
{
	
}


ClientConnectionHandler::~ClientConnectionHandler()
{
}

void ClientConnectionHandler::ProcessMessage(Network* network, RakNet::Packet* packet)
{
	switch(packet->data[1])
	{		
		case MessageIDs::LHMPID_CONNECTION_REFUSED:
		{
			OnConnectionRefused(packet);
		}
		break;
		case MessageIDs::LHMPID_CONNECTION_ACCEPTED:
		{
			OnConnectionAccepted(network, packet);
		}
		break;
		case MessageIDs::LHMPID_CONNECTION_DISCONNECTED:
		{
			OnConnectionDisconnected(network, packet);
		}
		break;
	}
}

void ClientConnectionHandler::OnConnectionAccepted(Network * network, RakNet::Packet * packet)
{
	MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Connection accepted", 0xFF0000);
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

	RakNet::RakString localModelName;
	bitStream.Read(localModelName);

	Core::GetCore()->GetGame()->SetLocalPlayer(new LocalPlayer(localModelName.C_String()));
	Core::GetCore()->GetGame()->GetLocalPlayer()->Spawn();

	int playerCount;
	bitStream.Read(playerCount);

	printf("Players count: %d\n", playerCount);
	for (int i = 0; i < playerCount; i++) {

		RakNet::RakString modelName;
		RakNet::RakNetGUID playerGuid;

		bitStream.Read(playerGuid);
		bitStream.Read(modelName);

		printf("Spawning %s, %s\n", playerGuid.ToString(), modelName.C_String());

		Player* newPlayer = new Player(modelName.C_String());
		newPlayer->Spawn();

		mPlayers->insert(std::make_pair(playerGuid, newPlayer));
	}
}

void ClientConnectionHandler::OnConnectionRefused(RakNet::Packet * packet) const
{
	MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Connection refused", 0xFF0000);

	/*RakNet::BitStream inStream(packet->data, packet->length, true);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));

	MessageIDs::RefusedMessageIDs RefuseReason;
	inStream.Read(RefuseReason);

	switch (RefuseReason)
	{
		case MessageIDs::REFUSED_CLIENT_VERSION:
		{
			MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Horuce kody treba", 0xFF0000);
		}
		break;
	}*/
}

void ClientConnectionHandler::OnConnectionDisconnected(Network * network, RakNet::Packet * packet) const
{
	MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Someone disconnected", 0xFF0000);
	RakNet::BitStream inStream(packet->data, packet->length, true);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakNetGUID guid;
	inStream.Read(guid);

	MafiaSDK::GetMission()->GetGame()->RemoveTemporaryActor(mPlayers->at(guid)->GetActor());
	delete mPlayers->at(guid);
	mPlayers->erase(guid);
}
