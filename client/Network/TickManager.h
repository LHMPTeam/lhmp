#pragma once

class TickManager
{
public:
	TickManager();
	~TickManager();
	void GameTick();
private:
	void Tick();
	RakNet::TimeMS mStartSyncTime;
	RakNet::TimeMS mLastTickBeginTime;
	RakNet::TimeMS mLastTickDuration;
	RakNet::TimeMS mNextTickTime;
	short mTicksToSend;
};