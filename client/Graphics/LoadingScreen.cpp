#include <stdinc.h>

LoadingScreen::LoadingScreen() : 
	mLoadingTexture(NULL),
	IsLoading(false)
{
}

LoadingScreen::~LoadingScreen()
{
}

void LoadingScreen::Init(IDirect3DDevice8 * newDevice)
{
	CreateTextures();
}

void LoadingScreen::Render()
{
	if (mLoadingTexture != NULL && IsLoading)
	{
		if (Core::GetCore()->GetGraphics()->GetSprite()->Begin() == S_OK)
		{
			D3DXVECTOR2 res = Core::GetCore()->GetGraphics()->GetResolution();
			D3DSURFACE_DESC desc;
			mLoadingTexture->GetLevelDesc(0, &desc);

			D3DXVECTOR2 scaling = { res.x / desc.Width, res.y / desc.Height };
			Core::GetCore()->GetGraphics()->GetSprite()->Draw(mLoadingTexture, NULL, &scaling, NULL, 0.0f, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			float loadValue = *(float*)(0x647B1C);

			Core::GetCore()->GetGraphics()->Clear(0, res.y - 12, res.x * loadValue, 10, D3DCOLOR_ARGB(255, 255, 255, 255));			
			Core::GetCore()->GetGraphics()->GetSprite()->End();
		}
	}
}

void LoadingScreen::OnDeviceReset()
{
	if (mLoadingTexture != NULL)
		mLoadingTexture->Release();

	CreateTextures();
}

void LoadingScreen::OnDeviceLost()
{
	if (mLoadingTexture != NULL)
		mLoadingTexture->Release();
}

void LoadingScreen::CreateTextures()
{
	auto currentDevice = Core::GetCore()->GetGraphics()->GetDevice();

	if (FAILED(D3DXCreateTextureFromFileExA(currentDevice, "lhmp/loadingscreen.png", D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR, 0x1000ff00,
		NULL, NULL, &mLoadingTexture)))
	{
		MessageBoxA(NULL, "Unable to create chat texture !", "Lost-Heaven Multiplayer ERROR", MB_OK);
		return;
	}
}
