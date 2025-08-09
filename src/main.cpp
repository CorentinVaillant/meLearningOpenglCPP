#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "macros.hpp"
#include "app.hpp"

#include <iostream>

int main() {

    expect_true(glfwInit(),"Failed to initialize GLFW",-1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    expect_ptr(window, "Failed to create GLFW window", -1);

    glfwMakeContextCurrent(window);

    expect_true(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD", -1);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  


    while(!glfwWindowShouldClose(window))
    {
        //updates
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    std::cout << "app closed \n";

    return 0;
}