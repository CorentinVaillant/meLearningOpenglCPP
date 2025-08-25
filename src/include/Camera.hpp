#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Movable.hpp"
class Camera{
    virtual glm::mat4 getProjectionMat()const = 0;
    virtual glm::mat4 getViewMat()const = 0;
    virtual glm::mat4 getProjectionViewMat()const = 0;
};


class StaticPerspectiveCamera : public Camera
{
protected:
    float m_fov;
    float m_aspect;
    float m_near, m_far;

public:
    // -- Constructors --
    StaticPerspectiveCamera();
    StaticPerspectiveCamera(float fov, float aspect, float near, float far);
    StaticPerspectiveCamera(float fov, GLFWwindow *window, float near, float far);

    // -- Getters --
    float getFov()const;
    float getAspect()const;
    float getNear()const;
    float getFar()const;

    // --Setters --
    void setFov(float rFov);
    void setAspect(float aspect);
    void setNear(float near);
    void setFar(float far);

    // -- Override Camera --
    glm::mat4 getProjectionMat()const override;
    glm::mat4 getViewMat()const override;
    glm::mat4 getProjectionViewMat()const override;

    
    // -- Private methodes --
private:
    static float windowAspect(GLFWwindow *window);
};

class FPSPerspectiveCam : public StaticPerspectiveCamera{
private:
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    
public:
    //-- Constructors --
    FPSPerspectiveCam();
    FPSPerspectiveCam(glm::vec3 position,glm::vec3 target, glm::vec3 worldUp );

    //-- Getters --
    glm::vec3 getPosition();
    glm::vec3 getFront();
    glm::vec3 getUp();

    glm::vec3 getTarget();

    //-- Setters --
    void setPosition(glm::vec3 position);
    void setFront(glm::vec3 front);
    void setUp(glm::vec3 up);

    //-- Methods --
    void translate(glm::vec3 trans);

    // -- Camera override --
    glm::mat4 getViewMat()const override;

};