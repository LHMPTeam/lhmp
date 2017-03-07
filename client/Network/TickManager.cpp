#include "stdinc.h"

TickManager::TickManager()
{
	mStartSyncTime = RakNet::GetTimeMS();
	mLastTickBeginTime = 0;
	mLastTickDuration = 0;
	mTicksToSend = 0;
}

TickManager::~TickManager()
{

}

void TickManager::GameTick()
{
	mLastTickDuration = RakNet::GetTimeMS() - mLastTickBeginTime;
	mLastTickBeginTime = RakNet::GetTimeMS();
	if (mLastTickDuration == 0)
	{
		mTicksToSend = Core::GetCore()->GetNetwork()->GetTickRate();
		mTicksToSend--;
		return Tick();
	}
	int deltaTime = RakNet::GetTimeMS() - mStartSyncTime;
	if (deltaTime >= 1000)
	{
		mStartSyncTime = RakNet::GetTimeMS();
		mTicksToSend = Core::GetCore()->GetNetwork()->GetTickRate();
	}
	if (mTicksToSend == 0) return;
	if (RakNet::GetTimeMS() >= mNextTickTime)
	{
		mTicksToSend--;
		return Tick();
	}
	//sleepTime = (deltaTime - (mLastTickDuration * mTicksToSend)) / mTicksToSend
	int sleepTime = round((float)(deltaTime - (mLastTickDuration * mTicksToSend)) / (float)mTicksToSend);
	mNextTickTime = RakNet::GetTimeMS() + sleepTime;
}

void TickManager::Tick()
{
	int deltaTime = RakNet::GetTimeMS() - mStartSyncTime;
	//printf("Delta: %i, TicksToSend: %i\n", deltaTime, mTicksToSend);

	if (Core::GetCore()->GetGame()->GetLocalPlayer() != nullptr)
	{
		Core::GetCore()->GetGame()->GetLocalPlayer()->Tick();
	}
}
