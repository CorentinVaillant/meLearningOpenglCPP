#pragma once

#include "GlBuffer.hpp"
#include "VertexLayout.hpp"

class VertexArray {
 private:
  GLuint m_glId;

 public:
  //-- Constructors --
  VertexArray();

  // delete copy
  VertexArray(const VertexArray&) = delete;
  VertexArray& operator=(const VertexArray&) = delete;

  // move
  VertexArray(VertexArray&& other) noexcept;
  VertexArray& operator=(VertexArray&& other) noexcept;

  //-- Destructor --
  ~VertexArray();

  //-- Methods --
  void bind() const;
  void unbind() const;

  template <typename T>
  void addBuffer(const VertexBuffer<T>& vb, const VertexLayout& layout) {
    bind();
    vb.bind();

    const auto& elements = layout.getElements();
    const size_t stride = layout.getStride();
    size_t i = 0;
    size_t offset = 0;
    for (const auto& element : elements) {
      glCall(glVertexAttribPointer(i, element.count, element.glType,
                                   element.normalized, stride, (void*)offset));
      glCall(glEnableVertexAttribArray(i));
      offset += glTypeSize(element.glType) * element.count;
      i++;
    }
  }
};
