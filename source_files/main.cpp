#include "../header_files/main.hpp"


using namespace std;


class c_HelloTriangleApplication {
public:
    void run() {
        vulkan.f_InitVulkan();
        f_MainLoop();
        f_Cleanup();
    }

private:

    c_Window window;

    c_VkSetup vulkan;

    void f_MainLoop() {
        while (!glfwWindowShouldClose(window.Window)) {
        glfwPollEvents();
    }

    }

    void f_Cleanup() {
        vkDestroyInstance(vulkan.o_instance, nullptr);
    
        glfwDestroyWindow(window.Window);

        glfwTerminate();

    }
};

int main() {

    c_HelloTriangleApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
