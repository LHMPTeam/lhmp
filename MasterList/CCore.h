// puts all classes together using singleton
#ifndef _CCORE_H
#define _CCORE_H

#include "CNetwork.h"
#include "CServers.h"

// Listening port (DON'T CHANGE)
#define PORT	50000

// Time between server query checks (seconds)
#define TIME_BETWEEN	10

#define QUERY_DELAY		1000*TIME_BETWEEN

#ifndef _WIN32
#include <unistd.h>
#include <time.h>
#include <pthread.h>
extern pthread_t tid;

extern void Sleep(unsigned int ms);
extern unsigned int GetTickCount();

#endif

class CCore
{
public:
	static CCore& getInstance()
	{
		static CCore    instance; 
		return instance;
	}

	bool StartMaster()
	{
		this->getServers()->Prepare();
		return this->p_network.StartServer(PORT);
	}

	void Pulse()
	{
		this->p_network.Pulse();
		this->p_servers.Pulse();
	}

	CNetwork* getNetwork()
	{
		return &this->p_network;
	}

	CServers* getServers()
	{
		return &this->p_servers;
	}


private:
	CCore() {};                

	CCore(CCore const&);             
	void operator=(CCore const&); 

	CNetwork		p_network;
	CServers		p_servers;

};

#endif