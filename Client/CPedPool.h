/**
	Lost Heaven Multiplayer

	Purpose: PED pool (array)

	@author Romop5
	@version 1.0 1/9/14
	@todo clean code
*/

#ifndef __PEDPOOL_H
#define	__PEDPOOL_H
#include "CPed.h"

class CPedPool
{
private:
	CPed* pedpool[100];
public:
	CPedPool();
	~CPedPool();
	void	New(int);
	CPed*	Return(int);
	int		GetPedIdByBase(DWORD);
	void	Delete(int);
};
#endif