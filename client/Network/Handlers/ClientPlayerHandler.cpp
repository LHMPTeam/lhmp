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

	printf("Spawned player !\n");

	mPlayers->insert(std::make_pair(playerGuid, newPlayer));
}