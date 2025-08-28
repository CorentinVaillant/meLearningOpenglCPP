#pragma once

#include <glad/glad.h>

#include <fstream>
#include <string>

template<const GLenum SHADER_TYPE>
class Shader {
  GLint m_glId;

 public:
  //-- constructors --
  Shader(const char* source);
  Shader(std::string source);
  Shader(std::fstream& source_stream);

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  //-- methods --
  GLuint getShader() const;

  //-- destructor --
  ~Shader();

 private:
  //-- private methods --
  static int compileShader(GLint glId, const char* source);
};


// -- Alias --
using VertexShader = Shader<GL_VERTEX_SHADER>;
using FragmentShader = Shader<GL_FRAGMENT_SHADER>;