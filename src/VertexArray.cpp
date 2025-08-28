#include "VertexArray.hpp"

#include <glad/glad.h>

//-- Constructors --
VertexArray::VertexArray() { glCall(glGenVertexArrays(1, &m_glId)); }

VertexArray::VertexArray(VertexArray&& other) noexcept : m_glId(other.m_glId) {
  other.m_glId = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
  if (this != &other) {
    if (m_glId != 0) {
      glDeleteVertexArrays(1, &m_glId);
    }

    m_glId = other.m_glId;
    other.m_glId = 0;
  }
  return *this;
}

//-- Destructor --
VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_glId);
  m_glId = 0;
}

//-- Methods --

void VertexArray::bind() const { glCall(glBindVertexArray(m_glId)); }

void VertexArray::unbind() const { glCall(glBindVertexArray(0)); }