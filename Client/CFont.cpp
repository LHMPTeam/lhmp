#include "CCore.h"
#include "CFont.h"

extern CCore *g_CCore;
CFont::CFont(char font[100], int size, DWORD flags)
{
	this->m_pFont = new CD3DFont(font, size, flags);
	this->m_pFont->InitDeviceObjects(g_CCore->GetGraphics()->GetDevice());
	this->m_pFont->RestoreDeviceObjects();
}

CFont::~CFont()
{
	this->m_pFont->DeleteDeviceObjects();
	delete this->m_pFont;
}

//----------------------------------------------------

void	CFont::OnDeviceLost()
{
	this->m_pFont->InvalidateDeviceObjects();
}

void	CFont::OnReset()
{
	this->m_pFont->RestoreDeviceObjects();
}

//----------------------------------------------------

void	CFont::DrawText(char text[], int x, int y, D3DCOLOR color, bool shadow )
{
	if (shadow)
	{
		this->m_pFont->DrawTextA((float)(x - 1), (float)(y), 0xFF000000, text);
		this->m_pFont->DrawTextA((float)(x + 1), (float)(y), 0xFF000000, text);
		this->m_pFont->DrawTextA((float)(x), (float)(y - 1), 0xFF000000, text);
		this->m_pFont->DrawTextA((float)(x), (float)(y + 1), 0xFF000000, text);
	}
	this->m_pFont->DrawTextA((float)x, (float) y, color, text);
}

void	CFont::DrawColoredText(CColoredText* text, int x, int y, bool shadow)
{
	int drawn = 0;
	text->StartGetting();
	while (CColoredStruct* strr = text->GetNext())
	{
		this->DrawTextA(strr->text,x + drawn, y, strr->color, true);
		//this->m_pFont->DrawTextA(x + drawn, y, strr->color, strr->text);
		drawn += strr->width;
	}
}

void	CFont::DrawColoredText(char* text, int x, int y,DWORD color, bool shadow)
{
	/*int drawn = 0;
	text->StartGetting();
	while (CColoredStruct* strr = text->GetNext())
	{
		this->m_pFont->DrawTextA(x + drawn, y, strr->color, strr->text);
		drawn += strr->width;
	}*/
	int drawn = 0;
	int start = Tools::getFirstColorStamp(text);
	if (start == -1)
	{
		this->DrawTextA(text,x + drawn, y, color, shadow);
		//this->m_pFont->DrawTextA(x + drawn, y, color, text);
	} else if (start == 0)
	{
		color = Tools::GetARGBFromString(text + 1);
		this->DrawColoredText(text + 7, x + drawn, y, color, shadow);
	}
	else {
		text[start] = 0x0;
		this->DrawTextA(text, x + drawn, y, color, shadow);
		//this->m_pFont->DrawTextA(x + drawn, y, color, text);
		drawn = this->GetFontWidth(text);
		text[start] = '#';
		color = Tools::GetARGBFromString(text + 1);
		this->DrawColoredText(text+7, x+drawn, y, color, shadow);

	}
}

//----------------------------------------------------

int		CFont::GetFontWidth(char text[], int len)
{
	char backup;
	if (len != -1)
	{
		backup = text[len];
		text[len] = 0x0;
	}
	SIZE textSize;
	this->m_pFont->GetTextExtent(text, &textSize);

	if (len != -1)
	{
		text[len] = backup;
	}

	return textSize.cx;
}
int		CFont::GetColoredTextWidth(char text[])
{
	int size = 0;
	if (Tools::IsEmptyString(text) != NULL)
	{
		int start;
		char* pointer = text;
		while (1 == 1)
		{
			start = Tools::getFirstColorStamp(text);
			if (start == -1)
			{
				size += this->GetFontWidth(pointer);
				break;
			}
			else if (start == 0)
			{
				pointer = pointer + 7;
			}
			else {
				size += this->GetFontWidth(pointer,start);
				pointer = pointer + start + 7;
			}
		}
	}
	return size;
}
int		CFont::GetStrlenForWidth(int size, char* text)
{
	int curSize = 0;
	int len = strlen(text);
	for (int i = 0; i < len; i++)
	{
		curSize += this->GetFontWidth(text + i, 1);
		if (curSize > size)
			return i;
	}
}