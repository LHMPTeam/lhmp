#include "CInterpolation.h"
#include "CCore.h"
extern CCore *g_CCore;
//#include "../shared/tools.h"
CInterpolation::CInterpolation()
{
	//InterPosition				= Tools::NullVect();
	InterPosition.x = -1985.966675f;
	InterPosition.y = 5.037054f;
	InterPosition.z = 4.284860f;
	timestamp					= RakNet::TimeMS();
	timeLastMessage				= RakNet::TimeMS();
	timeDiff					= RakNet::TimeMS();
	interpolationTick			= RakNet::TimeMS();
	interpolationGenerated		= RakNet::TimeMS();
	shouldUpdate				= true;
	interpolationCache			= Tools::NullVect();;
	bIsTempInterpolationRunning = false;
	bIsInterpolationRunning		= false;
	dwMsTempRun					= RakNet::TimeMS();

	actual						= Tools::NullVect(); 
	previous					= Tools::NullVect();
	timeLastMessage				= RakNet::TimeMS();
	interpolationVector			= Tools::NullVect(); // metres per ms
	tempVector					= Tools::NullVect();
}
void CInterpolation::SetUpInterpolation(Vector3D position)
{
	this->previous = this->actual;
	this->actual = position;
	this->timeDiff = (this->timestamp) - (this->timeLastMessage);

	if (this->timeDiff > 0)
	{
		interpolationVector.x = this->actual.x - this->previous.x;
		interpolationVector.x = (this->interpolationVector.x / ((float)this->timeDiff));
		interpolationVector.y = this->actual.y - this->previous.y;
		interpolationVector.y = (this->interpolationVector.y / ((float)this->timeDiff));
		interpolationVector.z = this->actual.z - this->previous.z;
		interpolationVector.z = (this->interpolationVector.z / ((float)this->timeDiff));
	}
	else
	{
		interpolationVector.x = 0;
		interpolationVector.y = 0;
		interpolationVector.z = 0;
	}

	this->timeLastMessage = this->timestamp;
	interpolationTick = RakNet::GetTimeMS();
	float defaultTickTime = 30.0f;

	if (this->timeDiff > 0)
	{
		RakNet::TimeMS thisTime = (RakNet::GetTimeMS()) - (this->timestamp);
		tempVector.x = (interpolationVector.x) + (((this->actual.x) - (this->InterPosition.x) + (interpolationVector.x*thisTime)) / (this->timeDiff));
		tempVector.y = (interpolationVector.y) + (((this->actual.y) - (this->InterPosition.y) + (interpolationVector.y*thisTime)) / (this->timeDiff));
		tempVector.z = (interpolationVector.z) + (((this->actual.z) - (this->InterPosition.z) + (interpolationVector.z*thisTime)) / (this->timeDiff));
	}
	else
	{
		tempVector.x = 0;
		tempVector.y = 0;
		tempVector.z = 0;
	}
	interpolationTick = RakNet::GetTimeMS();
	bIsTempInterpolationRunning = true;
	bIsInterpolationRunning = true;
	dwMsTempRun = 0;

	if (!(this->timeDiff > 0))
	{
		this->InterPosition = position;
	}

}

Vector3D CInterpolation::Interpolate()
{
	// this func should be called in Endscene
	// hardcoded by Romop5, 25.12 at 0:10 AM :D :D :D
		RakNet::TimeMS actualtime = RakNet::GetTimeMS();
		RakNet::TimeMS b = this->timestamp;
		//RakNet::TimeMS c = this->timeDiff;
		RakNet::TimeMS tickDiff = (actualtime - interpolationTick);

		if (this->bIsTempInterpolationRunning == true)
		{
			this->dwMsTempRun += (actualtime - interpolationTick);
			if (dwMsTempRun > 30)
			{
				this->bIsTempInterpolationRunning = false;
				RakNet::TimeMS tempCoef = 30 - (dwMsTempRun - tickDiff);
				RakNet::TimeMS normalCoef = dwMsTempRun - 30;
				//sprintf(buff, ": %f - %f - %f", (float)(actualtime - interpolationTick), (float)dwMsTempRun, (float)tickDiff);
				//g_CCore->GetLog()->AddLog(buff);
				//sprintf(buff, "coef: %f - %f", (float)tempCoef, (float)dwMsTempRun, (float)normalCoef);
				//g_CCore->GetLog()->AddLog(buff);
				this->InterPosition.x += (interpolationVector.x*((float)normalCoef));
				this->InterPosition.y += (interpolationVector.y*((float)normalCoef));
				this->InterPosition.z += (interpolationVector.z*((float)normalCoef));
				this->InterPosition.x += (tempVector.x*((float)tempCoef));
				this->InterPosition.y += (tempVector.y*((float)tempCoef));
				this->InterPosition.z += (tempVector.z*((float)tempCoef));
			}
			else
			{
				this->InterPosition.x += (tempVector.x*((float)tickDiff));
				this->InterPosition.y += (tempVector.y*((float)tickDiff));
				this->InterPosition.z += (tempVector.z*((float)tickDiff));
			}
		}
		else
		{
			this->InterPosition.x += (interpolationVector.x*((float)tickDiff));
			this->InterPosition.y += (interpolationVector.y*((float)tickDiff));
			this->InterPosition.z += (interpolationVector.z*((float)tickDiff));
		}
		interpolationTick = actualtime;
		return (this->InterPosition);
}

void	CInterpolation::SetTimestamp(RakNet::TimeMS time)
{
	//this->timeLastMessage	= this->timestamp;
	this->timestamp			= time;
}

Vector3D CInterpolation::GetPosition()
{
	return this->InterPosition;
}


void CInterpolation::SetPosition(Vector3D pos)
{
	this->InterPosition = pos;
}