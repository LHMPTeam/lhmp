#pragma once
constexpr unsigned long IDirect3DDevice8_ADDRESS = 0x101C597C;

class Graphics
{
public:
	Graphics();
	~Graphics();
	void HookDirectX();
	IDirect3DDevice8* Graphics::GetDevice();
	ID3DXSprite* GetSprite() { return mSprite; }
	bool IsDirectModuleLoaded();
	void Init(IDirect3DDevice8* newDevice);
	void OnSceneBegin();
	void OnSceneEnd();
	void OnDeviceReset();
	void OnDeviceLost();
	void Clear(int x, int y, int w, int h, D3DCOLOR color);
	int GetFontWidth(const wchar_t *text, LPD3DXFONT font);
	
	D3DXVECTOR2 GetRatio();
	D3DXVECTOR2 GetResolution();
	Chat* GetChat() { return mChat; }
	LoadingScreen* GetLoadingScreen() { return mLoadingScreen; }

private:
	LPD3DXSPRITE mSprite;
	IDirect3DDevice8* mDevice;
	Chat* mChat;
	LoadingScreen* mLoadingScreen;
};

