#pragma once
#include "../Game/Player.h"

#include <RakPeerInterface.h>

#include <string>

class Client
{
public:
	Client(std::wstring nickName, RakNet::SystemAddress systemAddress);
	~Client();

	Player* GetPlayer() { return mPlayer; }
	std::wstring GetNickName() const { return mNickName; }
private:
	RakNet::SystemAddress mSystemAddress;
	std::wstring mNickName;
	Player* mPlayer;
};

