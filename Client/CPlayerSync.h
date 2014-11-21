#include "../shared/structures.h"
class CPlayerSync
{
private:
	PlayerSync* players;
public:
	void			Init(int);
	PlayerSync*		GetPlayers();
};