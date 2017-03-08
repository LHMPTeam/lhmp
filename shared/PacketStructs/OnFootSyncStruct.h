#pragma once

#include <LinearMath.h>
#include <Utils.h>

struct OnFootSyncStruct
{
	Vector3D Position;
	unsigned short Rotation;
	char animationState;
	bool isCrouching;
	bool isAiming;
};