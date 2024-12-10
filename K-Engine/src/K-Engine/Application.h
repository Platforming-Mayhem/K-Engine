#pragma once
#include <K_Engine.h>

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