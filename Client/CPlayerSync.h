// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "../shared/structures.h"
class CPlayerSync
{
private:
	PlayerSync* players;
public:
	void			Init(int);
	PlayerSync*		GetPlayers();
};