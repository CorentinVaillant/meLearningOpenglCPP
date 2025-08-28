#include "Program.hpp"

#include <string>
#include <cstring>
#include <unordered_map>
#include <stdexcept>


using std::byte;


Program::Program(VertexShader&& vert_shad, FragmentShader&& frag_shad)
    : m_vert(std::move(vert_shad)), m_frag(std::move(frag_shad))
    {
        m_glId = glCall(glCreateProgram());
        attachGlShader(m_glId,m_vert,m_frag);
    }

Program::Program(const char* vert_shad_src, const char* frag_shad_src)
    : Program(VertexShader(vert_shad_src), FragmentShader(frag_shad_src))
    {}

Program::Program(Program&& rvalue)
    : m_vert(std::move(rvalue.m_vert)), 
    m_frag(std::move(rvalue.m_frag)), 
    m_glId(rvalue.m_glId) 
    {
        rvalue.m_glId = 0;
    }


Program::~Program() {
    if (m_glId != 0) {
        glDeleteProgram(m_glId);
        m_glId = 0;
    }
}

void Program::useProgram() {
    glCall(glUseProgram(m_glId));
    useUniformData();
}

const VertexShader& Program::getVertShader() const{
    return m_vert;
}

const FragmentShader& Program::getFragShader() const{
    return m_frag;
}

// -- Uniforms functions --

GLint Program::getUniformLocation(const char* name) {
    return glCall(glGetUniformLocation(m_glId, name));
}

template<typename T>
void Program::setUniformData(const char* name, GLenum glType,size_t count ,T* data){    

    Program::UniformData* p_ud;

    if (m_uniformPool.contains(name)){
        p_ud = &m_uniformPool[name];
    }else{
        m_uniformPool[name] = Program::UniformData();
        p_ud = &m_uniformPool[name];
        p_ud->glLocation = getUniformLocation(name);
    }
    
    p_ud->glType = glType;
    p_ud->count = count;
    p_ud->data =(byte*) (new T[count]);
    memcpy(p_ud->data,data,count*sizeof(T));
}
// Uniform 1 
void Program::setUniform1b(const char* name, bool value)         { setUniformData(name, GL_BOOL,1,&value);}
void Program::setUniform1i(const char* name, int value)          { setUniformData(name, GL_INT,1,&value);}
void Program::setUniform1u(const char* name, unsigned int value) { setUniformData(name, GL_UNSIGNED_INT,1,&value);}
void Program::setUniform1f(const char* name, float value)        { setUniformData(name, GL_FLOAT,1,&value);}

// Uniform 2
void Program::setUniform2b(const char* name, bool value[2])         { setUniformData(name, GL_BOOL,2 ,value);}
void Program::setUniform2i(const char* name, int value[2])          { setUniformData(name, GL_INT,2 ,value);}
void Program::setUniform2u(const char* name, unsigned int value[2]) { setUniformData(name, GL_UNSIGNED_INT,2 ,value);}
void Program::setUniform2f(const char* name, float value[2])        { setUniformData(name, GL_FLOAT,2,value);}


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

void Program::setUniform3b(const char* name, bool value[3])         { setUniformData(name, GL_BOOL,3 ,value);}
void Program::setUniform3i(const char* name, int value[3])          { setUniformData(name, GL_INT,3 ,value);}
void Program::setUniform3u(const char* name, unsigned int value[3]) { setUniformData(name, GL_UNSIGNED_INT,3 ,value);}
void Program::setUniform3f(const char* name, float value[3])        { setUniformData(name, GL_FLOAT,3,value);}

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
    for(auto& key_value : m_uniformPool){
        const UniformData& ud = key_value.second; 
        switch (ud.count)
        {
        case 1 :useUniformData1(ud.glLocation, ud.glType, ud.data);  break;
        case 2 :useUniformData2(ud.glLocation, ud.glType, ud.data);  break;
        case 3 :useUniformData3(ud.glLocation, ud.glType, ud.data);  break;
        case 4 :useUniformData4(ud.glLocation, ud.glType, ud.data);  break;
        case 9 :useUniformData9(ud.glLocation, ud.glType, ud.data);  break;
        case 16:useUniformData16(ud.glLocation, ud.glType, ud.data); break;
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

        GLint glLoc = glCall(glGetUniformLocation(m_glId, td.name.c_str()));
        if (glLoc == -1) continue;

        glCall(glActiveTexture(GL_TEXTURE0 + numTexture));
        glCall(glBindTexture(GL_TEXTURE_2D, td.glId));
        glCall(glUniform1i(glLoc, numTexture));

        numTexture++;
    }
}

//Type suported : GL_BOOL, GL_INT, GL_UNSIGNED_INT, GL_FLOAT

void Program::useUniformData1(GLuint glLocation, GLenum glType, byte* data) {
    switch (glType) {
    case GL_BOOL:
    case GL_INT:
        glUniform1i(glLocation, *(const int*)(data));
        break;
    case GL_UNSIGNED_INT:
        glUniform1ui(glLocation, *(const unsigned int*)(data));
        break;
    case GL_FLOAT:
        glUniform1f(glLocation, *(const float*)(data));
        break;
    default:
        throw std::runtime_error("Unsupported glType for 1-component uniform");
    }

    throwOnGlError("Error in useUniformData 1");
}

void Program::useUniformData2(GLuint glLocation, GLenum glType, byte* data) {
    switch (glType) {
    case GL_BOOL:
    case GL_INT:
        glUniform2iv(glLocation, 1, (const int*)(data));
        break;
    case GL_UNSIGNED_INT:
        glUniform2uiv(glLocation, 1, (const unsigned int*)(data));
        break;
    case GL_FLOAT:
        glUniform2fv(glLocation, 1, (const float*)(data));
        break;
    default:
        throw std::runtime_error("Unsupported glType for 2-component uniform");
    }

    throwOnGlError("Error in useUniformData 2");
}

void Program::useUniformData3 (GLuint glLocation,GLenum glType, byte* data){
    switch (glType) {
    case GL_BOOL:
    case GL_INT:
        glUniform3iv(glLocation, 1, (const int*)(data));
        break;
    case GL_UNSIGNED_INT:
        glUniform3uiv(glLocation, 1, (const unsigned int*)(data));
        break;
    case GL_FLOAT:
        glUniform3fv(glLocation, 1, (const float*)(data));
        break;
    default:
        throw std::runtime_error("Unsupported glType for 3-component uniform");
    }
    throwOnGlError("Error in useUniformData3");
}
void Program::useUniformData4 (GLuint glLocation,GLenum glType, byte* data){
    switch (glType) {
    case GL_BOOL:
    case GL_INT:
        glUniform4iv(glLocation, 1, (const int*)(data));
        break;
    case GL_UNSIGNED_INT:
        glUniform4uiv(glLocation, 1, (const unsigned int*)(data));
        break;
    case GL_FLOAT:
        glUniform4fv(glLocation, 1, (const float*)(data));
        break;
    default:
        throw std::runtime_error("Unsupported glType for 4-component uniform");
    }
    throwOnGlError("Error in useUniformData4");

}

void Program::useUniformData9(GLuint glLocation, GLenum glType, byte* data) {
    if (glType == GL_FLOAT) {
        glUniformMatrix3fv(glLocation, 1, GL_FALSE, (const float*)(data));
    } else {
        throw std::runtime_error("Unsupported non-float 3x3 uniform");
    }
    throwOnGlError("Error in useUniformData9");
}

void Program::useUniformData16(GLuint glLocation, GLenum glType, byte* data) {
    if (glType == GL_FLOAT) {
        glUniformMatrix4fv(glLocation, 1, GL_FALSE, (const float*)(data));
    } else {
        throw std::runtime_error("Unsupported non-float 4x4 uniform");
    }
    throwOnGlError("Error in useUniformData16");
}



void Program::clearUniforms(){
    m_uniformPool.clear();
    m_texturePool.clear();
}

void Program::attachGlShader(GLint glProgramId, VertexShader& vs, FragmentShader& fs){

    glCall(glAttachShader(glProgramId, vs.getShader()));
    glCall(glAttachShader(glProgramId, fs.getShader()));

    glCall(glLinkProgram(glProgramId));

    int success;
    char info_log[512];
    glCall(glGetProgramiv(glProgramId, GL_LINK_STATUS, &success));
    if (!success) {
        glCall(glGetProgramInfoLog(glProgramId, 512, NULL, info_log));
        std::string error = "ERROR::PROGRAM::LINK_FAILED\n";
        error += info_log;
        throw error;
    }
}
