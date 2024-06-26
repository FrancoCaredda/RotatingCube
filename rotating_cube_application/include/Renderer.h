#ifndef _RENDERER_H
#define _RENDERER_H

#include "QueueFamilyIndices.h"
#include "PhysicalDevice.h"

#include <vulkan/vulkan.h>
#include <vector>

class Renderer
{
public:
	static Renderer& GetRenderer() noexcept;

	void Init(struct ApplicationSpec& spec,
			  struct RendererSpec& rendererSpec,
			  class Window* window);
	void Terminate();

	~Renderer();
private:
	void InitInstance(ApplicationSpec& spec,
					  RendererSpec& rendererSpec);
	void InitSurface(class Window* window);
	void InitPhysicalDevice();
	void InitDevice(struct DeviceSpec& deviceSpec);
	void InitSwapchain(class Window* window);
private:
	Renderer() = default;
	static Renderer s_Renderer;

private: // Vulkan Instances
	VkInstance m_Instance = nullptr;

#ifdef _DEBUG
	VkDebugUtilsMessengerEXT m_DebugLayer = nullptr;
#endif // !_DEBUG

	VkSurfaceKHR m_Surface = nullptr;
	VkPhysicalDevice m_PhysicalDevice = nullptr;
	VkDevice m_Device = nullptr;
	VkQueue m_GraphicsQueue = nullptr;
	VkQueue m_PresentationQueue = nullptr;
	VkSwapchainKHR m_Swapchain = nullptr;

	QueueFamilyIndices m_Indices{};
	SurfaceProperties m_Properties{};
};

#endif // !_RENDERER_H
