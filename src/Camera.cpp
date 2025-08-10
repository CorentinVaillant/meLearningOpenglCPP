#include "Camera.hpp"

// -- Constructors --
PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
    {Transform();}
PerspectiveCamera::PerspectiveCamera(float fov, GLFWwindow *window, float near, float far)
    : PerspectiveCamera(fov,windowAspect(window), near,far)
    {}
PerspectiveCamera::PerspectiveCamera()
    : m_fov(M_PI_4), m_aspect(800.0f / 600.0f), m_near(0.1f), m_far(100.0f)
    {}



// -- Getters --
float PerspectiveCamera::getFov()const{return m_fov;}
float PerspectiveCamera::getAspect()const{return m_aspect;}
float PerspectiveCamera::getNear()const{return m_near;}
float PerspectiveCamera::getFar()const{return m_far;}

glm::mat4 PerspectiveCamera::getProjectionMat()const{return glm::perspective(m_fov,m_aspect,m_near,m_far);}
glm::mat4 PerspectiveCamera::getViewMat()const{return getTransformsInverse();}
glm::mat4 PerspectiveCamera::getProjectionViewMat()const{return getProjectionMat() * getViewMat();}

// --Setters --
void PerspectiveCamera::setFov(float rFov){m_fov = rFov;}
void PerspectiveCamera::setAspect(float aspect){m_aspect = aspect;}
void PerspectiveCamera::setNear(float near){m_near = near;}
void PerspectiveCamera::setFar(float far){m_far = far;}


// -- Private methods --
float PerspectiveCamera::windowAspect(GLFWwindow *window){
    int width, height;
    glfwGetWindowSize(window,&width,&height);
    return ((float)width) / ((float) height);
}