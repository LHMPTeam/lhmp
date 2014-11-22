#include "CPlayerPool.h"

CPlayerPool::CPlayerPool()
{
	maxPlayers = MAX_PLAYERS;
	for (int i = 0; i < maxPlayers; i++)
	{
		playerpool[i] = NULL;
	}
}
CPlayerPool::~CPlayerPool()
{
	for (int i = 0; i < maxPlayers; i++)
	{
		if(playerpool[i] != NULL)
		{
			delete playerpool[i];
		}
	}
}

void CPlayerPool::New(int index)
{
	//playerpool[index] = new CPlayer[1];
	CPlayer* pointer = new CPlayer;
	playerpool[index] = pointer;
}

CPlayer* CPlayerPool::Return(int index)
{
	return playerpool[index];
}

void CPlayerPool::Delete(int index)
{
	delete playerpool[index];
	playerpool[index] = NULL;
}

int CPlayerPool::GetID(CPlayer* base)
{
	for (int i = 0; i < maxPlayers; i++)
	{
		if (playerpool[i] == base)
		{
			return i;
		}
	}
	return -1;
}