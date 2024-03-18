#pragma once

#include "defines.h"
#include "core\asserts.h"

#include <vulkan/vulkan.h>

// Checks the given expresssions' return value against VK_SUCCESS.
#define VK_CHECK(expr)                  \
    {                                   \
        BBASSERT(expr == VK_SUCCESS);   \
    }                                   \

typedef struct vulkan_swapchain_support_info {
    VkSurfaceCapabilitiesKHR capabilities;
    u32 format_count;
    VkSurfaceFormatKHR* formats;
    u32 present_mode_count;
    VkPresentModeKHR* present_modes;
} vulkan_swapchain_support_info;

typedef struct vulkan_device {
    VkPhysicalDevice physical_device;
    VkDevice logical_device;
} vulkan_device;

typedef struct vulkan_context {
    VkInstance instance;
    VkAllocationCallbacks* allocator;
    VkSurfaceKHR surface;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

    vulkan_device device;
} vulkan_context;


