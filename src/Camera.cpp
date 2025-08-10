#include "Camera.hpp"

// -- Constructors --
PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far){}
PerspectiveCamera::PerspectiveCamera(float fov, GLFWwindow *window, float near, float far)
    : m_fov(fov), m_aspect(windowAspect(window)), m_near(near), m_far(far){}



// -- Getters --
float PerspectiveCamera::getFov()const{return m_fov;}
float PerspectiveCamera::getAspect()const{return m_aspect;}
float PerspectiveCamera::getNear()const{return m_near;}
float PerspectiveCamera::getFar()const{return m_far;}

// -- Private methods --
float PerspectiveCamera::windowAspect(GLFWwindow *window){
    int width, heigth;
    glfwGetWindowSize(window,&width,&heigth);
    return ((float)width) / ((float) heigth);
}