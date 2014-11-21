#ifndef PEDPOOL_H
#define	PEDPOOL_H
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