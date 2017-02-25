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
	mNetwork = new Network();
	mGame = new Game();
	mGame->Init();

	mIsRunning = true;
}

void Core::Tick()
{
	mGame->Tick();

	if (mNetwork->IsRunning())
	{
		mNetwork->Tick();
	}
}

Core* Core::CoreInstance;

Core* Core::GetCore()
{
	return CoreInstance;
}