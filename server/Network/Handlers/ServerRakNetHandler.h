#pragma once

class ServerRakNetHandler : MessageHandler
{
public:
	ServerRakNetHandler(std::map<RakNet::RakNetGUID, Client*> *Clients);
	~ServerRakNetHandler();

	void ProcessMessage(Network* network, RakNet::Packet* packet) override;

private:
	void OnClientConnected(RakNet::RakPeerInterface *peer, RakNet::Packet* packet) const;
	void OnClientDisconnected(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);

	std::map<RakNet::RakNetGUID, Client*> *mClients;
};

