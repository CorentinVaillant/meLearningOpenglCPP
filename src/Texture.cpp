#include "Texture.hpp"

#include <cstring>

#include "macros.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using std::memcpy;


// -- Constructors --
Texture::Texture(GLint internalformat,GLsizei width,GLsizei height,GLenum format,const std::vector<unsigned char> data ):
    m_glId(0),
    m_internalformat(internalformat),
    m_width(width),
    m_height(height),
    m_format(format),
    m_data(data)
    {}

Texture::Texture(GLint internalformat,GLsizei width,GLsizei height,GLenum format,const unsigned char* data ):
    Texture(internalformat,width,height,format,std::vector<unsigned char>(data, data + (size_t)(width*height) * bytePerPixel(format, GL_UNSIGNED_BYTE)))
    {}

Texture& Texture::operator=(Texture&& other) noexcept{
    if(this != &other){
        deleteGlTexture();

        m_glId = other.m_glId;
        m_internalformat = other.m_internalformat;
        m_width = other.m_width;
        m_height = other.m_height;
        m_format = other.m_format;

        m_data = std::move(other.m_data);

        other.m_glId = 0;
        other.m_data = std::vector<unsigned char>();
    }

    return *this;
}
Texture::Texture(Texture&& other) noexcept:
    m_glId(other.m_glId),
    m_internalformat(other.m_internalformat),
    m_width(other.m_width),
    m_height(other.m_height),
    m_format(other.m_format),
    m_data(other.m_data)
    {
        other.m_glId = 0;
        other.m_data = std::vector<unsigned char>();
    }


Texture::Texture(const char* path,GLint internalFormat,GLint format){
    stbi_set_flip_vertically_on_load(true);

    int width, height, _nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &_nrChannels, 0);
    expect_ptr(data,"could not load the image", -1);

    m_glId = 0;
    m_internalformat = internalFormat;
    m_width= width;
    m_height = height;
    m_format = format;
    m_data = std::vector<unsigned char>(data, data + (size_t)(width*height) * bytePerPixel(format, GL_UNSIGNED_BYTE));

    stbi_image_free(data);
}


// -- Destructors --
Texture::~Texture(){
    deleteGlTexture();
}

// -- Getters --
GLuint Texture::getGlId(){
    if(m_glId == 0)
        createGlTexture();
    return m_glId;
}

// -- Public methods --
void Texture::deleteGlTexture(){
    if(m_glId != 0)
        glDeleteTextures(1,&m_glId);
    m_glId = 0;
}

// -- Private methods --

void Texture::createGlTexture(){
    glGenTextures(1,&m_glId);
    if(!m_glId)
        throw "ERROR::TEXTURE::CREATION_FAILED \n";
    
    glBindTexture(GL_TEXTURE_2D,m_glId);
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalformat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    throwOnGlError("Error while creating a glTexture");
}

// -- Utils --
constexpr size_t Texture::bytePerPixel(GLenum format, GLenum type){
    
    size_t channels = 1;
    
    switch (format)
    {
    case GL_RED:    channels = 1; break;
    case GL_RG:     channels = 2; break;
    case GL_BGR: ;
    case GL_RGB :   channels = 3; break;
    case GL_BGRA : ;
    case GL_RGBA :  channels = 4; break;

    
    default: channels = 1; break; //Fallback
    }

    size_t bytesPerChannel = 1;
    switch (type) {
        case GL_UNSIGNED_BYTE:  ;
        case GL_BYTE:           bytesPerChannel = 1; break;

        case GL_UNSIGNED_SHORT: ;
        case GL_SHORT:          bytesPerChannel = 2; break;

        case GL_UNSIGNED_INT:   ;
        case GL_INT:            ;
        case GL_FLOAT:          bytesPerChannel = 4; break;
        default:                bytesPerChannel = 1; break; // Fallback
    }

    return channels * bytesPerChannel;

}