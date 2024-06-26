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
	QueueFamilyIndices& outIndices,
	SurfaceProperties& outSurfaceProps)
{
	for (int i = 0; i < m_Pool.size(); i++)
	{
		// Retrieving the required surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Pool[i], surface, &surfaceCapabilities);

		if (!(surfaceCapabilities.supportedUsageFlags &
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT))
			continue;

		VkSurfaceFormatKHR surfaceFormat = FindSuitableFormat(m_Pool[i], surface);

		SurfaceProperties properties{};
		properties.Usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		properties.MaxExtent = surfaceCapabilities.maxImageExtent;
		properties.MinExtent = surfaceCapabilities.minImageExtent;
		properties.CurrentExtent = surfaceCapabilities.currentExtent;
		properties.ColorSpace = surfaceFormat.colorSpace;
		properties.Format = surfaceFormat.format;
		properties.PresentMode = FindSuitablePresentationMode(m_Pool[i], surface);
		properties.MinImageCount = surfaceCapabilities.minImageCount;
		properties.MaxImageCount = surfaceCapabilities.maxImageCount;

		// Searching for the required queue families
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
				outSurfaceProps = properties;
				return m_Pool[i];
			}
		}
	}

	return nullptr;
}

VkSurfaceFormatKHR PhysicalDevicePool::FindSuitableFormat(VkPhysicalDevice device, VkSurfaceKHR surface) const noexcept
{
	uint32_t count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);

	std::vector<VkSurfaceFormatKHR> formats(count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, formats.data());

	for (auto& format : formats)
	{
		if (format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR &&
			format.format == VK_FORMAT_R8G8B8_SRGB)
			return format;
	}

	return formats[0];
}

VkPresentModeKHR PhysicalDevicePool::FindSuitablePresentationMode(VkPhysicalDevice device, VkSurfaceKHR surface) const noexcept
{
	uint32_t count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);

	std::vector<VkPresentModeKHR> modes(count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, modes.data());

	for (auto mode : modes)
	{
		if (mode == VK_PRESENT_MODE_FIFO_KHR)
			return mode;
	}

	return VK_PRESENT_MODE_IMMEDIATE_KHR;
}
