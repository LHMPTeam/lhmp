// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CSQFonts.h"

// Handles single texture

CSQFont::CSQFont(char* font, unsigned short size)
{
	this->p_font = new CFont(font, size);
	this->size = size;
	strcpy(this->fontName, font);
}
CSQFont::~CSQFont()
{
	if (this->p_font)
		delete this->p_font;
}


CFont*				CSQFont::GetFont()
{
	return this->p_font;
}
char*				CSQFont::GetName()
{
	return this->fontName;
}
unsigned short		CSQFont::GetSize()
{
	return this->size;
}

void	CSQFont::OnLostDevice()
{
	this->p_font->OnDeviceLost();
}
void	CSQFont::OnResetDevice()
{
	this->p_font->OnReset();
}
// Used for multiple references (2 demands, 1 object)
void	CSQFont::SetRefCount(unsigned short number)
{
	this->referenceCount = number;
}
unsigned short CSQFont::GetRefCount()
{
	return this->referenceCount;
}

CSQFonts::CSQFonts()
{
	// clear all slots
	for (int i = 0; i < MAX_FONTS; i++)
	{
		this->pool[i] = NULL;
	}
}
CSQFont*	CSQFonts::createFont(char* textureName,unsigned short size)
{
	int firstOne = -1;
	// check whether texture exists or find a fist free texture
	for (int i = 0; i < MAX_FONTS; i++)
	{
		if (pool[i] != NULL)
		{
			if (strcmp(textureName, pool[i]->GetName()) == 0 && pool[i]->GetSize() == size)
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
		pool[firstOne] = new CSQFont(textureName,size);
		// if texture loading has failed (bad image)
		if (pool[firstOne]->GetFont() == NULL)
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
void		CSQFonts::deleteFont(CSQFont* deletedImage)
{
	for (int i = 0; i < MAX_FONTS; i++)
	{
		if (pool[i] == deletedImage)
		{
			if (pool[i]->GetRefCount() > 1)
			{
				// if it has multiple references, decrease the references count
				pool[i]->SetRefCount(pool[i]->GetRefCount() - 1);
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
void	CSQFonts::OnLostDevice()
{
	for (int i = 0; i < MAX_FONTS; i++)
	{
		if (pool[i] != NULL)
		{
			pool[i]->OnLostDevice();
		}
	}
}
// Called by game 
void	CSQFonts::OnResetDevice()
{
	for (int i = 0; i < MAX_FONTS; i++)
	{
		if (pool[i] != NULL)
		{
			pool[i]->OnResetDevice();
		}
	}
}
