#include "CPlayerSync.h"

void CPlayerSync::Init(int count)
{
	players = new PlayerSync[count];
}
PlayerSync*		CPlayerSync::GetPlayers()
{
	return players;
}