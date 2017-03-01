#include <stdinc.h>

Chat::Chat() : 
	mChatTexture(NULL),
	mChatHeight(600),
	mChatWidth(800),
	mFontWeight(20),
	mShouldUpdateTexture(true),
	mIsTyping(false),
	mAnimationTranslation(0),
	mIsControlPressed(false),
	mIsAltPressed(false)
{	
}

Chat::~Chat()
{
}

void Chat::Init(IDirect3DDevice8* newDevice)
{
	auto screenRatio = Core::GetCore()->GetGraphics()->GetRatio();
	mChatWidth = screenRatio.x * 350;
	mChatHeight = screenRatio.y * 300;
	CreateTextures();
	RegisterInternalCommands();
}

void Chat::Render()
{
	if (mShouldUpdateTexture)
	{
		UpdateChatTexture();
		mShouldUpdateTexture = false;
	}

	if (mChatTexture != NULL)
	{
		if (Core::GetCore()->GetGraphics()->GetSprite()->Begin() == S_OK)
		{
			mAnimationTranslation -= 1;
			if (mAnimationTranslation < 0) mAnimationTranslation = 0;

			D3DXVECTOR2 scaling = { 1.0f, 1.0f };
			D3DXVECTOR2 translation = { 0.0f, mAnimationTranslation };
			Core::GetCore()->GetGraphics()->GetSprite()->Draw(mChatTexture, NULL, &scaling, NULL, 0.0f, &translation, D3DCOLOR_ARGB(255, 255, 255, 255));
			Core::GetCore()->GetGraphics()->GetSprite()->End();
		}
	}
}

void Chat::OnDeviceReset()
{
	if (mChatTexture != NULL)
		mChatTexture->Release();
	
	CreateTextures();
	mShouldUpdateTexture = true;
}

void Chat::OnDeviceLost()
{
	if (mChatTexture != NULL)
		mChatTexture->Release();
}

void Chat::AddMessage(std::wstring newMessage)
{
	mChatMessages.push_back(newMessage);
	
	if (((mChatMessages.size() + 4) * (mFontWeight + 3)) >= mChatHeight)
	{
		mChatMessages.erase(mChatMessages.begin());
		mAnimationTranslation = 20;
	}

	mShouldUpdateTexture = true;
}

void Chat::ProcessMessage(std::wstring messageToProcess)
{
	if (messageToProcess.find(L"/") != std::wstring::npos)
	{
		messageToProcess.erase(messageToProcess.begin());
		
		auto splitedMessage = WStringSplit(messageToProcess);

		for (auto currentCMD : mChatRegisteredCommands)
		{
			if (splitedMessage[0] == currentCMD.mCommandName) 
			{	
				splitedMessage.erase(splitedMessage.begin());
				
				if (currentCMD.mFunctionPointer != nullptr)
				{
					currentCMD.mFunctionPointer(splitedMessage);
				}
			}
		}
	}
	else
	{
		std::wstring nickName = Core::GetCore()->GetNetwork()->GetNickName();
		AddMessage(L"<" + nickName + L"> " + mTypingLine);

		//Send it 
		if (Core::GetCore()->GetNetwork()->IsConnected())
		{
			RakNet::BitStream bitStream;
			bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
			bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_CHATMSG));
			bitStream.Write(mTypingLine.size());
			bitStream.Write(mTypingLine.c_str());
			Core::GetCore()->GetNetwork()->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Core::GetCore()->GetNetwork()->GetServerAddress(), false);
		}
	}
}

void Chat::ProcessKeyboard(USHORT VKey, UINT Message)
{
	if (VKey == VK_CONTROL)	
		mIsControlPressed = (Message == WM_KEYDOWN);

	if(VKey == VK_RMENU)
		mIsAltPressed = (Message == WM_KEYDOWN);

	if (Message == WM_KEYDOWN || Message == WM_SYSKEYDOWN)
	{
		if (VKey == VK_T && !mIsTyping)
		{
			mTypingLine = L"";
			mIsTyping = true;
			mShouldUpdateTexture = true;
		}
		else if (VKey == VK_V && mIsTyping && mIsControlPressed)
		{
			if (OpenClipboard(NULL)) 
			{
				HANDLE handleClipboardData = GetClipboardData(CF_UNICODETEXT);
				std::wstring clipBoardData = std::wstring((wchar_t*)GlobalLock(handleClipboardData));
				
				if (clipBoardData.size() > 0)
					mTypingLine += clipBoardData;

				GlobalUnlock(handleClipboardData);
				CloseClipboard();
				mShouldUpdateTexture = true;
			}
		}
		else if (VKey == VK_RETURN && mIsTyping)
		{
			if (mTypingLine.size() > 0)
				ProcessMessage(mTypingLine);
			
			mIsTyping = false;
			mShouldUpdateTexture = true;
		}
		else if (VKey == VK_TAB && mIsTyping)
		{
			mTypingLine += L"	";	
			mShouldUpdateTexture = true;
		}
		else if (VKey == VK_BACK && mIsTyping)
		{
			if (mTypingLine.size() > 0)
				mTypingLine.pop_back();

			mShouldUpdateTexture = true;
		}
		else if (VKey == VK_ESCAPE && mIsTyping)
		{
			mIsTyping = false;
			mShouldUpdateTexture = true;
		}
		else if (mIsTyping)
		{
			const int BUFFER_LENGTH = 10;
			WCHAR wChars[BUFFER_LENGTH];
			BYTE  bKeyboardState[256];

			GetKeyboardState(bKeyboardState);
			UINT uScanCode = MapVirtualKey(VKey, MAPVK_VK_TO_VSC);
			int iResult = ToUnicode(VKey, uScanCode, bKeyboardState, wChars, 2, 0);

			if (iResult > 0)
			{
				if (iResult > 2) iResult = 2;

				wChars[iResult] = 0;

				std::wstring toAddToLine;
				for (int i = 0; i < iResult; i++)
				{
					toAddToLine += wChars[i];
				}
				
				int	lineWidth = Core::GetCore()->GetGraphics()->GetFontWidth(mTypingLine.c_str(), mChatFont);
				int toAddWidth = Core::GetCore()->GetGraphics()->GetFontWidth(toAddToLine.c_str(), mChatFont);

				if (lineWidth + toAddWidth < (mChatWidth - 34))
				{
					mTypingLine += toAddToLine;
					mShouldUpdateTexture = true;
				}
			}
		}
	}
}

void Chat::RegisterChatCMD(std::wstring cmdName, std::function<void(std::vector<std::wstring>)> args)
{
	mChatRegisteredCommands.push_back({ args, cmdName });
}

void Chat::UpdateChatTexture()
{
	IDirect3DSurface8* textureSurface, *oldDeviceTarget, *oldDeviceStencil;
	auto currentDevice = Core::GetCore()->GetGraphics()->GetDevice();

	if (!SUCCEEDED(mChatTexture->GetSurfaceLevel(0, &textureSurface)) ||
		 !SUCCEEDED(currentDevice->GetRenderTarget(&oldDeviceTarget)) ||
		 !SUCCEEDED(currentDevice->GetDepthStencilSurface(&oldDeviceStencil))) {
		return;
	}

	if (FAILED(currentDevice->SetRenderTarget(textureSurface, NULL)))
	{
		MessageBoxA(NULL, "Chat unable to SetRenderTarget", "Lost-Heaven Multiplayer ERROR", MB_OK);
	}

	Core::GetCore()->GetGraphics()->Clear(0, 0, mChatWidth, mChatHeight, D3DCOLOR_ARGB(0, 0, 0, 0));

	int currentY = 15;
	for (auto currentMessage : mChatMessages)
	{
		DrawTextShadow(currentMessage.c_str(), 15, currentY, 0xFFFFFFFF, true, mChatFont);
		currentY += (mFontWeight + 3);
	}

	currentY += (mFontWeight + 3);

	if (IsTyping())
	{
		DrawTextShadow((L"> " + mTypingLine).c_str(), 15, currentY, 0xFFFFFFFF, true, mChatFont);
	}

	currentDevice->SetRenderTarget(oldDeviceTarget, oldDeviceStencil);
	textureSurface->Release();
	oldDeviceTarget->Release();
	oldDeviceStencil->Release();
}

void Chat::DrawTextShadow(const wchar_t* text, int x, int y, D3DCOLOR color, bool ifShadow, LPD3DXFONT font)
{
	RECT rect;
	const int padding = 1;

	SetRect(&rect, x, y, x + 1000, y + 1000);

	if (ifShadow)
	{
		SetRect(&rect, x + padding, y, x + 1000, y + 1000);
		font->DrawTextW(text, wcslen(text), &rect, DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
		SetRect(&rect, x - padding, y, x + 1000, y + 1000);
		font->DrawTextW(text, wcslen(text), &rect, DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
		SetRect(&rect, x, y + padding, x + 1000, y + 1000);
		font->DrawTextW(text, wcslen(text), &rect, DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
		SetRect(&rect, x, y - padding, x + 1000, y + 1000);
		font->DrawTextW(text, wcslen(text), &rect, DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_XRGB(0, 0, 0));
		SetRect(&rect, x, y, x + 1000, y + 1000);
	}

	font->DrawTextW(text, wcslen(text), &rect, DT_NOCLIP | DT_SINGLELINE, color);
}

void Chat::CreateTextures()
{
	auto currentDevice = Core::GetCore()->GetGraphics()->GetDevice();

	if (FAILED(D3DXCreateTexture(currentDevice, mChatWidth, mChatHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &mChatTexture)))
	{
		MessageBoxA(NULL, "Unable to create chat texture !", "Lost-Heaven Multiplayer ERROR", MB_OK);
		return;
	}

	HFONT hFont = CreateFont(mFontWeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("tahomabd"));
	if (FAILED(D3DXCreateFont(currentDevice, hFont, &mChatFont)))
	{
		MessageBoxA(NULL, "Unable to create chat font !", "Lost-Heaven Multiplayer ERROR", MB_OK);
		return;
	}
}

void Chat::RegisterInternalCommands()
{
	RegisterChatCMD(L"exit", [&](std::vector<std::wstring> args) {

		exit(0);
	});

	RegisterChatCMD(L"connect", [&](std::vector<std::wstring> args) {

		if (args.size() > 0)
		{
			Core::GetCore()->GetNetwork()->Init();
			Core::GetCore()->GetNetwork()->Connect(std::string(args[0].begin(), args[0].end()).c_str(), 2715);
		}
		else AddMessage(L"You need to specify IP Address !");
	});

	RegisterChatCMD(L"hello", [&](std::vector<std::wstring> args) {

		AddMessage(L"Hello !");
	});
}
