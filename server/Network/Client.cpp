#include "Client.h"

Client::Client(std::string nickName, RakNet::SystemAddress systemAddress)
	:
	mNickName(nickName),
	mSystemAddress(systemAddress)
{
}

Client::~Client()
{
}
