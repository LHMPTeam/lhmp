#include <stdinc.h>

ServerPlayerHandler::ServerPlayerHandler(std::map<RakNet::RakNetGUID, Client*>* Clients)
	: mClients(Clients)
{
}

ServerPlayerHandler::~ServerPlayerHandler()
{
}

void ServerPlayerHandler::ProcessMessage(Network * network, RakNet::Packet * packet)
{
	switch (packet->data[1])
	{
	case MessageIDs::LHMPID_PLAYER_CHATMSG:
	{
		OnChatMessage(network->GetPeer(), packet);
	}
	break;

	case MessageIDs::LHMPID_PLAYER_ONHIT:
	{
		OnHit(network->GetPeer(), packet);
	}
	break;

	case MessageIDs::LHMPID_PLAYER_RESPAWN:
	{
		OnRespawn(network->GetPeer(), packet);
	}
	break;
	}
}

void ServerPlayerHandler::OnChatMessage(RakNet::RakPeerInterface *peer, RakNet::Packet * packet)
{
	auto nickName = mClients->at(packet->guid)->GetNickName();
	
	RakNet::BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));


	size_t messageLenght;
	inStream.Read(messageLenght);
	wchar_t* playerMessage = new wchar_t[messageLenght];
	inStream.Read(playerMessage);

	std::wstring sendMessage(playerMessage);
	
	RakNet::BitStream outStream;
	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
	outStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_CHATMSG));
	
	Core::GetCore()->LogW(L"<%s> %s", nickName.c_str(), playerMessage);

	//GUID
	outStream.Write(packet->guid);

	//Message
	outStream.Write(sendMessage.size());
	outStream.Write(sendMessage.c_str());

	peer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
}

void ServerPlayerHandler::OnHit(RakNet::RakPeerInterface *peer, RakNet::Packet* packet)
{
	RakNet::BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));

	//Get player 
	auto playerPtr = mClients->at(packet->guid)->GetPlayer();

	int hitType;
	Vector3D unk1, unk2, unk3;
	float damage;
	RakNet::RakNetGUID atackerGUID;
	unsigned long hittedPart;

	inStream.Read(hitType);
	inStream.Read(unk1);
	inStream.Read(unk2);
	inStream.Read(unk3);
	inStream.Read(damage);
	inStream.Read(atackerGUID);
	inStream.Read(hittedPart);

	//Update server health 
	if ((playerPtr->GetHealth() - damage) < 0.0f)
		playerPtr->SetHealth(0.0f);
	else
		playerPtr->SetHealth(playerPtr->GetHealth() - damage);

	//Broadcast it back !
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_ONHIT));
	bitStream.Write(packet->guid);
	bitStream.Write(hitType);
	bitStream.Write(unk1);
	bitStream.Write(unk2);
	bitStream.Write(unk3);
	bitStream.Write(damage);
	bitStream.Write(atackerGUID);
	bitStream.Write(hittedPart);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
}

void ServerPlayerHandler::OnRespawn(RakNet::RakPeerInterface *peer, RakNet::Packet* packet)
{
	RakNet::BitStream inStream(packet->data, packet->length, false);
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));
	inStream.IgnoreBytes(sizeof(RakNet::MessageID));

	//Get player 
	auto clientPtr = mClients->at(packet->guid);
	Core::GetCore()->LogW(L"<%s> respawning", clientPtr->GetNickName().c_str());

	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_RESPAWN));
	bitStream.Write(packet->guid);
	peer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
}