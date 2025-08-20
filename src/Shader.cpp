#include "Shader.hpp"

#include "glad/glad.h"
#include <iostream>

//-- Constructors --


Shader::Shader(GLenum type, std::string source) 
	: m_shader_type(type),m_source(source), m_compiled_shader(0) {}

Shader::Shader(GLenum type, const char* source): Shader(type,std::string(source)) {}

Shader::Shader(GLenum type, std::fstream& source_stream)
	: Shader(type, [&]() {
	std::string line, result;
	while (std::getline(source_stream, line))
		result += line + "\n";
	return result;
		}()) {}


Shader::Shader(Shader&& other) noexcept:
    m_shader_type(other.m_shader_type),
    m_source(std::move(other.m_source)),
    m_compiled_shader(other.m_compiled_shader)
    {
        other.m_compiled_shader = 0;
    }

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        deleteShader();

        m_shader_type = other.m_shader_type;
        m_source = std::move(other.m_source);
        m_compiled_shader = other.m_compiled_shader;

        other.m_compiled_shader = 0;
    }

    return *this;
}

//-- Destructor --
Shader::~Shader() {
    deleteShader();
}

//-- Methods --

GLuint Shader::getShader(){
    if (m_compiled_shader <= 0) {
        int success = compileShader();
        if (success <= 0) {
            std::string error = "Could not compile shader, return code :";
            error += std::to_string(success);
            throw error;
        }
    }

    return m_compiled_shader;
}

void Shader::deleteShader() {
    if (m_compiled_shader > 0) {
        glDeleteShader(m_shader_type);
        m_compiled_shader = 0;
    }
}

int Shader::compileShader(){

    //GLint& shader, const GLenum shader_type, const char* shader_src, bool check_compilation;

    m_compiled_shader = glCreateShader(m_shader_type);

    const char* shader_src = m_source.c_str();
    glShaderSource(m_compiled_shader, 1, &shader_src, nullptr);
    glCompileShader(m_compiled_shader);


    int  success;
    char info_log[512];
    glGetShaderiv(m_compiled_shader, GL_COMPILE_STATUS, &success);


    if (!success) {
        glGetShaderInfoLog(m_compiled_shader, 512, NULL, info_log);
        auto shaderTypeStr = m_shader_type == GL_FRAGMENT_SHADER 
            ? "FRAGMENT_" 
            : m_shader_type == GL_VERTEX_SHADER
                ? "VERTEX_"
                : ""
            ;
        std::cerr << "ERROR::"<<shaderTypeStr<< "SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
        return success;
    }

    return success ? 1 : 0;
}