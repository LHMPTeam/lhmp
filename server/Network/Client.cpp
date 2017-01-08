#include "Client.h"

Client::Client(std::string nickName, RakNet::SystemAddress systemAddress)
	:
	mNickName(nickName),
	mSystemAddress(systemAddress)
{
	mPlayer = new Player;
}

Client::~Client()
{
	delete mPlayer;
}
