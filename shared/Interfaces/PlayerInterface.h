#pragma once

#include <LinearMath.h>
#include <Utils.h>

class PlayerInterface
{
	virtual void SetPosition(Vector3D position) = 0;
	virtual Vector3D GetPosition() = 0;
	virtual void SetRotation(Vector3D rotation) = 0;
	virtual Vector3D GetRotation() = 0;
	virtual void SetModel(const std::string& modelName) = 0;
	virtual std::string GetModel() = 0;
	virtual void SetHealth(float health) = 0;
	virtual float GetHealth() = 0;
	virtual void SetAnimationState(char state) = 0;
	virtual char GetAnimationState() = 0;
};