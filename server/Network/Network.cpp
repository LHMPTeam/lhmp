#include "Network.h"

Network::Network()
{
}


Network::~Network()
{
}

void Network::Init()
{
	mPeer = RakNet::RakPeerInterface::GetInstance();
	mPeer->Startup(1, mSocketDescriptor, 1);

}

void Network::Tick()
{

}

