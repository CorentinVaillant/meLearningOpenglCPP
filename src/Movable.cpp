#include "Movable.hpp"

//== Transform Class ==//

// -- Constructors --

Transform::Transform(glm::vec3 position, glm::vec3 size, glm::quat rotation)
    : m_position(position), m_size(size), m_rotation(rotation) {}
Transform::Transform()
    : Transform(glm::vec3(0.0f), glm::vec3(1.0f), glm::quat(1.0f,0.0f,0.0f,0.0f)) {}
Transform::Transform(float position[3], float size[3], float rotation[4])
    : Transform(glm::vec3(position[0],position[1],position[2]), 
    glm::vec3(size[0],size[1],size[2]), 
    glm::quat(rotation[0],rotation[1],rotation[2],rotation[3])
    ) {}

// -- Movable implementation --

void Transform::setPos(glm::vec3 pos) {m_position = pos;}
void Transform::setSize(glm::vec3 size) {m_size = size;}
void Transform::setRotation(glm::quat rotation){m_rotation = rotation;}

glm::vec3 Transform::getPos() const {return m_position;}
glm::vec3 Transform::getSize() const {return m_size;}
glm::quat Transform::getRotation() const {return m_rotation;}

void Transform::translate(glm::vec3 trans) {m_position += trans;}
void Transform::scale(glm::vec3 scale) {m_size *= scale;}
void Transform::rotate(glm::quat rotation) {m_rotation = rotation * m_rotation;}

glm::mat4 Transform::getTransforms() const {
    glm::mat4 transforms = glm::translate(glm::mat4(1.0f), m_position);
    transforms *= glm::mat4_cast(m_rotation);
    return glm::scale(transforms, m_size);
}

glm::mat4 Transform::getTransformsInverse() const {
    // Prevent division by zero
    glm::vec3 safeInvScale(
        m_size.x != 0.0f ? 1.0f / m_size.x : 0.0f,
        m_size.y != 0.0f ? 1.0f / m_size.y : 0.0f,
        m_size.z != 0.0f ? 1.0f / m_size.z : 0.0f
    );

    glm::mat4 inv = glm::scale(glm::mat4(1.0f), safeInvScale);
    inv *= glm::mat4_cast(glm::inverse(m_rotation));
    return glm::translate(inv, -m_position);

}

