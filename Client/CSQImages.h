// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef _CSQUIRREL_IMAGES_H
#define _CSQUIRREL_IMAGES_H
#include "d3dx8.h"

#define MAX_TEXTURES 100

// Handles single texture
class CSQImage
{
public:
	CSQImage(char* textureName);
	~CSQImage();

	LPDIRECT3DTEXTURE8	GetTexture();
	char*				GetImageName();
	// intern things
	void	OnLostDevice();
	void	OnResetDevice();
	// Used for multiple references (2 demands, 1 object)
	void	SetRefCount(unsigned short);
	unsigned short GetRefCount();
private:
	void LoadTexture();
	LPDIRECT3DTEXTURE8 texture;
	char textureName[300];
	unsigned short referenceCount;
};

class CSQImages
{
public:
	CSQImages();
	CSQImage*	createTexture(char* textureName);
	void		deleteTexture(CSQImage*);
	// Called by game on device lost
	void	OnLostDevice();
	// Called by game 
	void	OnResetDevice();
private:
	CSQImage*	pool[MAX_TEXTURES];

};

#endif