#include "vulkan_renderer.hpp"
#include "calmar/core/asserting.hpp"
#include "calmar/core/application.hpp"
#include "calmar/core/util.hpp"
#include "calmar/platform/glfw/window_glfw.hpp"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_win32.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <map>
#include <set>
#include <algorithm>

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
    bool vulkan::enableValidationLayers;
    vulkanContext vulkan::context;
    const std::vector<const char*> vulkan::validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    const std::vector<const char*> vulkan::deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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
        createSwapChain();
        createImageViews();
        createGraphicsPipeline();
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
        for (auto imageView : context.swapChainImageViews) {
            vkDestroyImageView(context.logicalDevice, imageView, nullptr);
        }
        vkDestroySwapchainKHR(context.logicalDevice, context.swapChain, nullptr);
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

        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainUsable = false;
        if (extensionsSupported) {
            swapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainUsable = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainUsable;
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

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

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

    bool vulkan::checkDeviceExtensionSupport(VkPhysicalDevice device) {
        u32 extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    vulkan::swapChainSupportDetails vulkan::querySwapChainSupport(VkPhysicalDevice device) {
        swapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, context.surface, &details.capabilities);

        u32 formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, context.surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, context.surface, &formatCount, details.formats.data());
        }

        u32 presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, context.surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, context.surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR vulkan::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR vulkan::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }
    VkExtent2D vulkan::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != 0xffffffff) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            auto window = static_cast<GLFWwindow*>(application::getInstance()->display->getBackendHandle());
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<u32>(width),
                static_cast<u32>(height)};

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    void vulkan::createSwapChain() {
        swapChainSupportDetails swapChainSupport = querySwapChainSupport(context.physicalDevice);

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = context.surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        queueFamilyIndices indices = findQueueFamilies(context.physicalDevice);
        u32 queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;      // Optional
            createInfo.pQueueFamilyIndices = nullptr;  // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        ASSERT_MAKE_VULKAN_ERROR(vkCreateSwapchainKHR(context.logicalDevice, &createInfo, nullptr, &context.swapChain), "Failed to create swap chain.");

        vkGetSwapchainImagesKHR(context.logicalDevice, context.swapChain, &imageCount, nullptr);
        context.swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(context.logicalDevice, context.swapChain, &imageCount, context.swapChainImages.data());

        context.swapChainImageFormat = surfaceFormat.format;
        context.swapChainExtent = extent;

        CALMAR_TRACE("Vulkan: Sucessfully created swap chain.");
    }

    void vulkan::createImageViews() {
        context.swapChainImageViews.resize(context.swapChainImages.size());
        for (u64 i = 0; i < context.swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = context.swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = context.swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            ASSERT_MAKE_VULKAN_ERROR(vkCreateImageView(context.logicalDevice, &createInfo, nullptr, &context.swapChainImageViews[i]), "Failed to create image view.");
        }
    }

    void vulkan::createGraphicsPipeline() {
        auto vertShaderCode = util::getFileContentsBinary("../engine/assets/shaders/vulkan/bin/default2d_vert.spv");
        auto fragShaderCode = util::getFileContentsBinary("../engine/assets/shaders/vulkan/bin/default2d_frag.spv");

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        CALMAR_INFO("Vulkan: Created vertex shader module successfully.");

        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
        CALMAR_INFO("Vulkan: Created fragment shader module successfully.");

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        // VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pushConstantRangeCount = 0;

        ASSERT_MAKE_VULKAN_ERROR(vkCreatePipelineLayout(context.logicalDevice, &pipelineLayoutInfo, nullptr, &context.pipelineLayout), "Failed to create graphics pipeline.");

        vkDestroyShaderModule(context.logicalDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(context.logicalDevice, vertShaderModule, nullptr);

        CALMAR_TRACE("Vulkan: Created graphics pipeline.");
    }

    VkShaderModule vulkan::createShaderModule(const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const u32*>(code.data());

        VkShaderModule shaderModule;
        ASSERT_MAKE_VULKAN_ERROR(vkCreateShaderModule(context.logicalDevice, &createInfo, nullptr, &shaderModule), "Failed to create shader module.");

        return shaderModule;
    }
}  // namespace calmar
