#pragma once
#include <Interfaces/PlayerInterface.h>

enum RespawnStages
{
	RESPAWN_INIT,
	RESPAWN_PLAYER,
	RESPAWN_DONE
};

class LocalPlayer : PlayerInterface
{
public:
	LocalPlayer(std::string modelName);
	~LocalPlayer();

	void Spawn();
	void Respawn();

	void SetPosition(Vector3D position) override;
	Vector3D GetPosition() override;

	void SetRotation(unsigned short rotation) override;
	unsigned short GetRotation() override;

	void SetModel(const std::string& modelName) override;
	std::string GetModel() override;

	void SetHealth(float health) override;
	float GetHealth() override;

	void SetAnimationState(char state) override;
	char GetAnimationState() override;

	void SetIsCrouching(bool isCrouching) override;
	bool GetIsCrouching() override;

	void SetIsAiming(bool isCrouching) override;
	bool GetIsAiming() override;

	bool IsRespawning() const { return mIsRespawning; }
	void Tick();

private:
	bool mIsRespawning;
	MafiaSDK::C_Player* mPlayer;
	Vector3D mPosition;
	Vector3D mRotation;
	std::string mModelName;
	float mHealth;
	char mAnimationState;
	RakNet::TimeMS mRespawnTimer;
	RespawnStages mRespawnStage;
};