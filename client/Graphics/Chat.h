#pragma once

enum ChatVK : int
{
	VK_T = 0x54,
	VK_V = 0x56
};

struct ChatCMDStruct
{
	std::function<void(std::vector<std::wstring>)> mFunctionPointer;
	std::wstring mCommandName;
};

inline std::vector<std::wstring> WStringSplit(std::wstring toSplit)
{
	std::vector<std::wstring> returnVec;
	wchar_t * pwc;
	pwc = wcstok(&toSplit[0], L" ");

	while (pwc != NULL)
	{
		returnVec.push_back(pwc);
		pwc = wcstok(NULL, L" ");
	}

	return returnVec;
}

class Chat
{
public:
	Chat();
	~Chat();
	void Init(IDirect3DDevice8* newDevice);
	void Render();
	void OnDeviceReset();
	void OnDeviceLost();
	void AddMessage(std::wstring newMessage);
	void ProcessKeyboard(USHORT VKey, UINT Message);
	bool IsTyping() { return mIsTyping; }
	void RegisterChatCMD(std::wstring cmdName, std::function<void(std::vector<std::wstring>)> args);
private:
	void UpdateChatTexture();
	void CreateTextures();
	void RegisterInternalCommands();
	void ProcessMessage(std::wstring messageToProcess);
	bool mShouldUpdateTexture;
	bool mIsTyping;
	bool mIsControlPressed, mIsAltPressed;
	float mAnimationTranslation;
	int mChatWidth, mChatHeight;
	int mFontWeight;
	LPDIRECT3DTEXTURE8 mChatTexture;
	LPD3DXFONT mChatFont;
	std::vector<std::wstring> mChatMessages;
	std::vector<ChatCMDStruct> mChatRegisteredCommands;
	std::wstring mTypingLine;
};

