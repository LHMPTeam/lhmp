// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CPlayerSync.h"

void CPlayerSync::Init(int count)
{
	players = new PlayerSync[count];
}
PlayerSync*		CPlayerSync::GetPlayers()
{
	return players;
}