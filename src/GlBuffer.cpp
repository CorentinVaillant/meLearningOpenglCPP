#include "GlBuffer.hpp"

#include "gl_utils.hpp"

//-- Constructors --
template <typename T,const GLenum BufferType>
GlBuffer<T,BufferType>::GlBuffer(){
    glCall(glGenBuffers(1,&m_glId));
}
template <typename T,const GLenum BufferType>
GlBuffer<T,BufferType>::GlBuffer(const T* data,size_t size,GLenum usage): GlBuffer(){
    uploadData(data,size,usage);
}
template <typename T,const GLenum BufferType>
GlBuffer<T,BufferType>::GlBuffer(const T* data,size_t size):
    GlBuffer(data,size,GL_STATIC_DRAW)
{}
template <typename T,const GLenum BufferType>
GlBuffer<T,BufferType>::GlBuffer(GlBuffer&& rvalue):
    m_glId(rvalue.m_glId)
{
    rvalue.m_glId = 0;
}

template <typename T,const GLenum BufferType>
GlBuffer<T,BufferType>& GlBuffer<T,BufferType>::operator=(GlBuffer<T,BufferType>&& other) noexcept{
    if (this != &other) {
        glDeleteBuffers(1, &m_glId);
        m_glId = other.m_glId;
        other.m_glId = 0;
    }
    return *this;
}

// -- Destructor --
template <typename T,const GLenum BufferType>
GlBuffer<T,BufferType>::~GlBuffer(){
    glDeleteBuffers(1,&m_glId);
}

//-- Methods --
template <typename T,const GLenum BufferType>
void GlBuffer<T,BufferType>::uploadData(const T* data,size_t count, GLenum usage){
    bind();
    glCall(glBufferData(BufferType,count * sizeof(T),data, usage));
}
template <typename T,const GLenum BufferType>
void GlBuffer<T,BufferType>::bind() const{
    glCall(glBindBuffer(BufferType,m_glId));
}
template <typename T,const GLenum BufferType>
void GlBuffer<T,BufferType>::unbind() const{
    glCall(glBindBuffer(BufferType,0));
}


// -- Alias --
template class GlBuffer<float, GL_ARRAY_BUFFER>;
template class GlBuffer<unsigned int, GL_ELEMENT_ARRAY_BUFFER>;
