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
	Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"{FFCC2002}[LHMP] {FFf9f8f7}Connection accepted.");
	network->SetIsConnected(true);

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
	for (int i = 0; i < playerCount; i++) 
	{
		RakNet::RakNetGUID playerGuid;
		bitStream.Read(playerGuid);

		size_t nickNameLenght;
		bitStream.Read(nickNameLenght);
		wchar_t* allocatedNickName = new wchar_t[nickNameLenght];
		bitStream.Read(allocatedNickName);
		
		std::wstring nickNameString = std::wstring(allocatedNickName);

		RakNet::RakString modelName;
		bitStream.Read(modelName);

		printf("Spawning %s, %s\n", playerGuid.ToString(), modelName.C_String());

		Player* newPlayer = new Player(modelName.C_String());
		newPlayer->Spawn();
		newPlayer->SetNickName(nickNameString);

		mPlayers->insert(std::make_pair(playerGuid, newPlayer));
	}
}

void ClientConnectionHandler::OnConnectionRefused(RakNet::Packet * packet) const
{
	Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"{FFCC2002}[LHMP] {FFf9f8f7}Connection refused !");

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

	RakNet::BitStream inStream(packet->data, packet->length, true);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakNetGUID guid;
	inStream.Read(guid);
	Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"{FFCC2002}[LHMP] {FFf9f8f7}<" + mPlayers->at(guid)->GetNickName() + L"> Disconnected !");
	MafiaSDK::GetMission()->GetGame()->RemoveTemporaryActor(mPlayers->at(guid)->GetActor());
	delete mPlayers->at(guid);
	mPlayers->erase(guid);
}
