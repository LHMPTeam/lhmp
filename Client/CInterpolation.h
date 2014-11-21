/**
	Lost Heaven Multiplayer

	Purpose: used for entity's movement interpolation

	@author Romop5
	@version 1.0 1/9/14
	@todo clean code, add lag compensation, key sync
*/

#ifndef __C_INTERPOLATION
#define __C_INTERPOLATION
#include "../shared/structures.h"
class CInterpolation
{
	Vector3D		InterPosition;
	RakNet::TimeMS	timestamp;
	RakNet::TimeMS	timeLastMessage;
	RakNet::TimeMS	timeDiff;
	RakNet::TimeMS	interpolationTick;
	RakNet::TimeMS	interpolationGenerated;
	bool			shouldUpdate;
	Vector3D		interpolationCache;
	bool			bIsTempInterpolationRunning;
	bool			bIsInterpolationRunning;
	RakNet::TimeMS	dwMsTempRun;

	Vector3D		actual, previous;
	Vector3D		interpolationVector; // metres per ms
	Vector3D		tempVector;

public:
	CInterpolation();
	void		SetTimestamp(RakNet::TimeMS);
	void		SetUpInterpolation(Vector3D pos);
	Vector3D	Interpolate();
	Vector3D	GetTempVect();
	Vector3D	GetInterVect();
	Vector3D	GetPosition();
	void		SetPosition(Vector3D pos);
};
#endif