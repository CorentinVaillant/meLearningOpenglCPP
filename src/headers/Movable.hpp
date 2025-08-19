#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Movable
{
public :
    virtual ~Movable() = default;

    virtual void setPos(glm::vec3) = 0;
    virtual void setSize(glm::vec3) = 0;
    virtual void setRotation(glm::quat) = 0;

    virtual glm::vec3 getPos() const = 0;
    virtual glm::vec3 getSize() const = 0;
    virtual glm::quat getRotation() const = 0;
    
    virtual void translate(glm::vec3) = 0;
    virtual void scale(glm::vec3) = 0;
    virtual void rotate(glm::quat) = 0;

    virtual glm::mat4 getTransforms() const = 0;
    virtual glm::mat4 getTransformsInverse() const = 0;
};

class Transform : public Movable
{
protected :
    glm::vec3 m_position, m_size;
    glm::quat m_rotation;

public :
    // -- Constructors --
    Transform();
    Transform(glm::vec3 position, glm::vec3 size, glm::quat rotation);
    Transform(float position[3], float size[3], float rotation[4]);


    void setPos(glm::vec3 pos) override;
    void setSize(glm::vec3 size) override;
    void setRotation(glm::quat rotation) override;

    glm::vec3 getPos() const override;
    glm::vec3 getSize() const override;
    glm::quat getRotation() const override;
    
    void translate(glm::vec3 trans) override;
    void scale(glm::vec3 scale) override;
    void rotate(glm::quat rotation) override;

    glm::mat4 getTransforms() const override;
    glm::mat4 getTransformsInverse() const override;

};