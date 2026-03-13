#include "K-Engine/Time.h"
namespace K 
{
	float Time::deltaTimeValue = 1.0f / 60.0f;

	float Time::deltaTime()
	{
		if (K::Time::deltaTimeValue < 1.0f / 30.0f) 
		{
			return K::Time::deltaTimeValue;
		}
		else 
		{
			return 1.0f / 30.0f;
		}
	}
}