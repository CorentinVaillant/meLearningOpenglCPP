#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "macros.hpp"
#include "app.hpp"
#include "constants.hpp"
#include "Program.hpp"

using std::move;

int main() {

    //- Init glfw
    expect_true(glfwInit(),"Failed to initialize GLFW",-1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //- Open the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    expect_ptr(window, "Failed to create GLFW window", -1);

    glfwMakeContextCurrent(window);
    {

    expect_true(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD", -1);

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //- Init VAO (vertex array object)
    GLuint VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    //- Init VBO (vertex buffer object)
    GLuint VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // - Init EBO (element buffer object)
    GLuint EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //- Init Program and Shader
    Program program = Program(vertexShaderSource, fragmentShaderSource);
    
    // - Draw parameters
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    
    //- App Loop
    while(!glfwWindowShouldClose(window))
    {
        //updates
        processInput(window);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        program.useProgram();
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        
        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    }
    //- Stop App
    glfwTerminate();
    std::cout << "app closed \n";

    return 0;
}