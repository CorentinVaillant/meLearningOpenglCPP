#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

#include "Program.hpp"
#include "macros.hpp"
#include "Camera.hpp"
#include "gl_utils.hpp"


class App
{
private:
    static App s_App;

    GLFWwindow * m_window;

    GLuint m_VAO,m_VBO; 

    Texture m_texture1,m_texture2;
    Program m_program;
    PerspectiveCamera m_camera;

    double m_lastFrame;
    double m_dt;
    
public:
    // -- Making the class a singleton --
    static App& getInsteance();
    static void init();
    static void run();
    
    // -- deleting methods -- 
    App(const App&) = delete; 
    
    ~App();
private:
    App();
    void initGlfw();
    void initWindow();
    void _init();

    void _run();

    
    // -- Callbacks --
    static void framebuffer_size_callback(GLFWwindow* , int width, int height);  
    
    // -- Updates --
    void processInput(GLFWwindow *window);
    
    // -- Utils --
    glm::vec2 getResolution(GLFWwindow *window);
    
};