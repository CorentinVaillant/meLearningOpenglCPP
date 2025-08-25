#pragma once

#include <glad/glad.h>
#include <cstddef>


template <typename T, const GLenum BufferType>
class GlBuffer
{
protected:
    GLuint m_glId;

public:
    //-- Constructors --
    GlBuffer();
    GlBuffer(const T* data,size_t size,GLenum usage);
    GlBuffer(const T* data,size_t size);

    GlBuffer(GlBuffer&& rvalue);
    GlBuffer& operator=(GlBuffer&& other) noexcept;

    GlBuffer(GlBuffer&) = delete;
    GlBuffer& operator=(const GlBuffer&) = delete;


    //-- Destructor --
    ~GlBuffer();

    //-- Methods --
    void uploadData(const T* data,size_t count, GLenum usage);

    void bind() const;
    void unbind() const;

    inline GLuint getGlId() const{return m_glId;}
};

// -- Alias --

template <typename I>
using IndexBuffer = GlBuffer<I,GL_ELEMENT_ARRAY_BUFFER>;

template <typename V>
using VertexBuffer = GlBuffer<V,GL_ARRAY_BUFFER>;
