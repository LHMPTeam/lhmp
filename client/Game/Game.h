#pragma once


class Game
{
public:
	Game();
	~Game();
	void Init();
	void OnGameStart();
	void OnGameInit();
	void Tick();
	LocalPlayer* GetLocalPlayer() { return mLocalPlayer; }
	void SetLocalPlayer(LocalPlayer* localPlayer) { mLocalPlayer = localPlayer; }
	void UpdateConnectingCamera();
private:
	bool mShouldStart;
	int mConnectingCameraNumber;
	RakNet::TimeMS mCameraTimer;
	LocalPlayer* mLocalPlayer;
	TickManager* mTickManager;
};