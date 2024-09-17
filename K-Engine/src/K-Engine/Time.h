#pragma once
#include "build.h"

namespace K
{
	class K_API Time
	{
	public:
		static double startTime;
		static double endTime;
		static double deltaTimeValue;
		static double deltaTime();
	};
}