#pragma once
#include <MessageHandler.h>
class Network;

class ClientConnectionHandler : MessageHandler
{
public:
	ClientConnectionHandler();
	~ClientConnectionHandler();
	void ProcessMessage(Network* network, RakNet::Packet* packet) override;

private:
	void OnConnectionAccepted(Network* network, RakNet::Packet* packet) const;
};

