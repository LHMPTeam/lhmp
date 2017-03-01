#include <stdinc.h>

ClientPlayerHandler::ClientPlayerHandler(std::map<RakNet::RakNetGUID, Player*>* players)
	: mPlayers(players)
{

}


ClientPlayerHandler::~ClientPlayerHandler()
{
}

void ClientPlayerHandler::ProcessMessage(Network* network, RakNet::Packet* packet)
{
	switch (packet->data[1])
	{
	case MessageIDs::LHMPID_PLAYER_CREATE:
	{
		CreatePlayer(network, packet);
	}
	break;
	case MessageIDs::LHMPID_PLAYER_CHATMSG:
	{
		OnChatMessage(network->GetPeer(), packet);
	}
	break;
	}
}

void ClientPlayerHandler::CreatePlayer(Network * network, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakNetGUID playerGuid;
	bitStream.Read(playerGuid);
	RakNet::RakString modelName;
	bitStream.Read(modelName);
	Player* newPlayer = new Player(modelName.C_String());
	newPlayer->Spawn();

	mPlayers->insert(std::make_pair(playerGuid, newPlayer));
}

void ClientPlayerHandler::OnChatMessage(RakNet::RakPeerInterface * peer, RakNet::Packet * packet)
{
	RakNet::BitStream bitStream(packet->data, packet->length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
		
	size_t playerNameSize;
	bitStream.Read(playerNameSize);
	wchar_t* playerNameBuff = new wchar_t[playerNameSize];
	bitStream.Read(playerNameBuff);

	size_t playerMessageSize;
	bitStream.Read(playerMessageSize);
	wchar_t* playerMessageBuff = new wchar_t[playerMessageSize];
	bitStream.Read(playerMessageBuff);

	Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"<" + std::wstring(playerNameBuff) + L"> " + std::wstring(playerMessageBuff));
}
