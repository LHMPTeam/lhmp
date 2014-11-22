#include "../shared/limits.h"
#include "CPlayer.h"
class CPlayerPool
{
private:
	CPlayer*	playerpool[MAX_PLAYERS];
	int			maxPlayers;
public:
	CPlayerPool();
	~CPlayerPool();
	void New(int);
	void Delete(int);
	CPlayer* Return(int);
	int GetID(CPlayer*);
};