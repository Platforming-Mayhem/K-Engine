#include "Time.h"
namespace K 
{
	float Time::startTime;
	float Time::endTime;
	float Time::deltaTime()
	{
		return startTime - endTime;
	}
}