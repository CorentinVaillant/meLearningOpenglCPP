#include "App.hpp"

#include "constants.hpp"

using std::move;

//== App ==//

bool appInit = false;

App App::s_App;

App& App::getInsteance(){
    return App::s_App;
}

//-- Methods --
// -- Init --

App::App():
    m_texture1("../resources/container.jpg",GL_RGB,GL_RGB),
    m_texture2("../resources/awesomeface.png",GL_RGBA,GL_RGBA),
    m_program(Program(triangleVertShaderSrc,triangleFragShaderSrc))
    {}

void App::init(){
    printf("init app \n");
    if(!appInit)
        App::getInsteance()._init();
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
    glfwSetFramebufferSizeCallback(m_window, App::framebuffer_size_callback);
}
void App::_init(){
    initGlfw();
    initWindow();

    //- Init VAO (vertex array object)
    glGenVertexArrays(1,&m_VAO);
    glBindVertexArray(m_VAO);

    //- Init VBO (vertex buffer object)

    glGenBuffers(1,&m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // - Init EBO (element buffer object)
    // GLuint EBO;
    // glGenBuffers(1,&EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // - Loading Texture


    // - Draw parameters
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);  

    m_camera = PerspectiveCamera(M_PI_4,m_window, 0.1f,100.0f);
    m_camera.setPos(glm::vec3(0.,0.,3.));
    m_lastFrame = glfwGetTime();
    m_dt = 0;
}


// -- run --
void App::run(){
    printf("running app \n");
    if(appInit)
        App::getInsteance()._run();
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

        glm::qua camRotation(1.0f,0.0f,0.0f,0.0f);
        camRotation = glm::rotate(camRotation,M_PI_4f * (float)m_dt, glm::vec3(0.0f,0.0f,1.0f)); 
        m_camera.translate(glm::vec3(sin(time) * m_dt,0.,0.));
        m_camera.rotate(camRotation);
        glm::mat4 projection = m_camera.getProjectionMat();
        glm::mat4 view = m_camera.getViewMat();
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, m_texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, m_texture2);
        glBindVertexArray(m_VAO);
        
        for(int i(0); i< CUBE_POSITION_NUMBER; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float rotation = sin((float)(i + 1)) * time; 
            model = glm::rotate(model, rotation, glm::vec3(1.0f, 0.3f, 0.5f));
            
            glm::mat4 viewModel= view * model;

            m_program.clearUniforms();
            m_program.setUniformTexture2D("texture1",m_texture1);
            m_program.setUniformTexture2D("texture2",m_texture2);
            m_program.setUniformMat4fv("projection",glm::value_ptr(projection));
            m_program.setUniformMat4fv("viewModel",glm::value_ptr(viewModel));
            
            m_program.useProgram();
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        //check and call events and swap the buffers
        glfwSwapBuffers(m_window);
        glfwPollEvents();   
        
        //check for errors
        throwOnGlError("error detected after update");
    }

}

// -- free data --
App::~App(){
    glDeleteVertexArrays(1,&m_VAO);
    glDeleteBuffers(1,&m_VBO);

    m_texture1.deleteGlTexture();
    m_texture2.deleteGlTexture();

    m_program.deleteShaders();
    m_program.deleteProgram();

    //- Stop App
    glfwTerminate();
    std::cout << "app closed \n";
}

// -- Callbacks --

void App::framebuffer_size_callback(GLFWwindow* , int width, int height){
    glViewport(0, 0, width, height);

    App::getInsteance().m_camera.setAspect((float)width/(float)height);
}

// -- Updates --
void App::processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// -- Utils --
glm::vec2 App::getResolution(GLFWwindow *window){
    int width, heigth;
    glfwGetWindowSize(window,&width,&heigth);
    return glm::vec2((float)width,(float)heigth);
}