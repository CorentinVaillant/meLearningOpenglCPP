#include "Program.hpp"

#include <string>
#include <cstring>
#include <stdexcept>

using std::byte;

Program::Program(Shader&& vert_shad, Shader&& frag_shad)
    : m_vert(std::move(vert_shad)), m_frag(std::move(frag_shad)), m_program(0) 
    {}

Program::Program(const char* vert_shad_src, const char* frag_shad_src)
    : m_vert(VertexShader(vert_shad_src)), m_frag(FragmentShader(frag_shad_src)), m_program(0) 
    {}

Program::Program(Program&& rvalue)
    : m_vert(std::move(rvalue.m_vert)), 
    m_frag(std::move(rvalue.m_frag)), 
    m_program(rvalue.m_program) 
    {
        rvalue.m_program = 0;
    }


Program::~Program() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
    }
}

void Program::useProgram() {
    if (m_program == 0) {
        createGlProgram();
    }
    glUseProgram(m_program);
    useUniformData();
}

void Program::deleteProgram() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void Program::deleteShaders() {
    m_frag.deleteShader();
    m_vert.deleteShader();
}

const Shader& Program::getVertShader() const{
    return m_vert;
}

const Shader& Program::getFragShader() const{
    return m_frag;
}

// -- Uniforms functions --

GLint Program::getUniformLocation(const char* name) {
    return glGetUniformLocation(m_program, name);
}

template<typename T>
void Program::setUniformData(const char* name, GLenum glType,size_t number ,T* data){
    struct UniformData ud;
    ud.name = name;
    ud.glType = glType;
    ud.count = number;

    size_t offset = m_uniformPool.size();
    const size_t align = alignof(T);
    size_t padding = (align == 0) ? 0 : ( (align - (offset % align)) % align );
    offset += padding;

    const size_t nBytes = sizeof(T) * number;
    const size_t newSize = offset + nBytes;

    m_uniformPool.resize(newSize);
    std::memcpy(m_uniformPool.data() + offset, reinterpret_cast<const void*>(data), nBytes);

    ud.offset = offset;
    m_uniformsCardsPool.push_back(std::move(ud));


}
// Uniform 1 
void Program::setUniform1b(const char* name, bool value){
    setUniformData(name, GL_BOOL,1,&value);
}
void Program::setUniform1i(const char* name, int value) {
    setUniformData(name, GL_INT,1,&value);
}
void Program::setUniform1u(const char* name, unsigned int value) {
    setUniformData(name, GL_UNSIGNED_INT,1,&value);
}
void Program::setUniform1f(const char* name, float value) {
    setUniformData(name, GL_FLOAT,1,&value);
}

// Uniform 2

void Program::setUniform2b(const char* name, bool value[2]) {
    setUniformData(name,GL_BOOL,2 ,value);
}
void Program::setUniform2i(const char* name, int value[2]) {
    setUniformData(name,GL_INT,2 ,value);

}
void Program::setUniform2u(const char* name, unsigned int value[2]) {
    setUniformData(name,GL_UNSIGNED_INT,2 ,value);

}
void Program::setUniform2f(const char* name, float value[2]) { 
    setUniformData(name, GL_FLOAT,2,value);
}


void Program::setUniform2b(const char* name, bool v1, bool v2) {
    bool tab[2] = { v1, v2 };
    setUniform2b(name, tab);
}
void Program::setUniform2i(const char* name, int v1, int v2) {
    int tab[2] = { v1, v2 };
    setUniform2i(name, tab);
}
void Program::setUniform2u(const char* name, unsigned int v1, unsigned int v2) {
    unsigned int tmp[2] = { v1, v2 };
    setUniformData(name, GL_UNSIGNED_INT, 2, tmp);
}

void Program::setUniform2f(const char* name, float v1, float v2) {
    float tab[2] = { v1, v2 };
    setUniform2f(name, tab);
}

// Uniform 3

void Program::setUniform3b(const char* name, bool value[3]) {
    setUniformData(name,GL_BOOL,3 ,value);
}
void Program::setUniform3i(const char* name, int value[3]) {
    setUniformData(name,GL_INT,3 ,value);

}
void Program::setUniform3u(const char* name, unsigned int value[3]) {
    setUniformData(name,GL_UNSIGNED_INT,3 ,value);

}
void Program::setUniform3f(const char* name, float value[3]) { 
    setUniformData(name, GL_FLOAT,3,value);
}

void Program::setUniform3b(const char* name, bool v1, bool v2, bool v3) {
    bool tab[3] = { v1, v2, v3 };
    setUniform3b(name, tab);
}
void Program::setUniform3i(const char* name, int v1, int v2, int v3) {
    int tab[3] = { v1, v2, v3 };
    setUniform3i(name, tab);
}
void Program::setUniform3u(const char* name, unsigned int v1, unsigned int v2, unsigned int v3) {
    unsigned int tab[3] = { v1, v2, v3};
    setUniform3u(name, tab);
}

void Program::setUniform3f(const char* name, float v1, float v2, float v3) {
    float tab[3] = { v1, v2, v3 };
    setUniform3f(name, tab);
}

// Uniform mat4

void Program::setUniformMat4fv(const char* name, float matrix[16]) {
    setUniformData(name,GL_FLOAT,16,matrix);
}


void Program::setUniformTexture2D(const char* name,Texture& texture){
    TextureData td = {
        std::string(name),
        texture.getGlId(),
    };
    m_texturePool.push_back(td);
}

void Program::useUniformData(){
    //regular uniform types
    for(UniformData& ud : m_uniformsCardsPool){
        GLuint glLocation = glGetUniformLocation(m_program,ud.name.c_str());

        byte* dataPtr = reinterpret_cast<byte*>(m_uniformPool.data() + ud.offset);

        switch (ud.count)
        {
        case 1 :useUniformData1(glLocation, ud.glType, dataPtr);  break;
        case 2 :useUniformData2(glLocation, ud.glType, dataPtr);  break;
        case 3 :useUniformData3(glLocation, ud.glType, dataPtr);  break;
        case 4 :useUniformData4(glLocation, ud.glType, dataPtr);  break;
        case 9 :useUniformData9(glLocation, ud.glType, dataPtr);  break;
        case 16:useUniformData16(glLocation, ud.glType, dataPtr); break;
        default:
            break;
        }
    }

    // Textures
    GLint max_textures = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_textures);

    unsigned int numTexture = 0;
    for (TextureData& td : m_texturePool) {
        if (numTexture >= (unsigned int)max_textures) break;

        GLint glLoc = glGetUniformLocation(m_program, td.name.c_str());
        if (glLoc == -1) continue;

        glActiveTexture(GL_TEXTURE0 + numTexture);
        glBindTexture(GL_TEXTURE_2D, td.glId);
        glUniform1i(glLoc, numTexture);

        numTexture++;
    }

}

//Type suported : GL_BOOL, GL_INT, GL_UNSIGNED_INT, GL_FLOAT

void Program::useUniformData1(GLuint glLocation, GLenum glType, byte* data) {
    switch (glType) {
    case GL_BOOL:
    case GL_INT:
        glUniform1i(glLocation, *reinterpret_cast<const int*>(data));
        break;
    case GL_UNSIGNED_INT:
        glUniform1ui(glLocation, *reinterpret_cast<const unsigned int*>(data));
        break;
    case GL_FLOAT:
        glUniform1f(glLocation, *reinterpret_cast<const float*>(data));
        break;
    default:
        throw std::runtime_error("Unsupported glType for 1-component uniform");
    }
}

void Program::useUniformData2(GLuint glLocation, GLenum glType, byte* data) {
    switch (glType) {
    case GL_BOOL:
    case GL_INT:
        glUniform2iv(glLocation, 1, reinterpret_cast<const int*>(data));
        break;
    case GL_UNSIGNED_INT:
        glUniform2uiv(glLocation, 1, reinterpret_cast<const unsigned int*>(data));
        break;
    case GL_FLOAT:
        glUniform2fv(glLocation, 1, reinterpret_cast<const float*>(data));
        break;
    default:
        throw std::runtime_error("Unsupported glType for 2-component uniform");
    }
}

void Program::useUniformData3 (GLuint glLocation,GLenum glType, byte* data){
    switch (glType) {
    case GL_BOOL:
    case GL_INT:
        glUniform3iv(glLocation, 1, reinterpret_cast<const int*>(data));
        break;
    case GL_UNSIGNED_INT:
        glUniform3uiv(glLocation, 1, reinterpret_cast<const unsigned int*>(data));
        break;
    case GL_FLOAT:
        glUniform3fv(glLocation, 1, reinterpret_cast<const float*>(data));
        break;
    default:
        throw std::runtime_error("Unsupported glType for 3-component uniform");
    }
}
void Program::useUniformData4 (GLuint glLocation,GLenum glType, byte* data){
    switch (glType) {
    case GL_BOOL:
    case GL_INT:
        glUniform4iv(glLocation, 1, reinterpret_cast<const int*>(data));
        break;
    case GL_UNSIGNED_INT:
        glUniform4uiv(glLocation, 1, reinterpret_cast<const unsigned int*>(data));
        break;
    case GL_FLOAT:
        glUniform4fv(glLocation, 1, reinterpret_cast<const float*>(data));
        break;
    default:
        throw std::runtime_error("Unsupported glType for 4-component uniform");
    }
}

void Program::useUniformData9(GLuint glLocation, GLenum glType, byte* data) {
    if (glType == GL_FLOAT) {
        glUniformMatrix3fv(glLocation, 1, GL_FALSE, reinterpret_cast<const float*>(data));
    } else {
        throw std::runtime_error("Unsupported non-float 3x3 uniform");
    }
}

void Program::useUniformData16(GLuint glLocation, GLenum glType, byte* data) {
    if (glType == GL_FLOAT) {
        glUniformMatrix4fv(glLocation, 1, GL_FALSE, reinterpret_cast<const float*>(data));
    } else {
        throw std::runtime_error("Unsupported non-float 4x4 uniform");
    }
}


void Program::clearUniforms(){
    m_uniformPool.clear();
    m_uniformsCardsPool.clear();
    m_texturePool.clear();
}

void Program::createGlProgram(){

    m_program = glCreateProgram();
    if (!m_program) {
        throw "ERROR::PROGRAM::CREATION_FAILED \n";
    }
    glAttachShader(m_program, m_vert.getShader());
    glAttachShader(m_program, m_frag.getShader());

    glLinkProgram(m_program);

    int success;
    char info_log[512];
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_program, 512, NULL, info_log);
        std::string error = "ERROR::PROGRAM::LINK_FAILED\n";
        error += info_log;
        throw error;
    }
}