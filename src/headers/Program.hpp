#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

#include "Shader.hpp"
#include "Texture.hpp"

class Program
{
	struct UniformData;
	struct TextureData;

	Shader m_vert, m_frag;
	GLint m_program;

	std::vector<std::byte> m_uniformPool;
	std::vector<Program::UniformData> m_uniformsCardsPool;

	std::vector<Program::TextureData> m_texturePool;

public:
	Program(Shader&& vert_shad, Shader&& frag_shad);
	Program(const char* vert_shad_src, const char* frag_shad_src);
	Program(Program&& rvalue);

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

	void setUniformTexture2D(const char* name,Texture& texture);

	void clearUniforms();

	void useProgram();
	void deleteProgram();
	void deleteShaders();

	~Program();

	Program(Program&) = delete;

private:
    void createGlProgram();
	template<typename T> void setUniformData(const char* name, GLenum glType, size_t number ,T* data);
	void useUniformData();

	static void useUniformData1 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData2 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData3 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData4 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData9 (GLuint glLocation,GLenum glType, std::byte* data);
	static void useUniformData16(GLuint glLocation,GLenum glType, std::byte* data);

	struct UniformData
	{
		std::string name;
		GLenum glType;
		size_t count;
		size_t offset;
	};

	struct TextureData
	{
		std::string name;
		GLuint glId;
	};
	
	
};
