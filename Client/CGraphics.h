#ifndef CGRAPHIC_H
#define CGRAPHIC_H

#include "Windows.h"
//#include "CCore.h"
#include "d3dx8.h"
//#include <d3dx.h>

#include "CColoredText.h"

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
public:
	CGraphics();
	IDirect3DDevice8*	GetDevice();
	bool				bIsFullscreen;
	bool				bShowHud;
	HWND				hwnAppWindow;

	void				RenderLoadingScreen();
	void				Init(IDirect3DDevice8*);
	void				Render();
	void				RenderNametags();
	void				RenderMap();
	void				RenderScoreboard();
	void				RenderStatistics();

	void				OnLostDevice();
	void				OnResetDevice();

	Vector2D			GetResolution();
	//LPD3DXFONT			GetChatFont();
	void				TakeScreenshot();
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


	void				Test();
	void				CalcScreenPosition(Vector3D	,Vector3D *	);
	bool				IsCamFreezed;


	void				Clear(int, int, int, int, D3DCOLOR);
	void				FillARGB( int, int, int, int , D3DCOLOR);

	bool				renderMap;
	bool				renderScoreboard;
	bool				renderNetStat;

	void				ToggleInGameHud(bool);
	bool				IsInGameHudVisible();
	ID3DXSprite*		GetSprite();

	float				mapScale;

	void				SetDirect3D(IDirect3D8*);

	bool				IsShowNameTags();
	void				SetShowNameTags(bool);


	CColoredText*		textiq;
	CColoredText*		textiqSecond;
};

#endif