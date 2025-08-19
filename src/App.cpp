#include "App.hpp"

#include "constants.hpp"

using std::move;

//== App ==//

bool appInit = false;

App App::s_App;

App& App::getInstance(){
    return App::s_App;
}

//-- Methods --
// -- Init --

App::App():
    m_cubeProgram(Program(cubeVertShaderSrc,cubeFragShaderSrc)),
    m_lightProgram(Program(cubeVertShaderSrc, lightFragShaderSrc))
    {}

void App::init(){
    printf("init app \n");
    if(!appInit)
        App::getInstance()._init();
    appInit = true;
    printf("app init \n");
}

void App::initGlfw(){
    expect_true(glfwInit(),"Failed to initialize GLFW",-1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
void App::initWindow(){
    m_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    expect_ptr(m_window, "Failed to create GLFW window", -1);
    glfwMakeContextCurrent(m_window);
    expect_true(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD", -1);

    glViewport(0, 0, 800, 600);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    glfwSetFramebufferSizeCallback(m_window, App::framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window,mouse_callback);
}
void App::_init(){
    initGlfw();
    initWindow();

    //- Init VAO and VBO (vertex array object, vertex buffer object)

    glGenVertexArrays(1,&m_cubeVAO);
    glGenBuffers(1,&m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_cubeVAO);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &m_lightCubeVAO);
    glBindVertexArray(m_lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // - Loading Texture

    // [...]

    // - Draw parameters
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);  

    m_lastFrame = glfwGetTime();
    m_dt = 0;
}


// -- run --
void App::run(){
    printf("running app \n");
    if(appInit)
        App::getInstance()._run();
    else
        std::cerr << "App has not been init\n";
    printf("app runned \n");    
}
void App::_run(){
    while(!glfwWindowShouldClose(m_window))
    {
        float time = glfwGetTime();
        m_dt = time - m_lastFrame;
        m_lastFrame = time;

        //updates
        processInput(m_window);

        //renders
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 projection = m_camera.getProjectionMat();
        glm::mat4 view = m_camera.getViewMat();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 viewModel = view * model;

        // Cube
        m_cubeProgram.useProgram();
        m_cubeProgram.setUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
        m_cubeProgram.setUniform3f("lightColor",  1.0f, 1.0f, 1.0f);

        m_cubeProgram.setUniformMat4fv("viewModel", glm::value_ptr(viewModel));
        m_cubeProgram.setUniformMat4fv("projection", glm::value_ptr(projection));

        m_cubeProgram.useProgram();

        glBindVertexArray(m_cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Lamp
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
        model = glm::scale(model, glm::vec3(0.2f));

        viewModel = view * model;

        m_lightProgram.clearUniforms();
        m_lightProgram.setUniformMat4fv("viewModel", glm::value_ptr(viewModel));
        m_lightProgram.setUniformMat4fv("projection", glm::value_ptr(projection));
        m_lightProgram.useProgram();

        glBindVertexArray(m_lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //check and call events and swap the buffers
        glfwSwapBuffers(m_window);
        glfwPollEvents();   
        
        //check for errors
        throwOnGlError("error detected after update");
    }

}

// -- free data --
App::~App(){
    glDeleteVertexArrays(1,&m_cubeVAO);
    glDeleteVertexArrays(1,&m_lightCubeVAO);
    glDeleteBuffers(1,&m_VBO);
    glDeleteBuffers(1,&m_EBO);

    m_cubeProgram.deleteShaders();
    m_cubeProgram.deleteProgram();
    m_lightProgram.deleteShaders();
    m_lightProgram.deleteProgram();


    //- Stop App
    glfwTerminate();
    std::cout << "app closed \n";
}

// -- Callbacks --

void App::framebuffer_size_callback(GLFWwindow* , int width, int height){
    glViewport(0, 0, width, height);

    App::getInstance().m_camera.setAspect((float)width/(float)height);
}

//! there is still smt weird going on here
void App::mouse_callback(GLFWwindow*, double xpos, double ypos){
    auto& app = App::getInstance();
    float& lastX = app.m_cursorLastX;
    float& lastY = app.m_cursorLastY;
    float& camYaw = app.m_cameraYaw;
    float& camPitch = app.m_cameraPitch;

    if (app.m_firstMouse) {
        lastX = xpos;
        lastY = ypos;
        app.m_firstMouse = false;
    }


    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.002f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camYaw   += xoffset;
    camPitch += yoffset;

  float limit = glm::half_pi<float>() - 0.1f; // π/2 - 0.1
    if (camPitch > limit)
        camPitch = limit;
    if (camPitch < -limit)
        camPitch = -limit;

    glm::vec3 direction;
    direction.x = cos(camYaw) * cos(camPitch);
    direction.y = sin(camPitch);
    direction.z = sin(camYaw) * cos(camPitch);
    app.m_camera.setFront(glm::normalize(direction));

}


// -- Updates --
void App::processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 10.0f * m_dt;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera.translate(cameraSpeed * m_camera.getFront());
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera.translate(-cameraSpeed * m_camera.getFront());
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera.translate(-glm::normalize(glm::cross(m_camera.getFront(), m_camera.getUp())) * cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera.translate(+glm::normalize(glm::cross(m_camera.getFront(), m_camera.getUp())) * cameraSpeed);
}


// -- Utils --
glm::vec2 App::getResolution(GLFWwindow *window){
    int width, heigth;
    glfwGetWindowSize(window,&width,&heigth);
    return glm::vec2((float)width,(float)heigth);
}