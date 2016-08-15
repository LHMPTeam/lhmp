// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include "CColoredText.h"
#include "../shared/tools.h"

extern CCore* g_CCore;

CColoredText::CColoredText()
{
	this->pointer = NULL;
	this->start = NULL;
	this->elementsCount = NULL;
	this->wholeLength = NULL;
	this->wholeWidth = NULL;
}

CColoredText::~CColoredText()
{
	CColoredStruct* next = this->start;
	if (next != NULL)
	{
		do
		{
			CColoredStruct* pointer = next;
			next = next->next;
			delete[] pointer->text;
			delete(pointer);
		} while (next != NULL);
	}
}


CColoredText::CColoredText(char* input)
{
	this->pointer = NULL;
	this->start = NULL;
	this->elementsCount = NULL;
	this->wholeLength = NULL;
	this->wholeWidth = NULL;
	if (strlen(input) > 0)
	{
		this->MakeBlocksFromText(input);
		this->GetBlocksLength();
	}
}

void CColoredText::MakeBlocksFromText(char* input)
{
	int len = strlen(input);
	unsigned int textColor = 0xFFFFFFFF;
	char* color = this->FindNextColor(input);
	if (color == NULL)
	{
		this->CreateBlock(input, strlen(input), textColor);
	}
	else {
		if (len > 7)
		{
			if (color == input)
			{
				char* nextColor = this->FindNextColor(input + 7);
				if (nextColor == NULL)
				{
					this->CreateBlock(input + 7, strlen(input), Tools::GetARGBFromString(color + 1));
				}
				else {
					len = nextColor - input -7;
					if (len > 0)
						this->CreateBlock(input + 7, nextColor - input - 7, Tools::GetARGBFromString(color + 1));
					MakeBlocksFromText(nextColor);
				}
			}
			else {
				char* nextColor = this->FindNextColor(input + 7);
				if (nextColor == NULL)
				{
					this->CreateBlock(input, strlen(input), textColor);
				}
				else {
					len = nextColor - input - 7;
					if (len > 0)
						this->CreateBlock(input, nextColor - input, textColor);
					MakeBlocksFromText(nextColor);
				}
			}
		}
		else {
			return;
		}
	}
}


char*	CColoredText::FindNextColor(char* input)
{
	char*	color = strchr(input, '#');
	if (color == 0)
	{
		return NULL;
	}
	else {
		if (Tools::isHEXStamp(color))
		{
			return color;
		}
		else {
			return this->FindNextColor(color + 7);
		}
	}
}


void	CColoredText::CreateBlock(char* input, unsigned int length, unsigned int color)
{
	CColoredStruct* newstruct = new CColoredStruct();
	newstruct->next = NULL;
	newstruct->width = NULL;
	newstruct->color = color;
	char*	text = new char[length+1];
	strncpy(text, input,length);
	text[length] = '\0';
	newstruct->text = text;
	
	CColoredStruct* next = this->start;
	if (next == NULL)
	{
		this->start = newstruct;
	}
	else {
		while (next->next != NULL)
		{
			next = next->next;
		};
		next->next = newstruct;
	}

	this->wholeLength += length;
}

void	CColoredText::GetBlocksLength()
{
	unsigned int width = NULL;
	CColoredStruct* next = this->start;
	if (start != NULL)
	{
		do
		{
			//next->width = g_CCore->GetGraphics()->GetFontWidth(next->text);
			SIZE size = g_CCore->GetGraphics()->GetFont()->GetFontWidth(next->text);
			next->width = size.cx;
			width += next->width;
			next = next->next;
		} while (next != NULL);
		this->wholeWidth = width;
	}
}

void	CColoredText::StartGetting()
{
	pointer = this->start;
}
CColoredStruct*	CColoredText::GetNext()
{
	if (pointer == NULL)
	{
		return NULL;
	}
	else {
		CColoredStruct* actual = pointer;
		pointer = actual->next;
		return actual;
	}
}

__declspec(noinline) CColoredText* CColoredText::SplitText(unsigned int width, bool wordWrap)
{
	if (this->wholeWidth <= width)
	{
		return NULL;
	}
	else {
		CColoredStruct*		before = NULL;
		unsigned int desiredLength = NULL;
		if (this->start != NULL)
		{
			CColoredStruct* point = this->start;
			do {
				if (desiredLength + point->width > width)
				{
					// here we go
					unsigned int wantedWidth = (desiredLength + point->width)-width;
					unsigned int currentWidth = NULL;
					char*		textEnd = NULL;
					int len = strlen(point->text);
					textEnd = point->text;

					int enoughCharacters = g_CCore->GetGraphics()->GetFont()->GetStrlenForWidth(width - desiredLength, point->text);

					for (int i = enoughCharacters; i > 0; i--)
					{
						if (point->text[i] == ' ')
						{
							enoughCharacters = i;
							break;
						}
					}

					textEnd = point->text + enoughCharacters;



					// now we must do something :D
					// TODO: split block, create new instance, push blocks to new instance
					CColoredText* newClass = new CColoredText();
					CColoredStruct* remain = NULL;
					if (textEnd != point->text)
					{
						CColoredStruct* block = new CColoredStruct();
						int newLen = strlen(textEnd);
						block->text = new char[newLen + 1];
						strcpy(block->text, textEnd);
						block->color = point->color;
						block->width = currentWidth;
						newClass->PushBlock(block);
						textEnd[0] = '\0';
						point->width -= currentWidth;
						remain = point->next;
						point->next = NULL;
					}
					else {
						if (before != NULL)
							before->next = NULL;
						remain = point;
					}
					//this->wholeWidth -= currentWidth;
					// now push remaining blocks into new class

					if (remain != NULL)
					{
						//point->next = NULL;
						while (remain != NULL)
						{
							CColoredStruct* push = remain;
							remain = remain->next;
							this->wholeWidth -= push->width;
							newClass->PushBlock(push);
						};
						
					}
					this->ReCalculate();
					newClass->ReCalculate();
					return newClass;
				}
				else {
					desiredLength += point->width;
				}
				before = point;
				point = point->next;
			} while (point != NULL);
			return NULL;

		}
		else {
			return NULL;
		}

	}
}

__declspec(noinline) void	CColoredText::PushBlock(CColoredStruct* block)
{
	if (this->start != NULL)
	{
		CColoredStruct* point = this->start;
		while (point->next != NULL)
		{
			point = point->next;
		} 
		point->next = block;
	}
	else {
		this->start = block;
	}
	block->next = NULL;
	this->wholeLength += strlen(block->text);
	this->wholeWidth += block->width;

}

void	CColoredText::ReCalculate()
{
	this->wholeLength = 0;
	this->wholeWidth = 0;
	unsigned int width = NULL;
	unsigned int length = NULL;
	int count = NULL;
	this->elementsCount = 0;
	CColoredStruct* next = this->start;
	if (start != NULL)
	{
		do
		{
			count++;
			//next->width = g_CCore->GetGraphics()->GetFontWidth(next->text);
			SIZE size = g_CCore->GetGraphics()->GetFont()->GetFontWidth(next->text);
			next->width = size.cx;

			width += next->width;
			length += strlen(next->text);
			next = next->next;
		} while (next != NULL);
		this->wholeWidth = width;
		this->wholeLength = length;
		this->elementsCount = count;
	}
}

int CColoredText::GetElementCount()
{
	return this->elementsCount;
}