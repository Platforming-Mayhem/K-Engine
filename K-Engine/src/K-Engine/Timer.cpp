#include "Timer.h"

namespace K 
{
	Timer::Timer() 
	{

	}

	Timer::~Timer() 
	{
		
	}

	void Timer::Init() 
	{

	}

	void Timer::Update() 
	{
		if (this->startCountDown)
		{
			if (this->countDown > 0.0f)
			{
				this->countDown -= K::Time::deltaTime();
			}
			else
			{
				K::SceneManager::LoadNextScene();
				this->startCountDown = false;
			}
		}
	}

	void Timer::UpdateEditor() 
	{
		if (ImGui::CollapsingHeader("Timer Settings")) 
		{

		}
	}

	void Timer::Bind() 
	{

	}

	void Timer::Unbind() 
	{
		
	}
	
	void Timer::SetPropertyValues(const char* value, int valueIndex) 
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			switch (valueIndex)
			{
			case 0:
				break;
			}
		}
	}

	const char* Timer::GetName() 
	{
		return typeid(K::Timer).name();
	}

	const char* Timer::GetPropertyValues() 
	{
		return this->properties.c_str();
	}
}