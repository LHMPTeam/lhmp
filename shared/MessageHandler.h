#pragma once
#include "InternalPacket.h"

class Network;

class MessageHandler
{
public:
	virtual void ProcessMessage(Network* network, RakNet::Packet* packet) = 0;
};

