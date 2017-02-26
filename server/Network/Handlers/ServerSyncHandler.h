#pragma once

class ServerSyncHandler : MessageHandler
{
public:
	ServerSyncHandler(std::map<RakNet::RakNetGUID, Client*> *Clients);
	~ServerSyncHandler();
	void ProcessMessage(Network* network, RakNet::Packet* packet) override;
private:
	void OnClientFootSync(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);
	std::map<RakNet::RakNetGUID, Client*> *mClients;
};