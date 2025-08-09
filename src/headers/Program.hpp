#pragma once

#include <glad/glad.h>

#include "Shader.hpp"

class Program
{
	Shader m_vert, m_frag;
	GLint m_program;

public:
	Program(Shader&& vert_shad, Shader&& frag_shad);
	Program(const char* vert_shad_src, const char* frag_shad_src);

	const Shader& getVertShader() const;
	const Shader& getFragShader() const;
	GLint getUniformLocation(const char* name);

	void setUniform1b(const char* name, bool value);
	void setUniform1i(const char* name, int value);
	void setUniform1u(const char* name, unsigned int value);
	void setUniform1f(const char* name, float value);

	void setUniform2b(const char* name, bool v1, bool v2 );
	void setUniform2i(const char* name, int v1, int v2 );
	void setUniform2u(const char* name, unsigned int v1, unsigned int v2);
	void setUniform2f(const char* name, float v1, float v2);

	void setUniform2b(const char* name, bool value[2]);
	void setUniform2i(const char* name, int value[2]);
	void setUniform2u(const char* name, unsigned int value[2]);
	void setUniform2f(const char* name, float value[2]);

	void setUniformMat4fv(const char* name, float matrix[16]);

	void useProgram();
	void deleteProgram();
	void deleteShaders();


	~Program();

private:
    void createProgram();
};
