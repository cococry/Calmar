#include "vulkan_renderer.hpp"
#include "calmar/core/asserting.hpp"
#include "calmar/core/application.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_win32.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "calmar/platform/glfw/window_glfw.hpp"

#include <map>
#include <set>

#define ASSERT_MAKE_VULKAN_ERROR(vkFunc, errormsg)                                                           \
    if (vkFunc == VK_SUCCESS) {                                                                              \
    } else {                                                                                                 \
        CALMAR_ERROR("Vulkan function {0} failed to execute correctly. Vulkan error: {1}", #vkFunc, vkFunc); \
    }                                                                                                        \
    CALMAR_ASSERT_MSG(vkFunc == VK_SUCCESS, errormsg);

static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    CALMAR_DEBUG("Vulkan Validation: {0}", pCallbackData->pMessage);

    return VK_FALSE;
}

namespace calmar {
    const std::vector<const char*> vulkan::validationLayers = {
        "VK_LAYER_KHRONOS_validation"};
    bool vulkan::enableValidationLayers;
    vulkanContext vulkan::context;

    std::pair<bool, i32> vulkan::initWithWin32(vulkanContext* context, HWND* window) {
        std::pair<bool, i32> ret = {};
        ret.first = true;
        ret.second = VK_SUCCESS;

        // TODO: Implement

        return ret;
    }

    void vulkan::initWithGLFW(bool useValidationLayers) {
        createInstanceGLFW(useValidationLayers);
        setupDebugMessenger();
        createSurfaceGLFW();
        pickPhysicalDevice();
        createLogicalDevice();
    }
    void vulkan::createInstanceGLFW(bool useValidationLayers) {
        enableValidationLayers = useValidationLayers;

        if (enableValidationLayers && !checkValidationLayerSupport()) {
            CALMAR_ASSERT_MSG(false, "Tried to use vulkan validation layers but not supported.");
        }

        /* Creating the instance */

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Calmar Editor";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.pEngineName = "Calmar Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        ASSERT_MAKE_VULKAN_ERROR(vkCreateInstance(&createInfo, nullptr, &context.instance), "Failed to create vulkan instance.");
        CALMAR_TRACE("Vulkan: Successfully created instance.");
    }

    bool vulkan::checkValidationLayerSupport() {
        u32 layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> vulkan::getRequiredExtensions() {
        u32 glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void vulkan::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = vulkanDebugCallback;
    }

    void vulkan::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    VkResult vulkan::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void vulkan::setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        ASSERT_MAKE_VULKAN_ERROR(CreateDebugUtilsMessengerEXT(context.instance, &createInfo, nullptr, &context.debugMessenger), "Failed to create debug messenger.");
    }

    void vulkan::shutdown() {
        vkDestroyDevice(context.logicalDevice, nullptr);
        if (enableValidationLayers)
            DestroyDebugUtilsMessengerEXT(context.instance, context.debugMessenger, nullptr);

        vkDestroySurfaceKHR(context.instance, context.surface, nullptr);
        vkDestroyInstance(context.instance, nullptr);
    }

    void vulkan::pickPhysicalDevice() {
        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(context.instance, &deviceCount, nullptr);

        CALMAR_ASSERT_MSG(deviceCount != 0, "Selected GPU does not have vulkan support.");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(context.instance, &deviceCount, devices.data());

        for (const VkPhysicalDevice& device : devices) {
            if (isPhysicalDeviceGraphicsSuitable(device)) {
                context.physicalDevice = device;
                break;
            }
        }
        CALMAR_ASSERT_MSG(context.physicalDevice != VK_NULL_HANDLE, "Failed to find suitable vulkan GPU.");

        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        if (candidates.rbegin()->first > 0) {
            context.physicalDevice = candidates.rbegin()->second;
        } else {
            CALMAR_ASSERT_MSG(false, "Failed to find suitable vulkan GPU.");
        }

        VkPhysicalDeviceProperties deviceProps;
        vkGetPhysicalDeviceProperties(context.physicalDevice, &deviceProps);

        CALMAR_TRACE("Vulkan: Found suitable Vulkan supporting GPU: '{0}'", deviceProps.deviceName);
    }

    bool vulkan::isPhysicalDeviceGraphicsSuitable(VkPhysicalDevice device) {
        queueFamilyIndices indices = findQueueFamilies(device);

        return indices.isComplete();
    }

    i32 vulkan::rateDeviceSuitability(VkPhysicalDevice device) {
        int score = 0;

        VkPhysicalDeviceProperties deviceProps;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProps);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        score += deviceProps.limits.maxImageDimension2D;

        if (!deviceFeatures.geometryShader) {
            return 0;
        }

        return score;
    }

    vulkan::queueFamilyIndices vulkan::findQueueFamilies(VkPhysicalDevice device) {
        queueFamilyIndices indices;

        u32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, context.surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.isComplete()) {
                break;
            }
            i++;
        }

        return indices;
    }

    void vulkan::createLogicalDevice() {
        queueFamilyIndices indices = findQueueFamilies(context.physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                             indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (u32 queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(context.physicalDevice, &deviceFeatures);

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        ASSERT_MAKE_VULKAN_ERROR(vkCreateDevice(context.physicalDevice, &createInfo, nullptr, &context.logicalDevice), "Failed to create logical device");

        vkGetDeviceQueue(context.logicalDevice, indices.graphicsFamily.value(), 0, &context.graphicsQueue);
        vkGetDeviceQueue(context.logicalDevice, indices.presentFamily.value(), 0, &context.presentQueue);

        CALMAR_TRACE("Vulkan: Created logical device.");
    }

    void vulkan::createSurfaceGLFW() {
        auto window = static_cast<GLFWwindow*>(application::getInstance()->display->getBackendHandle());
        ASSERT_MAKE_VULKAN_ERROR(glfwCreateWindowSurface(context.instance, window, nullptr, &context.surface), "Vulkan: Failed to create GLFW window surface.");
    }

}  // namespace calmar
