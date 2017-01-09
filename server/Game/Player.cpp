#include "Player.h"



Player::Player(std::string modelName)
	: mModelName(modelName)
{
}


Player::~Player()
{
}

void Player::SetPosition(Vector3D position)
{
	mPosition = position;
}

Vector3D Player::GetPosition()
{
	return mPosition;
}

void Player::SetRotation(Vector3D rotation)
{
	mRotation = rotation;
}

Vector3D Player::GetRotation()
{
	return mRotation;
}

void Player::SetModel(const std::string& modelName)
{
	mModelName = modelName;
}

std::string Player::GetModel()
{
	return mModelName;
}

void Player::SetHealth(float health)
{
	mHealth = health;
}

float Player::GetHealth()
{
	return mHealth;
}

void Player::SetAnimationState(char state)
{
	mAnimationState = state;
}

char Player::GetAnimationState()
{
	return mAnimationState;
}
