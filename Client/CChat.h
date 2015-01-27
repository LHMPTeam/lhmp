/**
	Lost Heaven Multiplayer

	Purpose: chat system (message adding & rendering)

	@author Romop5
	@version 1.0 1/9/14
	@todo clean code / make it more formal
*/
#pragma once
#ifndef __CHAT_H
#define __CHAT_H
	#include <Windows.h>
	#include <d3d8.h>
	#include <d3dx8.h>
	#include <iostream>
	#include <stdio.h>
	#include "string.h"
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
	CChatStack*				ChatPoolStart;
	unsigned int			elementCount;

	int						animation;

	bool					shouldReRender;
public:
	std::string				ChatMessage;
	std::string				LastInput;
public:
	CChat();
	~CChat();
	// Dx callbacks

	LPDIRECT3DTEXTURE8		chatTexture;
	void					OnLostDevice();
	void					OnResetDevice();
	void					Render(IDirect3DDevice8*,LPD3DXFONT);

	void					AddMessage(std::string);
	void					DoneMessage();
	void					DoCommand(char[]);

	bool					IsTyping();
	void					SetTyping(bool);
	void					ProceedKeyboard(LPVOID);
	void					SetBackground(bool);
	bool					IsBackgroundActive();

	// chat rendering
	void					RenderTexture(IDirect3DDevice8*);
	void					DoRendering();

	bool					shouldWeRerender();
	void					SetRerenderState(bool);

	
};
#endif