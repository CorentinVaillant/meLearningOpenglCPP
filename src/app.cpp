#include "app.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "macros.hpp"

// -- Callbacks --

void framebuffer_size_callback(GLFWwindow* , int width, int height){
    glViewport(0, 0, width, height);
}

// -- Updates --
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
