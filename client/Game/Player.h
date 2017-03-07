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

	Interpolator* GetInterpolator() { return &mInterpolator; }
	MafiaSDK::C_Human* GetActor() { return mPlayer; }

	void SetNickName(std::wstring newName) { mNickName = newName; }
	std::wstring GetNickName() const { return mNickName; }

	void UpdateGameObject();
private:
	MafiaSDK::C_Human* mPlayer;
	Vector3D mPosition;
	unsigned short mRotation;
	std::string mModelName;
	std::wstring mNickName;
	float mHealth;
	char mAnimationState;
	Interpolator mInterpolator;
};