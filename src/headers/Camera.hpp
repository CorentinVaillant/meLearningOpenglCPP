#pragma once

#include <GLFW/glfw3.h>
#include <vec3.hpp>
#include <quaternion.hpp>

class Camera
{
    virtual void setPos(glm::vec3 pos);
    virtual void setSize(glm::vec3 size);
    virtual void setRotation(glm::quat rotation);

    virtual void translate(glm::vec3 pos);
    virtual void scale(glm::vec3 scale);
    virtual void rotate(glm::quat rotation);
};


class PerspectiveCamera
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

    // -- Private methodes --
private:
    static float windowAspect(GLFWwindow *window);
};