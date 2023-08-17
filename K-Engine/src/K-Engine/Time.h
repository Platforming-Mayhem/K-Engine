#pragma once
#include "build.h"

namespace K
{
	class K_API Time
	{
	public:
		static float startTime;
		static float endTime;
		static float deltaTime();
	};
}