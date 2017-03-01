#include "Client.h"

Client::Client(std::wstring nickName, RakNet::SystemAddress systemAddress)
	:
	mNickName(nickName),
	mSystemAddress(systemAddress)
{
	mPlayer = new Player("Tommy.i3d");
}

Client::~Client()
{
	delete mPlayer;
}
