#pragma once

#include <LinearMath.h>
#include <Utils.h>

struct OnFootSync
{
	Vector3D Position;
	Vector3D Rotation;
	char animationState;
	bool isCrouching;
};