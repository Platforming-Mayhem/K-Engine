#include "Time.h"
namespace K 
{
	float Time::startTime = 0.0f;
	float Time::endTime = 0.0f;
	float Time::deltaTimeValue = 0.0f;

	float Time::deltaTime()
	{
		if (K::Time::deltaTimeValue <= 0.02f)
			return K::Time::deltaTimeValue;
		else
			return 0.02f;
	}
}