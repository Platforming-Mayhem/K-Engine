#include "K-Engine/Time.h"
namespace K 
{
	float Time::deltaTimeValue = 1 / 60;

	float Time::deltaTime()
	{
		return K::Time::deltaTimeValue;
	}
}