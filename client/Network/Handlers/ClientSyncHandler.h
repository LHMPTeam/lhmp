#pragma once

class ClientSyncHandler : MessageHandler
{
public:
	ClientSyncHandler(std::map<RakNet::RakNetGUID, Player*>* players);
	~ClientSyncHandler();
	void ProcessMessage(Network* network, RakNet::Packet* packet) override;
private:
	void OnClientFootSync(Network* network, RakNet::Packet* packet);

	std::map<RakNet::RakNetGUID, Player*>* mPlayers;
};