#include "CCore.h"
#include "CPickup.h"

extern CCore* g_CCore;

#ifndef _WIN32
#include <sys/time.h>
extern unsigned long timeGetTime();
/*unsigned long timeGetTime()
{
struct timeval now;
gettimeofday(&now, NULL);
return now.tv_usec/1000;
}*/
#define sprintf_s sprintf
#define Sleep usleep
#endif

CPickup::CPickup()
{
	pSize = 1.0f;
	sprintf(pModel, "%s", "");
	pPos.x = 0.0f;
	pPos.y = 0.0f;
	pPos.z = 0.0f;
	pIsVisible = true;
	respawnTime = 0;
}
CPickup::~CPickup()
{
	this->SendDelete(-1);
}

CPickup::CPickup(int ID,char* model, int interval, float x, float y, float z, float size)
{
	//CPickup();
	this->ID = ID;
	this->SetModel(model);

	this->pPos.x = x;
	this->pPos.y = y;
	this->pPos.z = z;

	this->SetSize(size);

	this->interval = interval;


	this->pIsVisible = true;
	respawnTime = 0;
}

int		CPickup::GetID()
{
	return this->ID;
}
char*	CPickup::GetModel()
{
	return this->pModel;
}
void	CPickup::SetModel(char* model)
{
	sprintf(pModel, "%s", model);
}

void	CPickup::SetPosition(Vector3D pos)
{
	this->pPos = pos;
}
Vector3D CPickup::GetPosition()
{
	return this->pPos;
}

void	CPickup::SetSize(float size)
{
	this->pSize = size;
}
float	CPickup::GetSize()
{
	return this->pSize;
}
// -1 to all otherwise it's ID
void CPickup::SendIt(int IDorOthers)
{
	BitStream bsOut;
	char buff[250];
	sprintf(buff, "%s", this->GetModel());
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PICKUP_CREATE);
	bsOut.Write(this->GetID());
	bsOut.Write(buff);
	bsOut.Write(this->GetPosition());
	bsOut.Write(this->GetSize());
	bsOut.Write(this->IsVisible());
	if (IDorOthers == -1)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(IDorOthers),false);

	//printf("%s\n", GetModel());
}

bool	CPickup::IsVisible()
{
	return this->pIsVisible;
}
void	CPickup::SetVisible(bool should)
{
	this->pIsVisible = should;
}


bool	CPickup::IsTakingPickup(int ID)
{
	if ((int)(timeGetTime()-respawnTime) > interval || interval == -1)
	{
		Vector3D pos, playerpos = g_CCore->GetPlayerPool()->Return(ID)->GetPosition(), pickuppos = this->GetPosition();;
		pos.x = abs(pickuppos.x - playerpos.x);
		pos.y = abs(pickuppos.y - playerpos.y);
		pos.z = abs(pickuppos.z - playerpos.z);
		if (pos.x < 2.0 && pos.z < 2.0)
		{
			// take pickup
			int pickupID = g_CCore->GetPickupPool()->ReturnId(this);
			g_CCore->GetScripts()->onPickupTaken(pickupID, ID);
			if (interval == -1)
			{
				g_CCore->GetPickupPool()->Delete(pickupID);
			}
			else {
				respawnTime = timeGetTime();

				this->SendVisible(-1, false);
			}
			return true;
		}
	}
	return false;
}

void CPickup::SendVisible(int IDorOthers, bool visible)
{
	this->SetVisible(visible);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PICKUP_SETVISIBLE);
	bsOut.Write(this->GetID());
	bsOut.Write(this->IsVisible());

	if (IDorOthers == -1)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(IDorOthers), false);

}

void CPickup::SendDelete(int IDorOthers)
{
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PICKUP_DELETE);
	bsOut.Write(this->GetID());

	if (IDorOthers == -1)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(IDorOthers), false);
}

void CPickup::Tick()
{
	if (this->IsVisible() == false)
	{
		if ((int)(timeGetTime() - respawnTime) > interval)
		{
			this->SendVisible(-1, true);
		}
	}
}
