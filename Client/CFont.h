/**
	Lost Heaven Multiplayer

	Purpose: a CD3DFont proxy class with some extra drawing functions

	@author Romop5
	@version 1.0 1/9/14
*/

#include "CColoredText.h"
#include "../sdks/cd3dfont/d3dfont.h"
#ifndef C_FONT
#define C_FONT

class CFont{
private:
	CD3DFont*	m_pFont;
public:
	CFont(char font[100],int size,DWORD flags = 0UL);
	~CFont();

	// Dx callbacks
	void	OnDeviceLost();
	void	OnReset();

	// draw
	void	DrawText(char text[], int x , int y, D3DCOLOR color, bool shadow = true);
	void	DrawColoredText(CColoredText* text, int x, int y, bool shadow = true);
	void	DrawColoredText(char* text, int x, int y,DWORD color, bool shadow = true);

	// calculations

	SIZE	GetFontWidth(char[],int len = -1);
	int		GetColoredTextWidth(char[]);
	int		GetStrlenForWidth(int, char*);

};
#endif