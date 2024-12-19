#pragma once
#include "build.h"

namespace K 
{
	class K_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run(const char* windowName);
	};
}