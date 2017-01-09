#pragma once 

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include "LinearMath.h"

#define RADIAN 3.14f / 180.0f
#define MAX_USHORT  (unsigned short) (-1)

namespace Utils
{
	static inline const std::string currentDateTime() {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
		// for more information about date/time format
		strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

		return buf;
	}

	static unsigned short DegreeMapToShort(float degree)
	{
		return static_cast<unsigned short>(degree * MAX_USHORT / 360.0f);
	}

	static float DegreeShortToMap(unsigned short degree)
	{
		return static_cast<float>(degree * 360.0f / MAX_USHORT);
	}

	static inline float RotationToDegree(Vector3D vector)
	{
		float result = atan2f(vector.x, vector.z)*180.0f / 3.14f;	// actually, this converts to degrees
		if (result < 0)
		{
			result = 360.0f - result*(-1);
		}
		return result;
	}

	static inline Vector3D DegreeToRotation(float degree)
	{
		Vector3D ang;
		ang.x = (float)(sin(degree*RADIAN));
		ang.z = (float)(cos(degree*RADIAN));
		return ang;
	}

	static float inline Lerp(float v0, float v1, float t) {
		if (t > 1) t = 1;

		return (1 - t) * v0 + t * v1;
	}
}