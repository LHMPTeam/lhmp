/**
	Lost Heaven Multiplayer

	Purpose: used for entity's movement interpolation / smoothing

	@author Original: Romop5, Overhaul: qbt
	@version 1.1 12/14/2015
*/

#ifndef __C_INTERPOLATION
#define __C_INTERPOLATION
#include "../shared/structures.h"

class CInterpolation {
	Vector3D		InterPosition, InterRotation, InterRotation2;

	RakNet::TimeMS	timestamp;
	RakNet::TimeMS	timeLastMessage;
	RakNet::TimeMS	timeDiff;
	RakNet::TimeMS	interpolationTick;

	Vector3D		actualPos, actualRot, actualRot2, previousPos, previousRot, previousRot2;

public:
	CInterpolation();
	void		SetTimestamp(RakNet::TimeMS);

	void		SetUpInterpolation(Vector3D position);
	void		SetUpInterpolationRot(Vector3D rotation);
	void		SetUpInterpolationRotVehicle(Vector3D rotation, Vector3D rotation2);

	Vector3D	Interpolate();
	Vector3D	InterpolateRot();
	Vector3D	InterpolateRotVehicle();
	Vector3D	InterpolateRotVehicle2();
	Vector3D	GetTempVect();
	Vector3D	GetInterVect();
	Vector3D	GetPosition();

	void		SetPosition(Vector3D pos);
	float		Lerp(float v0, float v1, float t);
};

#endif