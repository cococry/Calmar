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

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;

        VkPipelineLayout pipelineLayout;
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

        struct swapChainSupportDetails {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

       public:
        static std::pair<bool, i32>
        initWithWin32(vulkanContext* context, HWND* window);

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

        static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        static swapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        static void createSwapChain();

        static void createImageViews();

        static void createGraphicsPipeline();

        static VkShaderModule createShaderModule(const std::vector<char>& code);

       private:
        static const std::vector<const char*> validationLayers;

        static const std::vector<const char*> deviceExtensions;

        static bool enableValidationLayers;
    };
}  // namespace calmar
