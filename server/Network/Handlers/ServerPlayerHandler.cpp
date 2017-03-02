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
