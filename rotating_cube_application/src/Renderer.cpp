#include "Renderer.h"
#include "Specification.h"
#include "DebugLayer.h"
#include "Window.h"
#include "PhysicalDevice.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

Renderer Renderer::s_Renderer;

void Renderer::Init(ApplicationSpec& spec,
					RendererSpec& rendererSpec,
					Window* window)
{
	InitInstance(spec, rendererSpec);
	InitSurface(window);
	InitPhysicalDevice();
}

void Renderer::Terminate()
{
	vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
#ifdef _DEBUG
	DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugLayer, nullptr);
#endif // _DEBUG
	vkDestroyInstance(m_Instance, nullptr);
	glfwTerminate();

	m_Instance = nullptr;
}

Renderer::~Renderer()
{
	if (m_Instance)
		Terminate();
}

void Renderer::InitInstance(ApplicationSpec& spec, RendererSpec& rendererSpec)
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = spec.AppName.c_str();
	appInfo.apiVersion = VK_API_VERSION_1_3;
	appInfo.applicationVersion = VK_MAKE_VERSION(spec.Major, spec.Minor, spec.Patch);

	VkInstanceCreateInfo instanceInfo{};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.ppEnabledExtensionNames = rendererSpec.Extensions.data();
	instanceInfo.ppEnabledLayerNames = rendererSpec.Layers.data();
	instanceInfo.enabledExtensionCount = rendererSpec.Extensions.size();
	instanceInfo.enabledLayerCount = rendererSpec.Layers.size();
#ifdef _DEBUG
	VkDebugUtilsMessengerCreateInfoEXT debugInfo = InitDebugLayerSpec();
	instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugInfo;
#endif // _DEBUG

	if (vkCreateInstance(&instanceInfo, nullptr, &m_Instance) != VK_SUCCESS)
		throw std::runtime_error::exception("Vulkan hasn't been initialized!");

#ifdef _DEBUG
	if (!LoadInitFunctions(m_Instance))
		throw std::runtime_error::exception("Debug lifecycle functions haven't been loaded!");

	if (CreateDebugUtilsMessengerEXT(m_Instance, &debugInfo, nullptr, &m_DebugLayer) != VK_SUCCESS)
		throw std::runtime_error::exception("Debug layer hasn't been initialized!");
#endif // _DEBUG
}

void Renderer::InitSurface(Window* window)
{
	if (glfwCreateWindowSurface(m_Instance, window->GetNative(), nullptr, &m_Surface) != VK_SUCCESS)
		throw std::runtime_error::exception("Surface hasn't been created!");
}

void Renderer::InitPhysicalDevice()
{
	PhysicalDevicePool pool(m_Instance);
	m_PhysicalDevice = pool.FindSuitableDevice(m_Surface, m_Indices);
}

void Renderer::InitDevice()
{
}

Renderer& Renderer::GetRenderer() noexcept
{
	return s_Renderer;
}
