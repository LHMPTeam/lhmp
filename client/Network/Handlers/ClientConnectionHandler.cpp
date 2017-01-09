#include <stdinc.h>

ClientConnectionHandler::ClientConnectionHandler(std::map<RakNet::RakNetGUID, Player*>* players)
{
	mPlayers = players;
}


ClientConnectionHandler::~ClientConnectionHandler()
{
}

void ClientConnectionHandler::ProcessMessage(Network* network, RakNet::Packet* packet)
{
	switch(packet->data[0])
	{
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Request accepted", 0xFF0000);
			OnRequestAccepted(network, packet);
		}
		break;
		
		case MessageIDs::ID_CONNECTION_REFUSED_LHMP:
		{
			OnConnectionRefused(packet);
		}
		break;
		
		case MessageIDs::MessageIDs::ID_CONNECTION_ACCEPTED_LHMP:
		{
			// TODO(zaklaus): We've got player data!
			OnConnectionAccepted(network, packet);
		}
		break;
		case MessageIDs::ID_CREATE_PLAYER:
		{
			OnCreatePlayer(network, packet);
		}
		break;
	}
}

void ClientConnectionHandler::OnRequestAccepted(Network *network, RakNet::Packet* packet) const
{
	//printf("Request accepted!\n");
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_CONNECTION_INIT_LHMP));
	bitStream.Write((int)BUILD_VERSION);
	bitStream.Write(RakNet::RakString(network->GetNickName().c_str()));
	network->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
	//printf("Sending init...\n");
}

void ClientConnectionHandler::OnConnectionAccepted(Network * network, RakNet::Packet * packet)
{
	//printf("Connection accepted!\n");

	network->SetServerAddress(packet->systemAddress);
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

	RakNet::RakString localModelName;
	bitStream.Read(localModelName);

	Core::GetCore()->GetGame()->SetLocalPlayer(new LocalPlayer(localModelName.C_String()));
	Core::GetCore()->GetGame()->GetLocalPlayer()->Spawn();
	//printf("spawned ped");

	int playerCount;
	bitStream.Read(playerCount);

	for (int i = 0; i < playerCount; i++) {

		RakNet::RakString modelName;
		RakNet::RakNetGUID playerGuid;

		bitStream.Read(playerGuid);
		bitStream.Read(modelName);

		Player* newPlayer = new Player(modelName.C_String());
		newPlayer->Spawn();

		mPlayers->insert(std::make_pair(playerGuid, newPlayer));
	}
	//printf("created others!");
	network->SetSyncReady(true);
	/*for (auto player : connectionStruct.players)
	{
		Player* newPlayer = new Player(player.second);
		newPlayer->Spawn();
		mPlayers->insert(std::make_pair(player.first, newPlayer));
	}*/
	

}

void ClientConnectionHandler::OnCreatePlayer(Network * network, RakNet::Packet * packet)
{
	//printf("Creating player...");
	//MessageBoxA(NULL, "CREATE", "CReATE", MB_OK);
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakNetGUID playerGuid;
	bitStream.Read(playerGuid);
	RakNet::RakString modelName;
	bitStream.Read(modelName);
	Player* newPlayer = new Player(modelName.C_String());
	newPlayer->Spawn();

	mPlayers->insert(std::make_pair(playerGuid, newPlayer));
}

void ClientConnectionHandler::OnConnectionRefused(RakNet::Packet * packet) const
{
	MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Connection refused", 0xFF0000);

	RakNet::BitStream inStream(packet->data, packet->length, true);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	// nvm, to mi pripomnina ze ani pri acceptnuti to nemame..
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
	}
}
