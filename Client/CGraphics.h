/**
	Lost Heaven Multiplayer

	Purpose: graphics center, handles all callbacks (init, device reset&lost),
	* contains usefull graphic utils 

	@author Romop5
	@version 1.0 1/9/14
*/

#ifndef __CGRAPHIC_H
#define __CGRAPHIC_H

#include "../sdks/cd3dfont/d3dfont.h"
#include "CFont.h"
#include "Windows.h"
#include "d3dx8.h"

#include "CColoredText.h"

struct my_vertex{
	FLOAT x, y, z, rhw; // The untransformed position for the vertex.
	DWORD color;        // The vertex color.
};


#define D3D8T_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

class CGraphics
{
private:

	LPD3DXSPRITE		m_sprite;
	IDirect3DDevice8*	m_DirectDevice;
	IDirect3D8*			m_Direct3D;
	LPDIRECT3DTEXTURE8	m_loadingscreen,m_nametag,m_gamemap,m_mapGUI;
	LPD3DXFONT			m_chatfont;
	HFONT				m_chatfontAPI;
	bool				m_bShowNameTags;
	CD3DFont*			m_d3dFont;
	CFont*				m_cFont;
	IDirect3DVertexBuffer8*	m_vb;
public:
	CGraphics();
	IDirect3DDevice8*	GetDevice();
	ID3DXSprite*		GetSprite();
	void				SetDirect3D(IDirect3D8*);

	bool				bIsFullscreen;
	bool				bShowHud;
	HWND				hwnAppWindow;

	// functions
	void				RenderLoadingScreen();
	void				Init(IDirect3DDevice8*);
	void				Render();
	void				RenderNametags();
	void				RenderMap();
	void				RenderScoreboard();
	void				RenderStatistics();
	void				TakeScreenshot();
	void				ToggleInGameHud(bool);
	bool				IsInGameHudVisible();

	// callbacks
	void				OnLostDevice();
	void				OnResetDevice();


	// text utils
	Vector2D			GetResolution();
	int					GetFontWidth(char []);
	int					GetLetterWidth(char c);
	int					GetColoredTextWidth(char[]);
	int					GetStrlenForWidth(int,char*);
	std::string			GetLastColorInText(char*,int);
	virtual void		DrawText(char [],int,int,D3DCOLOR,bool,LPD3DXFONT);
	virtual void		DrawText(char [],int,int,D3DCOLOR,bool);
	virtual void		DrawText(char [],int,int,D3DCOLOR,bool,bool,LPD3DXFONT);
	virtual void		DrawText(char [],int,int,D3DCOLOR,bool,bool);
	virtual void		DrawColoredText(CColoredText* text, int, int, bool);

	// other utils
	void				CalcScreenPosition(Vector3D	,Vector3D *	);
	void				Clear(int, int, int, int, D3DCOLOR);
	void				FillARGB(int x, int y, int w, int h, D3DCOLOR color);
	void				FillARGB(int x, int y,float z, int w, int h, D3DCOLOR color);

	// states
	bool				IsCamFreezed;
	bool				renderMap;
	bool				renderScoreboard;
	bool				renderNetStat;
	float				mapScale;

	bool				IsShowNameTags();
	void				SetShowNameTags(bool);


	CColoredText*		textiq;
	CColoredText*		textiqSecond;

	int					GetD3DSize(char[]);
	void				D3DDrawText(char[], int, int, D3DCOLOR, bool shadow);

	CFont*				GetFont();
};

#endif