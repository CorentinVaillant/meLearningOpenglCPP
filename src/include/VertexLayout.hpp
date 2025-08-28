#pragma once

#include <glad/glad.h>

#include <vector>

#include "gl_utils.hpp"

struct VertexElement {
  GLenum glType;
  size_t count;
  bool normalized;
};

class VertexLayout {
 private:
  std::vector<VertexElement> m_elements;
  size_t m_stride;

 public:
  //-- Constructors --
  VertexLayout() : m_stride(0) {};

  // -- Getters --
  const std::vector<VertexElement>& getElements() const { return m_elements; }
  size_t getStride() const { return m_stride; }

  // -- Methodes --
  template <typename T>
  void push(size_t count, bool normalized) {
    throw std::runtime_error("push is not implemented for this type");
  };

  template <typename T>
  VertexLayout& push(size_t count) {
    push<T>(count, GL_FALSE);
    return *this;
  }
};

// Push impl

template <>
inline void VertexLayout::push<GLfloat>(size_t count, bool normalized) {
  m_elements.push_back({GL_FLOAT, count, normalized});
  m_stride += count * glTypeSize(GL_FLOAT);
}

template <>
inline void VertexLayout::push<GLdouble>(size_t count, bool normalized) {
  m_elements.push_back({GL_DOUBLE, count, normalized});
  m_stride += count * glTypeSize(GL_DOUBLE);
}

template <>
inline void VertexLayout::push<GLint>(size_t count, bool normalized) {
  m_elements.push_back({GL_INT, count, normalized});
  m_stride += count * glTypeSize(GL_INT);
}

template <>
inline void VertexLayout::push<GLuint>(size_t count, bool normalized) {
  m_elements.push_back({GL_UNSIGNED_INT, count, normalized});
  m_stride += count * glTypeSize(GL_UNSIGNED_INT);
}

template <>
inline void VertexLayout::push<GLbyte>(size_t count, bool normalized) {
  m_elements.push_back({GL_BYTE, count, normalized});
  m_stride += count * glTypeSize(GL_BYTE);
}

template <>
inline void VertexLayout::push<GLubyte>(size_t count, bool normalized) {
  m_elements.push_back({GL_UNSIGNED_BYTE, count, normalized});
  m_stride += count * glTypeSize(GL_UNSIGNED_BYTE);
}
