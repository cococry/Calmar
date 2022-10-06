#pragma once

#include "calmar/core/defines.hpp"

#include <vulkan/vulkan.h>

#include <utility>

#include <Windows.h>

#include <vector>
#include <optional>

namespace calmar {
    struct vulkanContext {
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicalDevice;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSurfaceKHR surface;
    };

    class vulkan {
       public:
        struct queueFamilyIndices {
            std::optional<u32> graphicsFamily;
            std::optional<u32> presentFamily;

            bool isComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

       public:
        static std::pair<bool, i32> initWithWin32(vulkanContext* context, HWND* window);

        static void initWithGLFW(bool useValidationLayers);

        static void shutdown();

        static vulkanContext context;

       private:
        static void setupDebugMessenger();

        static void createInstanceGLFW(bool useValidationLayers);

        static bool checkValidationLayerSupport();

        static std::vector<const char*> getRequiredExtensions();

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

        static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

        static void pickPhysicalDevice();

        static bool isPhysicalDeviceGraphicsSuitable(VkPhysicalDevice device);

        static i32 rateDeviceSuitability(VkPhysicalDevice device);

        static queueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        static void createLogicalDevice();

        static void createSurfaceGLFW();

       private:
        static const std::vector<const char*> validationLayers;

        static bool enableValidationLayers;
    };
}  // namespace calmar
