#include "Program.hpp"

#include <string>

Program::Program(Shader&& vert_shad, Shader&& frag_shad)
    : m_vert(std::move(vert_shad)), m_frag(std::move(frag_shad)), m_program(0) {}

Program::Program(const char* vert_shad_src, const char* frag_shad_src)
    : m_vert(VertexShader(vert_shad_src)), m_frag(FragmentShader(frag_shad_src)), m_program(0) {}


Program::~Program() {
    if (m_program != 0) {
        glDeleteProgram(m_program); //I think there is a segfault here
    }
}

void Program::useProgram() {
    if (m_program == 0) {
        createProgram();
    }
    glUseProgram(m_program);
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

GLint Program::getUniformLocation(const char* name) {
    return glGetUniformLocation(m_program, name);
}

void Program::setUniform1b(const char* name, bool value){
    glUniform1i(getUniformLocation(name), value);
}
void Program::setUniform1i(const char* name, int value) {
    glUniform1i(getUniformLocation(name), value);
}
void Program::setUniform1u(const char* name, unsigned int value) {
    glUniform1ui(getUniformLocation(name), value);
}
void Program::setUniform1f(const char* name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Program::setUniform2b(const char* name, bool v1, bool v2) {
    glUniform2i(getUniformLocation(name), v1,v2);
}
void Program::setUniform2i(const char* name, int v1, int v2) {
    glUniform2i(getUniformLocation(name), v1,v2);
}
void Program::setUniform2u(const char* name, unsigned int v1, unsigned int v2) {
    glUniform2ui(getUniformLocation(name), v1,v2);
}
void Program::setUniform2f(const char* name, float v1, float v2) {
    glUniform2f(getUniformLocation(name), v1,v2);
}

void Program::setUniform2b(const char* name, bool value[2]) {setUniform2b(name, value[0], value[1]);}
void Program::setUniform2i(const char* name, int value[2]) {setUniform2i(name, value[0], value[1]);}
void Program::setUniform2u(const char* name, unsigned int value[2]) {setUniform2u(name, value[0], value[1]);}
void Program::setUniform2f(const char* name, float value[2]) { setUniform2f(name, value[0], value[1]); }

void Program::setUniformMat4fv(const char* name, float matrix[16]) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix);
}


void Program::createProgram(){

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