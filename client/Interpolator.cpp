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
	float deltaTickTime = static_cast<float>(RakNet::GetTimeMS() - mLastUpdate);
	float packetDeltaTime = static_cast<float>(Core::GetCore()->GetNetwork()->GetLastMessageTime() - Core::GetCore()->GetNetwork()->GetPreviousMessageTime());
	float packetDifferenceTime = static_cast<float>(RakNet::GetTimeMS() - Core::GetCore()->GetNetwork()->GetLastMessageTime());
	
	Vector3D returnLerped = oldPosition.Lerp(mToBeInterpolated, Core::GetCore()->GetGraphics()->GetChat()->GetDeltaLerp());
	mLastUpdate = RakNet::GetTimeMS();
	return returnLerped;
}
