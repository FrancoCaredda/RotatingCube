#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <vulkan/vulkan.h>

#include <string>
#include <vector>

struct ApplicationSpec
{
	std::string AppName;
	int Major, Minor, Patch;

	std::vector<const char*> Layers;
	std::vector<const char*> Extensions;
};

ApplicationSpec InitApplicationSpec(const std::string& appName, int major, int minor, int patch);

class Application
{
public:
	static Application& GetInstance();

	void Init(ApplicationSpec& appSpec);
	void Run();

	void Terminate();
private:
	Application() = default;
	static Application s_Application;

	VkApplicationInfo m_ApplicationInfo{};
	VkInstance m_Instance;

#ifdef _DEBUG
	VkDebugUtilsMessengerEXT m_DebugLayer;
#endif // _DEBUG

};

#endif // !_APPLICATION_H
