#pragma once

#include <fstream>
#include <string>

#include <glad/glad.h>


class Shader
{
	GLenum m_shader_type;
	std::string m_source;

	GLint m_compiled_shader;

public:
	//-- constructors --
	Shader(GLenum type, const char* source);
	Shader(GLenum type, std::string source);
	Shader(GLenum type, std::fstream& source_stream);

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	//-- methods --
	GLuint getShader();
	void deleteShader();


	//-- destructor --
	~Shader();

private:
	//-- private methods --
	int compileShader();
};

class VertexShader : public Shader {
public:
	VertexShader(const char* source) : Shader(GL_VERTEX_SHADER, source) {}
	VertexShader(std::string source) : Shader(GL_VERTEX_SHADER, source) {}
	VertexShader(std::fstream& source_stream) : Shader(GL_VERTEX_SHADER, source_stream) {}
};

class FragmentShader : public Shader {
public:
	FragmentShader(const char* source) : Shader(GL_FRAGMENT_SHADER, source) {}
	FragmentShader(std::string source) : Shader(GL_FRAGMENT_SHADER, source) {}
	FragmentShader(std::fstream& source_stream) : Shader(GL_FRAGMENT_SHADER, source_stream) {}
};