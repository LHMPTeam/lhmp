#include "Player.h"



Player::Player(std::string modelName)
	: mModelName(modelName),
	mHealth(200)
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

void Player::SetRotation(unsigned short rotation)
{
	mRotation = rotation;
}

unsigned short Player::GetRotation()
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

void Player::SetIsCrouching(bool isCrouching)
{
	mIsCrouching = isCrouching;
}

bool Player::GetIsCrouching()
{
	return mIsCrouching;
}


void Player::SetIsAiming(bool aiming)
{
	mIsAiming = aiming;
}

bool Player::GetIsAiming()
{
	return mIsAiming;
}

