#pragma once

class ClientPlayerHandler : MessageHandler
{
public:
	ClientPlayerHandler(std::map<RakNet::RakNetGUID, Player*>* players);
	~ClientPlayerHandler();
	void ProcessMessage(Network* network, RakNet::Packet* packet) override;
private:
	void CreatePlayer(Network* network, RakNet::Packet* packet);

	std::map<RakNet::RakNetGUID, Player*>* mPlayers;
};