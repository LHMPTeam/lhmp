#include <stdinc.h>

Core::Core()
{
	CoreInstance = this;
}

Core::~Core()
{
}

void Core::Init()
{
	mSandbox = new Sandbox();
	mNetwork = new Network();
	mGraphics = new Graphics();
	mGame = new Game();

	//Wait for directx to hook
	Sleep(200);

	while ( !mGraphics->IsDirectModuleLoaded())
	{
		Sleep(10);
	}

	mGraphics->HookDirectX();
	RawHook::Hook();
	
	mGame->Init();
	mGraphics->GetLoadingScreen()->SetLoading(true);
	mIsRunning = true;
}

void Core::Tick()
{
	mGame->Tick();

	if (mNetwork->IsRunning())
	{
		mNetwork->Tick();
	}

	mSandbox->Tick();
}

Core* Core::CoreInstance;

Core* Core::GetCore()
{
	return CoreInstance;
}