#pragma once
#include <Interfaces/PlayerInterface.h>

class Player : PlayerInterface
{
public:
	Player(std::string modelName);
	~Player();

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

	bool IsShooting() const { return mIsShooting; }
	void SetIsShooting(bool shooting) { mIsShooting = shooting; }

	Vector3D GetShootVec() const { return mShootPos; }
	void SetShootVec(const Vector3D shooting) { mShootPos = shooting; }

	Interpolator* GetInterpolator() { return &mInterpolator; }
	MafiaSDK::C_Human* GetActor() { return mPlayer; }

	void SetNickName(std::wstring newName) { mNickName = newName; }
	std::wstring GetNickName() const { return mNickName; }

	void UpdateGameObject();
private:
	MafiaSDK::C_Human* mPlayer;
	Vector3D mPosition;
	Vector3D mShootPos;
	bool mIsShooting;
	unsigned short mRotation;
	std::string mModelName;
	std::wstring mNickName;
	float mHealth;
	char mAnimationState;
	Interpolator mInterpolator;
};