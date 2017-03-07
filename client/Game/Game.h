#pragma once

const std::vector<std::pair<Vector3D, Vector3D>> mConnectingCameras = {

	{ { -1788.927612f, -4.542368f, -9.105090f }, { 0.982404f, 0.520000f, 1.017291f } },
	{ { -946.888489f, 7.61761f, 123.585570f },{ 1.345240f, 0.440000f, -0.436266f } },
	{ { -1118.757568f, 21.857584f, 466.080688f },{ 1.0f, -0.680000f, 0.927845f } },
	{ { -3517.833252f, 17.697630f, -606.585388f },{ -0.022980f, -0.440000f, -1.414027f } },
	{ { 912.252319f, 41.057575f, -903.820923f },{ 0.530041f, -0.120000f, 1.311128f } },
};

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