#ifndef _DEBUG_LAYER_H
#define _DEBUG_LAYER_H

#include <vulkan/vulkan.h>

VkDebugUtilsMessengerCreateInfoEXT InitDebugLayerSpec();

bool LoadInitFunctions(VkInstance instance);

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator);

VkBool32 VKAPI_PTR DebugUtilsMessengerCallbackEXT(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);

#endif // _DEBUG_LAYER_H
