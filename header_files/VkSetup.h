//
// Created by bigsmarty on 10/9/21.
//

#ifndef LOWLEVELVULKAN2DGAME_VKENGINE_H
#define LOWLEVELVULKAN2DGAME_VKENGINE_H

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool b_EnableValidationLayers = true;
#endif

#endif //LOWLEVELVULKAN2DGAME_VKENGINE_H
#include "main.hpp"

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

    VkInstance o_instance;

    void f_InitVulkan(){
        f_CreateInstance();
        f_SetupDebugMessenger();
        f_EnumerateExtensions();
    }

private:
    //OBJECTS

    VkDebugUtilsMessengerEXT debugMessenger;

    //OBJECTS END



    //STRUCTS

    //STRUCTS END

    //VECTORS
    std::vector<const char*> v_GetRequiredExtensions(){
        uint32_t r_glfwExtensionCount= 0;
        const char** r_glfwExtensions;
        r_glfwExtensions = glfwGetRequiredInstanceExtensions(&r_glfwExtensionCount);

        std::vector<const char*> v_Extensions(r_glfwExtensions, r_glfwExtensions + r_glfwExtensionCount);

        if(b_EnableValidationLayers) {
            v_Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return v_Extensions;
    }

    const std::vector<const char*> v_ValidationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };
    //END VECTORS

    //BOOLEANS
    bool b_CheckValidationLayers()
    {

        uint32_t r_LayerCount;
        vkEnumerateInstanceLayerProperties(&r_LayerCount, nullptr);

        std::vector<VkLayerProperties> v_AvailableLayers(r_LayerCount);
        vkEnumerateInstanceLayerProperties(&r_LayerCount, v_AvailableLayers.data());

        for (const char* layerName : v_ValidationLayers) {
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
    //END BOOLEANS

    //VOIDS
    void f_SetupDebugMessenger()
    {
        if (!b_EnableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = s_DebugCallback;
        createInfo.pUserData = nullptr; // Optional
    }

    void f_CreateInstance()
    {
        if (b_EnableValidationLayers && !b_CheckValidationLayers()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo s_AppInfo{};
        s_AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        s_AppInfo.pApplicationName = "LowLevelVulkan2DGameEngine";
        s_AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        s_AppInfo.pEngineName = "LowLevelVulkan2DGameEngine";
        s_AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        s_AppInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo s_CreateInfo{};
        s_CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        s_CreateInfo.pApplicationInfo = &s_AppInfo;
        if (b_EnableValidationLayers) {
            s_CreateInfo.enabledLayerCount = static_cast<uint32_t>(v_ValidationLayers.size());
            s_CreateInfo.ppEnabledLayerNames = v_ValidationLayers.data();
        } else {
            s_CreateInfo.enabledLayerCount = 0;
        }

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        s_CreateInfo.enabledExtensionCount = glfwExtensionCount;
        s_CreateInfo.ppEnabledExtensionNames = glfwExtensions;

        s_CreateInfo.enabledLayerCount = 0;

        VkResult r_result = vkCreateInstance(&s_CreateInfo, nullptr, &o_instance);

        if (vkCreateInstance(&s_CreateInfo, nullptr, &o_instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

        auto r_Extensions = v_GetRequiredExtensions();
        s_CreateInfo.enabledExtensionCount = static_cast<uint32_t>(r_Extensions.size());
        s_CreateInfo.ppEnabledExtensionNames = r_Extensions.data();
    }

    void f_EnumerateExtensions()
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
    //END VOIDS

    //STATIC
    static VKAPI_ATTR VkBool32 VKAPI_CALL s_DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData) {

        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        }

        return VK_FALSE;
    }
    //STATIC END

};
