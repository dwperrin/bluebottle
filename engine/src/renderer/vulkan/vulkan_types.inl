#pragma once

#include "defines.h"
#include "core\asserts.h"

#include <vulkan/vulkan.h>

// Checks the given expresssions' return value against VK_SUCCESS.
#define VK_CHECK(expr)                  \
    {                                   \
        BBASSERT(expr == VK_SUCCESS);   \
    }                                   \

typedef struct vulkan_context {
    VkInstance instance;
    VkAllocationCallbacks* allocator;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

} vulkan_context;


