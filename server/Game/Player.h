#pragma once

#include <Interfaces/PlayerInterface.h>
#include <string>

class Player : PlayerInterface
{
public:
	Player();
	~Player();

	void SetPosition(Vector3D position) override;
	Vector3D GetPosition() override;

	void SetRotation(Vector3D rotation) override;
	Vector3D GetRotation() override;
	
	void SetModel(const std::string& modelName) override;
	std::string GetModel() override;
	
	void SetHealth(float health) override;
	float GetHealth() override;
	
	void SetAnimationState(char state) override;
	char GetAnimationState() override;
	
private:
	Vector3D mPosition;
	Vector3D mRotation;
	std::string mModelName;
	float mHealth;
	char mAnimationState;
};

