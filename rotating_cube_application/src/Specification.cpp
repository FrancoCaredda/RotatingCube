#include "Specification.h"

#include <GLFW/glfw3.h>

void InitApplicationSpec(ApplicationSpec& outApplicationSpec, 
	const std::string& appName, int major, int minor, int patch)
{
	outApplicationSpec.AppName = appName;
	outApplicationSpec.Major = major;
	outApplicationSpec.Minor = minor;
	outApplicationSpec.Patch = patch;
}

void InitRendererSpec(RendererSpec& outRendererSpec, int width, int height)
{
	outRendererSpec.WindowWidth = width;
	outRendererSpec.WindowHeight = height;

	uint32_t count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);

	outRendererSpec.Extensions.reserve(count);
	for (int i = 0; i < count; i++)
		outRendererSpec.Extensions.push_back(extensions[i]);
}