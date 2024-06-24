#include "Application.h"

#include <iostream>

int main(int argc, char** argv)
{
	Application& app = Application::GetInstance();
	ApplicationSpec spec = InitApplicationSpec("RotatingCube", 0, 0, 1);

#ifdef _DEBUG
	spec.Extensions.push_back("VK_EXT_debug_utils");
	spec.Layers.push_back("VK_LAYER_KHRONOS_validation");
#endif // _DEBUG

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