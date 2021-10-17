//
// Created by bigsmarty on 10/9/21.
//

#ifndef LOWLEVELVULKAN2DGAME_VKENGINE_H
#define LOWLEVELVULKAN2DGAME_VKENGINE_H

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool b_enableValidationLayers = true;
#endif

#endif //LOWLEVELVULKAN2DGAME_VKENGINE_H
#include "main.h"

//TEMPLATES

template<class BsVk>
std::string_view as_string(const VkPhysicalDeviceType gpu_type) {
    switch (gpu_type) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            return "VK_PHYSICAL_DEVICE_TYPE_OTHER";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return "VK_PHYSICAL_DEVICE_TYPE_CPU";
        default:
            break;
    }
    return "Unknown";
}

//TEMPLATES END

#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                           \
			std::cout <<"Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)

class c_Window
{
public:

    uint32_t window_width = 800;
    uint32_t window_height = 600;

    GLFWwindow* Window;

    c_Window(){
        if (glfwVulkanSupported())
        {
            std::cerr << "Vulkan isn't supported!\n";
            exit(EXIT_FAILURE);
        }

        if (!glfwInit())
        {
            std::cerr << "Initializing GLFW failed!\n";
            exit(EXIT_FAILURE);
        }

        Window = glfwCreateWindow(window_width, window_height, "Vulkan", nullptr, nullptr);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        if (!Window)
        {
            glfwTerminate();
            glfwDestroyWindow(Window);
            std::cerr << "Initializing a GLFW window failed!\n";
            exit(EXIT_FAILURE);
        }
    }
};

class c_VkSetup{

public:

    //OBJECTS

    VkInstance instance;

    //OBJECTS END

    //VOIDS

    void f_initVulkan(){
        f_createInstance();
        f_enumerateExtensions();
        f_setupPhysicalDevice();
        f_createLogicalDevice();
    }

    void f_cleanupVkSetup()
    {
        f_cleanup();
    }

    //VOIDS END

private:
    //GLOBAL

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device;

    //GLOBAL END

    //STRUCTS

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool b_isComplete()
        {
            return graphicsFamily.has_value();
        }

    };

    QueueFamilyIndices f_findQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            if (indices.b_isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    //STRUCTS END

    //VECTORS
    std::vector<const char*> getRequiredExtensions(){
        uint32_t r_glfwExtensionCount= 0;
        const char** r_glfwExtensions;
        r_glfwExtensions = glfwGetRequiredInstanceExtensions(&r_glfwExtensionCount);

        std::vector<const char*> Extensions(r_glfwExtensions, r_glfwExtensions + r_glfwExtensionCount);

        if(b_enableValidationLayers) {
            Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return Extensions;
    }

    const std::vector<const char*> v_validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };
    //END VECTORS

    //BOOLEANS
    bool b_checkValidationLayers()
    {

        uint32_t LayerCount;
        vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

        std::vector<VkLayerProperties> v_AvailableLayers(LayerCount);
        vkEnumerateInstanceLayerProperties(&LayerCount, v_AvailableLayers.data());

        for (const char* layerName : v_validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : v_AvailableLayers) {
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

    bool b_isDeviceSuitable(VkPhysicalDevice device)
    {

        QueueFamilyIndices indices = f_findQueueFamilies(device);

        return indices.b_isComplete();

    }

    //END BOOLEANS

    //VOIDS
    /*
    void f_SetupDebugMessenger()
    {
        if (!b_EnableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = s_DebugCallback;
        createInfo.pUserData = nullptr; // Optional

    }*/

    void f_createInstance()
    {
        if (!b_checkValidationLayers()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo AppInfo{};
        AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        AppInfo.pApplicationName = "LowLevelVulkan2DGameEngine";
        AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        AppInfo.pEngineName = "LowLevelVulkan2DGameEngine";
        AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        AppInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &AppInfo;
        if (b_enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(v_validationLayers.size());
            createInfo.ppEnabledLayerNames = v_validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        VkResult r_result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

        auto r_Extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(r_Extensions.size());
        createInfo.ppEnabledExtensionNames = r_Extensions.data();
    }

    void f_enumerateExtensions()
    {
        uint32_t r_ExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &r_ExtensionCount, nullptr);

        std::vector<VkExtensionProperties> v_Extensions(r_ExtensionCount);

        vkEnumerateInstanceExtensionProperties(nullptr, &r_ExtensionCount, v_Extensions.data());

        std::cout << "available extensions:\n";

        for (const auto& extension : v_Extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }
    }

    void f_setupPhysicalDevice()
    {

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("Failed to find a GPU with Vulkan support!\n");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices) {
            int score = f_rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }

        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first > 0) {
            std::cout << "IT WORKS!!!\n";
            if (b_isDeviceSuitable(candidates.rbegin()->second))
            {
                physicalDevice = candidates.rbegin()->second;
                VkPhysicalDeviceProperties PrintChosenGPUProperties;
                vkGetPhysicalDeviceProperties(physicalDevice, &PrintChosenGPUProperties);
                std::cout << as_string<const VkPhysicalDeviceType>(PrintChosenGPUProperties.deviceType) << std::endl;
            }

        }

        else {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("Failed to find a suitable GPU!\n");
        }

        std::optional<uint32_t> graphicsFamily;

        graphicsFamily = 0;

        std::cout << std::boolalpha << graphicsFamily.has_value() << std::endl; // true

    }

    void f_createLogicalDevice()
    {

        QueueFamilyIndices indices = f_findQueueFamilies(physicalDevice);

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;

        if (b_enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(v_validationLayers.size());
            createInfo.ppEnabledLayerNames = v_validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        VkQueue graphicsQueue;

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);

    }

    void f_cleanup()
    {
        vkDestroyDevice(device, nullptr);
    }

    //VOIDS END

    //INTEGERS

    int f_rateDeviceSuitability(VkPhysicalDevice device)
    {

        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        int score = 0;

        // Discrete GPUs have a significant performance advantage
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += deviceProperties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!deviceFeatures.geometryShader) {
            std::cout << "GPU does not support geometry shaders!\n";
            return 0;
        }

        std::cout << score << std::endl;
        return score;
    }

    //INTEGERS END

    //STATIC
    /*static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {

        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        }

        return VK_FALSE;
    }*/


    //STATIC END

};


