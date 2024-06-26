#ifndef _PHYSICAL_DEVICE_H
#define _PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>
#include <vector>

struct SurfaceProperties
{
	uint32_t MinImageCount;
	uint32_t MaxImageCount;
	VkImageUsageFlags Usage;
	VkColorSpaceKHR ColorSpace;
	VkExtent2D MinExtent;
	VkExtent2D MaxExtent;
	VkExtent2D CurrentExtent;
	VkFormat Format;
	VkPresentModeKHR PresentMode;
};

class PhysicalDevicePool
{
public:
	PhysicalDevicePool(VkInstance instance);

	VkPhysicalDevice FindSuitableDevice(VkSurfaceKHR surface,
		struct QueueFamilyIndices& outIndices,
		SurfaceProperties& outSurfaceProps);
	inline const std::vector<VkPhysicalDevice>& GetPool() { return m_Pool; }
private:
	VkSurfaceFormatKHR FindSuitableFormat(VkPhysicalDevice device, VkSurfaceKHR surface) const noexcept;
	VkPresentModeKHR FindSuitablePresentationMode(VkPhysicalDevice device, VkSurfaceKHR surface) const noexcept;
private:
	std::vector<VkPhysicalDevice> m_Pool;
};


#endif // !_PHYSICAL_DEVICE_H
