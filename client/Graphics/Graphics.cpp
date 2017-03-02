#include <stdinc.h>

Graphics::Graphics() : 
	mDevice(nullptr),
	mChat(new Chat()),
	mLoadingScreen(new LoadingScreen()),
	mSprite(nullptr)
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
