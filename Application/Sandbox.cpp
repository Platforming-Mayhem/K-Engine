#include <K_Engine.h>

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
	sandbox->Run("K-Engine");
	delete sandbox;
}