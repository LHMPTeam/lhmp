#include <stdinc.h>

ClientRakNetHandler::ClientRakNetHandler(std::map<RakNet::RakNetGUID, Player*>* players)
	: mPlayers(players)
{
}

ClientRakNetHandler::~ClientRakNetHandler()
{
}

void ClientRakNetHandler::ProcessMessage(Network * network, RakNet::Packet * packet)
{
	switch (packet->data[0])
	{
	case ID_CONNECTION_REQUEST_ACCEPTED:
	{
		OnRequestAccepted(network, packet);
	}
	break;
	}
}

void ClientRakNetHandler::OnRequestAccepted(Network *network, RakNet::Packet* packet) const
{
	network->SetServerAddress(packet->systemAddress);
	MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Request accepted", 0xFF0000);
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION));
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION_INIT));
	bitStream.Write((int)BUILD_VERSION);
	bitStream.Write(RakNet::RakString(network->GetNickName().c_str()));
	network->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}