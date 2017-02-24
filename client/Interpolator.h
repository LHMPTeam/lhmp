#pragma once
class Interpolator
{
public:
	Interpolator();
	~Interpolator();

	void Set(const Vector3D & newValue);
	Vector3D LinearInterpolate(const Vector3D & oldPosition);


private:
	Vector3D mToBeInterpolated;
	RakNet::TimeMS mLastUpdate;
};

