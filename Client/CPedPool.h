// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

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