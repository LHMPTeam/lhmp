#pragma once
class LoadingScreen
{
public:
	LoadingScreen();
	~LoadingScreen();

	void Init(IDirect3DDevice8* newDevice);
	void Render();
	void OnDeviceReset();
	void OnDeviceLost();

	void SetLoading(bool isLoading) { IsLoading = isLoading; }
private:
	void CreateTextures();
	LPDIRECT3DTEXTURE8 mLoadingTexture;
	bool IsLoading;
};

