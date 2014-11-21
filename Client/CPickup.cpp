#include "CPickup.h"

CPickup::CPickup(char* model, Vector3D pos, float size, bool isVisible)
{
	this->SetPosition(pos);
	this->SetSize(size);
	this->SetVisible(isVisible);
	this->SetEntity(NULL);
	this->SetModel(model);
}

Vector3D CPickup::GetPosition()
{
	return this->pPosition;
}

void CPickup::SetPosition(Vector3D pos)
{
	this->pPosition = pos;
}

float CPickup::GetSize()
{
	return this->pSize;
}

void CPickup::SetSize(float f)
{
	this->pSize = f;
}


char*	CPickup::GetModel()
{
	return this->pModel;
}
void	CPickup::SetModel(char* model)
{
	sprintf(pModel, "%s", model);
}

bool	CPickup::IsVisible()
{
	return this->pIsVisible;
}
void	CPickup::SetVisible(bool f)
{
	this->pIsVisible = f;
}

DWORD	CPickup::GetEntity()
{
	return this->pEntity;
}
void	CPickup::SetEntity(DWORD e)
{
	this->pEntity = e;
}