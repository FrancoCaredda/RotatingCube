#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Window.h"

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

struct ApplicationSpec
{
	std::string AppName;
	int Major, Minor, Patch;

	int WindowWidth, WindowHeight;

	std::vector<const char*> Layers;
	std::vector<const char*> Extensions;
};

ApplicationSpec InitApplicationSpec(const std::string& appName, 
	int major, int minor, int patch,
	int width, int height);

class Application
{
public:
	static Application& GetInstance();

	void Init(ApplicationSpec& appSpec);
	void Run();

	void Terminate();
private:
	void InitWindow(ApplicationSpec& appSpec);
private:
	Application() = default;
	static Application s_Application;

	VkApplicationInfo m_ApplicationInfo{};
	VkInstance m_Instance;

	Window* m_Window;

#ifdef _DEBUG
	VkDebugUtilsMessengerEXT m_DebugLayer;
#endif // _DEBUG

};

#endif // !_APPLICATION_H
