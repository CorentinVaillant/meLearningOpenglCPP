#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

// -- Callbacks --
extern void framebuffer_size_callback(GLFWwindow* , int width, int height);  

// -- Updates --
extern void processInput(GLFWwindow *window);
