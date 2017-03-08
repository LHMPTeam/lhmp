#pragma once

#include <Interfaces/PlayerInterface.h>
#include <string>

class Player : public PlayerInterface
{
public:
	Player(std::string modelName);
	~Player();

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

	//void SetCurrentWeaponId(byte newId) { mCurrentWeaponId = newId; }
	//byte GetCurrentWeaponId() { return mCurrentWeaponId; }
private:
	Vector3D mPosition;
	//byte mCurrentWeaponId;
	unsigned short mRotation;
	std::string mModelName;
	float mHealth;
	char mAnimationState;
	bool mIsCrouching;
	bool mIsAiming;
};

