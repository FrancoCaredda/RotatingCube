#include "PhysicalDevice.h"
#include "QueueFamilyIndices.h"

PhysicalDevicePool::PhysicalDevicePool(VkInstance instance)
{
	uint32_t physicalDevicesCount;
	vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, nullptr);

	m_Pool.resize(physicalDevicesCount);
	vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, m_Pool.data());
}

VkPhysicalDevice PhysicalDevicePool::FindSuitableDevice(VkSurfaceKHR surface,
	QueueFamilyIndices& outIndices)
{
	for (int i = 0; i < m_Pool.size(); i++)
	{
		QueueFamilyIndices indices{};
		std::vector<VkQueueFamilyProperties> queueFamilies;

		uint32_t queueFamiliesCount;
		vkGetPhysicalDeviceQueueFamilyProperties(m_Pool[i], &queueFamiliesCount, nullptr);

		queueFamilies.resize(queueFamiliesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_Pool[i], &queueFamiliesCount, queueFamilies.data());

		for (int j = 0; j < queueFamilies.size(); j++)
		{
			VkBool32 surfaceSupported;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_Pool[i], j, surface, &surfaceSupported);

			if (queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsQueue = j;

			if (surfaceSupported)
				indices.presentationQueue = j;

			if (indices.graphicsQueue.has_value() &&
				indices.presentationQueue.has_value())
			{
				outIndices = indices;
				return m_Pool[i];
			}
		}
	}

	return nullptr;
}
