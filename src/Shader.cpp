#include "Shader.hpp"

#include <iostream>

#include "gl_utils.hpp"
#include "glad/glad.h"

//-- Constructors --
template<const GLenum SHADER_TYPE>
Shader<SHADER_TYPE>::Shader(const char* source) {
  glCall(m_glId = glCreateShader(SHADER_TYPE));
  compileShader(m_glId, source);
}

template<const GLenum SHADER_TYPE>
Shader<SHADER_TYPE>::Shader(std::string source)
    : Shader(source.c_str()) {}

template<const GLenum SHADER_TYPE>
Shader<SHADER_TYPE>::Shader(std::fstream& source_stream)
    : Shader<SHADER_TYPE>([&]() {
        std::string line, result;
        while (std::getline(source_stream, line)) result += line + "\n";
        return result;
      }()) {}

template<const GLenum SHADER_TYPE>
Shader<SHADER_TYPE>::Shader(Shader&& other) noexcept
    : m_glId(other.m_glId) {
  other.m_glId = 0;
}

template<const GLenum SHADER_TYPE>
Shader<SHADER_TYPE>& Shader<SHADER_TYPE>::operator=(Shader<SHADER_TYPE>&& other) noexcept {
  if (this != &other) {
    m_glId = other.m_glId;

    other.m_glId = 0;
  }

  return *this;
}

//-- Destructor --
template<const GLenum SHADER_TYPE>
Shader<SHADER_TYPE>::~Shader() {
  if (m_glId > 0) {
    glDeleteShader(m_glId);
    m_glId = 0;
  }
}

//-- Methods --

template<const GLenum SHADER_TYPE>
GLuint Shader<SHADER_TYPE>::getShader() const { return m_glId; }

template<const GLenum SHADER_TYPE>
int Shader<SHADER_TYPE>::compileShader(GLint glId, const char* source) {
  // check_compilation;

  glCall(glShaderSource(glId, 1, &source, nullptr));
  glCall(glCompileShader(glId));

  int success;
  char info_log[512];
  glCall(glGetShaderiv(glId, GL_COMPILE_STATUS, &success));

  if (!success) {
    glCall(glGetShaderInfoLog(glId, 512, NULL, info_log));
    auto shaderTypeStr = SHADER_TYPE == GL_FRAGMENT_SHADER ? "FRAGMENT_"
                         : SHADER_TYPE == GL_VERTEX_SHADER ? "VERTEX_"
                                                    : "";
    std::cerr << "ERROR::" << shaderTypeStr << "SHADER::COMPILATION_FAILED\n"
              << info_log << std::endl;
    return success;
  }

  return success ? 1 : 0;
}

// -- Alias --
template class Shader<GL_VERTEX_SHADER>;
template class Shader<GL_FRAGMENT_SHADER>;
