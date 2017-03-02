#include <stdinc.h>

Graphics::Graphics() : 
	mDevice(nullptr),
	mChat(new Chat()),
	mLoadingScreen(new LoadingScreen()),
	mSprite(nullptr),
	mNickNameFont(NULL),
	mNickNameFontSize(18)
{
}

Graphics::~Graphics()
{
}

void Graphics::HookDirectX()
{
	*(IDirect3DDevice8**)(IDirect3DDevice8_ADDRESS) = new Direct3DDevice8Proxy(GetDevice());
}

IDirect3DDevice8* Graphics::GetDevice()
{
	return *(IDirect3DDevice8**)(IDirect3DDevice8_ADDRESS);
}

bool Graphics::IsDirectModuleLoaded()
{
	return *(DWORD*)(IDirect3DDevice8_ADDRESS) != NULL;
}

void Graphics::Init(IDirect3DDevice8* newDevice)
{
	mDevice = newDevice;

	if (FAILED(D3DXCreateSprite(newDevice, &mSprite)))
	{
		MessageBoxA(NULL, "Unable to create sprite !", "Lost-Heaven Multiplayer ERROR", MB_OK);
	}

	HFONT hFont = CreateFont(mNickNameFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("verdana"));
	if (FAILED(D3DXCreateFont(newDevice, hFont, &mNickNameFont)))
	{
		MessageBoxA(NULL, "Unable to create font !", "Lost-Heaven Multiplayer ERROR", MB_OK);
		return;
	}

	mChat->Init(newDevice);
	mLoadingScreen->Init(newDevice);
}

void Graphics::OnSceneBegin()
{
}

void Graphics::OnSceneEnd()
{
	mChat->Render();
	mLoadingScreen->Render();

	DrawNickNames();
}

void Graphics::OnDeviceReset()
{
	mChat->OnDeviceReset();
	mLoadingScreen->OnDeviceReset();
}

void Graphics::OnDeviceLost()
{
	mChat->OnDeviceLost();
	mLoadingScreen->OnDeviceLost();
}

D3DXVECTOR2 Graphics::GetResolution()
{
	IDirect3DSurface8* pSurface;
	mDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pSurface);

	D3DSURFACE_DESC SurfaceDesc;
	pSurface->GetDesc(&SurfaceDesc);

	D3DXVECTOR2 returnVec = D3DXVECTOR2(SurfaceDesc.Width, SurfaceDesc.Height);
	pSurface->Release();

	return returnVec;
}

void Graphics::DrawNickNames()
{
	for (auto player : Core::GetCore()->GetNetwork()->GetPlayers())
	{
		auto playerPos = player.second->GetPosition();

		if (playerPos.Distance(Core::GetCore()->GetGame()->GetLocalPlayer()->GetPosition(), 50.0f) <= 1)
		{
			D3DXVECTOR3 playerPosEx = { playerPos.x,  playerPos.y + 2.1f, playerPos.z };
			D3DXVECTOR3 screenPos;
	
			Core::GetCore()->GetGraphics()->WorldToScreen(playerPosEx, &screenPos);
			int fontWidth = Core::GetCore()->GetGraphics()->GetFontWidth(player.second->GetNickName().c_str(), mNickNameFont);
			DrawTextShadow(player.second->GetNickName().c_str(), screenPos.x - (fontWidth / 2), screenPos.y, D3DCOLOR_ARGB(255, 255, 255, 255), true, mNickNameFont);
		}
	}
}

D3DXVECTOR2 Graphics::GetRatio()
{
	D3DXVECTOR2 screenResolution = GetResolution();
	return { screenResolution.x / 800, screenResolution.y / 600 };
}

void Graphics::Clear(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rec = { x, y, x + w, y + h };
	mDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 1.0f, 0);
}

int Graphics::GetFontWidth(const wchar_t * text, LPD3DXFONT font)
{
	RECT rcRect = { 0,0,0,0 };
	if (font)
	{
		font->DrawTextW(text, wcslen(text), &rcRect, DT_CALCRECT,
			D3DCOLOR_XRGB(0, 0, 0));
	}
	return rcRect.right - rcRect.left;
}

void Graphics::DrawTextShadow(const wchar_t* text, int x, int y, D3DCOLOR color, bool ifShadow, LPD3DXFONT font)
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

void Graphics::WorldToScreen(const D3DXVECTOR3 WorldPos, D3DXVECTOR3 * ScreenPos)
{
	D3DVIEWPORT8 viewPort;
	mDevice->GetViewport(&viewPort);

	D3DXMATRIX mProjection, mView, mWorld;
	mDevice->GetTransform(D3DTS_VIEW, &mView);

	D3DXMatrixIdentity(&mWorld);
	mDevice->GetTransform(D3DTS_PROJECTION, &mProjection);

	D3DXVec3Project(ScreenPos, &WorldPos, &viewPort, &mProjection, &mView, &mWorld);
}
