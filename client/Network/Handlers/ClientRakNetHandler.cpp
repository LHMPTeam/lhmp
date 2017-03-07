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

		case ID_CONNECTION_ATTEMPT_FAILED:
		{
			OnConnectionAttemptFailed(network, packet);
		}
		break;
	}
}

void ClientRakNetHandler::OnConnectionAttemptFailed(Network *network, RakNet::Packet* packet) const
{
	Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"{FFCC2002}[LHMP] {FFf9f8f7}Unable to connect !");
	Core::GetCore()->GetNetwork()->Connect(Core::GetCore()->GetNetwork()->GetServerConnectIP().c_str(), 27015);
}

void ClientRakNetHandler::OnRequestAccepted(Network *network, RakNet::Packet* packet) const
{
	network->SetServerAddress(packet->systemAddress);
	
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION));
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_CONNECTION_INIT));
	bitStream.Write((int)BUILD_VERSION);
	bitStream.Write(network->GetNickName().size());
	bitStream.Write(network->GetNickName().c_str());
	network->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}