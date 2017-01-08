#pragma once
#include <MessageHandler.h>
#include "../Network.h"

class ServerConnectionHandler : MessageHandler
{
public:
	ServerConnectionHandler(std::map<RakNet::RakNetGUID, Client> *Clients);
	~ServerConnectionHandler();

	void ProcessMessage(Network* network, RakNet::Packet* packet) override;

private:
	void OnClientConnected(RakNet::RakPeerInterface *peer, RakNet::Packet* packet) const;
	void OnClientDisconnected(RakNet::RakPeerInterface *peer, RakNet::Packet* packet);

	std::map<RakNet::RakNetGUID, Client> *mClients;
};

