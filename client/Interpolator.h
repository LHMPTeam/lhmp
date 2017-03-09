#pragma once
class Interpolator
{
public:
	Interpolator(int valuesCount);
	~Interpolator();
	void SetInitialValue(int index, float value) { mInitialValues[index] = value; };
	void SetTargetValue(int index, float value) { mTargetValues[index] = value; };
	float* Interpolate();
	void NewValues();
private:
	float* mInitialValues;
	float* mTargetValues;
	int mValuesCount;
	RakNet::TimeMS mLastUpdate;
	RakNet::TimeMS mPreviousUpdate;
};

