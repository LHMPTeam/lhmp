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
	DWORD alpha = color >> 24;

	if (shadow)
	{
		for (int i = 0; i < 4; i++) {
			this->m_pFont->DrawTextA((float)(x + 1), (float)(y + 1), (alpha << 24), text);					// 0xFF000000 is base color
			this->m_pFont->DrawTextA((float)(x + 2), (float)(y + 2), ((alpha/0xA) << 24), text);			// 0x38000000 is base color
		}

		//this->m_pFont->DrawTextA((float)(x + 2), (float)(y + 2), 0xFF000000, text);
	}
	this->m_pFont->DrawTextA((float)x, (float) y, color, text);
}

void	CFont::DrawColoredText(CColoredText* text, int x, int y, bool shadow)
{
	int drawn = 0;
	text->StartGetting();
	while (CColoredStruct* strr = text->GetNext())
	{
		this->DrawTextA(strr->text, x + drawn, y, strr->color, true);

		//TODO remove
		//char test[255];
		//sprintf(test, "%d", strr->width);
		//this->DrawTextA(test, x + drawn, y, 0xFFFF0000, true);
		//this->m_pFont->DrawTextA(x + drawn, y, strr->color, strr->text);
		drawn += strr->width;
	}
}

void	CFont::DrawColoredText(char* text, int x, int y, DWORD color, bool shadow)
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
		this->DrawTextA(text, x + drawn, y, color, shadow);
		//this->m_pFont->DrawTextA(x + drawn, y, color, text);
	}
	else if (start == 0)
	{
		color = Tools::GetARGBFromString(text + 1);
		this->DrawColoredText(text + 7, x + drawn, y, color, shadow);
	}
	else {
		text[start] = 0x0;
		this->DrawTextA(text, x + drawn, y, color, shadow);
		//this->m_pFont->DrawTextA(x + drawn, y, color, text);
		SIZE size = this->GetFontWidth(text);
		drawn = size.cx;
		text[start] = '#';
		color = Tools::GetARGBFromString(text + 1);
		this->DrawColoredText(text + 7, x + drawn, y, color, shadow);

	}
}

//----------------------------------------------------

SIZE	CFont::GetFontWidth(char text[], int len)
{
	SIZE textSize;
	if (this->m_pFont)
	{
		char backup;
		if (len != -1)
		{
			backup = text[len];
			text[len] = 0x0;
		}

		this->m_pFont->GetTextExtent(text, &textSize);

		if (len != -1)
		{
			text[len] = backup;
		}
	}
	else {
		textSize.cx = 0;
		textSize.cy = 0;
	}
	return textSize;
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
				SIZE size2 = this->GetFontWidth(pointer);
				size += size2.cx;
				break;
			}
			else if (start == 0)
			{
				pointer = pointer + 7;
			}
			else {
				SIZE size2 = this->GetFontWidth(pointer, start);
				size += size2.cx;
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
	int i;
	for (i = 0; i < len; i++)
	{
		SIZE size2 =  this->GetFontWidth(text + i, 1);
		curSize += size2.cx;
		if (curSize > size)
			return i;
	}
	return i;
}