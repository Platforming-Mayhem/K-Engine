#include "Time.h"
namespace K 
{
	double Time::startTime = 0.0f;
	double Time::endTime = 0.0f;
	double Time::deltaTimeValue = 0.0f;

	double Time::deltaTime() 
	{
		return K::Time::deltaTimeValue;
	}
}