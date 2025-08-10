#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>


// -- Callbacks --
extern void framebuffer_size_callback(GLFWwindow* , int width, int height);  

// -- Updates --
extern void processInput(GLFWwindow *window);

// -- Utils --
extern glm::vec2 getResolution(GLFWwindow *window);