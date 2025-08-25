#pragma  once

#include <glad/glad.h>
#include <vector>

#include "gl_utils.hpp"

class Texture
{
private:
    GLuint  m_glId;
    GLint   m_internalformat;
    GLsizei m_width;
    GLsizei m_height;
    GLenum  m_format;
    std::vector<unsigned char> m_data;
    
public:
    // -- Constructors --
    Texture(GLint internalformat,GLsizei width,GLsizei height,GLenum format,const std::vector<unsigned char> data);
    Texture(GLint internalformat,GLsizei width,GLsizei height,GLenum format,const unsigned char* data);
    Texture(const char* path,GLint internalFormat,GLint format);
    
	Texture& operator=(Texture&& other) noexcept;
	Texture(Texture&& other) noexcept;
    
	Texture& operator=(const Texture&) = delete;
    Texture(const Texture&) = delete;

    // -- Destructors --
    ~Texture();

    // -- Getters --
    GLuint getGlId();
    // -- Methods --
    void deleteGlTexture();

private:
    // -- Private methods --
    void createGlTexture();

    //-- Utils --
    static constexpr size_t bytePerPixel(GLenum format, GLenum type); 
    
};