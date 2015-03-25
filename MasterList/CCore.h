// puts all classes together using singleton
#ifndef _CCORE_H
#define _CCORE_H

#include "CNetwork.h"
#include "CSeversList.h"

// port can't be changed
#define PORT	50000

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
		return this->p_network.StartServer(PORT);
	}

	void Pulse()
	{
		this->p_network.Pulse();
		this->p_serverslist.Pulse();
	}

	CNetwork* getNetwork()
	{
		return &this->p_network;
	}

	CServersList* getServersList()
	{
		return &this->p_serverslist;
	}

private:
	CCore() {};                

	CCore(CCore const&);             
	void operator=(CCore const&); 

	CNetwork		p_network;
	CServersList	p_serverslist;

};

#endif