#pragma once
#ifndef CHAT_H
#define CHAT_H
	#include <Windows.h>
	#include <d3d8.h>
	#include <d3dx8.h>
	#include <iostream>
	#include <stdio.h>
	#include "string.h"
	//#include "directx8hook.h"
	#define CMD_MAX_ARGS 64
#include "CColoredText.h"

struct CChatStack
{
	CColoredText* text;
	CChatStack* next;
};
class CChat
{
private:
	bool m_bBackground;
	bool IsRendering;
	bool bIsTyping;
	int CHAT_WIDTH;
	int CHAT_MAX_LINES;
	int CHAT_LINES_PER_RENDER;
	int CHAT_POINTER;
	int CHAT_STARTX;
	CChatStack*					ChatPoolStart;
	//CChatStack*					ChatPoolEnd;
	unsigned int				elementCount;

	LPDIRECT3DTEXTURE8			chatTexture;
	int							animation;
public:
	std::string					ChatMessage;
	std::string					LastInput;
public:
	CChat();
	~CChat();
	void						Render(IDirect3DDevice8*,LPD3DXFONT);
	void						AddMessage(std::string);
	void						Init(IDirect3DDevice8*);
	void						DoneMessage();
	bool						IsTyping();
	void						SetTyping(bool);
	void						ProceedKeyboard(LPVOID);
	void						DoCommand(char []);
	void						SetBackground(bool);
	bool						GetBackground();
	void						RenderTexture(IDirect3DDevice8*);
	void						OnLostDevice();
	void						OnResetDevice();
	void						DoRendering();

	// heh, for fun
	bool		isCamOn;
	Vector3D	camPos;
	float		camR1,camR2,camR3;
	float		camDegree;
	float		camSpeed;
	bool		shouldReRender;
};
#endif