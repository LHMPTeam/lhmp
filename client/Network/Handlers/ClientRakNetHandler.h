#pragma once

class ClientRakNetHandler : MessageHandler
{
public:
	ClientRakNetHandler(std::map<RakNet::RakNetGUID, Player*>* players);
	~ClientRakNetHandler();
	void ProcessMessage(Network* network, RakNet::Packet* packet) override;
private:
	void OnRequestAccepted(Network * network, RakNet::Packet * packet) const;
	void OnConnectionAttemptFailed(Network *network, RakNet::Packet* packet) const;
	std::map<RakNet::RakNetGUID, Player*>* mPlayers;
};