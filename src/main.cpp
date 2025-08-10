#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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

    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // - Init EBO (element buffer object)
    GLuint EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //- Init Program and Shader
    Program program = Program(triangleVertShaderSrc,triangleFragShaderSrc);
    
    // - Draw parameters
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // - Loading Texture
    stbi_set_flip_vertically_on_load(true);

    int width_text1, height_text1, nrChannels_text1;
    unsigned char* data_text1 = stbi_load("../resources/container.jpg", &width_text1, &height_text1, &nrChannels_text1, 0);
    expect_ptr(data_text1,"could not load the image", -1);

    GLuint texture1;
    glGenTextures(1, &texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_text1, height_text1, 0, GL_RGB, GL_UNSIGNED_BYTE, data_text1);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data_text1);

    int width_text2, height_text2, nrChannels_text2;
    unsigned char* data_text2 = stbi_load("../resources/awesomeface.png", &width_text2, &height_text2, &nrChannels_text2, 0);
    expect_ptr(data_text2,"could not load the image", -1);

    GLuint texture2;
    glGenTextures(1, &texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_text2, height_text2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_text2);
    glGenerateMipmap(GL_TEXTURE_2D);



    stbi_image_free(data_text2);
    
    //- App Loop
    while(!glfwWindowShouldClose(window))
    {
        //updates
        processInput(window);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        int screenWidth, screenHeigth;
        glfwGetWindowSize(window,&screenWidth, &screenHeigth);
        float screenSize[2] = {(float) screenWidth, (float) screenHeigth};
        
        program.useProgram();
        program.setUniform2f("resolution", screenSize);
        program.setUniform1i("texture1",0);
        program.setUniform1i("texture2",1);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
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