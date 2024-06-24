#include "Application.h"

#ifdef _DEBUG
#include "DebugLayer.h"
#endif // _DEBUG

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <chrono>
#include <iostream>

Application Application::s_Application;

Application& Application::GetInstance()
{
	return s_Application;
}

void Application::Init(ApplicationSpec& appSpec)
{
	m_ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_ApplicationInfo.pApplicationName = appSpec.AppName.c_str();
	m_ApplicationInfo.apiVersion = VK_API_VERSION_1_3;
	m_ApplicationInfo.applicationVersion = VK_MAKE_VERSION(appSpec.Major, appSpec.Minor, appSpec.Patch);

	VkInstanceCreateInfo instanceInfo{};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &m_ApplicationInfo;
	instanceInfo.ppEnabledExtensionNames = appSpec.Extensions.data();
	instanceInfo.ppEnabledLayerNames = appSpec.Layers.data();
	instanceInfo.enabledExtensionCount = appSpec.Extensions.size();
	instanceInfo.enabledLayerCount = appSpec.Layers.size();
#ifdef _DEBUG
	VkDebugUtilsMessengerCreateInfoEXT debugInfo = InitDebugLayerSpec();
	instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugInfo;
#endif // _DEBUG

	if (vkCreateInstance(&instanceInfo, nullptr, &m_Instance) != VK_SUCCESS)
		throw std::runtime_error::exception("Vulkan hasn't been initialized!");

	if (!glfwInit())
		throw std::runtime_error::exception("GLFW hasn't been initialized!");

#ifdef _DEBUG
	if (!LoadInitFunctions(m_Instance))
		throw std::runtime_error::exception("Debug lifecycle functions haven't been loaded!");

	if (CreateDebugUtilsMessengerEXT(m_Instance, &debugInfo, nullptr, &m_DebugLayer) != VK_SUCCESS)
		throw std::runtime_error::exception("Debug layer hasn't been initialized!");
#endif // _DEBUG

	InitWindow(appSpec);

}

void Application::Run()
{
	auto now = std::chrono::system_clock::now();
	auto previous = now;

	while (!m_Window->IsClosed())
	{
		now = std::chrono::system_clock::now();
		auto deltaTime = std::chrono::
			duration_cast<std::chrono::duration<double>>(
				now - previous
			);

		previous = now;

		m_Window->Update(deltaTime.count());
		m_Window->Poll();
	}
}

void Application::Terminate()
{
	delete m_Window;
#ifdef _DEBUG
	DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugLayer, nullptr);
#endif // _DEBUG
	vkDestroyInstance(m_Instance, nullptr);
	glfwTerminate();
}

void Application::InitWindow(ApplicationSpec& appSpec)
{
	m_Window = new Window(m_Instance, appSpec.WindowWidth, appSpec.WindowHeight, appSpec.AppName);
}

ApplicationSpec InitApplicationSpec(const std::string& appName, 
	int major, int minor, int patch,
	int width, int height)
{
	ApplicationSpec spec{};
	spec.AppName = appName;
	spec.Major = major;
	spec.Minor = minor;
	spec.Patch = patch;

	spec.WindowWidth = width;
	spec.WindowHeight = height;

	uint32_t count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);

	spec.Extensions.reserve(count);
	for (int i = 0; i < count; i++)
		spec.Extensions.push_back(extensions[i]);

	return spec;
}
