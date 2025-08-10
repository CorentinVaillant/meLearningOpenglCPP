#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Movable.hpp"

class PerspectiveCamera : public Transform
{
private:
    float m_fov;
    float m_aspect;
    float m_near, m_far;

public:
    // -- Constructors --
    PerspectiveCamera(float fov, float aspect, float near, float far);
    PerspectiveCamera(float fov, GLFWwindow *window, float near, float far);

    // -- Getters --
    float getFov()const;
    float getAspect()const;
    float getNear()const;
    float getFar()const;

    glm::mat4 getProjectionMat()const;
    glm::mat4 getViewMat()const;
    glm::mat4 getProjectionViewMat()const;
    
    // -- Private methodes --
private:
    static float windowAspect(GLFWwindow *window);
};