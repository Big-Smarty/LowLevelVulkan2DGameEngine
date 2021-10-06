#include "../header_files/main.hpp"

using namespace std;

void ErrorCallback(int error, const char* Description)
{
    cerr << "Initialization of glfw failed! Error code: ";
    cerr << "Error: %s\n", Description;
    glfwTerminate();
}

int main()
{
    uint32_t WindowWidth = 800;
    uint32_t WindowHeight = 640;
    const char* WindowTitle = "pls work";

    if (!glfwInit)
    {
        //initialization of glfw failed
        glfwSetErrorCallback(ErrorCallback);
        return -1; 
    };

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

    if (Window == nullptr)
    {
        cerr << "Error: failed to create GLFW window!\n";
        glfwDestroyWindow(Window);
        return -1;
    };

    
    glfwDestroyWindow(Window);
    return 0;
}