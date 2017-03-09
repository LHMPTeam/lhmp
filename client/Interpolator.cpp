#include <stdinc.h>

Interpolator::Interpolator(int valuesCount)
{
	mValuesCount = valuesCount;
	mInitialValues = new float[valuesCount]();
	mTargetValues = new float[valuesCount]();
	mPreviousUpdate = RakNet::GetTimeMS();
	mLastUpdate = RakNet::GetTimeMS();
}

Interpolator::~Interpolator()
{

}

float * Interpolator::Interpolate()
{
	float lastInterpolationTime = static_cast<float>(mLastUpdate - mPreviousUpdate);
	float timeDelta = static_cast<float>(RakNet::GetTimeMS() - mLastUpdate);
	float lerpValue = timeDelta / lastInterpolationTime;
	if (lerpValue >= 1.0f) {
		return mTargetValues;
	}
	float* interpolatedValues = new float[mValuesCount]();
	for (int i = 0; i < mValuesCount; i++) {
		interpolatedValues[i] = Utils::Lerp(mInitialValues[i], mTargetValues[i], lerpValue);
	}
	//printf("Lerp(%f, %f, %f);\n", mInitialValues[0], mTargetValues[0], lerpValue);
	printf("LIT: %f, Delta: %f, Value: %f, %f, %f, %f\n", lastInterpolationTime, timeDelta, lerpValue, mInitialValues[0], interpolatedValues[0], mTargetValues[0]);
	return interpolatedValues;
}

void Interpolator::NewValues()
{
	mPreviousUpdate = mLastUpdate;
	mLastUpdate = RakNet::GetTimeMS();
	//printf("%d, %d\n", mPreviousUpdate, mLastUpdate);
}

