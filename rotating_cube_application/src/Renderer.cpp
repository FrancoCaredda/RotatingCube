#include "Renderer.h"
#include "Specification.h"
#include "DebugLayer.h"
#include "Window.h"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <algorithm>
#include <cmath>

Renderer Renderer::s_Renderer;

void Renderer::Init(ApplicationSpec& spec,
					RendererSpec& rendererSpec,
					Window* window)
{
	DeviceSpec deviceSpec{};

	deviceSpec.Extensions.push_back("VK_KHR_swapchain");

	InitInstance(spec, rendererSpec);
	InitSurface(window);
	InitPhysicalDevice();
	InitDevice(deviceSpec);
	InitSwapchain(window);
}

void Renderer::Terminate()
{
	vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
	vkDestroyDevice(m_Device, nullptr);
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
	m_PhysicalDevice = pool.FindSuitableDevice(m_Surface, m_Indices, m_Properties);
}

void Renderer::InitDevice(DeviceSpec& deviceSpec)
{
	const uint32_t indices[] = {
		m_Indices.graphicsQueue.value(),
		m_Indices.presentationQueue.value() 
	};
	uint32_t count = sizeof(indices) / sizeof(uint32_t);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	queueCreateInfos.reserve(count);

	float priority = 1.0f;
	for (int i = 0; i < count; i++)
	{
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = indices[i];
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &priority;

		queueCreateInfos.push_back(queueInfo);
	}

	if (indices[0] == indices[1])
		queueCreateInfos.pop_back();

	VkDeviceCreateInfo deviceInfo{};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceInfo.queueCreateInfoCount = queueCreateInfos.size();
	deviceInfo.ppEnabledExtensionNames = deviceSpec.Extensions.data();
	deviceInfo.ppEnabledLayerNames = deviceSpec.Layers.data();
	deviceInfo.enabledExtensionCount = deviceSpec.Extensions.size();
	deviceInfo.enabledLayerCount = deviceSpec.Layers.size();

	if (vkCreateDevice(m_PhysicalDevice, &deviceInfo, nullptr, &m_Device) != VK_SUCCESS)
		throw std::runtime_error::exception("Device hasn't been created!");

	vkGetDeviceQueue(m_Device, indices[0], 0, &m_GraphicsQueue);
	vkGetDeviceQueue(m_Device, indices[1], 0, &m_PresentationQueue);
}

void Renderer::InitSwapchain(Window* window)
{
	int width, height;
	glfwGetFramebufferSize(window->GetNative(), &width, &height);

	VkSwapchainCreateInfoKHR swapchainInfo{};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface = m_Surface;
	swapchainInfo.minImageCount = m_Properties.MinImageCount;
	swapchainInfo.imageUsage = m_Properties.Usage;
	swapchainInfo.imageColorSpace = m_Properties.ColorSpace;
	swapchainInfo.imageFormat = m_Properties.Format;
	swapchainInfo.presentMode = m_Properties.PresentMode;
	swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchainInfo.imageArrayLayers = 1;

	if (m_Properties.CurrentExtent.width == UINT32_MAX)
	{
		swapchainInfo.imageExtent = m_Properties.CurrentExtent;
	}
	else
	{
		swapchainInfo.imageExtent.width = 
			std::clamp<uint32_t>(width, m_Properties.MinExtent.width, m_Properties.MaxExtent.width);
		swapchainInfo.imageExtent.height =
			std::clamp<uint32_t>(height, m_Properties.MinExtent.height, m_Properties.MaxExtent.height); 
	}

	uint32_t indices[] = { m_Indices.graphicsQueue.value(), m_Indices.presentationQueue.value() };
	if (indices[0] ==
		indices[1])
	{
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	else
	{
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainInfo.pQueueFamilyIndices = indices;
		swapchainInfo.queueFamilyIndexCount = sizeof(indices) / sizeof(uint32_t);
	}

	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.clipped = VK_FALSE;
	swapchainInfo.oldSwapchain = m_Swapchain;

	if (vkCreateSwapchainKHR(m_Device, &swapchainInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
		throw std::runtime_error::exception("Swapchain hasn't been created!");
}

Renderer& Renderer::GetRenderer() noexcept
{
	return s_Renderer;
}
