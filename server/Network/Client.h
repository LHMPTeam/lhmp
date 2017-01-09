#pragma once
#include "../Game/Player.h"

#include <RakPeerInterface.h>

#include <string>

class Client
{
public:
	Client(std::string nickName, RakNet::SystemAddress systemAddress);
	~Client();

	Player* GetPlayer() { return mPlayer; }

private:
	RakNet::SystemAddress mSystemAddress;
	std::string mNickName;
	Player* mPlayer;
};

