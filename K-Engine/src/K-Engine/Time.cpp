#include "Time.h"
namespace K 
{
	float Time::startTime = 0.0f;
	float Time::endTime = 0.0f;
	float Time::deltaTimeValue = 0.0f;

	float Time::deltaTime()
	{
		return K::Time::deltaTimeValue;
	}
}