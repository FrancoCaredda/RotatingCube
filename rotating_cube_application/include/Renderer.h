#ifndef _RENDERER_H
#define _RENDERER_H

#include "QueueFamilyIndices.h"

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
	void InitDevice();
private:
	Renderer() = default;
	static Renderer s_Renderer;

private: // Vulkan Instances
	VkInstance m_Instance;

#ifdef _DEBUG
	VkDebugUtilsMessengerEXT m_DebugLayer;
#endif // !_DEBUG

	VkSurfaceKHR m_Surface;
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice m_Device;
	VkQueue m_GraphicsQueue;
	VkQueue m_PresentationQueue;

	QueueFamilyIndices m_Indices{};
};

#endif // !_RENDERER_H
