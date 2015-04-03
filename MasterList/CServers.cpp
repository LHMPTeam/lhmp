#include "CCore.h"
#include "CServers.h"

#include "../sdks/UDPWrapper/clhmpquery.h"

// start LHMP query interface and get ready
void Callback(unsigned int ID, void* data,unsigned char reason)
{
	CCore::getInstance().getServers()->HandleCallback(ID, data, reason);
}


void CServers::HandleCallback(unsigned int ID, void* data, unsigned char reason)
{
	if (this->pool.size() > 0)
	{
		std::vector <CServer>::iterator it = this->GetServerWithID(ID);
		if (it->GetID() == ID)
		{
			// ok, it exists
			if (reason == QUERY_OVERALL)
			{
				if (it->IsPending())
				{
					it->ChangeToPersistent();
					CCore::getInstance().getNetwork()->OnSuccess(it->GetHost());
				}
				//OverallPacket* packet = (OverallPacket*)data;

			}
			else {
				if (it->IsPending())
					CCore::getInstance().getNetwork()->OnFail(it->GetHost());
				// now just simply delete it
				this->pool.erase(it);
			}

		}
	}

}

void CServers::Prepare()
{
	// get instance
	this->query = CLHMPQuery::getInstance();

	query->Prepare((void*)&Callback);

	this->lastCheck = GetTickCount();
}

void CServers::AddServer(sockaddr_in addr, unsigned short port)
{
	for (std::vector<CServer>::iterator it = this->pool.begin(); it != this->pool.end(); ++it)
	{
#ifdef _WIN32
		if (it->GetHost().sin_addr.S_un.S_addr == addr.sin_addr.S_un.S_addr)
#else
		if (it->GetHost().sin_addr.s_addr == addr.sin_addr.s_addr)
#endif
		{
			if (it->GetPort() == port)
			{
				// if IsPending is true, then the previous response haven't come yet
				if (it->IsPending())
				{
					this->query->queryInfo(it->GetIP(), it->GetPort() + 1, it->GetID());
					return;
				}
				else {
					// if already in list
					CCore::getInstance().getNetwork()->GetServerHandle()->SendData(5, "LHMP2", (sockaddr*)&addr);
					return;
				}
			}
		}
	}
	this->pool.push_back(CServer(this->referenceID++,true, addr, port));

	this->query->queryInfo(this->pool.back().GetIP(), this->pool.back().GetPort()+1, this->pool.back().GetID());
}


std::vector<CServer>::iterator  CServers::GetServerWithID(unsigned int ID)
{
	for (std::vector<CServer>::iterator it = this->pool.begin(); it != this->pool.end(); ++it)
	{
		if (it->GetID() == ID)
			return it;
	}
	return this->pool.begin();
}

std::vector <CServer>* CServers::GetPool()
{
	return &this->pool;
}

void CServers::Pulse()
{
	unsigned int time = GetTickCount();
	// Fix tick count overflow
	if (time < this->lastCheck)
		this->lastCheck = time;

	// Query servers every @QUERY_DELAY miliseconds to remove offline from list
	if ((time - this->lastCheck) > QUERY_DELAY)
	{
		if (this->pool.size() > 0)
		{
			for (std::vector<CServer>::iterator it = this->pool.begin(); it != this->pool.end(); ++it)
			{
				this->query->queryInfo(it->GetIP(), it->GetPort()+1, it->GetID());
			}
		}
		printf("[Status] Servers count: %d\n", this->pool.size());
		this->lastCheck = GetTickCount();
	}

}