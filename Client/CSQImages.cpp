// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include "CSQImages.h"

extern CCore* g_CCore;

CSQImage::CSQImage(char* _in_textureName)
{
	strcpy(this->textureName, _in_textureName);
	this->LoadTexture();
	this->referenceCount = 1;
}
CSQImage::~CSQImage()
{
	// TODO delete texture
}

LPDIRECT3DTEXTURE8	CSQImage::GetTexture()
{
	return this->texture;
}
char*				CSQImage::GetImageName()
{
	return this->textureName;
}

// intern things
void	CSQImage::OnLostDevice()
{
	// if exists, release it
	if(this->texture)
		this->texture->Release();
}
void	CSQImage::OnResetDevice()
{
	// reload it
	this->LoadTexture();
}

void	CSQImage::SetRefCount(unsigned short newCount)
{
	this->referenceCount = newCount;
}
unsigned short CSQImage::GetRefCount()
{
	return this->referenceCount;
}

void CSQImage::LoadTexture()
{
	if (D3DXCreateTextureFromFileExA(g_CCore->GetGraphics()->GetDevice(), this->textureName, D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR, 0x10000000,
		NULL, NULL, &this->texture) != D3D_OK)
	{
		this->texture = NULL;
	}
}

//-------------------------------------------------------------------------

CSQImages::CSQImages()
{
	// clear all slots
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		this->pool[i] = NULL;
	}
}
CSQImage*	CSQImages::createTexture(char* textureName)
{
	int firstOne = -1;
	// check whether texture exists or find a fist free texture
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (pool[i] != NULL)
		{
			if (strcmp(textureName, pool[i]->GetImageName()) == 0)
			{
				// if object already exists, increase ref count 
				pool[i]->SetRefCount(pool[i]->GetRefCount() + 1);
				// and return the same handle
				return pool[i];
			}
		}
		else {
			if (firstOne == -1)
			{
				firstOne = i;
			}
		}
	}
	// if there is an empty slot
	if (firstOne != -1)
	{
		pool[firstOne] = new CSQImage(textureName);
		// if texture loading has failed (bad image)
		if (pool[firstOne]->GetTexture() == NULL)
		{
			// delete and return NULL
			delete pool[firstOne];
			return NULL;
		}
		return pool[firstOne];
	}

	// in the end, if we haven't been successful, return NULL
	return NULL;
}
void		CSQImages::deleteTexture(CSQImage* deletedImage)
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (pool[i] == deletedImage)
		{
			if (pool[i]->GetRefCount() > 1)
			{
				// if it has multiple references, decrease the references count
				pool[i]->SetRefCount(pool[i]->GetRefCount() -1);
			}
			else {
				delete pool[i];
				pool[i] = NULL;
			}
			return;
		}
	}
}
// Called by game on device lost
void	CSQImages::OnLostDevice()
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (pool[i] != NULL)
		{
			pool[i]->OnLostDevice();
		}
	}
}
// Called by game 
void	CSQImages::OnResetDevice()
{
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (pool[i] != NULL)
		{
			pool[i]->OnResetDevice();
		}
	}
}
