// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __ENGINESTACK_H
#define __ENGINESTACK_H
struct EngineStackMessage
{
	unsigned int		messageId;
	DWORD				data;
	EngineStackMessage* next;
};

class CEngineStack
{
private:
	unsigned int		messageCount;
	EngineStackMessage*	start;
	EngineStackMessage* end;

	// used for deleting messages
	template<class T>
	void DeleteData(T *p)
	{
		delete p;
	}
public:
	CEngineStack();
	~CEngineStack();
	void				AddMessage(unsigned int,DWORD);
	void				DoMessage();
	unsigned int		GetCount();
	void				Flush();

};
#endif