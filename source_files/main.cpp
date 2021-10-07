#include "../header_files/main.hpp"
#define GLFW_INCLUDE_VULKAN

using namespace std;



class c_Window 
{
    public: 

    uint32_t WindowWidth = 800;
    uint32_t WindowHeight = 600;

    GLFWwindow* Window;

    c_Window(){
        if (!glfwInit())
        {
            cerr << "Initializing GLFW failed!\n";
            exit(EXIT_FAILURE);
        }

        Window = glfwCreateWindow(WindowWidth, WindowHeight, "Vulkan", nullptr, nullptr);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        if (!Window)
        {
            glfwTerminate();
            glfwDestroyWindow(Window);
            cerr << "Initializing a GLFW window failed!\n";
            exit(EXIT_FAILURE);
        }
    }   
};

class c_InitVulkan {

};

class c_HelloTriangleApplication {
public:
    void run() {
        f_InitVulkan();
        f_MainLoop();
        f_Cleanup();
    }

private:

    class c_Window window;  

    void f_InitVulkan() {

    }

    void f_MainLoop() {
        while (!glfwWindowShouldClose(window.Window)) {
        glfwPollEvents();
    }

    }

    void f_Cleanup() {
        cout << "Its in cleanup before glfwDestroyWindow\n";
    
        glfwDestroyWindow(window.Window);
        cout << "Its in cleanup after glfwDestroyWindow\n";

        glfwTerminate();
        cout << "Its in cleanup after glfwTerminate\n";

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
