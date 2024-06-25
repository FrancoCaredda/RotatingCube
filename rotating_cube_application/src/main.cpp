#include "Application.h"
#include "Specification.h"

#include <iostream>

int main(int argc, char** argv)
{
	Application& app = Application::GetInstance();

	ApplicationSpec spec{};
	InitApplicationSpec(spec, "RotatingCube", 0, 0, 1);

	try
	{
		app.Init(spec);
		app.Run();
		app.Terminate();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	

	return 0;
}