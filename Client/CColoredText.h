#ifndef CCOLOREDTEXT_H
#define CCOLOREDTEXT_H

struct CColoredStruct
{
	CColoredStruct* next;
	unsigned int	color;
	unsigned int	width;
	char*			text;
	CColoredStruct()
	{
		next = NULL;
		color = NULL;
		width = NULL;
		text = NULL;
	}
};

class CColoredText{
private:
	unsigned int wholeLength;
	unsigned int wholeWidth;
	unsigned int elementsCount;
	CColoredStruct*		start;

	// getting
	CColoredStruct*		pointer;


	void	MakeBlocksFromText(char* input);
	char*	FindNextColor(char* input);
	void	CreateBlock(char* input, unsigned int length, unsigned int color);
	void	GetBlocksLength();
public:

	CColoredText();
	CColoredText(char* input);
	~CColoredText();

	// it will split text to fit width and return the rest in new CColoredText instance
	// returns NULL if text width equals or is smaller than desired width
	CColoredText* SplitText(unsigned int width, bool wordWrap);

	void	PushBlock(CColoredStruct* block);

	void				StartGetting();
	CColoredStruct*		GetNext();

	void	ReCalculate();

	int GetElementCount();
};
#endif