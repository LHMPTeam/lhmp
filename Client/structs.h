// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef STRUCTS_H
#define STRUCTS_H
#include<string>

struct Vector3D{
	float x;
	float y;
	float z;
};

struct _Player{
	Vector3D playerPos;
	int iPlayerID;
	char sName[80];
	float fHealth;
	int iCurrAmmo;
	int iMaxAmmo;
};

struct _server{
	int playerid;
	int max_players;
	int server_port;
	std::string server_name;
};

#endif