#ifndef ENGINESTACK_H
#define ENGINESTACK_H
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
public:
	CEngineStack();
	~CEngineStack();
	void				AddMessage(unsigned int,DWORD);
	void				DoMessage();
	unsigned int		GetCount();
};
#endif