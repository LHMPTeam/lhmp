#ifndef POOL_H
#define	POOL_H

template <class object> class CPool
{
private:
	object**	pool;
	int			size;
public:
	CPool();
	~CPool();
	void	New(int);
	object*	Return(int);
	int		GetObjectIdByBase(DWORD);
	void	Delete(int);
};
#endif