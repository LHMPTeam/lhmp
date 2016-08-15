// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef _CSQUIRREL_FONTS_H
#define _CSQUIRREL_FONTS_H

#include "d3dx8.h"
#include "CFont.h"

#define MAX_FONTS 100

// Handles single texture
class CSQFont
{
public:
	CSQFont(char* font, unsigned short size);
	~CSQFont();

	CFont*				GetFont();
	char*				GetName();
	unsigned short		GetSize();
	// intern things
	void	OnLostDevice();
	void	OnResetDevice();
	// Used for multiple references (2 demands, 1 object)
	void	SetRefCount(unsigned short);
	unsigned short GetRefCount();
private:
	CFont* p_font;
	char fontName[300];
	unsigned short size;

	unsigned short referenceCount;
};

class CSQFonts
{
public:
	CSQFonts();
	CSQFont*	createFont(char* textureName,unsigned short fontsize);
	void		deleteFont(CSQFont*);
	// Called by game on device lost
	void	OnLostDevice();
	// Called by game 
	void	OnResetDevice();
private:
	CSQFont*	pool[MAX_FONTS];

};

#endif