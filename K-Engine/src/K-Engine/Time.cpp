#include "Time.h"
namespace K 
{
	float Time::startTime = 0.0f;
	float Time::endTime = 0.0f;
	float Time::deltaTime()
	{
		float delta = startTime - endTime;
		if (delta < 0.1f)
			return delta;
		else
			return 0.1f;
	}
}