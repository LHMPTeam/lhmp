#include <stdinc.h>

Interpolator::Interpolator() : 
	mLastUpdate(RakNet::GetTimeMS())
{
}

Interpolator::~Interpolator()
{
}

void Interpolator::Set(const Vector3D & newValue)
{
	mToBeInterpolated = newValue;
}

Vector3D Interpolator::LinearInterpolate(const Vector3D & oldPosition)
{
	float amount = static_cast<float>(RakNet::GetTimeMS() / static_cast<float>(mLastUpdate));

	if (amount >= 1.0f) 
	{
		mLastUpdate = RakNet::GetTimeMS() + (RakNet::GetTimeMS() - Core::GetCore()->GetNetwork()->GetLastMessageTime());
		amount = 1.0f;
	}

	return oldPosition.Lerp(mToBeInterpolated, amount);
}
