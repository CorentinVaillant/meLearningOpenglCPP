#include "Camera.hpp"

#include "gl_utils.hpp"

//== MARK: StaticPerspectiveCamera Class ==//


// -- Constructors --
StaticPerspectiveCamera::StaticPerspectiveCamera(float fov, float aspect, float near, float far)
    : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
    {Transform();}
StaticPerspectiveCamera::StaticPerspectiveCamera(float fov, GLFWwindow *window, float near, float far)
    : StaticPerspectiveCamera(fov,windowAspect(window), near,far)
    {}
StaticPerspectiveCamera::StaticPerspectiveCamera()
    : m_fov(M_PI_4), m_aspect(800.0f / 600.0f), m_near(0.1f), m_far(100.0f)
    {}

// -- Getters --
float StaticPerspectiveCamera::getFov()const{return m_fov;}
float StaticPerspectiveCamera::getAspect()const{return m_aspect;}
float StaticPerspectiveCamera::getNear()const{return m_near;}
float StaticPerspectiveCamera::getFar()const{return m_far;}

glm::mat4 StaticPerspectiveCamera::getProjectionMat()const{return glm::perspective(m_fov,m_aspect,m_near,m_far);}
glm::mat4 StaticPerspectiveCamera::getViewMat()const{return glm::mat4(1.0f);}
glm::mat4 StaticPerspectiveCamera::getProjectionViewMat()const{return getProjectionMat() * getViewMat();}

// --Setters --
void StaticPerspectiveCamera::setFov(float rFov){m_fov = rFov;}
void StaticPerspectiveCamera::setAspect(float aspect){m_aspect = aspect;}
void StaticPerspectiveCamera::setNear(float near){m_near = near;}
void StaticPerspectiveCamera::setFar(float far){m_far = far;}


// -- Public methods --


// -- Private methods --
float StaticPerspectiveCamera::windowAspect(GLFWwindow *window){
    int width, height;
    glfwGetWindowSize(window,&width,&height);
    return ((float)width) / ((float) height);
}

//== MARK: FPSPerspectiveCamera Class ==//

    //-- Constructors --
    FPSPerspectiveCam::FPSPerspectiveCam():
        m_position(glm::vec3(0.0f, 0.0f, 3.0f)),
        m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
        m_up(glm::vec3(0.0f, 1.0f, 0.0f))
    {
        m_fov = M_PI_4;
        m_aspect = (4.0f/3.0f);
        m_near = 0.1f;
        m_far = 100.0f;

    }

    FPSPerspectiveCam::FPSPerspectiveCam(glm::vec3 position,glm::vec3 target, glm::vec3 worldUp ):
    m_position(position), m_front(target), m_up(worldUp)
    {}

    
    //-- Getters --
    glm::vec3 FPSPerspectiveCam::getPosition(){return m_position;}
    glm::vec3 FPSPerspectiveCam::getFront(){return m_front;}
    glm::vec3 FPSPerspectiveCam::getUp(){return m_up;}

    glm::vec3 FPSPerspectiveCam::getTarget(){return m_position + m_front;}

    //-- Setters --
    void FPSPerspectiveCam::setPosition(glm::vec3 position){m_position = position;}
    void FPSPerspectiveCam::setFront(glm::vec3 targetPosition){m_front = targetPosition;}
    void FPSPerspectiveCam::setUp(glm::vec3 up){m_up = up;}

    //-- Methods --
    void FPSPerspectiveCam::translate(glm::vec3 trans){m_position += trans;}

    // -- Camera override --
    glm::mat4 FPSPerspectiveCam::getViewMat()const{
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

