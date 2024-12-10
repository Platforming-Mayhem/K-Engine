#include <K-Engine/Application.h>

class Sandbox : public K::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{

	}
};

int main() 
{
	Sandbox* sandbox = new Sandbox();
	sandbox->Run("K-Engine Demo #1");
	delete sandbox;
}