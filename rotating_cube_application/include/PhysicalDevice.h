#ifndef _PHYSICAL_DEVICE_H
#define _PHYSICAL_DEVICE_H

#include <vulkan/vulkan.h>
#include <vector>

class PhysicalDevicePool
{
public:
	PhysicalDevicePool(VkInstance instance);

	VkPhysicalDevice FindSuitableDevice(VkSurfaceKHR surface,
		struct QueueFamilyIndices& outIndices);
	inline const std::vector<VkPhysicalDevice>& GetPool() { return m_Pool; }
private:
	std::vector<VkPhysicalDevice> m_Pool;
};


#endif // !_PHYSICAL_DEVICE_H
