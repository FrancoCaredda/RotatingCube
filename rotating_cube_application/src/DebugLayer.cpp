#include "DebugLayer.h"

#include <iostream>

static PFN_vkCreateDebugUtilsMessengerEXT pCreateDebugUtilsMessengerEXT;
static PFN_vkDestroyDebugUtilsMessengerEXT pDestroyDebugUtilsMessengerEXT;

VkDebugUtilsMessengerCreateInfoEXT InitDebugLayerSpec()
{
    VkDebugUtilsMessengerCreateInfoEXT debugInfo{};

    debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    debugInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugInfo.pfnUserCallback = DebugUtilsMessengerCallbackEXT;

    return debugInfo;
}

VkBool32 VKAPI_PTR DebugUtilsMessengerCallbackEXT(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
                                                  VkDebugUtilsMessageTypeFlagsEXT messageTypes, 
                                                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
                                                  void* pUserData)
{
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        std::cerr << "[ERROR]: ";

        std::cerr << pCallbackData->pMessage << "\n";

        if (pCallbackData->pQueueLabels)
        {
            std::cerr << "[QUEUES]: ";
            for (int i = 0; i < pCallbackData->queueLabelCount - 1; i++)
                std::cerr << pCallbackData->pQueueLabels[i].pLabelName << ",";
            std::cerr << pCallbackData->pQueueLabels[pCallbackData->queueLabelCount - 1].pLabelName << "\n";
        }

        if (pCallbackData->pCmdBufLabels)
        {
            std::cerr << "[CMDB]: ";

            for (int i = 0; i < pCallbackData->cmdBufLabelCount - 1; i++)
                std::cerr << pCallbackData->pCmdBufLabels[i].pLabelName << ",";
            std::cerr << pCallbackData->pCmdBufLabels[pCallbackData->cmdBufLabelCount - 1].pLabelName << "\n";

        }

        std::cerr << std::endl;

        return true;
    }
     
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        std::cout << "[WARNING]: ";
    else
        std::cout << "[INFO]: ";

    std::cout << pCallbackData->pMessage << "\n";

    if (pCallbackData->pQueueLabels)
    {
        std::cout << "[QUEUES]: ";
        for (int i = 0; i < pCallbackData->queueLabelCount - 1; i++)
            std::cout << pCallbackData->pQueueLabels[i].pLabelName << ",";
        std::cout << pCallbackData->pQueueLabels[pCallbackData->queueLabelCount - 1].pLabelName << "\n";
    }

    if (pCallbackData->pCmdBufLabels)
    {
        std::cout << "[CMDB]: ";

        for (int i = 0; i < pCallbackData->cmdBufLabelCount - 1; i++)
            std::cout << pCallbackData->pCmdBufLabels[i].pLabelName << ",";
        std::cout << pCallbackData->pCmdBufLabels[pCallbackData->cmdBufLabelCount - 1].pLabelName << "\n";

    }

    std::cout << std::endl;

    return true;
}


bool LoadInitFunctions(VkInstance instance)
{
    pCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    pDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

    return pCreateDebugUtilsMessengerEXT != nullptr &&
        pDestroyDebugUtilsMessengerEXT != nullptr;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, 
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                      const VkAllocationCallbacks* pAllocator, 
                                      VkDebugUtilsMessengerEXT* pMessenger)
{
    return pCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, 
                                   VkDebugUtilsMessengerEXT messenger, 
                                   const VkAllocationCallbacks* pAllocator)
{
    pDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
}
