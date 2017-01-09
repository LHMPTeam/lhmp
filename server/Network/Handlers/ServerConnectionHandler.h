#pragma once

class ServerConnectionHandler : MessageHandler
{
public:
	ServerConnectionHandler(std::map<RakNet::RakNetGUID, Client*> *Clients);
	~ServerConnectionHandler();

	void ProcessMessage(Network* network, RakNet::Packet* packet) override;

private:
	void OnClientInit(RakNet::RakPeerInterface *peer, RakNet::Packet* packet) const;

	std::map<RakNet::RakNetGUID, Client*> *mClients;
};

