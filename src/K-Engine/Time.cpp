#include "K-Engine/Time.h"
namespace K 
{
	float Time::deltaTimeValue = 1.0f / 60.0f;

	float Time::deltaTime()
	{
		return K::Time::deltaTimeValue;
	}
}