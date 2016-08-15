// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

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

	std::string				lastInputs[10];
	unsigned short			lastInputPointer;
	unsigned short			lastInputNext;
public:
	void					AddNewLastMessage(std::string);
	std::string				GetNextLast();
	std::string				GetPreviousLast();
	std::string				ChatMessage;
	std::string				LastInput;

	CChat();
	~CChat();
	// Dx callbacks

	LPDIRECT3DTEXTURE8		chatTexture;
	void					OnLostDevice();
	void					OnResetDevice();
	void					Render(IDirect3DDevice8*,LPD3DXFONT);

	void					AddMessage(std::string);
	void					AddDebugMessage(char* format, ...);
	void					DoneMessage();
	void					DoCommand(char[]);

	bool					IsTyping();
	void					SetTyping(bool);
	void					ProceedKeyboard(LPVOID);
	void					SetBackground(bool);
	bool					IsBackgroundActive();

	void					ClearChat();

	// chat rendering
	void					RenderTexture(IDirect3DDevice8*);
	void					DoRendering();

	bool					shouldWeRerender();
	void					SetRerenderState(bool);

	
};
#endif