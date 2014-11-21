#include "CPedPool.h"
#include "CCore.h"

extern CCore *g_CCore;
CPedPool::CPedPool()
{
	for(int i = 0; i < MAX_PLAYERS;i++)
		pedpool[i] = NULL;
	//pedpool = new CPed*[100];
}
CPedPool::~CPedPool()
{
	delete[] &pedpool;
}
void CPedPool::New(int index)
{
	if(pedpool[index] == 0)
	{
		CPed* pointer = new CPed;
		pedpool[index] = pointer;
		g_CCore->GetEngineStack()->AddMessage(ES_CREATEPLAYER,index);
	} else
	{
		CPed *ped = pedpool[index];
		/*if (ped->GetEntity() == 0)
		{
			g_CCore->GetEngineStack()->AddMessage(ES_CREATEPLAYER,index);
		} else
		{
			g_CCore->GetChat()->AddMessage("Allready done");
		}*/
		g_CCore->GetEngineStack()->AddMessage(ES_CREATEPLAYER, index);
		ped->SetActive(1);
	}
}

CPed* CPedPool::Return(int slot)
{
	return pedpool[slot];
}

int CPedPool::GetPedIdByBase(DWORD base)
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		CPed* ped = this->Return(i);
		if(ped == NULL) continue;
		if (ped->GetEntity() == base) return i;
	}
	return -1;
}

void CPedPool::Delete(int index)
{
	delete pedpool[index];
	pedpool[index] = NULL;
}