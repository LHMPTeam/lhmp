#pragma once

#include <PlayerInterface.h>

class Player : PlayerInterface
{
public:
	Player();
	~Player();

	void SetPosition(Vector3D position) override;

private:
	Vector3D mPosition;
};

