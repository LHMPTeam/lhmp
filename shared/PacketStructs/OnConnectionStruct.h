#pragma once

struct OnConnectionStruct
{
	char playerModel[255];
	int playerCount;
	RakNet::RakNetGUID* guids;
	
	//std::map<RakNet::RakNetGUID, std::string> players;
};