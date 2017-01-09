#pragma once

class ClientConnectionHandler : MessageHandler
{
public:
	ClientConnectionHandler(std::map<RakNet::RakNetGUID, Player*>* players);
	~ClientConnectionHandler();
	void ProcessMessage(Network* network, RakNet::Packet* packet) override;
private:
	void OnRequestAccepted(Network * network, RakNet::Packet * packet) const;
	void OnConnectionAccepted(Network* network, RakNet::Packet* packet);
	void OnCreatePlayer(Network * network, RakNet::Packet * packet);
	void OnConnectionRefused(RakNet::Packet* packet) const;

	std::map<RakNet::RakNetGUID, Player*>* mPlayers;
};