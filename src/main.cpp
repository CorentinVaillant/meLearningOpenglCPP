#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwTerminate();
    std::cout << "everythings fine ! \n";
    return 0;
}