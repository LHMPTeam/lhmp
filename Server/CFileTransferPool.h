/*
#include "CFileTransfer.h"

struct CFileTransfers
{
	CFileTransfers* next;
	CFileTransfer* transfer;
};
class CFileTransferPool
{
private:
	CFileTransfers* pool;
	int		unicateID;
public:
	CFileTransferPool()
	{
		unicateID = 1;
	}
	int NewTransfer(RakNet::SystemAddress receiver, unsigned char type)
	{
		CFileTransfer* newTF = new CFileTransfer(unicateID, receiver, type);
		CFileTransfers* part = new CFileTransfers();
		part->next = NULL;
		part->transfer = newTF;
		if (pool != NULL)
			part->next = pool;
		pool = part;
		unicateID++;
		return unicateID-1;
	}
	CFileTransfer*	GetID(int unicateID)
	{
		CFileTransfers* pointer = pool;
		while (pointer != NULL)
		{
			if (pointer->transfer->GetID() == unicateID)
				return pointer->transfer;
			pointer = pointer->next;
		}
		return NULL;
	}

	bool DeleteID(int unicateID)
	{
		CFileTransfers* pointer = pool,*before = NULL;
		while (pointer != NULL)
		{
			if (pointer->transfer->GetID() == unicateID)
			{
				if (before == NULL)
				{
					this->pool = pointer->next;
				}
				else {
					before->next = pointer->next;
				}
				delete[] pointer->transfer;
				delete[] pointer;

				return true;
			}
			before = pointer;
			pointer = pointer->next;
		}
		return false;
	}
};
*/